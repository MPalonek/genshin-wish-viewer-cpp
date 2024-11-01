#include "importerItem.h"
#include <format>

ImporterItem::ImporterItem(std::string fileLocation) : m_imageLocation(fileLocation)
{
    m_textPixPos.reserve(28); // there shouldnt be more than 7 rows of 4 elements
}

ImporterItem::~ImporterItem()
{
}

std::vector<PixPos>& ImporterItem::GetTextSnippets()
{
    LoadGrayscaleImage();
    BinarizeImage();
    ProcessImage();
    FindTextPositons();
    ValidateAndReorganizeBoxPix();
    return m_textPixPos;
}

void ImporterItem::LoadGrayscaleImage()
{
    // Load image
    Pix* image = pixRead(m_imageLocation.c_str());
    if (!image) {
        log(std::format("Failed to load image {}", m_imageLocation), Logger::ERROR);
    }

    // Convert input image to grayscale
    m_image = pixConvertRGBToGrayFast(image);

    // Save dimensions
    m_width = pixGetWidth(m_image);
    m_height = pixGetHeight(m_image);
    m_depth = pixGetDepth(m_image);
    log(std::format("Loaded image {} - Width: {}, Height {}, Depth {}", m_imageLocation, m_width, m_height, m_depth), Logger::DEBUG);
    
    pixDestroy(&image);
}

void ImporterItem::BinarizeImage()
{
    // sx, sy - desired tile dimensions; actual size may vary
    // smoothx, smoothy - half-width of convolution kernel applied to threshold array: use 0 for no smoothing
    // scorefract - fraction of the max Otsu score; typ. 0.1; use 0.0 for standard Otsu
    int sx{ 2048 }; // if bigger than img will get global threshold
    int sy{ 2048 };
    int smoothx{ 0 }; // no smoothing used for global threshold
    int smoothy{ 0 };
    float scorefract{ 0.2f }; // scorefract is the fraction of the maximum Otsu score
    PIX* pixThreshold{ NULL };

    pixOtsuAdaptiveThreshold(m_image, sx, sy, smoothx, smoothy, scorefract, &pixThreshold, &m_binaryImage);
    
    // Get threshold value from Otsu function
    l_uint32 threshold{};
    pixGetPixel(pixThreshold, 0, 0, &threshold);
    log(std::format("Binarized image - Threshold {}", threshold), Logger::DEBUG);

    pixDestroy(&pixThreshold);

    // To display image you need IrfanView
    //setLeptDebugOK(1);
    //pixDisplay(m_binaryImage, 0, 0);
}

void ImporterItem::ProcessImage()
{
    // erode - to get rid of small artifacts
    SEL* selE = selCreateBrick(3, 3, 1, 1, SEL_HIT);
    Pix* erode = pixErode(NULL, m_binaryImage, selE);

    // dilate - to make big what was left after erosion (preferably blobs were text is)
    SEL* selD = selCreateBrick(5, 32, 2, 15, SEL_HIT); // check out this special mask! 
    Pix* dilate = pixDilate(NULL, erode, selD);
    //pixDisplay(dilate, 0, 0);

    // close twice - to get even better blobs were text is
    // Step 1: Use a small square mask to close minor gaps, edges, and holes.
    // Step 2: Use a thin vertical mask to remove narrow extrusions, especially when blobs are close horizontally.
    SEL* selC = selCreateBrick(5, 5, 2, 2, SEL_HIT);
    SEL* selC2 = selCreateBrick(10, 1, 4, 0, SEL_HIT);

    // First closing pass with small square mask
    Pix* close = pixClose(NULL, dilate, selC);
    //pixDisplay(close, 0, 0);

    // Second closing pass with thin vertical mask for targeted extrusion removal
    m_blobsBinaryImage = pixCreate(m_width, m_height, 8);
    pixClose(m_blobsBinaryImage, close, selC2);
    //pixDisplay(m_blobsBinaryImage, 0, 0);

    log(std::format("Processed image"), Logger::DEBUG);

    pixDestroy(&erode);
    pixDestroy(&dilate);
    pixDestroy(&close);
}

void ImporterItem::FindTextPositons()
{
    // box each blob
    Boxa* boxes = pixConnCompBB(m_blobsBinaryImage, 8); // 8-connected components

    // Save blobs positions and images
    for (l_int32 i = 0; i < boxaGetCount(boxes); ++i) {
        Box* box = boxaGetBox(boxes, i, L_CLONE);
        int x, y, w, h;
        // Use Leptonica's boxGetGeometry to get the box coordinates and size
        boxGetGeometry(box, &x, &y, &w, &h);

        // make rectangles bigger
        int padding = 5; // Adjust this value as needed
        x -= padding;
        y -= padding;
        w += 2 * padding;
        h += 2 * padding;

        // save image and positions of text
        Position pos(x, y, w, h);
        Box* expandedBox = pos.CreateBox();
        m_textPixPos.emplace_back(PixPos{ pixClipRectangle(m_binaryImage, expandedBox, NULL), pos });

        boxDestroy(&box);
        boxDestroy(&expandedBox);
    }
    log(std::format("Found {} boxes with text", m_textPixPos.size()));

    boxaDestroy(&boxes);
}

// Custom comparator function for PixPos to sort by y first, then x
bool comparePositions(const PixPos& a, const PixPos& b) {
    const int clearance = 20; // y values can vary a little bit

    if (std::abs(a.pos.y - b.pos.y) <= clearance) {
        return a.pos.x < b.pos.x; // If y is the "same", compare x
    }
    return a.pos.y < b.pos.y;     // Otherwise, compare y
}

void ImporterItem::ValidateAndReorganizeBoxPix()
{
    if (m_textPixPos.size() <= 28 && ((m_textPixPos.size() % 4 == 0) || (m_textPixPos.size() % 3 == 0))) {
        std::sort(m_textPixPos.begin(), m_textPixPos.end(), comparePositions);
    }
    else {
        log(std::format("Validation error - got {} text blobs. Attempting to recover...", m_textPixPos.size()), Logger::WARNING);
        //DisplayImageWithBoxes();

        bool found{ false };

        for (int i = 0; i < m_textPixPos.size(); ++i)
        {
            for (int j = 0; j < m_textPixPos.size(); ++j)
            {
                if (i != j && DoIntersect(m_textPixPos[i].pos, m_textPixPos[j].pos)) {
                    MergeBoxes(m_textPixPos[i].pos, m_textPixPos[j].pos);
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        if (found) {
            ValidateAndReorganizeBoxPix();
            log(std::format("Recovered from validation error. Got {} text blobs.", m_textPixPos.size()), Logger::WARNING);
            //DisplayImageWithBoxes();
        }
        else {
            // we know size is wrong, but we didnt manage to mergeBoxes - we are cooked
            log(std::format("Validation error - got {} text blobs AND FAILED TO RECOVER!", m_textPixPos.size()), Logger::ERROR);
            exit(1); //maybe throw with meaningful message
        }

    }
}

bool ImporterItem::DoIntersect(const Position& a, const Position& b) {
    int y_pad = 2;
    int x_pad = 0;
    return (
        (a.x < b.x1()) &&   // a's left edge is left of b's right edge
        (a.x1() > b.x) &&   // a's right edge is right of b's left edge
        ((a.y - y_pad) < (b.y1() + y_pad)) &&   // a's top edge is above b's bottom edge (adjusted by y_pad)
        ((a.y1() - y_pad) > (b.y + y_pad))      // a's bottom edge is below b's top edge (adjusted by y_pad)
        );
}

void ImporterItem::MergeBoxes(const Position& a, const Position& b) {
    // Find the top-left corner (smallest x and y)
    int merged_x = std::min(a.x, b.x);
    int merged_y = std::min(a.y, b.y);

    // Find the bottom-right corner (largest x and y)
    int merged_x_max = std::max(a.x1(), b.x1());
    int merged_y_max = std::max(a.y1(), b.y1());

    // Calculate the new width and height
    int merged_w = merged_x_max - merged_x;
    int merged_h = merged_y_max - merged_y;

    // Replace the box
    // 1. Find the box in the vector
    auto boxesToRemove = { a, b };
    for (auto boxToRemove : boxesToRemove) {
        auto it = std::find_if(m_textPixPos.begin(), m_textPixPos.end(),
            [&boxToRemove](const PixPos& item) {
                return item.pos == boxToRemove; // Compare based on the Box part of the struct
            });

        // 2. Remove the box from the vector
        if (it != m_textPixPos.end()) {
            m_textPixPos.erase(it);
        }
        else {
            std::cout << "Box not found!" << std::endl;
        }
    }

    // 3. Insert merged box
    Position pos(merged_x, merged_y, merged_w, merged_h);
    Box* expandedBox = pos.CreateBox();
    m_textPixPos.emplace_back(PixPos{ pixClipRectangle(m_binaryImage, expandedBox, NULL), pos });
}

void ImporterItem::SaveImageAsCsv()
{
    // Get image dimensions
    PIX* image = m_image;    
    l_int32 width, height;
    pixGetDimensions(image, &width, &height, NULL);

    // Open file for writing
    std::ofstream file("data.csv");
    if (!file) {
        log("Error: Could not open file for writing.", Logger::ERROR);
    }

    // Create vector for values (if you want to inspect it further)
    std::vector<std::vector<int>> imgVec{};
    imgVec.reserve(height);

    // Write values to vector and csv
    for (l_int32 y = 0; y < height; ++y) {
        std::vector<int> row{};
        row.reserve(width);
        for (l_int32 x = 0; x < width; ++x) {
            l_uint32 pixelValue{};
            pixGetPixel(image, x, y, &pixelValue);
            row.push_back(pixelValue);
            file << pixelValue;
            if (x < width) {
                file << ",";  // Separate values with a comma
            }
        }
        imgVec.push_back(row);
        file << "\n";  // New line after each row
    }

    // Close the file
    file.close();

    /*
    Then to generate histogram, run this script in python:
        import matplotlib.pyplot as plt
        import pandas as pd

        # Read the CSV file into a DataFrame
        data = pd.read_csv('..\\data.csv', header=None)

        # Convert the DataFrame column to a list
        values = data.values.flatten()

        # Create a histogram
        plt.hist(values, bins='auto', edgecolor='black')

        # Add labels and title
        plt.xlabel('Value')
        plt.ylabel('Frequency')
        plt.title('Histogram of Values')

        # Show the plot
        plt.show()
    */
}

void ImporterItem::DisplayImageWithBoxes()
{
    // To display image you need IrfanView
    setLeptDebugOK(1);
    
    // Create RGB Pix which have boxes with detected text and log boxes info 
    Pix* imageWithBoxes = pixConvertTo32(m_image);
    for (size_t i = 0; i < m_textPixPos.size(); ++i) {
        auto& pos = m_textPixPos[i].pos;
        Box* box = pos.CreateBox();
        pixRenderBoxArb(imageWithBoxes, box, 2, 255, 0, 0);  // Draw the box borders on the result image (,, 2px border,R,G,B)
        boxDestroy(&box);
        log(std::format("Box {:2}, X: {:4}, Y: {:4}, W: {:4}, H {:4}, X1: {:4}, Y1: {:4}", i, pos.x, pos.y, pos.w, pos.h, pos.x1(), pos.y1()), Logger::DEBUG);
    }

    pixDisplay(imageWithBoxes, 0, 0);
    pixDestroy(&imageWithBoxes);
}

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
    SEL* selD = selCreateBrick(3, 32, 1, 15, SEL_HIT); // check out this special mask! 
    Pix* dilate = pixDilate(NULL, erode, selD);

    // close - to get even better blobs were text is
    SEL* selC = selCreateBrick(9, 9, 4, 4, SEL_HIT);
    m_blobsBinaryImage = pixCreate(m_width, m_height, 8);
    pixClose(m_blobsBinaryImage, dilate, selC);

    log(std::format("Processed image"), Logger::DEBUG);

    pixDestroy(&erode);
    pixDestroy(&dilate);
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
    if ((m_textPixPos.size() % 4 == 0) || (m_textPixPos.size() % 3 == 0)) {
        std::sort(m_textPixPos.begin(), m_textPixPos.end(), comparePositions);
    }
    else {
        log(std::format("Validation error - got {} text blobs.", m_textPixPos.size()), Logger::ERROR);
    }

    // 

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
        log(std::format("Box {:2}, X: {:4}, Y: {:4}, W: {:4}, H {:4}", i, pos.x, pos.y, pos.w, pos.h), Logger::DEBUG);
    }

    pixDisplay(imageWithBoxes, 0, 0);
    pixDestroy(&imageWithBoxes);
}

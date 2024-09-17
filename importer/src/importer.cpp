#include "importer.h"
#include "logger/src/logger.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <filesystem>


// https://docs.opencv.org/4.x/dd/d6e/tutorial_windows_visual_studio_opencv.html
// https://tpgit.github.io/Leptonica

// this function is for finding out the best otsu values
void saveOtsuBinarizedImg(std::string imgPath, int matrixSize)
{
    // Load image
    Pix* image = pixRead(imgPath.c_str());

    // Print out H W D
    int width = pixGetWidth(image);
    int height = pixGetHeight(image);
    int depth = pixGetDepth(image);
    std::stringstream log;
    log << "image Width: " << width << ", Height: " << height << ", Depth: " << depth;
    Logger::getInstance().addLog("importer", log);

    // Convert input image to grayscale
    Pix* gray = pixConvertRGBToGrayFast(image);
    
    // Displaying image in irfanView
    setLeptDebugOK(1);
    pixDisplay(gray, 0, 0);

    // Create bunch of Pixs for binarization
    Pix* ppixth = pixCreate(width, height, 8);
    Pix* ppixd1 = pixCreate(width, height, 8);
    Pix* ppixd2 = pixCreate(width, height, 8);
    Pix* ppixd3 = pixCreate(width, height, 8);

    // Pixa to concatate all pix's
    Pixa* pixa = pixaCreate(9);

    int sx = matrixSize;
    int sy = sx;

    for (int i = 0; i <= 2; i++)
    {
        // Otsu
        pixOtsuAdaptiveThreshold(gray, sx, sy, i, i, 0, &ppixth, &ppixd1);
        pixOtsuAdaptiveThreshold(gray, sx, sy, i, i, 0.1, &ppixth, &ppixd2);
        pixOtsuAdaptiveThreshold(gray, sx, sy, i, i, 0.2, &ppixth, &ppixd3);

        // Add pix to pixa
        pixaAddPix(pixa, ppixd1, L_INSERT);
        pixaAddPix(pixa, ppixd2, L_INSERT);
        pixaAddPix(pixa, ppixd3, L_INSERT);

        //pixWrite((filename+std::string("-0.jpg")).c_str(), ppixd, 1); // in case you want to save single pix
    }

    std::stringstream ss;
    ss << "__output/fixme" << "_Otsu-" << sx << "-" << sy;
    std::string filename = ss.str();

    Pix* tiledPix = pixaDisplayTiledInColumns(pixa, 3, 1, 10, 1);
    pixWrite((filename + std::string(".jpg")).c_str(), tiledPix, IFF_PNG);

    // Cleanup...
    pixDestroy(&image);
    pixDestroy(&gray);
    pixDestroy(&ppixth);
    pixDestroy(&ppixd1);
    pixDestroy(&ppixd2);
    pixDestroy(&ppixd3);
}

void attempt(std::string imgPath)
{
    setLeptDebugOK(1);

    // Load image
    Pix* image = pixRead(imgPath.c_str());

    // Convert input image to grayscale
    Pix* gray = pixConvertRGBToGrayFast(image);

    // Create bunch of Pixs for binarization
    Pix* ppixth = NULL;
    Pix* ppixd = NULL;

    int sx = 32;
    int sy = sx;

    // Otsu
    pixOtsuAdaptiveThreshold(gray, sx, sy, 2, 2, 0.2f, NULL, &ppixd);
    pixDisplay(ppixd, 0, 0);
    
    // Create a 3x3 structuring element for erosion
    

    // erode - to get rid of small artifacts
    SEL* sele = selCreateBrick(3, 3, 1, 1, SEL_HIT);
    Pix* erode = pixErode(NULL, ppixd, sele);
    pixDisplay(erode, 0, 0);

    // dilate - to make big what was left after erosion (preferably blobs were text is)
    SEL* seld = selCreateBrick(32, 32, 15, 15, SEL_HIT);
    Pix* dilate = pixDilate(NULL, erode, seld);
    pixDisplay(dilate, 0, 0);

    // split image into rows (6 rows max?)

}

void attempt2(std::string imgPath)
{
    setLeptDebugOK(1);
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    char* outText;

    // Load image
    Pix* image = pixRead(imgPath.c_str());

    // Convert input image to grayscale
    Pix* gray = pixConvertRGBToGrayFast(image);
    //pixDisplay(gray, 0, 0);


    // Get image dimensions
    l_int32 width, height;
    pixGetDimensions(gray, &width, &height, NULL);

    std::vector<std::vector<int>> grayArr{};
    // Print the matrix of pixel values
    for (l_int32 y = 0; y < height; ++y) {
        std::vector<int> temp{};
        temp.reserve(width);
        for (l_int32 x = 0; x < width; ++x) {
            l_uint32 pixelValue{};
            pixGetPixel(gray, x, y, &pixelValue);
            temp.push_back(pixelValue);
        }
        grayArr.push_back(temp);
    }



    // otsu
    Pix* ppixth = NULL;
    Pix* pixBinary = NULL;

    int sx = 2048;
    int sy = sx;

    int smoothx = 2;
    int smoothy = smoothx;

    pixOtsuAdaptiveThreshold(gray, sx, sy, smoothx, smoothy, 0.2f, NULL, &pixBinary);
    pixDisplay(pixBinary, 0, 0);

    // erode - to get rid of small artifacts
    SEL* sele = selCreateBrick(3, 3, 1, 1, SEL_HIT);
    Pix* erode = pixErode(NULL, pixBinary, sele);
    pixDisplay(erode, 0, 0);

    // dilate - to make big what was left after erosion (preferably blobs were text is)
    SEL* seld = selCreateBrick(3, 32, 1, 15, SEL_HIT); // check out this special mask! 
    Pix* dilate = pixDilate(NULL, erode, seld);
    pixDisplay(dilate, 0, 0);

    // close - to get even better blobs were text is
    SEL* selc = selCreateBrick(9, 9, 4, 4, SEL_HIT);
    Pix* close = pixCreate(width, height, 8);
    pixClose(close, dilate, selc);
    pixDisplay(close, 0, 0);

    // box each blob
    Boxa* boxes = pixConnCompBB(close, 8); // 8-connected components
    Pix* resultImage = pixConvert1To32(NULL, pixBinary, 0xFFFFFFFF, 0);  // White background for binary image
    for (l_int32 i = 0; i < boxaGetCount(boxes); ++i) {
        Box* box = boxaGetBox(boxes, i, L_CLONE);
        int x, y, w, h;
        // Use Leptonica's boxGetGeometry to get the box coordinates and size
        boxGetGeometry(box, &x, &y, &w, &h);
        std::stringstream log;
        log << "Box " << std::setw(2) << i << " - x: " << std::setw(4) << x << ", y: " << std::setw(3) << y << ", w:" << w << ", h:" << h;
        
        // make rectangles bigger
        int padding = 5; // Adjust this value as needed
        x -= padding;
        y -= padding;
        w += 2 * padding;
        h += 2 * padding;
        Box* expandedBox = boxCreate(x, y, w, h);

        // crop/cut image into smaller one
        PIX* pixCropped = pixClipRectangle(pixBinary, expandedBox, NULL);
        std::string filename = "blob_" + std::to_string(i + 1) + ".png";
        pixWrite(filename.c_str(), pixCropped, IFF_PNG);

        //OCR
        api->SetImage(pixCropped);
        // Get OCR result
        outText = api->GetUTF8Text();
        printf("OCR output:\n%s", outText);

        std::string z = outText;
        //removeSpecialChars(z);
        log << "\t" << z;
        Logger::getInstance().addLog("importer", log);

        pixRenderBoxArb(resultImage, box, 1, 255, 0, 0);  // Draw the box borders on the result image
        boxDestroy(&box);
    }
    pixDisplay(resultImage, 0, 0);

    










    int i = 0;

}

void edgeDetection(std::string_view imgPath)
{
    // Load image
    std::unique_ptr<Pix*> image = std::make_unique<Pix*>(pixRead(imgPath.data()));
    //Pix* image = pixRead(imgPath.data());
    pixWrite("__output/sobelEdge_0.jpg", *image, IFF_PNG);

    Pix* gray = pixConvertRGBToGrayFast(*image);
    pixWrite("__output/sobelEdge_1.jpg", gray, IFF_PNG);

    // Pixa to concatate all pix's
    Pixa* pixa = pixaCreate(24);
    Pixa* pixa_b = pixaCreate(12);

    int mult[3] = {3,5,9};

    for (int i = 0; i < 3; i++)
    {
        int sx = mult[i];
        Pix* proc = pixDilateGray(gray, sx, sx);
        int width = pixGetWidth(proc);
        int height = pixGetHeight(proc);
        Pix* ppixth = pixCreate(width, height, 8);
        Pix* binarized = pixCreate(width, height, 8);
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1f, &ppixth, &binarized);

        Boxa* boxes = pixConnCompBB(binarized, 8);

        Pix* resultImage = pixCopy(nullptr, proc);
        for (l_int32 i = 0; i < boxaGetCount(boxes); ++i) {
            Box* box = boxaGetBox(boxes, i, L_CLONE);
            pixRenderBoxArb(resultImage, box, 1, 255,0,0);  // Draw the box borders on the result image
            boxDestroy(&box);
        }
        
        // Add pix to pixa
        pixaAddPix(pixa, proc, L_INSERT);
        pixaAddPix(pixa, binarized, L_INSERT);
        pixaAddPix(pixa, resultImage, L_INSERT);
    }
    for (int i = 0; i < 3; i++)
    {
        int sx = mult[i];
        Pix* proc = pixErodeGray(gray, sx, sx);
        int width = pixGetWidth(proc);
        int height = pixGetHeight(proc);
        Pix* ppixth = pixCreate(width, height, 8);
        Pix* binarized = pixCreate(width, height, 8);
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1f, &ppixth, &binarized);

        Boxa* boxes = pixConnCompBB(binarized, 8);

        Pix* resultImage = pixCopy(nullptr, proc);
        for (l_int32 i = 0; i < boxaGetCount(boxes); ++i) {
            Box* box = boxaGetBox(boxes, i, L_CLONE);
            pixRenderBoxArb(resultImage, box, 1, 255, 0, 0);  // Draw the box borders on the result image
            boxDestroy(&box);
        }

        // Add pix to pixa
        pixaAddPix(pixa, proc, L_INSERT);
        pixaAddPix(pixa, binarized, L_INSERT);
        pixaAddPix(pixa, resultImage, L_INSERT);
    }


    for (int i = 0; i < 3; i++)
    {
        int sx = mult[i];
        Pix* proc = pixCloseGray(gray, sx, sx);
        int width = pixGetWidth(proc);
        int height = pixGetHeight(proc);
        Pix* ppixth = pixCreate(width, height, 8);
        Pix* binarized = pixCreate(width, height, 8);
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1f, &ppixth, &binarized);

        Boxa* boxes = pixConnCompBB(binarized, 8);

        Pix* resultImage = pixCopy(nullptr, proc);
        for (l_int32 i = 0; i < boxaGetCount(boxes); ++i) {
            Box* box = boxaGetBox(boxes, i, L_CLONE);
            pixRenderBoxArb(resultImage, box, 1, 255, 0, 0);  // Draw the box borders on the result image
            boxDestroy(&box);
        }

        // Add pix to pixa
        pixaAddPix(pixa_b, proc, L_INSERT);
        pixaAddPix(pixa_b, binarized, L_INSERT);
        pixaAddPix(pixa_b, resultImage, L_INSERT);
    }
    for (int i = 0; i < 3; i++)
    {
        int sx = mult[i];
        Pix* proc = pixOpenGray(gray, sx, sx);
        int width = pixGetWidth(proc);
        int height = pixGetHeight(proc);
        Pix* ppixth = pixCreate(width, height, 8);
        Pix* binarized = pixCreate(width, height, 8);
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1f, &ppixth, &binarized);

        Boxa* boxes = pixConnCompBB(binarized, 8);

        Pix* resultImage = pixCopy(nullptr, proc);
        for (l_int32 i = 0; i < boxaGetCount(boxes); ++i) {
            Box* box = boxaGetBox(boxes, i, L_CLONE);
            pixRenderBoxArb(resultImage, box, 1, 255, 0, 0);  // Draw the box borders on the result image
            boxDestroy(&box);
        }

        // Add pix to pixa
        pixaAddPix(pixa_b, proc, L_INSERT);
        pixaAddPix(pixa_b, binarized, L_INSERT);
        pixaAddPix(pixa_b, resultImage, L_INSERT);
    }

    Pix* edges1 = pixSobelEdgeFilter(gray, L_HORIZONTAL_EDGES);
    pixWrite("__output/sobelEdge_2_S_H.jpg", edges1, IFF_PNG);

    Pix* edges2 = pixSobelEdgeFilter(gray, L_VERTICAL_EDGES);
    pixWrite("__output/sobelEdge_3_S_V.jpg", edges2, IFF_PNG);

    Pix* edges3 = pixSobelEdgeFilter(gray, L_ALL_EDGES);
    pixWrite("__output/sobelEdge_4_S_A.jpg", edges3, IFF_PNG);

    Pix* edges4 = pixTwoSidedEdgeFilter(gray, L_HORIZONTAL_EDGES);
    pixWrite("__output/sobelEdge_5_TSE_H.jpg", edges4, IFF_PNG);

    Pix* edges5 = pixTwoSidedEdgeFilter(gray, L_VERTICAL_EDGES);
    pixWrite("__output/sobelEdge_5_TSE_V.jpg", edges5, IFF_PNG);

    // Threshold the edge image
    Pix* thresholded = pixThresholdToBinary(edges3, 1);
    pixWrite("__output/thresholded1.jpg", thresholded, IFF_PNG);

    Pix* thresholded1 = pixThresholdToBinary(edges3, 10);
    pixWrite("__output/thresholded10.jpg", thresholded1, IFF_PNG);

    Pix* thresholded2 = pixThresholdToBinary(edges3, 20);
    pixWrite("__output/thresholded20.jpg", thresholded2, IFF_PNG);

    Pix* thresholded3 = pixThresholdToBinary(edges3, 30);
    pixWrite("__output/thresholded30.jpg", thresholded3, IFF_PNG);

    // Apply morphological operations to enhance horizontal lines
    Pix* processed = pixMorphSequence(thresholded2, "e3.3 + c3.3", 0);
    pixWrite("__output/processed.jpg", processed, IFF_PNG);

    std::string filename = "__output/dilate_erode";
    Pix* tiledPix = pixaDisplayTiledInColumns(pixa, 3, 1, 10, 1);
    pixWrite((filename + std::string(".jpg")).c_str(), tiledPix, IFF_PNG);
    std::string filename_b = "__output/open_close";
    Pix* tiledPix_b = pixaDisplayTiledInColumns(pixa_b, 3, 1, 10, 1);
    pixWrite((filename_b + std::string(".jpg")).c_str(), tiledPix_b, IFF_PNG);

}

int main()
{
    /*
	std::cout << "opencv test\n";
	cv::Mat image;
	image = cv::imread("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style1_6.JPG", cv::IMREAD_COLOR);
	cv::namedWindow("Display2", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display2", image);
	cv::waitKey(0);
    

    std::cout << "tesseract test\n";
    char* outText;
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    

    // Open input image with leptonica library
    setLeptDebugOK(1);
    Pix* image = pixRead("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_6.JPG");
    //pixDisplay(image, 0, 0);

    std::cout << "image depth: " << pixGetDepth(image) << std::endl;
    int width = pixGetWidth(image);
    int height = pixGetHeight(image);
    int depth = pixGetDepth(image);

    // Convert input image to grayscale
    Pix* gray = pixConvertRGBToGrayFast(image);

    //pixDisplay(gray, 0, 0);

    // Binarize the grayscale image using a threshold value of 128
    Pix* bin_global = pixThresholdToBinary(gray, 128);


    Pix* ppixth = pixCreate(width, height, 8);
    Pix* ppixd = pixCreate(width, height, 8);
    Pix* ppixd2 = pixCreate(width, height, 8);
    Pix* ppixd3 = pixCreate(width, height, 8);

    Pixa* pixa = pixaCreate(9);

    int sx = 256;
    int sy = sx;

    for (int i = 0; i <= 2; i++)
    {
        pixOtsuAdaptiveThreshold(gray, sx, sy, i, i, 0, &ppixth, &ppixd);
        pixOtsuAdaptiveThreshold(gray, sx, sy, i, i, 0.1, &ppixth, &ppixd2);
        pixOtsuAdaptiveThreshold(gray, sx, sy, i, i, 0.2, &ppixth, &ppixd3);

        pixaAddPix(pixa, ppixd, L_INSERT);
        pixaAddPix(pixa, ppixd2, L_INSERT);
        pixaAddPix(pixa, ppixd3, L_INSERT);

        //pixWrite((filename+std::string("-0.jpg")).c_str(), ppixd, 1);
    }

    std::stringstream ss;
    ss << "Style3_Otsu-" << sx << "-" << sy;
    std::string filename = ss.str();

    Pix* tiledPix = pixaDisplayTiledInColumns(pixa, 3, 1, 10, 1);
    pixWrite((filename + std::string(".jpg")).c_str(), tiledPix, IFF_PNG);


    /*
    pixOtsuAdaptiveThreshold(gray, sx, sy, 0, 0, 0, &ppixth, &ppixd);
    pixOtsuAdaptiveThreshold(gray, sx, sy, 1, 1, 0, &ppixth, &ppixd2);
    pixOtsuAdaptiveThreshold(gray, sx, sy, 2, 2, 0, &ppixth, &ppixd3);
    pixOtsuAdaptiveThreshold(gray, sx, sy, 0, 0, 0.1, &ppixth, &ppixd4);
    pixOtsuAdaptiveThreshold(gray, sx, sy, 1, 1, 0.1, &ppixth, &ppixd5);
    pixOtsuAdaptiveThreshold(gray, sx, sy, 2, 2, 0.1, &ppixth, &ppixd6);
    //pixSauvolaBinarize(gray, 8, 0.2, 1, nullptr, nullptr, nullptr, nullptr);

    //pixDisplayWithTitle(bin_global, 0, 0, "Global", 1);
    pixWrite("gray.jpg", gray, 1);
    //pixWrite("Otsu-th", gray, 1);
    pixWrite("Otsu-24-24-0-0-0.jpg", ppixd, 1);
    pixWrite("Otsu-24-24-1-1-0.jpg", ppixd2, 1);
    pixWrite("Otsu-24-24-2-2-0.jpg", ppixd3, 1);
    pixWrite("Otsu-24-24-1-1-01.jpg", ppixd4, 1);
    pixWrite("Otsu-24-24-1-1-02.jpg", ppixd5, 1);

    std::cout << "image depth: " << pixGetDepth(gray) << std::endl;
    */

    /*
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    delete api;
    delete[] outText;
    pixDestroy(&image);
    */
    
    //std::filesystem::create_directory("__output");
    //attempt2("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_6.JPG");
    //saveOtsuBinarizedImg("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_6.JPG", 20);
    //edgeDetection("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_6.JPG");

    Importer i;


	return 0;

}


void convertImageToText(std::string imgPath)
{

    // tesseract must be initilized
    // load image
    // otsu threshold
    // morphological shit (erode, dilate, close)
    // box blobs (return coordinates)
    // snip img
    // do ocr on snips






}

Importer::Importer()
{
    InitilizeTesseract();


    Logger::getInstance(Logger::DEBUG);
    auto a = ExtractWishesFromImage("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_5.JPG");
    for (auto& wish : a) {
        log(std::format("Wish - itemType: {}, itemName: {}, date: {}, rarity: {}", wish.itemType, wish.itemName, wish.date, wish.itemRarity));
    }
    a = ExtractWishesFromImage("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_6.JPG");
    for (auto& wish : a) {
        log(std::format("Wish - itemType: {}, itemName: {}, date: {}, rarity: {}", wish.itemType, wish.itemName, wish.date, wish.itemRarity));
    }
}

Importer::~Importer()
{
    CleanupTesseract();
}

void Importer::InitilizeTesseract()
{
    std::lock_guard<std::mutex> lock(m_tesseractApiMutex);
    m_tesseractApi = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (m_tesseractApi->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
}

void Importer::CleanupTesseract()
{
    std::lock_guard<std::mutex> lock(m_tesseractApiMutex);
    m_tesseractApi->End();
    delete m_tesseractApi;
}

std::string Importer::ExtractTextFromPix(Pix* image)
{
    std::lock_guard<std::mutex> lock(m_tesseractApiMutex);
    // OCR
    m_tesseractApi->SetImage(image);
    m_tesseractOutputText = m_tesseractApi->GetUTF8Text();

    // Save to std::string and remove special characters
    std::string output(m_tesseractOutputText ? m_tesseractOutputText : "");
    StripString(output);
    log(std::format("Extracted text: {}", output), Logger::DEBUG);

    if (m_tesseractOutputText) {
        delete[] m_tesseractOutputText;
    }

    return output;
}

unsigned int Importer::ExtractRarityFromText(std::string& itemName)
{
    if (itemName.find("4-Star") != std::string::npos) {
        return 4u;
    }
    else if (itemName.find("5-Star") != std::string::npos) {
        return 5u;
    }
    else {
        return 3u;
    }
}

std::vector<wishEntry> Importer::ExtractWishFromPixPos(const PixPos& pp)
{
    return std::vector<wishEntry>();
}

std::vector<wishEntry> Importer::ExtractWishesFromImage(const std::string& imgPath)
{
    ImporterItem item(imgPath);
    auto& vec = item.GetTextSnippets();

    // format vec into packets of 3 items
    auto pixposVec = combinePixPos(vec);

    std::vector<wishEntry> wishVec;

    // ocr into single wish
    for (auto& pixposPacket : pixposVec) {
        std::string itemType = ExtractTextFromPix(pixposPacket[0].pix);
        std::string itemName = ExtractTextFromPix(pixposPacket[1].pix);
        std::string date = ExtractTextFromPix(pixposPacket[2].pix);
        wishEntry wish{ itemType, itemName, date, ExtractRarityFromText(itemName) };
        wishVec.emplace_back(wish);
    }

    // remove header, if there is one (copy first element, lowercase it, check)
    std::string first = wishVec[0].itemType;
    std::transform(first.begin(), first.end(), first.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (first == "item type") {
        wishVec.erase(wishVec.begin());
    }

    return wishVec;
}

std::vector<std::vector<PixPos>> Importer::combinePixPos(std::vector<PixPos>& ppVec)
{
    std::vector<std::vector<PixPos>> retVec;
    if (ppVec.size() % 4 == 0) {
        // new-style wish
        for (int i = 0; i < ppVec.size(); i += 4) {
            std::vector<PixPos> temp{ ppVec[i], ppVec[i + 1], ppVec[i + 3] };
            retVec.emplace_back(temp);
        }
    }
    else if (ppVec.size() % 3 == 0) {
        // old-style wish
    }
    else {
        // something went wrong
    }


    return retVec;
}

void Importer::StripString(std::string& str)
{
    // This function is to remove special characters that tesseract found (like newline, tab)
    std::string result{};
    result.reserve(str.size()); // Reserve enough space to avoid multiple reallocations

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\n') {
            if (i > 0 && str[i - 1] == '-') {
                // Skip the newline
                continue;
            }
            else {
                // Replace newline with space
                result += ' ';
            }
        }
        else if (str[i] == '\t' || str[i] == '\r') {
            // Skip tabs and carriage returns
            continue;
        }
        else {
            // Copy other characters
            result += str[i];
        }
    }

    // Trim trailing space if exists
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    // Replace the original string with the result
    str = std::move(result);
}

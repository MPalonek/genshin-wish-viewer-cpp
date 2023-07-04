#include "importer.h"
#include "logger/src/logger.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

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
    ss << "fixme" << "_Otsu-" << sx << "-" << sy;
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

void edgeDetection(std::string_view imgPath)
{
    // Load image
    std::unique_ptr<Pix*> image = std::make_unique<Pix*>(pixRead(imgPath.data()));
    //Pix* image = pixRead(imgPath.data());
    pixWrite("sobelEdge_0.jpg", *image, IFF_PNG);

    Pix* gray = pixConvertRGBToGrayFast(*image);
    pixWrite("sobelEdge_1.jpg", gray, IFF_PNG);

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
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1, &ppixth, &binarized);

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
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1, &ppixth, &binarized);

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
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1, &ppixth, &binarized);

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
        pixOtsuAdaptiveThreshold(proc, 32, 32, 1, 1, 0.1, &ppixth, &binarized);

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
    pixWrite("sobelEdge_2_S_H.jpg", edges1, IFF_PNG);

    Pix* edges2 = pixSobelEdgeFilter(gray, L_VERTICAL_EDGES);
    pixWrite("sobelEdge_3_S_V.jpg", edges2, IFF_PNG);

    Pix* edges3 = pixSobelEdgeFilter(gray, L_ALL_EDGES);
    pixWrite("sobelEdge_4_S_A.jpg", edges3, IFF_PNG);

    Pix* edges4 = pixTwoSidedEdgeFilter(gray, L_HORIZONTAL_EDGES);
    pixWrite("sobelEdge_5_TSE_H.jpg", edges4, IFF_PNG);

    Pix* edges5 = pixTwoSidedEdgeFilter(gray, L_VERTICAL_EDGES);
    pixWrite("sobelEdge_5_TSE_V.jpg", edges5, IFF_PNG);

    // Threshold the edge image
    Pix* thresholded = pixThresholdToBinary(edges3, 1);
    pixWrite("thresholded1.jpg", thresholded, IFF_PNG);

    Pix* thresholded1 = pixThresholdToBinary(edges3, 10);
    pixWrite("thresholded10.jpg", thresholded1, IFF_PNG);

    Pix* thresholded2 = pixThresholdToBinary(edges3, 20);
    pixWrite("thresholded20.jpg", thresholded2, IFF_PNG);

    Pix* thresholded3 = pixThresholdToBinary(edges3, 30);
    pixWrite("thresholded30.jpg", thresholded3, IFF_PNG);

    // Apply morphological operations to enhance horizontal lines
    Pix* processed = pixMorphSequence(thresholded2, "e3.3 + c3.3", 0);
    pixWrite("processed.jpg", processed, IFF_PNG);

    std::string filename = "dilate_erode";
    Pix* tiledPix = pixaDisplayTiledInColumns(pixa, 3, 1, 10, 1);
    pixWrite((filename + std::string(".jpg")).c_str(), tiledPix, IFF_PNG);
    std::string filename_b = "open_close";
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
    
    saveOtsuBinarizedImg("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_6.JPG", 32);
    edgeDetection("D:/Repo/c++/genshin-wish-viewer-cpp/importer/importerTest/img/Style3_6.JPG");


	return 0;

}
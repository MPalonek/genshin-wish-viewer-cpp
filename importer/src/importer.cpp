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
	return 0;

}
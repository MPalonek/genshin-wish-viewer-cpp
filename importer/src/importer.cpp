#include "importer.h"
#include "logger/src/logger.h"
#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <filesystem>


// https://tpgit.github.io/Leptonica

Importer::Importer()
{
    InitilizeTesseract();
}

Importer::~Importer()
{
    CleanupTesseract();
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
        unsigned int rarity = ExtractRarityFromText(itemName);
        wishEntry wish{ itemType, itemName, date, rarity };
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

std::vector<std::vector<PixPos>> Importer::combinePixPos(std::vector<PixPos>& ppVec)
{
    std::vector<std::vector<PixPos>> retVec;
    if (ppVec.size() % 4 == 0) {
        // new-style wish
        for (int i = 0; i < ppVec.size(); i += 4) {
            std::vector<PixPos> temp;
            temp.emplace_back(std::move(ppVec[i]));
            temp.emplace_back(std::move(ppVec[i + 1]));
            temp.emplace_back(std::move(ppVec[i + 3]));
            retVec.emplace_back(std::move(temp));
        }
    }
    else if (ppVec.size() % 3 == 0) {
        // old-style wish
        for (int i = 0; i < ppVec.size(); i += 3) {
            std::vector<PixPos> temp;
            temp.emplace_back(std::move(ppVec[i]));
            temp.emplace_back(std::move(ppVec[i + 1]));
            temp.emplace_back(std::move(ppVec[i + 2]));
            retVec.emplace_back(std::move(temp));
        }
    }
    else {
        // something went wrong
        log(std::format("combinePixPos failed - ppVec size: {}", ppVec.size()), Logger::ERROR);
    }
    return retVec;
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
    size_t pos = itemName.find(" 4");
    if (pos != std::string::npos) {
        itemName = itemName.substr(0, pos);
        return 4u;
    }

    pos = itemName.find(" 5");
    if (pos != std::string::npos) {
        itemName = itemName.substr(0, pos);
        return 5u;
    }

    return 3u;
}

void Importer::StripString(std::string& str)
{
    // This function is to remove special characters that tesseract found (like newline, tab)
    std::string result{};
    result.reserve(str.size()); // Reserve enough space to avoid multiple reallocations

    for (size_t i = 0; i < str.size(); ++i) {
        char ch = str[i];

        // Check if character is an ASCII letter/digit or hpyhen/colon 
        if (std::isalnum(ch) || ch == '-' || ch == ':') {
            result += ch;
        }
        // Space handling (only allow one space and newline treatment)
        else if (ch == ' ' || ch == '\n') {
            if (!result.empty() && result.back() != ' ') {
                result += ' ';
            }
        }
        // Allow apostrophe or right single quotation mark first char
        else if (ch == '\'' || (ch == '\xE2')) {
            result += '\'';
        }
        // Skip any other characters (tabs, punctuation, etc.)
    }

    // Trim trailing space if exists
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    // Replace the original string with the result
    str = std::move(result);
}

void Importer::ReplaceWeirdCharsInString(std::string& str)
{
    // Replace right single quotation mark char into regular ASCII apostrophe
    size_t pos;
    while ((pos = str.find("’")) != std::string::npos) {
        str.replace(pos, 3, "'");
    }
}
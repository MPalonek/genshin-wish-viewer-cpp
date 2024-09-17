#ifndef IMPORTER_H
#define IMPORTER_H

#include <string>
#include "importerItem.h"
#include <tesseract/baseapi.h>

struct wishEntry
{
	/*
	* https://github.com/jvergerolle/Genshin-Impact-Wish-history-API
	JSON EXAMPLE
	{
		"uid": "700000000",
		"gacha_type": "301",  // 100 = begginer banner, 200 = permanent banner, 301 = character event banner, 302 = weapon event banner, 400 = character event banner 2
		"item_id": "",
		"count": "1",
		"time": "2021-11-24 22:18:45",
		"name": "Albedo",
		"lang": "en-us",
		"item_type": "Character",
		"rank_type": "5",
		"id": "1637787960000243756"
	},
	*/
	std::string itemType;
	std::string itemName;
	std::string date;
	unsigned int itemRarity;

	wishEntry(std::string t, std::string n, std::string d, unsigned int r) : itemType(t), itemName(n), date(d), itemRarity(r) {}
};

class Importer
{
public:
	Importer();
	~Importer();

	void InitilizeTesseract();
	void CleanupTesseract();
	std::string ExtractTextFromPix(Pix* image);
	std::vector<wishEntry> ExtractWishFromPixPos(const PixPos& pp);

	std::vector<wishEntry> ExtractWishesFromImage(const std::string& imgPath);
	unsigned int ExtractRarityFromText(std::string& itemName);

	std::vector<std::vector<PixPos>> combinePixPos(std::vector<PixPos>& ppVec);



private:
	std::mutex m_tesseractApiMutex;
	tesseract::TessBaseAPI* m_tesseractApi;
	char* m_tesseractOutputText;

	void StripString(std::string& str);

	void log(const std::string& log, Logger::logLevel level = Logger::INFO) {
		Logger::getInstance().addLog("Importer", log, level);
	}

};
void saveOtsuBinarizedImg(std::string filename, int matrixSize);

#endif /* IMPORTER_H */
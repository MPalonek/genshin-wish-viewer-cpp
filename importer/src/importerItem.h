#ifndef IMPORTERITEM_H
#define IMPORTERITEM_H

#include <string>
#include <leptonica/allheaders.h>
#include "logger/src/logger.h"

struct Position
{
	int x;
	int y;
	int w;
	int h;

	Position() : x(0), y(0), w(0), h(0) {}
	Position(const Position& pos_) : x(pos_.x), y(pos_.y), w(pos_.w), h(pos_.h) {}
	Position(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_) {}
	Box* CreateBox() { return boxCreate(x, y, w, h); } // you have to do boxDestroy() after!
};

struct PixPos
{
	Pix* pix;
	Position pos;
	
	PixPos() : pix(NULL), pos(0, 0, 0, 0) {}
	PixPos(Pix* pix_, Position pos_) : pix(pix_), pos(pos_) {}
};

class ImporterItem
{
public:
	ImporterItem(std::string fileLocation);
	~ImporterItem();

	std::vector<PixPos>& GetTextSnippets();

private:
	void LoadGrayscaleImage();
	void BinarizeImage();
	void ProcessImage();
	void FindTextPositons();
	void ValidateAndReorganizeBoxPix();

	// Functions for debugging
	void SaveImageAsCsv();
	void DisplayImageWithBoxes();

	std::string m_imageLocation;
	Pix* m_image{ NULL };
	Pix* m_binaryImage{ NULL };
	Pix* m_blobsBinaryImage{ NULL };
	int m_width{ 0 };
	int m_height{ 0 };
	int m_depth{ 0 };
	std::vector<PixPos> m_textPixPos;

	void log(const std::string& log, Logger::logLevel level = Logger::INFO) {
		Logger::getInstance().addLog("ImporterItem", log, level);
	}

};

#endif /* IMPORTERITEM_H */
#ifndef _BMPIMAGE_H_
#define _BMPIMAGE_H_

#include <iostream>
#include <fstream>
#include "Maze.h"
using namespace std;

class BMPImage {
public:
	static const int HEADER_LENGTH = 54;
	static const int COLOR_TABLE_LENGTH = 1024;

private:
	char *header = nullptr, *colorTable = nullptr;
	int width, height, bitDepth, size, offset, gap1Length, gap2Length, rowSize;
	char *data = nullptr, *gap1 = nullptr, *gap2 = nullptr;
public:
	BMPImage() {
	}
	BMPImage(int w, int h, char *d);
	BMPImage(const Maze &m) : BMPImage(m.getWidth(), m.getHeight(), m.convertToPixelData()) {}
	BMPImage(const BMPImage &img) {
		copy(img);
	}
	BMPImage(BMPImage &&img) {
		move(img);
	}
	~BMPImage() {
		deleteImg();
	}

	BMPImage& operator=(const BMPImage &img) {
		if (this != &img) {
			deleteImg();
			copy(img);
		}
		return *this;
	}
	BMPImage& operator=(BMPImage &&img) {
		if (this != &img) {
			deleteImg();
			move(img);
		}
		return *this;
	}

	friend istream& operator>>(istream &is, BMPImage &img);
	friend ostream& operator<<(ostream &os, const BMPImage &img);

	Maze convertToMaze() const;

	void exportAsBMP(string filename);
	void importFromBMP(string filename);
private:
	void deleteImg();
	void copy(const BMPImage &img);
	void move(BMPImage &img);
};

#endif
#include "BMPImage.h"

//int BMPImage::HEADER_LENGTH = 54;
//int BMPImage::COLOR_TABLE_LENGTH = 1024;

BMPImage::BMPImage(int w, int h, char* d) {
	width = w;
	height = h;
	bitDepth = 24;	// rgb format
	gap1Length = gap2Length = 0;

	rowSize = (bitDepth * width + 31) / 32 * 4;
	size = rowSize * height;

	header = new char[54];
	memset(header, 0, 54);
	header[0] = 'B';
	header[1] = 'M';
	*(int*) &header[2] = 54 + size;	// total size of an image
	*(int*) &header[10] = 54;		// header size
	*(int*) &header[14] = 40;		// info header size
	*(int*) &header[18] = w;		// width in pixels
	*(int*) &header[22] = h;		// height in pixels
	*(short*) &header[26] = 1;		// planes, must be 1
	*(short*) &header[28] = 24;		// bitdepth
	*(int*) &header[38] = 2835;		// the horizontal resolution of the image. (pixel per metre, signed integer)
	*(int*) &header[42] = 2835;		// the vertical resolution of the image. (pixel per metre, signed integer)

	data = new char[size];
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {	// actual data
			data[i * rowSize + j * 3 + 0] = d[i * width * 3 + j * 3 + 0];
			data[i * rowSize + j * 3 + 1] = d[i * width * 3 + j * 3 + 1];
			data[i * rowSize + j * 3 + 2] = d[i * width * 3 + j * 3 + 2];
		}
		for (j = 3 * width; j < rowSize; j++) {	// padding
			data[i * rowSize + j] = 0;
		}
	}
}

Maze BMPImage::convertToMaze() {
	int w = width, h = height, i, j;
	char r, g, b;
	char **maze = new char*[h];
	for (i = 0; i < h; i++) {
		maze[i] = new char[w];
	}
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			b = data[(h - i - 1) * rowSize + j * 3 + 0];
			g = data[(h - i - 1) * rowSize + j * 3 + 1];
			r = data[(h - i - 1) * rowSize + j * 3 + 2];
			maze[i][j] = r * g * b ? 0 : 1;
		}
	}
	return Maze(w, h, maze);
}

void BMPImage::deleteImg() {
	delete data;
	delete gap1;
	delete gap2;
	data = gap1 = gap2 = nullptr;
}

void BMPImage::copy(const BMPImage & img) {
	width = img.width;
	height = img.height;
	bitDepth = img.bitDepth;
	size = img.size;
	offset = img.offset;
	gap1Length = img.gap1Length;
	gap2Length = img.gap2Length;
	rowSize = img.rowSize;

	int i;
	header = new char[HEADER_LENGTH];
	for (i = 0; i < HEADER_LENGTH; i++)
		header[i] = img.header[i];
	if (img.colorTable) {
		colorTable = new char[COLOR_TABLE_LENGTH];
		for (i = 0; i < COLOR_TABLE_LENGTH; i++)
			colorTable[i] = img.colorTable[i];
	}
	data = new char[size];
	for (i = 0; i < size; i++)
		data[i] = img.data[i];
	if (gap1Length) {
		gap1 = new char[gap1Length];
		for (i = 0; i < gap1Length; i++)
			gap1[i] = img.gap1[i];
	}
	if (gap2Length) {
		gap2 = new char[gap2Length];
		for (i = 0; i < gap2Length; i++)
			gap2[i] = img.gap2[i];
	}
}

void BMPImage::move(BMPImage & img) {
	width = img.width;
	height = img.height;
	bitDepth = img.bitDepth;
	size = img.size;
	offset = img.offset;
	gap1Length = img.gap1Length;
	gap2Length = img.gap2Length;
	rowSize = img.rowSize;

	header = img.header;
	img.header = nullptr;
	colorTable = img.colorTable;
	img.colorTable = nullptr;
	data = img.data;
	img.data = nullptr;
	gap1 = img.gap1;
	img.gap1 = nullptr;
	gap2 = img.gap2;
	img.gap2 = nullptr;
}

istream & operator>>(istream & is, BMPImage & img) {
	img.deleteImg();

	img.header = new char[BMPImage::HEADER_LENGTH];
	is.read(img.header, sizeof(char) * BMPImage::HEADER_LENGTH);

	img.width = *(int*) &img.header[18];
	img.height = *(int*) &img.header[22];
	img.bitDepth = *(int*) &img.header[28];
	img.offset = *(int*) &img.header[10];

	//img.size = img.width * img.height;
	//cout << "original size: " << img.size << endl;
	img.rowSize = (img.bitDepth * img.width + 31) / 32 * 4;	// including padding
	//cout << "calculated size: " << img.rowSize * img.height << endl;
	img.size = img.rowSize * img.height;	// not necessarily width*height, because of padding

	if (img.bitDepth <= 8) {
		img.colorTable = new char[BMPImage::COLOR_TABLE_LENGTH];
		is.read(img.colorTable, BMPImage::COLOR_TABLE_LENGTH);
	} else
		img.size *= img.bitDepth / 8;

	img.gap1Length = 0;
	img.gap1 = nullptr;
	char c;
	while (is.tellg() < img.offset) {
		c = is.get();
		char *tmp = new char[img.gap1Length + 1];
		if (img.gap1)
			strcpy(tmp, img.gap1);
		delete img.gap1;
		img.gap1 = tmp;
		img.gap1[img.gap1Length++] = c;
	}

	img.data = new char[img.size * sizeof(char)];
	is.read(img.data, sizeof(char) * img.size);

	img.gap2Length = 0;
	img.gap2 = nullptr;
	while ((c = is.get()) != EOF) {
		char *tmp = new char[img.gap2Length + 1];
		strcpy(tmp, img.gap2);
		delete img.gap2;
		img.gap2 = tmp;
		img.gap2[img.gap2Length++] = c;
	}

	is.clear();
	is.seekg(0, is.beg);

	return is;
}

ostream & operator<<(ostream & os, const BMPImage & img) {
	os.write(img.header, sizeof(char) * BMPImage::HEADER_LENGTH);
	if (img.bitDepth <= 8)
		os.write(img.colorTable, sizeof(char) * BMPImage::COLOR_TABLE_LENGTH);
	if (img.gap1Length)
		os.write(img.gap1, sizeof(char) * img.gap1Length);
	os.write(img.data, sizeof(char) * img.size);
	if (img.gap2Length)
		os.write(img.gap2, sizeof(char) * img.gap2Length);
	return os;
}

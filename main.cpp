#include "BMPImage.h"
#include <ctime>


int main() {

	clock_t t1, t2;
	double d;
	t1 = clock();

	srand(time(nullptr));

	/*BMPImage *i1 = new BMPImage();

	ifstream in1("test.bmp", ios_base::binary);
	if (!in1.is_open()) exit(1);
	in1 >> *i1;
	in1.close();
	//cout << *i1;

	ofstream out1("test_cpy.bmp", ios_base::binary);
	if (!out1.is_open()) exit(2);
	out1 << *i1;
	out1.close();*/

	/*BMPImage *i2 = new BMPImage(*i1);
	BMPImage *i3 = new BMPImage(), *i4 = new BMPImage();
	*i3 = *i1;
	*i4 = BMPImage(*i1);

	ofstream out2("test_cpy2.bmp", ios_base::binary);
	if (!out2.is_open()) exit(2);
	out2 << *i2;
	out2.close();

	ofstream out3("test_cpy3.bmp", ios_base::binary);
	if (!out3.is_open()) exit(2);
	out3 << *i3;
	out3.close();

	ofstream out4("test_cpy4.bmp", ios_base::binary);
	if (!out4.is_open()) exit(2);
	out4 << *i4;
	out4.close();*/

	/*Maze m;
	cin >> m;
	cout << "\nM1:\n" << m << endl;;
	char *data = m.convertToPixelData();

	BMPImage *test = new BMPImage(m.getWidth(), m.getHeight(), data);
	ofstream out("out.bmp", ios_base::binary);
	if (!out.is_open()) exit(2);
	out << *test;
	out.close();

	Maze m2 = test->convertToMaze();
	cout << "\nM2:\n" << m2 << '\n';*/

	/*BMPImage *i = new BMPImage();

	ifstream in("braid200.bmp", ios_base::binary);
	if (!in.is_open()) exit(2);
	in >> *i;
	in.close(); 

	Maze m = i->convertToMaze();
	//cout << m;*/

	Maze m = Maze::generateMaze(501, 501, 10);
	//cout << m;

	BMPImage *test = new BMPImage(m.getWidth(), m.getHeight(), m.convertToPixelData());
	ofstream out("out.bmp", ios_base::binary);
	if (!out.is_open()) exit(2);
	out << *test;
	out.close();

	t2 = clock();
	d = (double) (t2 - t1) / CLOCKS_PER_SEC * 1000;
	cout << "Time elapsed: " << d << "ms" << endl;
}

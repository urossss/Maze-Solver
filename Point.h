#ifndef _POINT_H_
#define _POINT_H_

#include <iostream>
using namespace std;

struct Point {
	int y = 0, x = 1;

	friend ostream& operator<<(ostream& os, const Point &p) {
		return os << "(" << p.y << ", " << p.x << ")";
	}

	int manhattanDistance(const Point &p) const {
		return abs(p.x - x) + abs(p.y - y);
	}
};

#endif
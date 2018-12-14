#ifndef _POINT_H_
#define _POINT_H_

#include <iostream>

struct Point {
	int y = 0, x = 1;

	friend ostream& operator<<(ostream& os, const Point &p) {
		return os << "(" << p.y << ", " << p.x << ")";
	}
};

#endif
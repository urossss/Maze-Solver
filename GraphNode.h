#ifndef _GRAPHNODE_H_
#define _GRAPHNODE_H_


#include "Point.h"

class GraphNode {
	friend class MazeGraph;

	GraphNode *up, *down, *left, *right;
	Point pos;
	bool visit;
public:
	GraphNode(int y, int x) : pos({ y, x }) {
		up = down = left = right = nullptr;
		visit = false;
	}
	~GraphNode() {}
};

#endif
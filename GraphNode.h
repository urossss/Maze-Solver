#ifndef _GRAPHNODE_H_
#define _GRAPHNODE_H_


#include "Point.h"

class GraphNode {
	friend class MazeGraph;

	GraphNode *up, *down, *left, *right;
	GraphNode *prev;
	Point pos;
	bool visited;
public:
	GraphNode(int y, int x) : pos({ y, x }) {
		up = down = left = right = prev =  nullptr;
		visited = false;
	}
	~GraphNode() {}
};

#endif
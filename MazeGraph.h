#ifndef _MAZEGRAPH_H_
#define _MAZEGRAPH_H_


#include "GraphNode.h"
#include "Maze.h"
#include <list>
using namespace std;

class MazeGraph {

	GraphNode *start;
	list<GraphNode*> exit;
	list<GraphNode*> nodes;

	int nodeCount = 0;

	MazeGraph() {}
	MazeGraph(const MazeGraph &) = delete;
public:
	~MazeGraph();

	static MazeGraph* createGraph(const Maze &m);

	list<Point> solveBFS();
	list<Point> solveDFS();

	void printNodes() const {
		for (GraphNode *node : nodes) {
			cout << node->pos << endl;
		}
	}
};

#endif
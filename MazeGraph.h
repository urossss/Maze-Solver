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

	enum HeuristicType {
		NONE, STRAIGHT_LINE
	};
	list<Point> solveDijkstraWithHeuristic(HeuristicType);

public:
	~MazeGraph();

	static MazeGraph* createGraph(const Maze &m);

	list<Point> solveBFS();
	list<Point> solveDFS();
	list<Point> solveDijkstra();
	list<Point> solveAStar();

	void printNodes() const {
		for (GraphNode *node : nodes) {
			cout << node->pos << endl;
		}
	}
};

#endif
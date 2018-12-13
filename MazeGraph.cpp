#include "MazeGraph.h"


MazeGraph* MazeGraph::createGraph(const Maze & m) {
	int x, y;
	MazeGraph *g = new MazeGraph();
	GraphNode *node = nullptr, *leftNode = nullptr;
	GraphNode **topNodes = new GraphNode*[m.width];
	for (x = 0; x < m.width; x++)
		topNodes[x] = nullptr;

	for (x = 1; x < m.width; x++) {	// start row
		if (m.maze[0][x] == 0) {
			node = new GraphNode(0, x);
			break;
		}
	}
	if (x == m.width) {
		delete g;
		return nullptr;
	}

	g->start = node;
	g->nodes.push_back(node);
	g->nodeCount = 1;

	for (y = 1; y < m.height - 1; y++) {
		bool prev = false, curr = false, next = !m.maze[y][1];	// wall = false, path = true
		leftNode = nullptr;

		for (x = 1; x < m.width; x++) {
			prev = curr;
			curr = next;
			next = !m.maze[y][x + 1];

			node = nullptr;

			if (!curr)	// wall
				continue;
			if (prev) {
				if (next) {	// path path path
					if (!m.maze[y - 1][x] || !m.maze[y + 1][x]) {
						node = new GraphNode(y, x);	// node if at junction
						if (leftNode)
							leftNode->right = node;
						node->left = leftNode;
						leftNode = node;
					}
				} else {	// path path wall
					node = new GraphNode(y, x);	// node at the end of the corridor
					if (leftNode)
						leftNode->right = node;
					node->left = leftNode;
					leftNode = nullptr;
				}
			} else {
				if (next) {	// wall path path
					node = new GraphNode(y, x);	// node at the start of the corridor
					leftNode = node;
				} else {	// wall path wall
					if (m.maze[y - 1][x] || m.maze[y + 1][x])
						node = new GraphNode(y, x);	// node in dead end
				}
			}

			if (node) {	// created a new node, now try to connect it up and down
				g->nodes.push_back(node);
				g->nodeCount++;

				if (!m.maze[y - 1][x]) {	// if path above, connect to node above
					node->up = topNodes[x];
					if (topNodes[x])
						topNodes[x]->down = node;
				}

				if (!m.maze[y + -1][x]) {	// if path below, put this one in topNodes for new connection below
					topNodes[x] = node;
				} else {
					topNodes[x] = nullptr;
				}
			}
		}
	}

	for (x = 1; x < m.width - 1; x++) {	// end row
		if (!m.maze[m.height - 1][x]) {
			node = new GraphNode(m.height - 1, x);
			if (topNodes[x])
				topNodes[x]->down = node;
			node->up = topNodes[x];

			g->nodes.push_back(node);
			g->exit.push_back(node);
			g->nodeCount++;
		}
	}

	delete[] topNodes;

	cout << g->nodeCount << endl;

	return g;
}

MazeGraph::~MazeGraph() {
	for (GraphNode *node : nodes) {
		delete node;
	}
	nodes.clear();
	exit.clear();
}

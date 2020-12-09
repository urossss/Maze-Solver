#include "MazeGraph.h"
#include <queue>
#include <stack>
#include <ctime>
#include <unordered_map>
#include <vector>


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
	topNodes[node->pos.x] = node;
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

				if (!m.maze[y + 1][x]) {	// if path below, put this one in topNodes for new connection below
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

	cout << "Number of nodes: " << g->nodeCount << endl;

	return g;
}

list<Point> MazeGraph::solveBFS() {
	list<Point> sol;

	queue<GraphNode*> q;
	q.push(start);

	bool solved = false;
	GraphNode* curr = nullptr;
	int visited = 0;
	while (!q.empty()) {
		curr = q.front();

		if (!curr->visited) {
			curr->visited = true;
			visited++;
		}

		for (GraphNode *pnode : exit) {
			if (pnode == curr) {
				solved = true;
				break;
			}
		}

		if (solved)
			break;

		if (curr->down && !curr->down->visited) {
			curr->down->prev = curr;
			curr->down->visited = true;
			visited++;
			q.push(curr->down);
		}
		if (curr->right && !curr->right->visited) {
			curr->right->prev = curr;
			curr->right->visited = true;
			visited++;
			q.push(curr->right);
		}
		if (curr->left && !curr->left->visited) {
			curr->left->prev = curr;
			curr->left->visited = true;
			visited++;
			q.push(curr->left);
		}
		if (curr->up && !curr->up->visited) {
			curr->up->prev = curr;
			curr->up->visited = true;
			visited++;
			q.push(curr->up);
		}

		q.pop();
	}

	if (!solved) {
		cout << "Maze is not solvable.\n";
		return sol;
	}

	int length = 0;
	while (curr) {
		sol.push_front(Point(curr->pos));
		if (curr->prev) {
			length += curr->pos.manhattanDistance(curr->prev->pos);
		}
		curr = curr->prev;
	}

	cout << "Nodes visited: " << visited << endl;
	cout << "Path node count: " << sol.size() << endl;
	cout << "Path length: " << length << endl;

	for (GraphNode *node : nodes) {
		node->visited = false;
	}

	return sol;
}

list<Point> MazeGraph::solveDFS() {
	list<Point> sol;

	stack<GraphNode*> s;
	s.push(start);

	bool solved = false;
	GraphNode* curr = nullptr;
	int visited = 0;
	while (!s.empty()) {
		curr = s.top();
		if (!curr->visited) {
			curr->visited = true;
			visited++;

			for (GraphNode *pnode : exit) {
				if (pnode == curr) {
					solved = true;
					break;
				}
			}
		}

		if (solved)
			break;

		if (curr->down && !curr->down->visited) {
			curr->down->prev = curr;
			s.push(curr->down);
			continue;
		}
		if (curr->right && !curr->right->visited) {
			curr->right->prev = curr;
			s.push(curr->right);
			continue;
		}
		if (curr->left && !curr->left->visited) {
			curr->left->prev = curr;
			s.push(curr->left);
			continue;
		}
		if (curr->up && !curr->up->visited) {
			curr->up->prev = curr;
			s.push(curr->up);
			continue;
		}
		s.pop();
	}

	if (!solved) {
		cout << "Maze is not solvable.\n";
		return sol;
	}

	int length = 0;
	while (curr) {
		sol.push_front(Point(curr->pos));
		if (curr->prev) {
			length += curr->pos.manhattanDistance(curr->prev->pos);
		}
		curr = curr->prev;
	}

	cout << "Nodes visited: " << visited << endl;
	cout << "Path node count: " << sol.size() << endl;
	cout << "Path length: " << length << endl;

	//clock_t t1 = clock();
	for (GraphNode *node : nodes) {
		node->visited = false;
	}
	//clock_t t2 = clock();
	//double d = (double) (t2 - t1) / CLOCKS_PER_SEC;
	//cout << "Time elapsed for reseting: " << d << "s" << endl;

	return sol;
}

list<Point> MazeGraph::solveDijkstra() {
	list<Point> sol;

	unordered_map<GraphNode*, int> dist;
	unordered_map<GraphNode*, vector<GraphNode*>> path;
	for (GraphNode *node : this->nodes) {
		dist[node] = INT_MAX;
	}
	dist[this->start] = 0;
	path[this->start] = { this->start };

	GraphNode *end = nullptr;
	int visited = 0;

	for (int i = 0; i < this->nodeCount; i++) {
		GraphNode *curr = nullptr;
		for (GraphNode *node : this->nodes) {
			if (!node->visited && (!curr || dist[node] < dist[curr])) {
				curr = node;
			}
		}

		for (GraphNode *pnode : exit) {
			if (pnode == curr) {
				end = curr;
				break;
			}
		}

		curr->visited = true;
		visited++;

		if (curr->down && !curr->down->visited) {
			int d = curr->pos.manhattanDistance(curr->down->pos);
			if (dist[curr] + d < dist[curr->down]) {
				dist[curr->down] = dist[curr] + d;
				path[curr->down] = path[curr];
				path[curr->down].push_back(curr->down);
			}
		}
		if (curr->right && !curr->right->visited) {
			int d = curr->pos.manhattanDistance(curr->right->pos);
			if (dist[curr] + d < dist[curr->right]) {
				dist[curr->right] = dist[curr] + d;
				path[curr->right] = path[curr];
				path[curr->right].push_back(curr->right);
			}
		}
		if (curr->left && !curr->left->visited) {
			int d = curr->pos.manhattanDistance(curr->left->pos);
			if (dist[curr] + d < dist[curr->left]) {
				dist[curr->left] = dist[curr] + d;
				path[curr->left] = path[curr];
				path[curr->left].push_back(curr->left);
			}
		}
		if (curr->up && !curr->up->visited) {
			int d = curr->pos.manhattanDistance(curr->up->pos);
			if (dist[curr] + d < dist[curr->up]) {
				dist[curr->up] = dist[curr] + d;
				path[curr->up] = path[curr];
				path[curr->up].push_back(curr->up);
			}
		}
	}

	if (!end) {
		cout << "Maze is not solvable.\n";
		return sol;
	}

	GraphNode *prev = nullptr;
	int length = 0;
	for (GraphNode *node : path[end]) {
		sol.push_back(Point(node->pos));
		if (prev) {
			length += node->pos.manhattanDistance(prev->pos);
		}
		prev = node;
	}

	cout << "Nodes visited: " << visited << endl;
	cout << "Path node count: " << sol.size() << endl;
	cout << "Path length: " << length << endl;

	for (GraphNode *node : nodes) {
		node->visited = false;
	}

	return sol;
}

MazeGraph::~MazeGraph() {
	for (GraphNode *node : nodes) {
		delete node;
	}
	nodes.clear();
	exit.clear();
}

#include "Maze.h"
#include <stack>
#include <algorithm>


Maze Maze::generateMaze(int w, int h) {
	return Maze::generateMaze(w, h, 1);
}

Maze Maze::generateMaze(int w, int h, int e) {
	Maze m;
	int i, j;
	stack<Point> s;

	m.width = w > 5 ? w : 5;
	m.height = h > 5 ? h : 5;

	m.start = { 0, 1 };
	m.numberOfExits = 0;
	int exitDistance = (m.width - e) / e;

	m.maze = new char*[m.height];
	bool **visit = new bool*[m.height];
	for (i = 0; i < m.height; i++) {
		m.maze[i] = new char[m.width];
		visit[i] = new bool[m.width];
		for (j = 0; j < m.width; j++) {
			m.maze[i][j] = 1;
			visit[i][j] = false;
		}
	}

	m.maze[m.start.y][m.start.x] = 0;
	visit[m.start.y][m.start.x] = true;
	s.push(m.start);
	while (!s.empty()) {
		Point curr = s.top();

		bool move[4] = { 0 };
		Point p;
		if (curr.y & 1) {
			p = { curr.y, curr.x - 1 };	// left
			if (p.x > 0 && curr.y < m.height - 1 && !visit[p.y][p.x]) {
				if (m.maze[p.y - 1][p.x - 1] && m.maze[p.y - 1][p.x] && m.maze[p.y][p.x - 1]
					&& (p.y == m.height - 1 || (m.maze[p.y + 1][p.x - 1] && m.maze[p.y + 1][p.x])))
					move[0] = true;
			}
			p = { curr.y, curr.x + 1 };	// right
			if (p.x < m.width - 1 && curr.y < m.height - 1 && !visit[p.y][p.x]) {
				if (m.maze[p.y - 1][p.x + 1] && m.maze[p.y - 1][p.x] && m.maze[p.y][p.x + 1]
					&& (p.y == m.height - 1 || (m.maze[p.y + 1][p.x + 1] && m.maze[p.y + 1][p.x])))
					move[1] = true;
			}
		}
		p = { curr.y - 1, curr.x };	// up
		if (p.y > 0 && !visit[p.y][p.x]) {
			if (m.maze[p.y][p.x + 1] && m.maze[p.y][p.x - 1]
				&& m.maze[p.y - 1][p.x + 1] && m.maze[p.y - 1][p.x] && m.maze[p.y - 1][p.x - 1])
				move[2] = true;
		}
		p = { curr.y + 1, curr.x };	// down
		if (p.y <= m.height - 1 && !visit[p.y][p.x]) {
			bool b = false;
			if (m.maze[p.y][p.x + 1] && m.maze[p.y][p.x - 1])
				b = true;
			if (b && (p.y < m.height - 1 && m.maze[p.y + 1][p.x + 1] && m.maze[p.y + 1][p.x] && m.maze[p.y + 1][p.x - 1]))
				move[3] = true;
			if (b && p.y == m.height - 1 && m.numberOfExits < e)
				move[3] = true;
			if (move[3] && p.y == m.height - 1)
				if (e == 1) {
					if (m.width - p.x > 3)
						move[3] = false;
				} else {
					for (Point pexit : m.exit) {
						if (abs(p.x - pexit.x) < exitDistance) {
							move[3] = false;
							break;
						}
					}
				}
		}

		if (move[0] || move[1] || move[2] || move[3]) {
			int r;
			while (1) {
				r = rand() / (RAND_MAX + 1.0) * 4;
				if (move[r])
					break;
			}
			switch (r) {
			case 0:	// left
				p = { curr.y, curr.x - 1 };
				break;
			case 1:	// right
				p = { curr.y, curr.x + 1 };
				break;
			case 2:	// up
				p = { curr.y - 1, curr.x };
				break;
			case 3:	// down
				p = { curr.y + 1, curr.x };
				break;
			}
			m.maze[p.y][p.x] = 0;
			visit[p.y][p.x] = true;

			if (p.y == m.height - 1) {
				m.exit.push_back({ p.y, p.x });
				m.numberOfExits++;
			}

			s.push({ p.y, p.x });
		} else {
			s.pop();
		}
	}

	for (int i = 0; i < w * h / 2; i++) {
		int x = 1 + rand() % (w - 2);
		int y = 1 + rand() % (h - 2);
		if (m.maze[y][x]) {
			if ((m.maze[y][x - 1] && m.maze[y][x + 1] && !m.maze[y - 1][x] && !m.maze[y + 1][x]) ||
				(!m.maze[y][x - 1] && !m.maze[y][x + 1] && m.maze[y - 1][x] && m.maze[y + 1][x])) {
				m.maze[y][x] = 0;
			}
		}
	}

	for (i = 0; i < m.height; i++)
		delete[] visit[i];
	delete visit;

	return m;
}

char* Maze::convertToPixelData() const {
	char *res = new char[width * height * 3];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			// 1 - wall, black
			// 0 - path, white
			// 2 - solution, red
			res[3 * ((height - i - 1) * width + j) + 0] = (maze[i][j] != 0 && maze[i][j] != 1) ? 0 : maze[i][j] - 1;	// b
			res[3 * ((height - i - 1) * width + j) + 1] = (maze[i][j] != 0 && maze[i][j] != 1) ? 0 : maze[i][j] - 1;	// g
			res[3 * ((height - i - 1) * width + j) + 2] = (maze[i][j] != 0 && maze[i][j] != 1) ? 255 : maze[i][j] - 1;	// r
		}
	return res;
}

void Maze::setPathColor(list<Point> path) {
	Point p1, p2;
	int x, y;
	for (list<Point>::iterator i = path.begin(); i != path.end(); ) {
		p1 = *i++;
		if (i == path.end())
			break;
		p2 = *i;
		if (p1.x == p2.x) {
			int y0 = min(p1.y, p2.y), y1 = max(p1.y, p2.y);
			for (y = y0, x = p1.x; y <= y1; y++) {
				maze[y][x] = 2;
			}
		} else {
			int x0 = min(p1.x, p2.x), x1 = max(p1.x, p2.x);
			for (x = x0, y = p1.y; x <= x1; x++) {
				maze[y][x] = 2;
			}
		}
	}
}

void Maze::deleteMaze() {
	for (int i = 0; i < height; i++)
		delete maze[i];
	delete maze;
}

void Maze::copy(const Maze &m) {
	width = m.width;
	height = m.height;
	maze = new char*[height];
	for (int i = 0; i < height; i++) {
		maze[i] = new char[width];
		for (int j = 0; j < width; j++) {
			maze[i][j] = m.maze[i][j];
		}
	}
	start = m.start;
	exit = m.exit;
}

void Maze::move(Maze &m) {
	width = m.width;
	height = m.height;
	maze = m.maze;
	m.maze = nullptr;
	m.width = m.height = 0;
	start = m.start;
	exit = m.exit;
}

istream& operator>>(istream & is, Maze & m) {
	m.deleteMaze();
	is >> m.width >> m.height;
	m.width = (m.width > 5 ? m.width : 5);
	m.height = (m.height > 5 ? m.height : 5);
	int x;
	m.maze = new char*[m.height];
	for (int i = 0; i < m.height; i++) {
		m.maze[i] = new char[m.width];
		for (int j = 0; j < m.width; j++) {
			cin >> x;
			m.maze[i][j] = x;
		}
	}
	return is;
}

ostream& operator<<(ostream & os, const Maze & m) {
	for (int i = 0; i < m.height; i++) {
		for (int j = 0; j < m.width; j++) {
			os << (int) m.maze[i][j] << ' ';
		}
		os << endl;
	}
	return os;
}

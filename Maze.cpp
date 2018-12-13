#include "Maze.h"
#include <stack>


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
			if (p.x > 0 && !visit[p.y][p.x]) {
				if (m.maze[p.y - 1][p.x - 1] && m.maze[p.y - 1][p.x] && m.maze[p.y][p.x - 1] 
					&& (curr.y == m.height - 1 || (m.maze[p.y + 1][p.x - 1] && m.maze[p.y + 1][p.x])))
					move[0] = true;
			}
			p = { curr.y, curr.x + 1 };	// right
			if (p.x < m.width - 1 && !visit[p.y][p.x]) {
				if (m.maze[p.y - 1][p.x + 1] && m.maze[p.y - 1][p.x] && m.maze[p.y][p.x + 1]
					&& (curr.y == m.height - 1 || (m.maze[p.y + 1][p.x + 1] && m.maze[p.y + 1][p.x])))
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

	for (i = 0; i < m.height; i++)
		delete[] visit[i];
	delete visit;

	return m;
}

char* Maze::convertToPixelData() const {
	char *res = new char[width * height * 3];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			res[3 * ((height - i - 1) * width + j) + 0] = !maze[i][j] ? 255 : 0;
			res[3 * ((height - i - 1) * width + j) + 1] = !maze[i][j] ? 255 : 0;
			res[3 * ((height - i - 1) * width + j) + 2] = !maze[i][j] ? 255 : 0;
		}
	return res;
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

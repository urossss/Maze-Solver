#ifndef _MAZE_H_
#define _MAZE_H_


#include <iostream>
#include <list>
#include "Point.h"
using namespace std;

class Maze {
	friend class MazeGraph;

	Point start = { 0, 1 };
	list<Point> exit;
	int numberOfExits = 0;

	int width = 0, height = 0;
	char** maze = nullptr;
public:
	Maze() {
	}
	Maze(int w, int h, char **m) {
		width = w;
		height = h;
		maze = m;
		for (int i = 0; i < w; i++) {
			if (maze[0][i] == 0) {
				start = { 0, i };
			}
			if (maze[height - 1][i] == 0) {
				exit.push_back({ height - 1, i });
			}
		}
	}
	Maze(const Maze &m) {
		copy(m);
	}
	Maze(Maze &&m) {
		move(m);
	}
	~Maze() {
		deleteMaze();
	}

	Maze& operator=(const Maze &m) {
		if (this != &m) {
			deleteMaze();
			copy(m);
		}
		return *this;
	}
	Maze& operator=(Maze &&m) {
		if (this != &m) {
			deleteMaze();
			move(m);
		}
		return *this;
	}

	int getWidth() const {
		return width;
	}
	int getHeight() const {
		return height;
	}
	char** getMaze() const {
		return maze;
	}

	friend istream& operator>>(istream &is, Maze &m);
	friend ostream& operator<<(ostream &os, const Maze &m);

	char* convertToPixelData() const;

	static Maze generateMaze(int w, int h);
	static Maze generateMaze(int w, int h, int e);
private:
	void deleteMaze();
	void copy(const Maze &);
	void move(Maze &);
};

#endif
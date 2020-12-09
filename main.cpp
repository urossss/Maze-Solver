#include "BMPImage.h"
#include "MazeGraph.h"
#include <ctime>
#include <string>


void printMainMenu() {
	cout << "\nChoose one of the following options:\n";
	cout << "	1. Create a maze\n";
	cout << "	2. Create a graph from a maze\n";
	cout << "	3. Solve a maze\n";
	cout << "	0. Exit\n";
	cout << "Your choice: ";
}

void printMazeLoadingMenu() {
	cout << "\nChoose how to create a maze:\n";
	cout << "	1. Generate a maze\n";
	cout << "	2. Read a maze from a bitmap file\n";
	cout << "	3. Read a maze from standard input\n";
	cout << "	0. Exit\n";
	cout << "Your choice: ";
}

void printSolveMenu() {
	cout << "\nChoose a solving method:\n";
	cout << "	1. BFS\n";
	cout << "	2. DFS\n";
	cout << "	3. Dijkstra\n";
	cout << "	0. Exit\n";
	cout << "Your choice: ";
}

MazeGraph* createGraph(const Maze &m) {
	clock_t t1, t2;
	double d;
	MazeGraph *g;
	t1 = clock();
	cout << "\nCreating a graph..." << endl;
	g = MazeGraph::createGraph(m);
	t2 = clock();
	d = (double) (t2 - t1) / CLOCKS_PER_SEC;
	cout << "Time elapsed: " << d << "s" << endl;
	return g;
}

int main() {

	clock_t t1, t2;
	double d;
	//t1 = clock();

	srand(time(nullptr));

	int choice, w, h, e;
	string fileName;
	BMPImage *i = nullptr;
	MazeGraph *g = nullptr;
	Maze m, *sol = nullptr;
	bool done = false;

	while (1) {
		printMainMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			done = false;
			delete g;
			g = nullptr;
			delete i;
			i = nullptr;
			while (!done) {
				printMazeLoadingMenu();
				cin >> choice;
				switch (choice) {
				case 1:
					cout << "Enter maze dimensions: ";
					cin >> w >> h;
					cout << "Enter number of exits: ";
					cin >> e;
					cout << "\nGenerating a maze..." << endl;
					t1 = clock();
					m = Maze::generateMaze(w, h, e);
					t2 = clock();
					d = (double) (t2 - t1) / CLOCKS_PER_SEC;
					cout << "Time elapsed: " << d << "s" << endl;

					i = new BMPImage(m);
					i->exportAsBMP("maze.bmp");
					delete i;
					i = nullptr;

					done = true;
					break;
				case 2:
					cout << "Enter filename: ";
					cin >> fileName;
					cout << "\nLoading a maze..." << endl;
					t1 = clock();
					i = new BMPImage();
					i->importFromBMP(fileName);
					m = i->convertToMaze();
					t2 = clock();
					d = (double) (t2 - t1) / CLOCKS_PER_SEC;
					cout << "Time elapsed: " << d << "s" << endl;

					delete i;
					i = nullptr;

					done = true;
					break;
				case 3:
					cout << "Enter maze dimensions and maze data (1 for walls and 0 for path): ";
					cin >> m;
					i = new BMPImage(m);
					i->exportAsBMP("maze.bmp");

					delete i;
					i = nullptr;

					done = true;
					break;
				case 0:
					exit(0);
				default:
					cout << "Invalid choice, try again.\n";
					break;
				}
			}
			break;
		case 2:
			if (!done) {
				cout << "You need to load a maze first.\n";
				break;
			}
			delete g;
			g = createGraph(m);
			break;
		case 3:
			if (!done) {
				cout << "You need to load a maze first.\n";
				break;
			}
			if (!g)
				g = createGraph(m);

			sol = new Maze(m);

			done = false;
			while (!done) {
				printSolveMenu();
				cin >> choice;
				switch (choice) {
				case 1:
					t1 = clock();
					cout << "\nSolving..." << endl;
					sol->setPathColor(g->solveBFS());
					t2 = clock();
					done = true;
					break;
				case 2:
					t1 = clock();
					cout << "\nSolving..." << endl;
					sol->setPathColor(g->solveDFS());
					t2 = clock();
					done = true;
					break;
				case 3:
					t1 = clock();
					cout << "\nSolving..." << endl;
					sol->setPathColor(g->solveDijkstra());
					t2 = clock();
					done = true;
					break;
				case 0:
					exit(0);
				default:
					cout << "Invalid choice, try again.\n";
					break;
				}
			}
			
			d = (double) (t2 - t1) / CLOCKS_PER_SEC;
			cout << "Time elapsed: " << d << "s" << endl;

			i = new BMPImage(*sol);
			cout << "Enter a name of solution output file: ";
			cin >> fileName;
			i->exportAsBMP(fileName);

			delete sol;
			sol = nullptr;
			delete i;
			i = nullptr;
			break;
		case 0:
			exit(0);
		default:
			cout << "Invalid choice, try again.\n";
			break;
		}
	}

}

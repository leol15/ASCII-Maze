
#include <termios.h>
#include <iostream>
#include <ctime>

#include "Maze.h"

using namespace std;

void disableInputBuffering() {
	struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &t);
}

void clear() {
	cout << "\x1B[2J\x1B[H";
}

int main(int argc, char const *argv[]) {

	// for characters
	disableInputBuffering();
	// init maze
	Maze maze(30, 24);
	maze.CreateMaze_BackTrack();
	// Maze maze;

	char act;
	// cout << "Start" << endl;
	clock_t ti;
	while (1) {
		clear();
		cout << maze;
		cin >> act;
		// arrow keys
		if (act==27) {
			cin >> act;
			if (act == 91) {
				cin >> act;
		        if (act==65)
		        	act = 'w';
		        if (act==66)
		        	act = 's';
		        if (act==67)
		        	act = 'd';
		        if (act==68)
		        	act = 'a';
			}
		}
		// actions
		if (act == 'w' || act == 's' || act == 'a' || act == 'd') {
			if (time(nullptr) - ti < 1) {
				// fast move
				maze.Action(act);
				maze.Action(act);
			}
			ti = time(nullptr);
			maze.Action(act);
		}
		if (act == 'r')
			maze.Recreate();
		if (act == 'q')
			break;
	}
	clear();
	return 0;
}



#include <termios.h>
#include <iostream>

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
	Maze maze(40, 30);
	maze.CreateMaze_BackTrack();
	// Maze maze;

	char act;
	// cout << "Start" << endl;
	clear();
	while (1) {
		cout << maze;
		cin >> act;
		if ((act==27)) {
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
		clear();
		if (act == 'w')
			maze.MoveUp();
		if (act == 's')
			maze.MoveDown();
		if (act == 'a')
			maze.MoveLeft();
		if (act == 'd')
			maze.MoveRight();
		if (act == 'q')
			break;
		if (act == 'r') {
			maze.Recreate();
			clear();
		}
	}
	return 0;
}
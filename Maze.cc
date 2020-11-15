
#include "Maze.h"

#include <vector>
#include <iostream>
#include <string>
#include <unistd.h> 	// for timer
#include <stdlib.h> 	/* srand, rand */
#include <time.h>       /* time */
#include <stack>


// local helper
static char** init_grid(int w, int h, char c) {
	char ** tmp = new char*[h];
	for (int i = 0; i < h; i++) {
		tmp[i] = new char[w];
		for (int j = 0; j < w; j++) {
			tmp[i][j] = c;
		}
	}
	return tmp;
}

// default ctor
Maze::Maze() : w_(11), h_(10), x_(0), y_(1) {
	std::string map[] = {"# #########",
						 "#     ## ##",
						 "#####    ##",
						 "#   # ## ##",
						 "# #   ## ##",
						 "# ###### ##",
						 "# ###    ##",
						 "#   #######",
						 "###       #",
						 "######### #"};
	grid_ = init_grid(w_, h_, EMPTY_);
	for (int i = 0; i < w_; i++) {
		for (int j = 0; j < h_; j++) {
			grid_[i][j] = map[i][j];
		}
	}
}

Maze::Maze(int width, int height) : w_(width), h_(height), x_(0), y_(0) {
	grid_ = init_grid(w_, h_, EMPTY_);
}

Maze::~Maze() {
	for (int i = 0; i < h_; i++) {
		delete[] grid_[i];
	}
	delete[] grid_;
}


void Maze::Recreate() {
	CreateMaze_Div();
}

std::ostream &operator<<(std::ostream &out, const Maze &m) {
	// print the maze
	for (int i = 0; i < m.h_; i++) {
		for (int j = 0; j < m.w_; j++) {
			if (i == m.x_ && j == m.y_) {
				out << "\033[41m" << 'O' << "\033[m";
			} else if (m.grid_[i][j] == m.WALL_) {
				out << "\033[100m" << m.grid_[i][j] << "\033[m";
			} else {
				out << m.grid_[i][j];
			}
		}
		out << std::endl;
	}
	return out;
}

// actions 
void Maze::Action(char action) {
	if (action == 'w')
		MoveTo(x_ - 1, y_);
	if (action == 's')
		MoveTo(x_ + 1, y_);
	if (action == 'd')
		MoveTo(x_, y_ + 1);
	if (action == 'a')
		MoveTo(x_, y_ - 1);
}

void Maze::MoveTo(int x, int y) {
	if (isTile(x, y, EMPTY_))
		x_ = x, y_ = y;
}


bool Maze::isTile(int x, int y, char goal) {
	return x >= 0 && x < h_ && y >= 0 && y < w_
		&& grid_[x][y] == goal;
}

void Maze::resetGrid(char tile) {
	for (int i = 0; i < h_; i++)
		for (int j = 0; j < w_; j++)
			grid_[i][j] = tile;
}



// Maze Implementations

// divde & conquer
void Maze::CreateMaze_Div() {
	resetGrid(EMPTY_);
	for (int i = 0; i < h_; i++) {
		grid_[i][0] = WALL_;
		grid_[i][w_ - 1] = WALL_;
	}
	for (int i = 0; i < w_; i++) {
		grid_[0][i] = WALL_;
		grid_[h_ - 1][i] = WALL_;
	}
	x_ = 0, y_ = 1;
	grid_[x_][y_] = EMPTY_;
	grid_[h_ - 1][w_ - 2] = EMPTY_;

	// recursive process
	srand(time(NULL));
	CreateMaze_Div_Helper(1, 1, w_ - 2,  h_ - 2, 49);
}

// (x,y) ~ (x+w,y+h) are all empty
void Maze::CreateMaze_Div_Helper(int x, int y, int w, int h, int depth) {
	// base case
	if (w <= 2 || h <= 2 || w > 200 || h > 200)
		return;
	// decision
	int div_x, div_y;
	int ctr = 0;
	while (true) {
		// random location
		div_x = x + 1 + rand() % (h - 2);
		div_y = y + 1 + rand() % (w - 2);
		// check blockage ---
		if (grid_[div_x][y - 1] == EMPTY_ ||
			grid_[div_x][y + w] == EMPTY_ ||
			grid_[x - 1][div_y] == EMPTY_ ||
			grid_[x + h][div_y] == EMPTY_) {
		} else {
			break;
		}
		if (ctr > 100) {
			// ok just make holes
			ctr = -1;
			break;
		}
		ctr++;
	}

	// build walls
	for (int i = 0; i < w; i++) {
		grid_[div_x][y + i] = WALL_;
	}
	for (int i = 0; i < h; i++) {
		grid_[x + i][div_y] = WALL_;
	}

	// make holes for parent
	if (ctr == -1) {
		// std::cout << "More holes: " << div_x  << ", " << div_y << std::endl;
		if (grid_[div_x][y - 1] == EMPTY_)
			grid_[div_x][y] = EMPTY_;
		if (grid_[div_x][y + w] == EMPTY_)
			grid_[div_x][y + w - 1] = EMPTY_;
		if (grid_[x - 1][div_y] == EMPTY_)
			grid_[x][div_y] = EMPTY_;
		if (grid_[x + h][div_y] == EMPTY_)
			grid_[x + h - 1][div_y] = EMPTY_;
	}

	// find sub box sizes
	int w1 = div_y - y;
	int w2 = w - w1 - 1;
	int h1 = div_x - x;
	int h2 = h - h1 - 1;
	// create holes

	grid_[div_x][div_y + 1 + (rand() % (w2))] = EMPTY_;
	grid_[div_x][y + (rand() % w1)] = EMPTY_;
	grid_[x + rand() % h1][div_y] = EMPTY_;

	// animation
	std::cout << "\x1B[2J\x1B[H";
	std::cout << *this;
	usleep(100000);
	// recurse
	CreateMaze_Div_Helper(x,         y,         w1, h1, depth + 1);
	CreateMaze_Div_Helper(div_x + 1, div_y + 1, w2, h2, depth + 1);
	CreateMaze_Div_Helper(x,         div_y + 1, w2, h1, depth + 1);
	CreateMaze_Div_Helper(div_x + 1, y,         w1, h2, depth + 1);
}


void Maze::CreateMaze_BackTrack() {
	srand(time(nullptr));
	// fill with wall
	resetGrid(WALL_);
	// stack
	std::stack<std::pair<int, int>> st;
	st.push(std::pair<int,int>(0, 0));
	// random 
	int x_off[4] = {1, -1, 0, 0};
	int y_off[4] = {0, 0, 1, -1};

	while (!st.empty()) {
		auto p = st.top();
		st.pop();
		// remove wall?
		int ct = 0;
		for (int i = 0; i < 4; i++) {
			if (isTile(p.first + x_off[i], p.second + y_off[i], EMPTY_))
				ct++;
		}
		// skip & dont remove block
		if (ct > 1)
			continue;

		grid_[p.first][p.second] = EMPTY_;

		// shuffle order
		for (int i = 0; i < 10; i++) {
			int idx = 1 + rand() % 3, tmp;
			
			tmp = x_off[idx];
			x_off[idx] = x_off[0];
			x_off[0] = tmp;

			tmp = y_off[idx];
			y_off[idx] = y_off[0];
			y_off[0] = tmp; 
		}

		// check 4 sides
		for (int i = 0; i < 4; i++) {
			// where to explore? should push this point?
			if (!isTile(p.first + x_off[i], p.second + y_off[i], WALL_))
				continue;
			// check empty space next to
			int ct = 0;
			for (int j = 0; j < 4; j++) {
				if (isTile(p.first + x_off[i] + x_off[j],
					p.second + y_off[i] + y_off[j], EMPTY_))
					ct++;
			}
			// explore!
			if (ct <= 1)
				st.push(std::pair<int, int>(p.first + x_off[i], p.second + y_off[i]));
		}
		// animation
		std::cout << "\x1B[2J\x1B[H";
		std::cout << *this << std::flush;
		usleep(50000);
	}

}






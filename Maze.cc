
#include "Maze.h"

#include <vector>
#include <iostream>
#include <string>
#include <unistd.h> 	// for timer
#include <stdlib.h> 	/* srand, rand */
#include <time.h>       /* time */
#include <stack>


// local helper
static char** init_grid(dim w, dim h, char c) {
	char ** tmp = new char*[h];
	for (dim i = 0; i < h; i++) {
		tmp[i] = new char[w];
		for (dim j = 0; j < w; j++) {
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
	grid_ = init_grid(w_, h_, ' ');
	for (dim i = 0; i < w_; i++) {
		for (dim j = 0; j < h_; j++) {
			grid_[i][j] = map[i][j];
		}
	}
}

Maze::Maze(int width, int height) : w_(width), h_(height), x_(0), y_(0) {
	grid_ = init_grid(w_, h_, ' ');
}

Maze::~Maze() {
	for (dim i = 0; i < h_; i++) {
		delete[] grid_[i];
	}
	delete[] grid_;
}


void Maze::Recreate() {
	CreateMaze_Div();
}

std::ostream &operator<<(std::ostream &out, const Maze &m) {
	// print the maze
	for (dim i = 0; i < m.h_; i++) {
		for (dim j = 0; j < m.w_; j++) {
			if (i == m.x_ && j == m.y_) {
				out << '@';
			} else {
				out << m.grid_[i][j];
			}
		}
		out << std::endl;
	}
	return out;
}

// actions 
void Maze::MoveUp() {
	if (!isTile(x_ - 1, y_, ' '))
		return;
	x_--;
}
void Maze::MoveDown() {
	if (!isTile(x_ + 1, y_, ' '))
		return;
	x_++;
}
void Maze::MoveLeft() {
	if (!isTile(x_, y_ - 1, ' '))
		return;
	y_--;
}
void Maze::MoveRight() {
	if (!isTile(x_, y_ + 1, ' '))
		return;
	y_++;
}

bool Maze::isTile(dim x, dim y, char goal) {
	return x >= 0 && x < h_ && y >= 0 && y < w_
		&& grid_[x][y] == goal;
}

void Maze::resetGrid(char tile) {
	for (dim i = 0; i < h_; i++)
		for (dim j = 0; j < w_; j++)
			grid_[i][j] = tile;
}



// Maze Implementations

// divde & conquer
void Maze::CreateMaze_Div() {
	resetGrid(' ');
	for (dim i = 0; i < h_; i++) {
		grid_[i][0] = '#';
		grid_[i][w_ - 1] = '#';
	}
	for (dim i = 0; i < w_; i++) {
		grid_[0][i] = '#';
		grid_[h_ - 1][i] = '#';
	}
	x_ = 0;
	y_ = 1;
	grid_[0][1] = ' ';
	grid_[h_ - 1][w_ - 2] = ' ';

	// recursive process
	srand(time(NULL));
	CreateMaze_Div_Helper(1, 1, w_ - 2,  h_ - 2, 49);
}

// (x,y) ~ (x+w,y+h) are all empty
void Maze::CreateMaze_Div_Helper(dim x, dim y, dim w, dim h, dim depth) {
	// base case
	if (w <= 2 || h <= 2 || w > 200 || h > 200)
		return;
	// decision
	dim div_x, div_y;
	int ctr = 0;
	while (true) {
		// random location
		div_x = x + 1 + rand() % (h - 2);
		div_y = y + 1 + rand() % (w - 2);
		// check blockage ---
		if (grid_[div_x][y - 1] == ' ' ||
			grid_[div_x][y + w] == ' ' ||
			grid_[x - 1][div_y] == ' ' ||
			grid_[x + h][div_y] == ' ') {
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
	for (dim i = 0; i < w; i++) {
		grid_[div_x][y + i] = (char) depth;
	}
	for (dim i = 0; i < h; i++) {
		grid_[x + i][div_y] = (char) depth;
	}

	// make holes for parent
	if (ctr == -1) {
		// std::cout << "More holes: " << div_x  << ", " << div_y << std::endl;
		if (grid_[div_x][y - 1] == ' ')
			grid_[div_x][y] = ' ';
		if (grid_[div_x][y + w] == ' ')
			grid_[div_x][y + w - 1] = ' ';
		if (grid_[x - 1][div_y] == ' ')
			grid_[x][div_y] = ' ';
		if (grid_[x + h][div_y] == ' ')
			grid_[x + h - 1][div_y] = ' ';
	}

	// find sub box sizes
	dim w1 = div_y - y;
	dim w2 = w - w1 - 1;
	dim h1 = div_x - x;
	dim h2 = h - h1 - 1;
	// create holes

	grid_[div_x][div_y + 1 + (rand() % (w2))] = ' ';
	grid_[div_x][y + (rand() % w1)] = ' ';
	grid_[x + rand() % h1][div_y] = ' ';

	// animation
	std::cout << "\x1B[2J\x1B[H";
	std::cout << *this;
	usleep(100000);
	// recurse
	CreateMaze_Div_Helper(x,         y,         w1, h1, depth + 1);
	CreateMaze_Div_Helper(x,         div_y + 1, w2, h1, depth + 1);
	CreateMaze_Div_Helper(div_x + 1, y,         w1, h2, depth + 1);
	CreateMaze_Div_Helper(div_x + 1, div_y + 1, w2, h2, depth + 1);
}


void Maze::CreateMaze_BackTrack() {
	srand(time(nullptr));
	// fill with wall
	resetGrid('#');
	// stack
	std::stack<std::pair<dim, dim>> st;
	st.push(std::pair<dim,dim>(0, 0));
	// random 
	int x_off[4] = {1, -1, 0, 0};
	int y_off[4] = {0, 0, 1, -1};

	while (!st.empty()) {
		auto p = st.top();
		st.pop();
		// remove wall?
		int ct = 0;
		for (int i = 0; i < 4; i++) {
			if (isTile(p.first + x_off[i], p.second + y_off[i], ' '))
				ct++;
		}
		// skip & dont remove block
		if (ct > 1)
			continue;

		grid_[p.first][p.second] = ' ';

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
			if (!isTile(p.first + x_off[i], p.second + y_off[i], '#'))
				continue;
			// check empty space next to
			int ct = 0;
			for (int j = 0; j < 4; j++) {
				if (isTile(p.first + x_off[i] + x_off[j],
					p.second + y_off[i] + y_off[j], ' '))
					ct++;
			}
			// explore!
			if (ct <= 1)
				st.push(std::pair<int, int>(p.first + x_off[i], p.second + y_off[i]));
		}
		// animation
		std::cout << "\x1B[2J\x1B[H";
		std::cout << *this;
		usleep(20000);
	}

}






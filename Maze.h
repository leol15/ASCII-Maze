
#ifndef _MAZE_H_
#define _MAZE_H_

#include <ostream>
#include <vector>


class Maze {
public:
	Maze();
	Maze(int width, int height);
	~Maze();
    virtual void Recreate();
    
    void Action(char action);

    virtual void CreateMaze_Div();
	virtual void CreateMaze_Div_Helper(int x, int y, int w, int h, int depth);

	virtual void CreateMaze_BackTrack();

    friend std::ostream &operator<<(std::ostream &out, const Maze &m);

private:
	char WALL_ = ' ';
	char EMPTY_ = '-';
	char VISITED_ = '=';
	char ** grid_;
	int w_;
	int h_;

	int x_;
	int y_;

	// test if point x, y  is goal, with bounds checking
	bool isTile(int x, int y, char goal);
	void resetGrid(char tile);

    void MoveTo(int x, int y);
};



#endif	// _MAZE_H_
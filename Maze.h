
#ifndef _MAZE_H_
#define _MAZE_H_

#include <ostream>
#include <vector>

typedef unsigned long dim;

class Maze {
public:
	Maze();
	Maze(int width, int height);
	~Maze();
    virtual void Recreate();
    
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    virtual void CreateMaze_Div();
	virtual void CreateMaze_Div_Helper(dim x, dim y, dim w, dim h, dim depth);

	virtual void CreateMaze_BackTrack();

    friend std::ostream &operator<<(std::ostream &out, const Maze &m);

private:
	char ** grid_;
	dim w_;
	dim h_;

	dim x_;
	dim y_;

	// test if point x, y is goal, with bounds checking
	bool isTile(dim x, dim y, char goal);
	void resetGrid(char tile);

};



#endif	// _MAZE_H_
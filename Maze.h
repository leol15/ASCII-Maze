
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
    
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    virtual void Recreate();
    virtual void CreateMaze();
	virtual void CreateMazeHelper(dim x, dim y, dim w, dim h, dim depth);

    friend std::ostream &operator<<(std::ostream &out, const Maze &m);

private:
	char ** grid_;
	dim w_;
	dim h_;

	dim x_;
	dim y_;

	bool isGood(dim x, dim y);

};



#endif	// _MAZE_H_
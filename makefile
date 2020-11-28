
default:
	@echo "None"

keys: keys.cc
	g++ -Wall -g -o keys $^

both: main run

run: 
	./runMaze 20 20

main: Maze.o runMaze.o
	g++ -Wall -g -o runMaze $^

runMaze.o: RunMaze.cc
	g++ -Wall -g -o runMaze.o -c RunMaze.cc

Maze.o: Maze.h Maze.cc
	g++ -Wall -g -o Maze.o -c Maze.cc

clean:
	rm *.o runMaze

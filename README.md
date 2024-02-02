# Maze Generator using DFS
This was my final project for Introduction to Programming.
It generates a playable nxn maze which can be saved as .svg and a .dat with the following structure:
1st byte represents the size of the maze (number of lines/columns)
The other bytes represent the cells of the maze linewise:
enum {
CELL_PATH_N  =  0b1,
CELL_PATH_E  =  0b10,
CELL_PATH_S  =  0b100,
CELL_PATH_W  =  0b1000,
CELL_VISITED  =  0b10000,
CURRENT_CELL  =  0b100000,
};

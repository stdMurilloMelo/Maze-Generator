# Maze Generator using DFS
This was my final project for Introduction to Programming. <br>
It generates a playable nxn maze which can be saved as .svg and a .dat with the following structure: <br>

1st byte represents the size of the maze (number of lines/columns) <br>

The other bytes represent the cells of the maze linewise:  <br> 

enum { <br>
CELL_PATH_N  =  0b1, <br>
CELL_PATH_E  =  0b10, <br>
CELL_PATH_S  =  0b100, <br>
CELL_PATH_W  =  0b1000, <br>
CELL_VISITED  =  0b10000, <br>
CURRENT_CELL  =  0b100000, <br>
};


https://github.com/user-attachments/assets/04b27c09-de27-4833-ad7a-fb188afb0779


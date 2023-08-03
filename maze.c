#include <stdlib.h>
#include "maze.h"

void init_MazeStack(MazeStack stack, int stack_size)
{
    stack.top = -1;
    stack.array = (CellPair*)malloc(stack_size*sizeof(CellPair));
}

void stack_push(MazeStack stack, unsigned int x, unsigned int y)
{
    stack.top++;
    stack.array[stack.top].x = x;
    stack.array[stack.top].y = y;
}

void pop(MazeStack stack)
{
    stack.top--;
}

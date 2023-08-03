#include <stdlib.h>

enum
{
    CELL_PATH_N = 0b1,
    CELL_PATH_E = 0b10,
    CELL_PATH_S = 0b100,
    CELL_PATH_W = 0b1000,
    CELL_VISITED = 0b10000,
};

typedef struct cell_pair
{
    unsigned char x;
    unsigned char y;
}CellPair;

typedef struct maze_stack
{
    int top;
    CellPair *array;
}MazeStack;


void init_MazeStack(MazeStack* stack, unsigned char stack_size)
{
    stack->top = -1;
    stack->array = (CellPair*)malloc(stack_size * sizeof(CellPair));
}

void maze_stack_push(MazeStack* stack, unsigned char x0, unsigned char y0)
{
    stack->top++;
    (stack->array[stack->top]).x = x0;
    (stack->array[stack->top]).y = y0;
}

void maze_stack_pop(MazeStack* stack)
{
    stack->top--;
}

typedef struct maze_neighbours_stack
{
    int top;
    unsigned char *array;
}NeighboursStack;

void init_NeighboursStack(NeighboursStack* stack, unsigned char stack_size)
{
    stack->top = -1;
    stack->array = (unsigned char*)malloc(stack_size * sizeof(unsigned char));
}

void neighbours_stack_push(NeighboursStack* stack, unsigned char element)
{
    stack->top++;
    stack->array[stack->top] = element;
}

void neighbours_stack_pop(NeighboursStack* stack)
{
    stack->top--;
}
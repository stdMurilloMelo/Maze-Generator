#include <stdlib.h>

#define offset(x0, y0) (m_stack.array[m_stack.top].y + y0) * MazeWidth + (m_stack.array[m_stack.top].x + x0)

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
    unsigned int x;
    unsigned int y;
}CellPair;

typedef struct maze_stack
{
    int top;
    CellPair *array;
}MazeStack;


void init_MazeStack(MazeStack* stack, unsigned int stack_size)
{
    stack->top = -1;
    stack->array = (CellPair*)calloc(stack_size, sizeof(CellPair));
}

void maze_stack_push(MazeStack* stack, unsigned int x0, unsigned int y0)
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
    unsigned int array[4];
}NeighboursStack;

void init_NeighboursStack(NeighboursStack* stack)
{
    stack->top = -1;
}

void neighbours_stack_push(NeighboursStack* stack, unsigned int element)
{
    stack->top++;
    stack->array[stack->top] = element;
}

void neighbours_stack_pop(NeighboursStack* stack)
{
    stack->top--;
}
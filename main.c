#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze.h"

#define offset(x0, y0) (m_stack.array[m_stack.top].y + y0) * MazeWidth + (m_stack.array[m_stack.top].x + x0)

// gcc -o main main.c -I/mingw64/include/SDL2 -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2

void drawPseudoPixel(SDL_Renderer *renderer, int x, int y, int r, int g, int b, int size)
{
    // Set the color of the square
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Draw the square
    SDL_Rect squareRect = {x * size, y * size, size, size};
    SDL_RenderFillRect(renderer, &squareRect);
}

int main(int argc, char *argv[])
{
    // Window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    int window_width = 1200, window_height = 600;
    SDL_Window *window = SDL_CreateWindow("Maze Generator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Maze parameters
    unsigned char MazeWidth = 10, MazeHeight = 10, NumVisitedCells, PathWidth = 3;
    unsigned char *m_cells;
    m_cells = (unsigned char *)calloc(MazeWidth * MazeHeight, sizeof(unsigned char));
    MazeStack m_stack;
    init_MazeStack(&m_stack, MazeWidth * MazeHeight);
    maze_stack_push(&m_stack, 0, 0);
    m_cells[0] = CELL_VISITED;
    NumVisitedCells = 1;

    // Create Maze
    while (NumVisitedCells < MazeWidth * MazeHeight)
    {
        // Create a set of unvisted neighbours
        NeighboursStack neighbours;
        init_NeighboursStack(&neighbours, 4);
        // North neighbour
        if (m_stack.array[m_stack.top].y > 0 && (m_cells[offset(0, -1)] & CELL_VISITED) == 0)
            neighbours_stack_push(&neighbours, 0);
        // East neighbour
        if (m_stack.array[m_stack.top].x < MazeWidth - 1 && (m_cells[offset(1, 0)] & CELL_VISITED) == 0)
            neighbours_stack_push(&neighbours, 1);
        // South neighbour
        if (m_stack.array[m_stack.top].y < MazeHeight - 1 && (m_cells[offset(0, 1)] & CELL_VISITED) == 0)
            neighbours_stack_push(&neighbours, 2);
        // West neighbour
        if (m_stack.array[m_stack.top].x > 0 && (m_cells[offset(-1, 0)] & CELL_VISITED) == 0)
            neighbours_stack_push(&neighbours, 3);

        // Are there any neighbours available?
        if (neighbours.top != -1)
        {
            // Choose one available neighbour at random
            srand(time(NULL)); // Seed the random number generator with the current time
            int next_cell_dir = neighbours.array[rand() % (neighbours.top + 1)];

            switch (next_cell_dir)
            {
            case 0: // North
                m_cells[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
                m_cells[offset(0, 0)] |= CELL_PATH_N;
                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x + 0, m_stack.array[m_stack.top].y - 1);
                break;

            case 1: // East
                m_cells[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
                m_cells[offset(0, 0)] |= CELL_PATH_E;
                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x + 1, m_stack.array[m_stack.top].y + 0);
                break;

            case 2: // South
                m_cells[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
                m_cells[offset(0, 0)] |= CELL_PATH_S;
                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x + 0, m_stack.array[m_stack.top].y + 1);
                break;

            case 3: // West
                m_cells[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
                m_cells[offset(0, 0)] |= CELL_PATH_W;
                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x - 1, m_stack.array[m_stack.top].y + 0);
                break;
            }

            NumVisitedCells++;
        }
        else
        {
            // No available neighbours so backtrack!
            maze_stack_pop(&m_stack);
        }
        // Draw maze
        for (size_t x = 0; x < MazeWidth; x++)
        {
            for (size_t y = 0; y < MazeHeight; y++)
            {
                for (size_t px = 0; px < PathWidth; px++)
                {
                    for (size_t py = 0; py < PathWidth; py++)
                    {
                        if (m_cells[y * MazeWidth + x] & CELL_VISITED)
                            drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 0, 0, 255, 8);
                        else
                            drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 255, 0, 0, 8);
                    }
                }

                for (int p = 0; p < PathWidth; p++)
                {
                    if (m_cells[y * MazeWidth + x] & CELL_PATH_S)
                        drawPseudoPixel(renderer, x * (PathWidth + 1) + p, y * (PathWidth + 1) + PathWidth, 0, 0, 255, 8); // Draw South Passage
                    if (m_cells[y * MazeWidth + x] & CELL_PATH_E)
                        drawPseudoPixel(renderer, x * (PathWidth + 1) + PathWidth, y * (PathWidth + 1) + p, 0, 0, 255, 8); // Draw East Passage
                }
            }
        }
    }

    int running = 1;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                // Check the specific key that was pressed
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    printf("Up arrow key was pressed.\n");
                    break;
                case SDLK_DOWN:
                    printf("Down arrow key was pressed.\n");
                    break;
                case SDLK_LEFT:
                    printf("Left arrow key was pressed.\n");
                    break;
                case SDLK_RIGHT:
                    printf("Right arrow key was pressed.\n");
                    break;
                case SDLK_SPACE:
                    printf("Space key was pressed.\n");
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                // Check the specific key that was released
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    printf("Up arrow key was released.\n");
                    break;
                case SDLK_DOWN:
                    printf("Down arrow key was released.\n");
                    break;
                case SDLK_LEFT:
                    printf("Left arrow key was released.\n");
                    break;
                case SDLK_RIGHT:
                    printf("Right arrow key was released.\n");
                    break;
                case SDLK_SPACE:
                    printf("Space key was released.\n");
                    break;
                default:
                    break;
                }
            }
        }
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

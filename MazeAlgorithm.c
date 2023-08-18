#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "MazeGeneratorStuff.h"

#define WINDOW_WIDTH 0.8*displayMode.h
#define WINDOW_HEIGHT 0.8*displayMode.h

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    window = SDL_CreateWindow("Maze Algorithm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window creation failed: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return -1;
    }

    int Width = 25, Height = 25, PathWidth = 3;
    int *cells = calloc(Width * Height, sizeof(int));
    int NumVisitedCells;
    MazeStack m_stack;
    init_MazeStack(&m_stack, Width * Height);

    srand(time(NULL));
    int x = rand() % Width, y = rand() % Height;

    maze_stack_push(&m_stack, x, y);
    cells[y * Width + x] = CELL_VISITED;
    NumVisitedCells = 1;

    // Create Maze
    int last_cell_dir, count_repeated__cell_dir = 0, max_repeated_cell_dir;
    SDL_Event event;
    int run = 1;
    while (run)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                run = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    while (NumVisitedCells < Width * Height)
                    {
                        // Create a set of unvisted neighbours
                        NeighboursStack neighbours;
                        init_NeighboursStack(&neighbours);
                        // North neighbour
                        if (m_stack.array[m_stack.top].y > 0 && (cells[offset(0, -1)] & CELL_VISITED) == 0)
                            neighbours_stack_push(&neighbours, 0);
                        // East neighbour
                        if (m_stack.array[m_stack.top].x < Width - 1 && (cells[offset(1, 0)] & CELL_VISITED) == 0)
                            neighbours_stack_push(&neighbours, 1);
                        // South neighbour
                        if (m_stack.array[m_stack.top].y < Height - 1 && (cells[offset(0, 1)] & CELL_VISITED) == 0)
                            neighbours_stack_push(&neighbours, 2);
                        // West neighbour
                        if (m_stack.array[m_stack.top].x > 0 && (cells[offset(-1, 0)] & CELL_VISITED) == 0)
                            neighbours_stack_push(&neighbours, 3);

                        // Any neighbours available?
                        if (neighbours.top != -1)
                        {
                            // Choose one available neighbour at random
                            int next_cell_dir = neighbours.array[rand() % (neighbours.top + 1)];
                            max_repeated_cell_dir = rand() % ((Width / 3 < Height / 3) ? Width / 3 : Height / 3) + 1;

                            if (next_cell_dir == last_cell_dir)
                            {
                                count_repeated__cell_dir++;
                            }
                            if ((count_repeated__cell_dir >= max_repeated_cell_dir && neighbours.top > 0) || (rand() % 4 == 0 && neighbours.top > 0))
                            {
                                do
                                {
                                    next_cell_dir = neighbours.array[rand() % (neighbours.top + 1)];
                                } while (next_cell_dir == last_cell_dir);

                                count_repeated__cell_dir = 0;
                            }

                            switch (next_cell_dir)
                            {
                            case 0: // North
                                cells[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
                                cells[offset(0, 0)] |= CELL_PATH_N;
                                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x + 0, m_stack.array[m_stack.top].y - 1);
                                break;

                            case 1: // East
                                cells[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
                                cells[offset(0, 0)] |= CELL_PATH_E;
                                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x + 1, m_stack.array[m_stack.top].y + 0);
                                break;

                            case 2: // South
                                cells[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
                                cells[offset(0, 0)] |= CELL_PATH_S;
                                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x + 0, m_stack.array[m_stack.top].y + 1);
                                break;

                            case 3: // West
                                cells[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
                                cells[offset(0, 0)] |= CELL_PATH_W;
                                maze_stack_push(&m_stack, m_stack.array[m_stack.top].x - 1, m_stack.array[m_stack.top].y + 0);
                                break;
                            }

                            last_cell_dir = next_cell_dir;

                            NumVisitedCells++;
                        }
                        else
                        {
                            // No available neighbours so backtrack
                            maze_stack_pop(&m_stack);
                        }
                        for (int x = 0; x < Width; x++)
                        {
                            for (int y = 0; y < Height; y++)
                            {
                                for (int px = 0; px < PathWidth; px++)
                                {
                                    for (int py = 0; py < PathWidth; py++)
                                    {
                                        if (cells[y * Width + x] & CELL_VISITED)
                                            drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 255, 255, 255, WINDOW_HEIGHT/100);
                                        else
                                            drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 0, 0, 255, WINDOW_HEIGHT/100);
                                        drawPseudoPixel(renderer, m_stack.array[m_stack.top].x * (PathWidth + 1) + px, m_stack.array[m_stack.top].y * (PathWidth + 1) + py, 255, 0, 0, WINDOW_HEIGHT/100);
                                    }
                                }

                                for (int p = 0; p < PathWidth; p++)
                                {
                                    if (cells[y * Width + x] & CELL_PATH_S)
                                        drawPseudoPixel(renderer, x * (PathWidth + 1) + p, y * (PathWidth + 1) + PathWidth, 255, 255, 255, WINDOW_HEIGHT/100); // Draw South Passage
                                    if (cells[y * Width + x] & CELL_PATH_E)
                                        drawPseudoPixel(renderer, x * (PathWidth + 1) + PathWidth, y * (PathWidth + 1) + p, 255, 255, 255, WINDOW_HEIGHT/100); // Draw East Passage
                                }
                            }
                        }
                        SDL_RenderPresent(renderer);
                    }
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

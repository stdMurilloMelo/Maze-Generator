#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze.h"

// gcc -o main main.c -I/mingw64/include/SDL2 -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2
// gcc -o main main.c -lSDL2

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
    SDL_Window *window = SDL_CreateWindow("Maze Generator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width/2, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Maze parameters
    unsigned int MazeWidth = 20, MazeHeight = 20, NumVisitedCells, PathWidth = 3;
    unsigned int *m_cells;
    m_cells = (unsigned int *)calloc(MazeWidth * MazeHeight, sizeof(unsigned int));
    MazeStack m_stack;
    init_MazeStack(&m_stack, MazeWidth * MazeHeight);
    srand(time(NULL));
    int x = rand() % MazeWidth, y = rand() % MazeHeight;
    maze_stack_push(&m_stack, x, y);
    m_cells[y * MazeWidth + x] = CELL_VISITED;
    NumVisitedCells = 1;

    // Create Maze
    int last_cell_dir, count_repeated__cell_dir = 0, max_repeated_cell_dir;
    while (NumVisitedCells < MazeWidth * MazeHeight)
    {
        // Create a set of unvisted neighbours
        NeighboursStack neighbours;
        init_NeighboursStack(&neighbours);
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
            max_repeated_cell_dir = rand() % ((MazeWidth/3 < MazeHeight/3) ? MazeWidth/3 : MazeHeight/3);

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

            last_cell_dir = next_cell_dir;

            NumVisitedCells++;
        }
        else
        {
            // No available neighbours so backtrack
            maze_stack_pop(&m_stack);
        }
    }
    for (int x = 1; x < MazeWidth - 1; x++)
    {
        for (int y = 1; y < MazeHeight - 1; y++)
        {
            srand(time(NULL));
            int do_it = rand() % 5;
            if (do_it == 0)
            {
                int dir = rand() % 4;
                switch (dir)
                {
                case 0:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_N;
                    m_cells[(y-1) * MazeWidth + x] |= CELL_PATH_S;
                    break;
                case 1:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_E;
                    m_cells[y * MazeWidth + (x+1)] |= CELL_PATH_W;
                    break;
                case 2:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_S;
                    m_cells[(y+1) * MazeWidth + x] |= CELL_PATH_N;
                    break;
                case 3:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_W;
                    m_cells[y * MazeWidth + (x-1)] |= CELL_PATH_E;
                    break;
                default:
                    break;
                }
                dir = rand() % 4;
                switch (dir)
                {
                case 0:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_N;
                    m_cells[(y-1) * MazeWidth + x] |= CELL_PATH_S;
                    break;
                case 1:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_E;
                    m_cells[y * MazeWidth + (x+1)] |= CELL_PATH_W;
                    break;
                case 2:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_S;
                    m_cells[(y+1) * MazeWidth + x] |= CELL_PATH_N;
                    break;
                case 3:
                    m_cells[y * MazeWidth + x] |= CELL_PATH_W;
                    m_cells[y * MazeWidth + (x-1)] |= CELL_PATH_E;
                    break;
                default:
                    break;
                }
            }
        }
    }
    m_cells[0] |= CELL_PATH_E;
    m_cells[m_cells[0 * MazeWidth + 1]] |= CELL_PATH_W;
    m_cells[0] |= CELL_PATH_S;
    m_cells[m_cells[1 * MazeWidth + 0]] |= CELL_PATH_N;

    // Draw maze
    for (int x = 0; x < MazeWidth; x++)
    {
        for (int y = 0; y < MazeHeight; y++)
        {
            for (int px = 0; px < PathWidth; px++)
            {
                for (int py = 0; py < PathWidth; py++)
                {
                    if (m_cells[y * MazeWidth + x] & CELL_VISITED)
                        drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 255, 255, 255, 6);
                    else
                        drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 255, 255, 255, 6);
                }
            }

            for (int p = 0; p < PathWidth; p++)
            {
                if (m_cells[y * MazeWidth + x] & CELL_PATH_S)
                    drawPseudoPixel(renderer, x * (PathWidth + 1) + p, y * (PathWidth + 1) + PathWidth, 255, 255, 255, 6); // Draw South Passage
                if (m_cells[y * MazeWidth + x] & CELL_PATH_E)
                    drawPseudoPixel(renderer, x * (PathWidth + 1) + PathWidth, y * (PathWidth + 1) + p, 255, 255, 255, 6); // Draw East Passage
            }
        }
    }
    for (int px = 0; px < PathWidth; px++)
    {
        for (int py = 0; py < PathWidth; py++)
        {
            drawPseudoPixel(renderer, 0 * (PathWidth + 1) + px, 0 * (PathWidth + 1) + py, 255, 0, 0, 6);
        }
    }
    for (int px = 0; px < PathWidth; px++)
    {
        for (int py = 0; py < PathWidth; py++)
        {
            drawPseudoPixel(renderer, (MazeWidth - 1) * (PathWidth + 1) + px, (MazeHeight - 1) * (PathWidth + 1) + py, 0, 255, 0, 6);
        }
    }
    for (int px = 0; px < PathWidth; px++)
    {
        for (int py = 0; py < PathWidth; py++)
        {
            drawPseudoPixel(renderer, (MazeWidth)/2 * (PathWidth + 1) + px, (MazeHeight)/2 * (PathWidth + 1) + py, 255, 255, 0, 6);
        }
    }
    SDL_RenderPresent(renderer);

    int running = 1;
    SDL_Event event;
    int x_position = 0, y_position = 0;
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
                    if (!(y_position == 0) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_N))
                    {
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 255, 255, 6);
                            }
                        }
                        y_position--;
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 0, 0, 6);
                            }
                        }
                        SDL_RenderPresent(renderer);
                    }
                    break;
                case SDLK_DOWN:
                    printf("Down arrow key was pressed.\n");
                    if (!(y_position == MazeHeight - 1) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_S))
                    {
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 255, 255, 6);
                            }
                        }
                        y_position++;
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 0, 0, 6);
                            }
                        }
                        SDL_RenderPresent(renderer);
                    }
                    break;
                case SDLK_LEFT:
                    printf("Left arrow key was pressed.\n");
                    if (!(x_position == 0) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_W))
                    {
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 255, 255, 6);
                            }
                        }
                        x_position--;
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 0, 0, 6);
                            }
                        }
                        SDL_RenderPresent(renderer);
                    }
                    break;
                case SDLK_RIGHT:
                    printf("Right arrow key was pressed.\n");
                    if (!(x_position == MazeWidth - 1) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_E))
                    {
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 255, 255, 6);
                            }
                        }
                        x_position++;
                        for (int px = 0; px < PathWidth; px++)
                        {
                            for (int py = 0; py < PathWidth; py++)
                            {
                                drawPseudoPixel(renderer, x_position * (PathWidth + 1) + px, y_position * (PathWidth + 1) + py, 255, 0, 0, 6);
                            }
                        }
                        SDL_RenderPresent(renderer);
                    }
                    break;
                case SDLK_SPACE:
                    printf("Space key was pressed.\n");
                    break;
                default:
                    break;
                }
            }
            /* else if (event.type == SDL_KEYUP)
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
            } */
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

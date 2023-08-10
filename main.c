#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze.h"

// gcc -o main main.c -I/mingw64/include/SDL2 -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2
// gcc -o main main.c -lSDL2



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
    unsigned int *m_cells, MazeWidth = 20, MazeHeight = 20, PathWidth = 3;
    m_cells = (unsigned int *)calloc(MazeWidth * MazeHeight, sizeof(unsigned int));
    
    // Create maze
    create_maze(m_cells, MazeWidth, MazeHeight);

    // Draw maze
    draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth);
    
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
                    break;
                default:
                    break;
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

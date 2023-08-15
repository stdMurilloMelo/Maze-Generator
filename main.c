#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Window parameters
    SDL_Init(SDL_INIT_VIDEO);
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    int window_height = 0.8 * displayMode.h, window_width = window_height;

    // Maze parameters
    int *m_cells;
    int MazeWidth, MazeHeight, PathWidth = 3;
    scanf("%d", &MazeHeight);
    MazeWidth = MazeHeight;
    int PixelSize = (window_height) / (4.0 * MazeHeight);
    m_cells = (int *)calloc(MazeWidth * MazeHeight, sizeof(int));
    m_cells[0] |= CURRENT_CELL;

    // Create window
    SDL_Window *window = SDL_CreateWindow("Maze Generator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create maze
    create_maze(m_cells, MazeWidth, MazeHeight);

    // Draw maze
    draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);

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
                        m_cells[y_position * MazeWidth + x_position] &= ~CURRENT_CELL;
                        y_position--;
                        m_cells[y_position * MazeWidth + x_position] |= CURRENT_CELL;
                        draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);
                    }
                    break;
                case SDLK_DOWN:
                    if (!(y_position == MazeHeight - 1) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_S))
                    {
                        m_cells[y_position * MazeWidth + x_position] &= ~CURRENT_CELL;
                        y_position++;
                        m_cells[y_position * MazeWidth + x_position] |= CURRENT_CELL;
                        draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);
                    }
                    break;
                case SDLK_LEFT:
                    if (!(x_position == 0) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_W))
                    {
                        m_cells[y_position * MazeWidth + x_position] &= ~CURRENT_CELL;
                        x_position--;
                        m_cells[y_position * MazeWidth + x_position] |= CURRENT_CELL;
                        draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);
                    }
                    break;
                case SDLK_RIGHT:
                    if (!(x_position == MazeWidth - 1) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_E))
                    {
                        m_cells[y_position * MazeWidth + x_position] &= ~CURRENT_CELL;
                        x_position++;
                        m_cells[y_position * MazeWidth + x_position] |= CURRENT_CELL;
                        draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);
                    }
                    break;
                case SDLK_SPACE:
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    window_width = event.window.data1;
                    window_height = event.window.data2;
                    PixelSize = (window_height) / (4.0 * MazeHeight); // fix it later
                    SDL_DestroyRenderer(renderer);
                    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                    draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);
                }
            }
        }
        if ((x_position == MazeWidth - 1) && (y_position == MazeHeight - 1))
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
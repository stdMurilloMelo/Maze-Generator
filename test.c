#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "maze.h"

#define WINDOW_WIDTH 0.8 * displayMode.w
#define WINDOW_HEIGHT 0.8 * displayMode.h

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *textTexture = NULL;

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1)
    {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        return -1;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    window = SDL_CreateWindow("Maze Generator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/teluguvijayam/NATS.ttf", 36);
    if (!font)
    {
        printf("Font loading failed\n");
        return -1;
    }

    bool running = true;
    while (running)
    {
        // Get input in menu
        bool menu = true;
        SDL_Color textColor = {255, 255, 255, 255};
        char inputText[30] = "\0";
        int inputNumber = 0;
        SDL_Event menu_event;
        while (menu)
        {
            while (SDL_PollEvent(&menu_event) != 0)
            {
                if (menu_event.type == SDL_QUIT)
                {
                    menu = false;
                    running = false;
                }
                else if (menu_event.type == SDL_KEYDOWN)
                {
                    if (menu_event.key.keysym.sym == SDLK_RETURN)
                    {
                        sscanf(inputText, "%d", &inputNumber);
                        if (inputNumber > 2)
                            menu = false;
                    }
                    else if (menu_event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0)
                    {
                        inputText[strlen(inputText) - 1] = '\0';
                    }
                    else if (menu_event.key.keysym.sym >= SDLK_0 && menu_event.key.keysym.sym <= SDLK_9)
                    {
                        strcat(inputText, SDL_GetKeyName(menu_event.key.keysym.sym));
                    }
                }
            }

            SDL_DestroyRenderer(renderer);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            char displayText[50];
            sprintf(displayText, "Type maze size: %s", inputText);
            renderText(renderer, font, displayText, textColor);

            SDL_Rect textRect = {0, 0, 0, 0};
            SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
            textRect.x = (WINDOW_WIDTH - textRect.w) / 2;
            textRect.y = (WINDOW_HEIGHT - textRect.h) / 2;

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);
        }

        bool maze = true;
        if (!running)
            maze = false;
        while (maze)
        {
            int *m_cells;
            int MazeWidth = inputNumber, MazeHeight = inputNumber, PathWidth = 3;
            int PixelSize = SDL_round(WINDOW_HEIGHT) / (4 * MazeHeight);
            m_cells = (int *)calloc(MazeWidth * MazeHeight, sizeof(int));
            m_cells[0] |= CURRENT_CELL;
            create_maze(m_cells, MazeWidth, MazeHeight);

            SDL_DestroyRenderer(renderer);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);

            bool play_maze = true;
            SDL_Event play_event;
            int x_position = 0, y_position = 0;
            while (play_maze)
            {
                while (SDL_PollEvent(&play_event))
                {
                    if (play_event.type == SDL_QUIT)
                    {
                        play_maze = false;
                        maze = false;
                        running = false;
                    }
                    else if (play_event.type == SDL_KEYDOWN)
                    {
                        switch (play_event.key.keysym.sym)
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
                            play_maze = false;
                            maze = false;
                            free(m_cells);
                            break;
                        case SDLK_g:
                            saveMazeAsSVG("test.svg", m_cells, MazeWidth, MazeHeight, PathWidth);
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

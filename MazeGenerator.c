#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "MazeGeneratorStuff.h"

#define WINDOW_WIDTH 0.8 * displayMode.w
#define WINDOW_HEIGHT 0.8 * displayMode.h

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *textTexture = NULL;

void renderText(SDL_Renderer *renderer, TTF_Font *menufont, const char *text, SDL_Color color)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(menufont, text, color);
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

    TTF_Font *menufont = TTF_OpenFont("/usr/share/fonts/truetype/teluguvijayam/NATS.ttf", WINDOW_WIDTH / 10);
    if (!menufont)
    {
        printf("Font loading failed\n");
        return -1;
    }

    TTF_Font *mazefont = TTF_OpenFont("/usr/share/fonts/truetype/teluguvijayam/NATS.ttf", WINDOW_WIDTH / 25);
    if (!mazefont)
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
            renderText(renderer, menufont, displayText, textColor);

            SDL_Rect textRect = {0, 0, 0, 0};
            SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
            textRect.x = (WINDOW_WIDTH - textRect.w) / 2;
            textRect.y = (WINDOW_HEIGHT - textRect.h) / 2;

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            char minText[] = "(min=3)";
            SDL_Surface *minSurface = TTF_RenderText_Solid(menufont, minText, textColor);
            SDL_Texture *minTexture = SDL_CreateTextureFromSurface(renderer, minSurface);

            SDL_Rect minRect = {0, 0, 0, 0};
            SDL_QueryTexture(minTexture, NULL, NULL, &minRect.w, &minRect.h);
            minRect.x = (WINDOW_WIDTH - minRect.w) / 2;
            minRect.y = textRect.y + textRect.h;

            SDL_RenderCopy(renderer, minTexture, NULL, &minRect);

            SDL_FreeSurface(minSurface);
            SDL_DestroyTexture(minTexture);
            SDL_DestroyTexture(textTexture);

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

            SDL_Color maze_textColor = {255, 255, 255, 255};
            char moveText[] = "Arrows or WASD: move";
            SDL_Surface *moveSurface = TTF_RenderText_Solid(mazefont, moveText, maze_textColor);
            SDL_Texture *moveTexture = SDL_CreateTextureFromSurface(renderer, moveSurface);

            SDL_Rect moveRect = {0, 0, 0, 0};
            SDL_QueryTexture(moveTexture, NULL, NULL, &moveRect.w, &moveRect.h);
            moveRect.x = (WINDOW_WIDTH / 2 - moveRect.w) / 2 + WINDOW_WIDTH / 2;
            moveRect.y = (WINDOW_HEIGHT - moveRect.h) / 4;

            SDL_RenderCopy(renderer, moveTexture, NULL, &moveRect);

            char quitText[] = "Esc: return to menu";
            SDL_Surface *quitSurface = TTF_RenderText_Solid(mazefont, quitText, maze_textColor);
            SDL_Texture *quitTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);

            SDL_Rect quitRect = {0, 0, 0, 0};
            SDL_QueryTexture(quitTexture, NULL, NULL, &quitRect.w, &quitRect.h);
            quitRect.x = (WINDOW_WIDTH / 2 - quitRect.w) / 2 + WINDOW_WIDTH / 2;
            quitRect.y = moveRect.y + moveRect.h;

            SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

            char saveText[] = "Space: save maze";
            SDL_Surface *saveSurface = TTF_RenderText_Solid(mazefont, saveText, maze_textColor);
            SDL_Texture *saveTexture = SDL_CreateTextureFromSurface(renderer, saveSurface);

            SDL_Rect saveRect = {0, 0, 0, 0};
            SDL_QueryTexture(saveTexture, NULL, NULL, &saveRect.w, &saveRect.h);
            saveRect.x = (WINDOW_WIDTH / 2 - saveRect.w) / 2 + WINDOW_WIDTH / 2;
            saveRect.y = quitRect.y + quitRect.h;

            SDL_RenderCopy(renderer, saveTexture, NULL, &saveRect);

            SDL_RenderPresent(renderer);

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
                        case SDLK_w:
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
                        case SDLK_s:
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
                        case SDLK_a:
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
                        case SDLK_d:
                            if (!(x_position == MazeWidth - 1) && (m_cells[y_position * MazeWidth + x_position] & CELL_PATH_E))
                            {
                                m_cells[y_position * MazeWidth + x_position] &= ~CURRENT_CELL;
                                x_position++;
                                m_cells[y_position * MazeWidth + x_position] |= CURRENT_CELL;
                                draw_maze(renderer, m_cells, MazeWidth, MazeHeight, PathWidth, PixelSize);
                            }
                            break;
                        case SDLK_ESCAPE:
                            play_maze = false;
                            maze = false;
                            free(m_cells);
                            break;
                        case SDLK_SPACE:
                            int check1 = saveMazeAsSVG("MazeImage.svg", m_cells, MazeWidth, MazeHeight, PathWidth);
                            int check2 = saveMazeData("MazeData.dat", m_cells, MazeWidth, MazeHeight);

                            char *save_msgText;
                            SDL_Color save_msgColor = {0, 0, 0, 255};

                            if (check1 == 0 || check2 == 0)
                            {
                                save_msgText = "ERROR: Maze not Saved!";
                                save_msgColor.r = 255;
                            }
                            else
                            {
                                save_msgText = "Maze Saved!";
                                save_msgColor.g = 255;
                            }

                            SDL_Surface *save_msgSurface = TTF_RenderText_Solid(mazefont, save_msgText, save_msgColor);
                            SDL_Texture *save_msgTexture = SDL_CreateTextureFromSurface(renderer, save_msgSurface);

                            SDL_Rect save_msgRect = {0, 0, 0, 0};
                            SDL_QueryTexture(save_msgTexture, NULL, NULL, &save_msgRect.w, &save_msgRect.h);
                            save_msgRect.x = (WINDOW_WIDTH / 2 - save_msgRect.w) / 2 + WINDOW_WIDTH / 2;
                            save_msgRect.y = saveRect.y + saveRect.h;

                            SDL_RenderCopy(renderer, save_msgTexture, NULL, &save_msgRect);

                            SDL_RenderPresent(renderer);
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    TTF_CloseFont(menufont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

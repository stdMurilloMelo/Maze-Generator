#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define offset(x0, y0) (m_stack.array[m_stack.top].y + y0) * Width + (m_stack.array[m_stack.top].x + x0)

enum
{
    CELL_PATH_N = 0b1,
    CELL_PATH_E = 0b10,
    CELL_PATH_S = 0b100,
    CELL_PATH_W = 0b1000,
    CELL_VISITED = 0b10000,
    CURRENT_CELL = 0b100000,
};

typedef struct cell_pair
{
    int x;
    int y;
} CellPair;

typedef struct maze_stack
{
    int top;
    CellPair *array;
} MazeStack;

void init_MazeStack(MazeStack *stack, int stack_size)
{
    stack->top = -1;
    stack->array = (CellPair *)calloc(stack_size, sizeof(CellPair));
}

void maze_stack_push(MazeStack *stack, int x0, int y0)
{
    stack->top++;
    (stack->array[stack->top]).x = x0;
    (stack->array[stack->top]).y = y0;
}

void maze_stack_pop(MazeStack *stack)
{
    stack->top--;
}

typedef struct maze_neighbours_stack
{
    int top;
    int array[4];
} NeighboursStack;

void init_NeighboursStack(NeighboursStack *stack)
{
    stack->top = -1;
}

void neighbours_stack_push(NeighboursStack *stack, int element)
{
    stack->top++;
    stack->array[stack->top] = element;
}

void neighbours_stack_pop(NeighboursStack *stack)
{
    stack->top--;
}

void create_maze(int *cells, int Width, int Height)
{
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
            max_repeated_cell_dir = rand() % ((Width / 3 < Height / 3) ? Width / 3 : Height / 3);

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
    }

    for (int x = 1; x < Width - 1; x++)
    {
        for (int y = 1; y < Height - 1; y++)
        {
            int add_path = rand() % 50;
            if (add_path == 0)
            {
                int dir = rand() % 4;
                switch (dir)
                {
                case 0:
                    cells[y * Width + x] |= CELL_PATH_N;
                    cells[(y - 1) * Width + x] |= CELL_PATH_S;
                    break;
                case 1:
                    cells[y * Width + x] |= CELL_PATH_S;
                    cells[(y + 1) * Width + x] |= CELL_PATH_N;
                    break;
                case 2:
                    cells[y * Width + x] |= CELL_PATH_E;
                    cells[y * Width + (x + 1)] |= CELL_PATH_W;
                    break;
                case 3:
                    cells[y * Width + x] |= CELL_PATH_W;
                    cells[y * Width + (x - 1)] |= CELL_PATH_E;
                    break;
                default:
                    break;
                }
            }
        }
    }
    free(m_stack.array);
}

void drawPseudoPixel(SDL_Renderer *renderer, int x, int y, int r, int g, int b, int size)
{
    // Set the color of the square
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Draw the square
    SDL_Rect squareRect = {x * size, y * size, size, size};
    SDL_RenderFillRect(renderer, &squareRect);
}

void draw_maze(SDL_Renderer *renderer, int *cells, int Width, int Height, int PathWidth, int PixelSize)
{
    for (int x = 0; x < Width; x++)
    {
        for (int y = 0; y < Height; y++)
        {
            for (int px = 0; px < PathWidth; px++)
            {
                for (int py = 0; py < PathWidth; py++)
                {
                    if (cells[y * Width + x] & CELL_VISITED)
                        drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 255, 255, 255, PixelSize);
                    else
                        drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 0, 0, 255, PixelSize);
                    if (cells[y * Width + x] & CURRENT_CELL)
                        drawPseudoPixel(renderer, x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 255, 0, 0, PixelSize);
                }
            }

            for (int p = 0; p < PathWidth; p++)
            {
                if (cells[y * Width + x] & CELL_PATH_S)
                    drawPseudoPixel(renderer, x * (PathWidth + 1) + p, y * (PathWidth + 1) + PathWidth, 255, 255, 255, PixelSize); // Draw South Passage
                if (cells[y * Width + x] & CELL_PATH_E)
                    drawPseudoPixel(renderer, x * (PathWidth + 1) + PathWidth, y * (PathWidth + 1) + p, 255, 255, 255, PixelSize); // Draw East Passage
            }
        }
    }
    // green cell at the end
    for (int px = 0; px < PathWidth; px++)
    {
        for (int py = 0; py < PathWidth; py++)
        {
            drawPseudoPixel(renderer, (Width - 1) * (PathWidth + 1) + px, (Height - 1) * (PathWidth + 1) + py, 0, 255, 0, PixelSize);
        }
    }
    SDL_RenderPresent(renderer);
}

#define drawPixelSVG(x, y, size) fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"white\" />\n", x, y, size, size)

void saveMazeAsSVG(const char *filename, int *cells, int Width, int Height, int PathWidth)
{
    int sz = 500 / (4 * Height);
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.");
        return;
    }

    fprintf(file, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(file, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
                  "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
    fprintf(file, "<svg width=\"500\" height=\"500\" xmlns=\"http://www.w3.org/2000/svg\">\n");

    fprintf(file, "<rect x=\" 0 \" y=\" 0 \" width=\" 500 \" height=\" 500 \" fill=\" black \" />\n");

    for (int x = 0; x < Width; x++)
    {
        for (int y = 0; y < Height; y++)
        {
            for (int px = 0; px < PathWidth; px++)
            {
                for (int py = 0; py < PathWidth; py++)
                {
                    drawPixelSVG((x * (PathWidth + 1) + px) * sz, (y * (PathWidth + 1) + py) * sz, sz);
                }
            }

            for (int p = 0; p < PathWidth; p++)
            {
                if (cells[y * Width + x] & CELL_PATH_S)
                    drawPixelSVG((x * (PathWidth + 1) + p) * sz, (y * (PathWidth + 1) + PathWidth) * sz, sz); // Draw South Passage
                if (cells[y * Width + x] & CELL_PATH_E)
                    drawPixelSVG((x * (PathWidth + 1) + PathWidth) * sz, (y * (PathWidth + 1) + p) * sz, sz); // Draw East Passage
            }
        }
    }
    fprintf(file, "</svg>\n");

    fclose(file);
}

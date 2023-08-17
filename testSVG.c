#include <stdio.h>
#include "maze.h"

#define drawPixelSVG(x, y, size) fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"white\" />\n", x, y, size, size)

void saveMazeAsSVG(const char *filename, int *cells, int Width, int Height, int PathWidth, int PixelSize)
{
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
                    drawPixelSVG(x * (PathWidth + 1) + px, y * (PathWidth + 1) + py, 500 / (4 * Height));
                }
            }

            for (int p = 0; p < PathWidth; p++)
            {
                if (cells[y * Width + x] & CELL_PATH_S)
                    drawPixelSVG(x * (PathWidth + 1) + p, y * (PathWidth + 1), 500 / (4 * Height)); // Draw South Passage
                if (cells[y * Width + x] & CELL_PATH_E)
                    drawPixelSVG(x * (PathWidth + 1) + p, y * (PathWidth + 1) + p, 500 / (4 * Height)); // Draw East Passage
            }
        }
    }

    fprintf(file, "</svg>\n");

    fclose(file);
}

int main()
{
    // Suppose you have generated mazeSVGContent as the SVG content.
    int *mazeSVGContent = {0};

    saveMazeAsSVG("maze.svg", mazeSVGContent, 10, 10, 3);

    return 0;
}

#define drawPixelSVG(x, y, size) fprintf(file, "<path d=\"M%d %d L%d %d L%d %d L%d %d Z\" fill=\"white\" />\n", x*size, y*size, (x+1)*size, y*size, (x+1)*size, (y+1)*size, x*size, (y+1)*size)

/*
 * termip.h
 *
 *  Created on: 12 de mai de 2017
 *      Author: Gustavo Teodoro Laureano
 *      email: gustavo@inf.ufg.br
 *
 *		>> CSI codes description
 *      https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 *      http://wiki.bash-hackers.org/scripting/terminalcodes
 *
 *		>> List of UNICODE characters
 *      https://en.wikipedia.org/wiki/List_of_Unicode_characters
 *      https://en.wikipedia.org/wiki/Box-drawing_character
 */

#ifndef _TERMIP_H_
#define _TERMIP_H_

#include "termip_defs.h"

/*********************************************************************
 * Auxiliary Functions
 *********************************************************************/

/**
 * Same of getchar without '\n'. Get the key immediately pressed.
 * 
 * @return ASCII code of pressed key.
 */
int getch();

/**
 * Same of getchar without '\n' and with timeout delay.
 * It gets the key pressed. This function waits a key
 * in time interval sec (in seconds) + usec (in microseconds).
 * If neither key is pressed it returns -1.
 * @param sec seconds
 * @param usec microseconds
 * @return code of key pressed
 */
int getch_timeout( int sec, int usec );

/*
 * Clear screen and moves cursor to home.
 * Screen is fully with previous background color.
 */
void clear();

/**
 * Moves cursor to position (row,col)
 *
 * @param row
 * @param col
 */
void gotoxy(int row, int col);

/**
 * Set foreground color using a color from definition type COLOR_*
 *
 * @param color is a definition from COLOR_*
 * @param b 0 for half bright and 1 to high bright
 */
void set_fg(int color, char b);

/**
 * Set background color using a color from definition type COLOR_*
 *
 * @param color is a definition from COLOR_*
 * @param b 0 for half bright and 1 to high bright
 */
void set_bg(int color, char b);

/**
 * Set foreground color using predefined 256-color mode
 *
 * @param color: 	0-7 	for Standard colors (half bright)
 * 					8-15 	for High-intensity colors (full bright)
 * 					16-231 	from color palette
 * 					232-255 for gray scalecolors (black to white)
 */
void set_fg_color_mode(int color);

/**
 * Set background color using predefined 256-color mode
 *
 * @param color: 	0-7 	for Standard colors (half bright)
 * 					8-15 	for High-intensity colors (full bright)
 * 					16-231 	from color palette
 * 					232-255 for gray scalecolors (black to white)
 */
void set_bg_color_mode(int color);

/**
 * Set foreground color using R G B
 *
 * @param r 	red channel (0-255)
 * @param g 	green channel (0-255)
 * @param b 	blue channel (0-255)
 */
void set_fg_rgb(int r, int g, int b);

/**
 * Set background color using R G B
 *
 * @param r 	red channel (0-255)
 * @param g 	green channel (0-255)
 * @param b 	blue channel (0-255)
 */
void set_bg_rgb(int r, int g, int b);

/**
 * Get number of columns of terminal window
 * @return number of columns
 */
int get_terminal_ncols();

/**
 * Get number of rows of terminal window
 * @return number of rows
 */
int get_terminal_nrows();

void cursor_up(int n);
void cursor_down(int n);
void cursor_righ(int n);
void cursor_left(int n);

void scrow_up(int n);
void scrow_down(int n);

/**
 * Draw window border
 *
 * @param r
 * @param c
 * @param width
 * @param height
 * @param name
 */
void draw_window_border(int r, int c, int width, int height, const char * name);

void draw_window(int row, int col, int width, int height, const char * border, const char * back, const char * name);

void eval( const char * code );
#endif /* _TERMIP_H_ */

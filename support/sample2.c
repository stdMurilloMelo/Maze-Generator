
#include "termip.h"

#include <stdio.h>

int main() {
    
    int nrows, ncols;
    int key, c = ' ';
    int count = 0;
	int R = 10, C = 10;

	eval( BG_DEFAULT FG_DEFAULT CURSOR_INVISIBLE );
    clear();

    while(1) {

		eval( BG_DEFAULT FG_DEFAULT CURSOR_INVISIBLE );
        clear();

        nrows = get_terminal_nrows();
        ncols = get_terminal_ncols();

		// Imprime borda no terminal
        draw_window_border(1, 1, ncols, nrows, " Terminal "  );

        // Win1
		draw_window(3, 3, 20, 5, FG_CYAN, BG_DEFAULT, "|" ATTR_BLINK_ON ATTR_ITALIC_ON " info " ATTR_ITALIC_OFF ATTR_BLINK_OFF "|");
        gotoxy(5,5);
        printf("linhas: " FG_YELLOW "%d" FG_DEFAULT, nrows);
        gotoxy(6,5);
        printf("colunas: " FG_YELLOW "%d" FG_DEFAULT, ncols);
		
        // Win2
        draw_window(3, 25, ncols-25-1, nrows-3-1, FG_MAGENTA ATTR_BOLD, BG_cyan, " World Map ");
        gotoxy(5,27);
        printf( BG_cyan "Ola mundo...");

        // Win3
        draw_window(9, 3, 20, 5, FG_red ATTR_BOLD, BG_black, " ASCII ");
		gotoxy(11,5);
		printf( FG_cyan "code: " FG_YELLOW "%d", c );
		gotoxy(12,5);
		printf( FG_cyan "symbol: " FG_YELLOW "'%c'", c );
	
        // Win4
        draw_window(15, 3, 20, 4, FG_WHITE, BG_DEFAULT, " clock ");
        gotoxy(17,6);
        printf( FG_YELLOW "%2dm %2ds %03dms", count/(60*10)%12, (count/10)%(60), (count%10)*100  );
		
		// Win5
        draw_window(20, 3, 20, 3, FG_WHITE, BG_DEFAULT, " " FG_RED ATTR_BOLD ATTR_UNDERLINE_ON "cursor" FG_WHITE ATTR_UNDERLINE_OFF " ");
		gotoxy(22,8);
        printf( FG_YELLOW "%d,%d", R,C );

		gotoxy(R,C);
		printf( BG_cyan FG_RGB(0,0,0) "|+|" );

		// Descarrega todo o texto impresso pelo printf no terminal
		fflush(stdout);
		
		// Espera uma tecla ser pressionada em um intervalo de 200ms
        key = getch_timeout(1,200000);
		if( key != -1 ) c= key;
		count += 2;

		// Finaliza o programa (27 = ESC)
        if( key == 27 ) break;


		if( key == 23361 ) { // UP
			R--;
		} else if( key == 23362 ) {	// DOWN
			R++;
		} else if( key == 23363 ) {	// RIGHT
			C++;
		} else if( key == 23364 ) {	// LEFT
			C--;
		}

		// Limita posicao do cursor para ficar dentro da janela World Map
		if( R < 4 ) R = 4;
		if( R > nrows - 2 ) R = nrows-2;
		if( C < 26 ) C = 26;
		if( C > ncols-4) C = ncols-4;

    }

    eval( ATTR_RESET_ALL CURSOR_VISIBLE );
    gotoxy(nrows,1);
	eval("\n");

    return 0;
}

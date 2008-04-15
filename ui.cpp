/* C */
#include <curses.h>

/* C++ */
#include <cstdio>
#include "socktroll.hpp"


/* Windows */
WINDOW *logwin;   /* "paper trail" */
WINDOW *inputwin; /* cmd line */

void ui_init( void )
{
    initscr();
    logwin = newwin( 22, 80, 0, 0 );
    inputwin = newwin( 2, 80, 22, 0 );
    keypad( inputwin, TRUE );
    scrollok( logwin, TRUE );
    echo();
    mvwhline( inputwin, 0, 0, ACS_HLINE, 80 );
    ui_refresh();
}

void ui_stop( void )
{
    endwin();
}

void ui_refresh( void )
{
    /* update the windows */
    wrefresh( logwin );
    wrefresh( inputwin );
}

void ui_print( char *fmt, ... )
{
    va_list ap;
    
    va_start( ap, fmt );
    vwprintw( logwin, fmt, ap );
    va_end( ap );
    wprintw( logwin, "\n" );
    ui_refresh();
}

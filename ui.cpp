/* C++ */
#include <cstdio>
#include <string>
#include "socktroll.hpp"


UI::UI( void )
{
    initscr();
    logwin = newwin( 22, 80, 0, 0 );
    inputwin = newwin( 2, 80, 22, 0 );
    keypad( inputwin, TRUE );
    scrollok( logwin, TRUE );
    echo();
    mvwhline( inputwin, 0, 0, ACS_HLINE, 80 );
    this->refresh();
}

UI::~UI( void )
{
    endwin();
}

void UI::refresh( void )
{
    /* update the windows */
    wrefresh( logwin );
    wrefresh( inputwin );
}

void UI::print( char *fmt, ... )
{
    va_list ap;
    
    va_start( ap, fmt );
    vwprintw( logwin, fmt, ap );
    va_end( ap );
    wprintw( logwin, "\n" );
    this->refresh();
}

std::string UI::input( void )
{
    /*
      FIXME: Not sure that this is how we should handle input
             Maybe a mvwgetch is better
    */
    int x; //, buf;
    char buf[500];

    mvwgetnstr( inputwin, 1, 0, buf, 500 );

    //buf = wgetch( inputwin ); /* FIXME: local echo and internal buffer */

    //if( buf == '\n' )
    for( x = 0; x <= 81; x++ )
        mvwprintw( inputwin, 1, x, " " );

    this->refresh();
    
    return std::string( buf );
}

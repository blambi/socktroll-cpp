/* C++ */
#include <cstdio>
#include <string>
#include "socktroll.hpp"


UI::UI( void )
{
    setlocale( LC_CTYPE, "" ); /* WHY!!! */
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

Glib::ustring UI::input( void )
{
    int x;
    gunichar raw_buf[500]; /* wint_t is the same as gunichar */
    Glib::ustring buf;    

    mvwgetn_wstr( inputwin, 1, 0, raw_buf, 500 );

    for( x = 0; x <= 81; x++ )
        mvwprintw( inputwin, 1, x, " " );

    this->refresh();

    for( x = 0; x < 500; x++ )
    {
        if( raw_buf[x] == NULL )
            break;
        buf.append( 1, raw_buf[x] );
    }

    return buf;
}

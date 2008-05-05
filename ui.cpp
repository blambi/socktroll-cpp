/* C++ */
#include <cstdio>
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

void UI::print( Glib::ustring line )
{
    wprintw( logwin, "%s\n", line.c_str() );
    this->refresh();
}

Glib::ustring UI::input( void )
{
    int x;
    gunichar raw_buf[500]; /* wint_t is the same as gunichar */
    Glib::ustring buf;    

    if( mvwgetn_wstr( inputwin, 1, 0, raw_buf, 500 ) != OK )
    {
        this->print( " [E] oh no input error" );
        return Glib::ustring("");
    }

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

bool UI::auth_dlg( void )
{
    /* A nicer way to auth or something ;) */   
    WINDOW *authwin;
    std::string nick;
    gunichar raw_buf[20];
    int x;

    authwin = newwin( 4, 22, 9, 20 );
    wborder( authwin, 0, 0, 0, 0, 0, 0, 0, 0 ); /* use default */
    
    mvwprintw( authwin, 1, 1, "Your nick:" );

    while( ! protocol->auth( nick ) ) /* first time is not ok but well.. */
    {
        wrefresh( authwin );

        if( mvwgetn_wstr( authwin, 2, 1, raw_buf, 20 ) == OK )
        {
            /* make ustring from gunichar.. might write a rutine soon */
            for( x = 0; x < 20; x++ )
            {
                if( raw_buf[x] == NULL )
                    break;
                nick.append( 1, raw_buf[x] );
            }
        } /* e o input */
        else
            nick = "";
    }

    delwin( authwin );
    this->refresh();
    return true;
}

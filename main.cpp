/* C stuff */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h> // for tval

/* C++ stuff */
#include <iostream>
#include "socktroll.hpp"
#include <cstdio>

using namespace std;

int main( int argc, char **argv )
{
    Network* net;
    string temp;
    
    int ret_val;
    uint port;
    fd_set fdset;
    struct timeval tval;

    cout << "socktroll client written in C++" << endl;

    if( argc != 3 and argc != 2 )
    {
        cerr << "usage: " << (char*)argv[0] << " host [port]" << endl;
        cerr << "    default port = 6000" << endl;
        return EXIT_FAILURE;
    }
    else
        temp = string( (char*)argv[1] );

    for( int x=0; x < argc; x++ )
        printf( "%d: %s\n", x, argv[x] );

    if( argc == 3 )
    {
        port = atoi(argv[2]);
    }
    else
        port = 6000;

    net = new Network( temp, port );
    
    net->send( "nick kurt" ); /* FIXME: Should be from the UI */
    
    ui_init();

    while(1) /* HACK: sort of a ncurses powered logger ;) */
    {
        if( ( temp = net->recv() ).empty() )
        {
            ui_print( "! Connection broken" );
            return EXIT_FAILURE;
        }
                
        ui_print( "%s", temp.c_str() );
    }
    

    delete net;
    ui_stop();
    return EXIT_SUCCESS;
}

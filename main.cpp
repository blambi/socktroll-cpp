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

/* Semi global */
Network* net;
UI* ui;

int main( int argc, char **argv )
{
    string temp;
    uint port;

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
    
    ui = new UI();

    while(1) /* HACK: sort of a ncurses powered logger ;) */
    {
        if( ( temp = net->recv() ).empty() )
            fatal_error( "Lost connection" );
                
        ui->print( "%s", temp.c_str() );
        net->send( ui->input() );
    }
    

    delete net;
    delete ui;
    return EXIT_SUCCESS;
}

void fatal_error( std::string msg )
{
    /* Usage: when all goes wrong we wish to die nicely */
    if( net )
        delete net;
    
    if( ui )
        delete ui;

    cout << "Error: " << msg << endl;

    exit( EXIT_FAILURE );
}

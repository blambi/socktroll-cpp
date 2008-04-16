/* C stuff */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/* C++ stuff */
#include <iostream>
#include "socktroll.hpp"
#include <cstdio>

using namespace std;

/* Semi global */
Network* net;
UI* ui;
void SIGIO_handler( int signal );

int main( int argc, char **argv )
{
    string temp;
    uint port;
    struct sigaction sig_handler;

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
       
    /* bind up the signal handler */
    sig_handler.sa_handler = SIGIO_handler;
    if( sigfillset( &sig_handler.sa_mask ) < 0 )
        fatal_error( "Unable to initialize signal set" );
    /* SIGIO should call this handler */
    sig_handler.sa_flags = 0;
    if( sigaction( SIGIO, &sig_handler, 0 ) < 0 )
        fatal_error( "Unable to bind SIGIO to sig_handler" );

    //signal( SIGIO, SIGIO_handler );

    ui = new UI();
    net = new Network( temp, port );

    net->send( "nick kurt" ); /* FIXME: Should be done from the UI */

    /* UI input loop */
    while(1)
    {
        temp = ui->input();
        ui->print( "%s", temp.c_str() );

        /*net->send( temp ); /* FIXME: Read chars and tell
                            * program that its done when it
                            * really is */
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

void SIGIO_handler( int signal )
{
    string temp;

    /* Handle data */
    if( net != NULL )
    {
        temp = net->getmsg();
        if( ! temp.empty() )
            ui->print( "%s", temp.c_str() );
    }
}

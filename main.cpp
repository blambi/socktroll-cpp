/* C stuff */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/* C++ stuff */
#include <iostream>
#include <cstdio>
#include "socktroll.hpp"

using namespace std;

/* global */
Network* net;
Protocol* protocol;
UI* ui;

void SIGIO_handler( int signal );

int main( int argc, char **argv )
{
    string temp;
    Glib::ustring input;
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
    protocol = new Protocol();
    net = new Network( temp, port );


    /* Get header */
    temp = net->getmsg();
    if( ! temp.empty() )
        protocol->parse( temp );
    else
        fatal_error( "Didn't receive a head" );

    /* auth */
    protocol->auth();

    /* UI input loop */
    while(1)
    {
        input = ui->input();
        
        //ui->print( "-x-'%ls'-x-", input.c_str() );
        if( ! input.empty() )
        {
            if( input[0] != '/' )
                protocol->msg( input );
            else
                protocol->cmd( input.substr( 1 ) );
        }
    }
    
    delete protocol;
    delete net;
    delete ui;
    return EXIT_SUCCESS;
}

void fatal_error( Glib::ustring msg )
{
    /* Usage: when all goes wrong we wish to die nicely */
    if( net )
        delete net;
    
    if( ui )
        delete ui;

    cout << endl;
    cerr << "Error: " << msg << endl;

    exit( EXIT_FAILURE );
}

void SIGIO_handler( int signal )
{
    Glib::ustring temp;

    /* Handle data */
    if( net != NULL )
    {
        temp = net->getmsg();
        if( ! temp.empty() and temp != "\n" ) /* Not sure why we get them */
            protocol->parse( temp );
    }
}

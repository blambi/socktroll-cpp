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

    cout << "Connecting to: " << temp << ":" << port << endl;

    net = new Network( temp, port );

    cout << "net_fd: " << net->get_fd() << endl;

    net->send( "nick kurt" );

    /* main loop */
    while(1)
    {
        /* fdset setup */
        FD_ZERO( &fdset );
        FD_SET( 0, &fdset ); /* add stdin */
        FD_SET( net->get_fd(), &fdset );
        
        /* tval setup */
        tval.tv_usec = 0;
        tval.tv_sec = 5;

        /* main part of the loop */
        ret_val = select( net->get_fd() +1, &fdset, NULL, NULL, &tval );
        
        if( ret_val == -1 )
        {
            cerr << "Error: select()" << endl;
        }
        else if( ret_val )
        {
            if( FD_ISSET( net->get_fd(), &fdset ))
            {
                if( ( temp = net->recv() ).empty() )
                {
                    cerr << "Connection broken" << endl;
                    return EXIT_FAILURE;
                }
                
                cout << "got: ";
                cout << temp;
            }
            else /* stdin */
            {
                getline( cin, temp );

                /* hantera det */

                net->send( temp );
            }
        }
        
    }
    delete net;
    return EXIT_SUCCESS;
}

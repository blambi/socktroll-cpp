/* C stuff */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h> // for tval

/* C++ stuff */

#include <iostream>
#include "socktroll.hpp"

using namespace std;

int main( int argc, int *argv )
{
    Network* net = new Network( "127.0.0.1", 6000 );
    string temp;

    int ret_val;
    fd_set fdset;
    struct timeval tval;

    cout << "socktroll client written in C++" << endl;

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
                cout << "got net data:";
                cout << net->recv();
            }
            else /* stdin */
            {
                cin >> temp;
                net->send( temp );
            }
        }
        
    }
    delete net;
    return EXIT_SUCCESS;
}

/*
  Network layer:
   Both Lowlevel and socktroll handling
*/
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /* for portability */
#include <iostream>
#include "socktroll.hpp"

Network::Network( std::string host, uint port )
{
    struct sockaddr_in socket_addr;
    int socket_addr_lookup;

#ifdef DEBUG
    printf( "Network: creating socket to ( host=%s, port=%d )\n",
            host.c_str(), port );
#endif

    /* store away variables */
    this->host = host;
    this->port = port;

    /* create socket */
    socket_fd = socket( PF_INET, SOCK_STREAM, 0 );
    
    if( socket_fd == -1 )
    {
        fprintf( stderr, "Couldn't create socket!\n" );
        exit( 1 );
    }

    /* set up address */
    memset( &socket_addr, 0, sizeof( socket_addr ) );
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons( port );
    
    socket_addr_lookup = inet_pton( AF_INET, host.c_str(),
                                    &socket_addr.sin_addr );

    if( socket_addr_lookup == 0 )
    {
        fprintf( stderr, "Not a valid host address\n" );
        exit( 1 );
    }
    else if( socket_addr_lookup < 0 )
    {
        fprintf( stderr, "Not a supported address family\n" );
        exit( 1 );
    }

    /* connect */
    if( -1 == connect( socket_fd, (struct sockaddr*) &socket_addr,
                       sizeof( socket_addr ) ) )
    {
        fprintf( stderr, "Couldn't connect to host\n" );
        exit( 1 );
    }
}

Network::~Network( void )
{
#ifdef DEBUG
    printf( "Network: deconstructing\n" );
#endif
    shutdown( socket_fd, SHUT_RDWR );
    close( socket_fd );
}

int Network::send( std::string msg )
{
    /* 
       Not the most pretty function to send messeges to the server but
       well it works.
    */
#ifdef DEBUG
    printf( "Network: sending '%s'\n", msg.c_str() );
#endif

    std::string buff;

    buff = msg;
    buff.append( "\n" );        

    if( -1 == write( socket_fd, buff.c_str(), buff.size()))
    {
        fprintf( stderr, "Error sending data! '%s'", buff.c_str() );
        return  -1;
    }
    return 0;
}

std::string Network::recv( void )
{
    /* FIXME: Somehow we are going to make this allright! */
#ifdef DEBUG
    printf( "Network: recv\n" );
#endif
    char buff[500];
    int length;
    
    memset( &buff, 0, 500 );

    length = read( socket_fd, &buff, sizeof(buff));

    if( -1 == length )
    {
        fprintf( stderr, "Error reading data!\n" );
        exit( 1 );
    }

    return std::string( buff );
}

int Network::get_fd( void )
{
    return socket_fd;
}

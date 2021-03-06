/*
  Network layer:
   Both Lowlevel and socktroll handling
*/
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/file.h>  /* FASYNC and O_NONBLOCK */
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /* for portability */
#include <unistd.h>    /* getpid etc */
#include <errno.h>
#include <iostream>
#include "socktroll.hpp"

Network::Network( std::string host, uint port )
{
    struct sockaddr_in socket_addr;
    int socket_addr_lookup;

    struct hostent *hostinfo;

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
        fatal_error( "Couldn't create socket!" );
 
    /* This will use the FIRST ip registered to a hostname */
    hostinfo = gethostbyname( host.c_str() );
    if( hostinfo->h_addr_list[0] == NULL )
        fatal_error( std::string( "Couldn't lookup " ) + host );
    else
    {
        host = std::string(
            inet_ntoa( *(struct in_addr*)( hostinfo -> h_addr_list[0] ))
            );
    }

    /* set up address */
    memset( &socket_addr, 0, sizeof( socket_addr ) );
    socket_addr.sin_family = hostinfo->h_addrtype;
    socket_addr.sin_port = htons( port );
   
#ifdef DEBUG
    std::cout << "h_addr: '" << host << "'" << std::endl;
#endif

    /* We should now support IPv6 and IPv4 =) */
    socket_addr_lookup = inet_pton( hostinfo->h_addrtype,
                                    host.c_str(),
                                    &socket_addr.sin_addr );

    if( socket_addr_lookup == 0 )
        fatal_error( "Not a valid host address" );
    else if( socket_addr_lookup < 0 )
        fatal_error( "Not a supported address family" );

    /* set ownership to us so we can change it */
    if( fcntl( socket_fd, F_SETOWN, getpid() ) < 0 )
        fatal_error( "Unable to claim ownership of process" );

    /* connect */
    if( -1 == connect( socket_fd, (struct sockaddr*) &socket_addr,
                       sizeof( socket_addr ) ) )
        fatal_error( "Couldn't connect to host" );
}

Network::~Network( void )
{
#ifdef DEBUG
    printf( "Network: deconstructing\n" );
#endif
    shutdown( socket_fd, SHUT_RDWR );
    close( socket_fd );
}

int Network::send( Glib::ustring msg )
{
    /* 
       Not the most pretty function to send messeges to the server but
       well it works.
    */
#ifdef DEBUG
    printf( "Network: sending '%s'\n", msg.c_str() );
#endif

    Glib::ustring buff;

    buff = msg;
    buff.append( "\n" );

    if( -1 == write( socket_fd, buff.c_str(), buff.size()))
    {
        fprintf( stderr, "Error sending data! '%s'", buff.c_str() );
        return  -1;
    }
    return 0;
}

Glib::ustring Network::getmsg( void )
{
    /* FIXME: Somehow we are going to make this allright! */
#ifdef DEBUG
    printf( "Network: recv\n" );
#endif
    char buff[500];
    Glib::ustring sbuff;
    int length;
    
    memset( &buff, 0, 500 );

    /* get stuff */
    if( ( length = recv( socket_fd, buff, 500, 0 ) ) < 0 )
    {
        if( errno != EWOULDBLOCK )
            fatal_error( "recv() failed" );
    }

    /* post */

    sbuff = Glib::ustring( buff );

    if( sbuff[ sbuff.size() -1 ] == '\n' )
        return sbuff.erase( sbuff.size() -1, 1 ); /* Remove \n */
    else
        return sbuff;
}

int Network::get_fd( void )
{
    return socket_fd;
}

void Network::set_async( void )
{
    /* enable async mode / non-blocking */
    if( fcntl( net->get_fd(), F_SETFL, O_NONBLOCK | FASYNC ) < 0 )
        fatal_error( "Unable to put socket into Nonblocking/Async mode" );
}

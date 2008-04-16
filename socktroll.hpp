#ifndef __SOCKTROLL_HPP
#define __SOCKTROLL_HPP

/* C++ includes */
#include <string>

/* C includes */
#include <sys/socket.h>
#include <curses.h>

/* Compiler fix for some systems */
#ifndef EXIT_SUCCESS
#include <cstdlib>
#endif

void fatal_error( std::string msg );

/*
  Network (tcp/client) socket
 */
class Network
{
public:
    Network( std::string host, uint port );
    ~Network( void );
    int send( std::string msg );
    std::string getmsg();
    int get_fd( void );
private:
    std::string host;
    uint port;
    int socket_fd; /* not right maybe */
};

/*
  ui
*/
class UI
{
public:
    UI( void );
    ~UI( void );
    void print( char *fmt, ... );
    std::string input( void );
private:
    void refresh( void );

    WINDOW *logwin;   /* "paper trail" */
    WINDOW *inputwin; /* cmd line */
};

#endif /* __SOCKTROLL_HPP */

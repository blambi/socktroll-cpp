#ifndef __SOCKTROLL_HPP
#define __SOCKTROLL_HPP

/* C++ includes */
#include <string>

#include <cwchar>
#include <locale>
#include <glibmm-2.4/glibmm.h>
/* C includes */
#include <sys/socket.h>
#include <wctype.h> 
#include <ncursesw/ncurses.h>

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
    Glib::ustring input( void );
private:
    void refresh( void );

    WINDOW *logwin;   /* "paper trail" */
    WINDOW *inputwin; /* cmd line */
};

extern Network* net;
extern UI* ui;

/*
  Protocol handler
*/
class Protocol
{
public:
    Protocol( void );
    ~Protocol( void );
    /* do stuff with something we recived */
    void parse( std::string message );
    void msg( std::string message ); /* text message */
    void cmd( std::string command ); /* command */
    void auth( void ); /* auth input */
private:
    std::string nick;
    bool auth_ok;
};

#endif /* __SOCKTROLL_HPP */

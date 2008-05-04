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

void fatal_error( Glib::ustring msg );

/*
  Network (tcp/client) socket
 */
class Network
{
public:
    Network( std::string host, uint port );
    ~Network( void );
    int send( Glib::ustring msg );
    Glib::ustring getmsg();
    int get_fd( void );
    void set_async( void );
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
    void print( Glib::ustring line );
    Glib::ustring input( void );
    bool auth_dlg( void );
private:
    void refresh( void );

    WINDOW *logwin;   /* "paper trail" */
    WINDOW *inputwin; /* cmd line */
};

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
//    void parse_auth( std::string message );
    void msg( std::string message ); /* text message */
    void cmd( std::string command ); /* command */
    bool auth( std::string t_nick ); /* auth input */
private:
    std::string nick;
    bool auth_ok;
};

/* Global objects */
extern Network* net;
extern UI* ui;
extern Protocol* protocol;

#endif /* __SOCKTROLL_HPP */

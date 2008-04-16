#ifndef __SOCKTROLL_HPP
#define __SOCKTROLL_HPP

/* C++ includes */
#include <string>

/* C includes */
#include <sys/socket.h>

/*
  Network (tcp/client) socket
 */
class Network
{
public:
    Network( std::string host, uint port );
    ~Network( void );
    int send( std::string msg );
    std::string recv();
    int get_fd( void );
private:
    std::string host;
    uint port;
    int socket_fd; /* not right maybe */
};

/*
  ui
*/
void ui_init( void );
void ui_stop( void );
void ui_refresh( void );
void ui_print( char *fmt, ... );
std::string ui_input( void );

#endif /* __SOCKTROLL_HPP */

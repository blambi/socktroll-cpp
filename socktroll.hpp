#ifndef __SOCKTROLL_HPP
#define __SOCKTROLL_HPP

/* C++ includes */
#include <string>

/* C includes */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
  Network (tcp/client) socket
 */
class Network
{
public:
    Network( std::string host, uint port );
    ~Network( void );
    int send( std::string msg );
    //string* recv();
private:
    std::string host;
    uint port;
    int socket_fd; /* not right maybe */
};

#endif /* __SOCKTROLL_HPP */

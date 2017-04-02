/*Class for making a socket*/

//include protection
#ifndef STANDARDSOCKET_H

#define STANDARDSOCKET_H


/*#############################################################################################################

Section:                                          ~inludes

#############################################################################################################*/

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <errno.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>


/*#############################################################################################################

Section:                                          ~defines

#############################################################################################################*/

#define SOCKET_ERROR -1
#define BUFFER_SIZE 100

//stanardsocket parameter "modus"
const int STREAM = 1;
const int DGRAM = 2;

/*#############################################################################################################

Section:                                          ~Class "StandardSocket"

#############################################################################################################*/

class StandardSocket
{
    
public:
    //Constructor
    StandardSocket(const std::string *p_hostname, int port, int modus);
    //destructor
    ~StandardSocket();

    int getSocketIdentity() const;
    const struct sockaddr_in* p_sockAddrIn() const;
    int setOption() const;
    int connectToServer() const;
    int bindToPort() const;
    int listenQue( int length ) const;
    int shut() const;
    void printDetails() const;


private:
    struct sockaddr_in m_socketAddrIn;
    int m_socketId;
    
};// end class StandardSocket
    
#endif

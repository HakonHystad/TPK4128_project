/*11-02-17*****************************************************************************************************
***************************************************************************************************************
**                                                                                                           **
**                                              httpPostMaker.h                                              **
**                                                                                                           **
***************************************************************************************************************
**************************************************************************************************************/

/* a class for easing the process of making http post requests */

#ifndef _HTTP_POST_MAKER_H_
#define _HHTP_POST_MAKER_H_

/*#############################################################################################################

Section:                                          ~libs

#############################################################################################################*/

#include <string>
#include <iostream>

#include "standardsocket.h"

/*#############################################################################################################

Section:                                          ~class declaration

#############################################################################################################*/

class HttpPostMaker : public StandardSocket
{
public:

    HttpPostMaker();
    HttpPostMaker( std::string *hostname, int port, int modus, std::string domain="" );
    ~HttpPostMaker();
    
    int connect();
    
    void setDomain( std::string domain );

    int sendMsg(std::string msg);
    bool sendPOST();
    int receive( char *buffer, int bufferSize );

    bool isConnected();
    

    std::string bodyLength();
    void addToBody( std::string bodyItem );
    void clearBody();

    std::string getPOST();

private:

    int m_socket_fd;

    bool firstBody;
    
    std::string m_domain;
    std::string m_header;
    std::string m_body;
};




#endif// header guard

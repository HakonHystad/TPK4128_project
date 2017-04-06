/*11-02-17*****************************************************************************************************
***************************************************************************************************************
**                                                                                                           **
**                                             httpPostMaker.cpp                                             **
**                                                                                                           **
***************************************************************************************************************
**************************************************************************************************************/

#include "httpPostMaker.h"

/*#############################################################################################################

Section:                                          ~class definition

#############################################################################################################*/

/*=============================================================================================================

NAME:                                             ~HttpPostMaker(default constructor)
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

HttpPostMaker::HttpPostMaker()
    : firstBody(true)
{
    //empty
}

/*=============================================================================================================

NAME:                                             ~HttpPostMaker(constructor)
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

HttpPostMaker::HttpPostMaker( std::string *hostname, int port, int modus, std::string domain /*=""*/)
    : StandardSocket( hostname, port, modus ),
      firstBody(true)
{
    if( domain != "" )
	setDomain( domain );
}

HttpPostMaker::~HttpPostMaker()
{
    close( m_socket_fd );
}

/*=============================================================================================================

NAME:                                             ~connect
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

int HttpPostMaker::connect()
{
    connectToServer();

    m_socket_fd = getSocketIdentity();

    return m_socket_fd;
}

/*=============================================================================================================

NAME:                                             ~setDomain
ARGUMENT(S):
DESCRIPTION: set the domain and make the header
RETURN:

==============================================================================================================*/

void HttpPostMaker::setDomain( std::string domain )
{
    m_header = "POST " + domain + " HTTP/1.1\nContent-Type: application/x-www-form-urlencoded\nHost: \nContent-Length: ";

    m_domain = domain;
}

/*=============================================================================================================

NAME:                                             ~send
ARGUMENT(S):
DESCRIPTION: send the made packet
RETURN:

==============================================================================================================*/

int HttpPostMaker::sendMsg( std::string msg )
{

    const char *message = msg.c_str();
    size_t length = strlen( message );

    int bytesSent = send( m_socket_fd, message, length, 0 );
    
    if ( bytesSent == -1 )
    {
	std::cout<<"Could not send\n";
	perror("send(..)");
    }
    else if ( bytesSent == 0 )
    {
	std::cout<<"Other end closed connection\n";
    }

    return bytesSent;
}

/*=============================================================================================================

NAME:                                             ~sendPOST
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

bool HttpPostMaker::sendPOST()
{
    int status = sendMsg( getPOST() );
    
    if( status<=0 )
	return false;

    clearBody();
    return true;
}

/*=============================================================================================================

NAME:                                             ~receive
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

int HttpPostMaker::receive( char *buffer, int bufferSize )
{
    int bytesRecvd = recv( m_socket_fd, buffer, bufferSize-1, 0);

    if ( bytesRecvd == -1 )// return
    {
	std::cerr<<"Could not receive\n";
	perror("recv(..)");
    }
    else if ( bytesRecvd == 0 )
    	std::cerr<<"Other end closed connection\n";
     

    return bytesRecvd;
}

/*=============================================================================================================

NAME:                                             ~isConnected
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

bool HttpPostMaker::isConnected()
{
    char c;
    int byteRead = recv( m_socket_fd, &c, 1, MSG_PEEK );

    if( byteRead<=0)
	return false;
    return true;
}

/*=============================================================================================================

NAME:                                             ~addToBody
ARGUMENT(S):
DESCRIPTION: adds the given strig to the body of the POST request
RETURN:

==============================================================================================================*/

void HttpPostMaker::addToBody( std::string bodyItem )
{
    if( firstBody )
    {
	m_body = bodyItem;
	firstBody = false;
    }
    else
    {
	m_body += "&" + bodyItem;
    }
}

/*=============================================================================================================

NAME:                                             ~clearBody
ARGUMENT(S):
DESCRIPTION: clear the body string, will be called when request is given 
RETURN:

==============================================================================================================*/

void HttpPostMaker::clearBody()
{
    firstBody = true;
    m_body.clear();
}

/*=============================================================================================================

NAME:                                             ~bodyLength
ARGUMENT(S):
DESCRIPTION:
RETURN: nr of char in POSTbody

==============================================================================================================*/

std::string HttpPostMaker::bodyLength()
{
    int length = m_body.length();
    return ( std::to_string( length ) + "\n\n" );
}

/*=============================================================================================================

NAME:                                             ~getPOST
ARGUMENT(S):
DESCRIPTION: formats and concatenate post request
RETURN: formated post request as c-string

==============================================================================================================*/

std::string HttpPostMaker::getPOST()
{
    std::string POSTreq = m_header + bodyLength() + m_body;

    return POSTreq;
}

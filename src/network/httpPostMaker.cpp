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

HttpPostMaker::HttpPostMaker( int clientID, std::string domain /*=""*/)
    : ServerClientCom( clientID ),
      firstBody(true)
{
    if( domain != "" )
	setDomain( domain );
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

void HttpPostMaker::send()
{
    sendMsg( getPOST() );
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

    clearBody();
    firstBody = true;

    return POSTreq;
}

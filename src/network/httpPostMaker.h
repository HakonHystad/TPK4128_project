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

#include "serverClientCom.h"

/*#############################################################################################################

Section:                                          ~class declaration

#############################################################################################################*/

class HttpPostMaker : public ServerClientCom
{
public:

    HttpPostMaker();
    HttpPostMaker( int clientID, std::string domain="" );

    
    void setDomain( std::string domain );

    void send();

    std::string bodyLength();
    void addToBody( std::string bodyItem );
    void clearBody();

    std::string getPOST();

private:

    bool firstBody;
    
    std::string m_domain;
    std::string m_header;
    std::string m_body;
};




#endif// header guard

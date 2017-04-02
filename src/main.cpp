/*02-04-17*****************************************************************************************************
***************************************************************************************************************
**                                                                                                           **
**                                                 main.cpp                                                  **
**                                                                                                           **
***************************************************************************************************************
**************************************************************************************************************/

/*#############################################################################################################

Section:                                          ~libs

#############################################################################################################*/

#include <iostream>
#include <string>

#include "RFID/MFrec.h"
#include "network/standardsocket.h"
#include "network/httpPostMaker.h"
#include "network/serverClientCom.h"

int main()
{
    /*-------------------------------------- setup network COM ---------------------------------------*/

    std::string serverName = "folk.ntnu.no";
    std::string domain = "/haakonhy/php/requestHandling.php";
    
    StandardSocket *p_socketToServer = new StandardSocket( &serverName, 80, STREAM );
    
    p_socketToServer->connectToServer();// blocking

    int clientSocketId = p_socketToServer->getSocketIdentity();

    if( clientSocketId <0 )
    {
	std::cout << "no clientSocket\n";
	return 1;
    }

    ServerClientCom COM = ServerClientCom( clientSocketId );// handles sending receiving

    HttpPostMaker REQ = HttpPostMaker( domain ); 


   
    /*-------------------------------------- try default keys  ---------------------------------------*/

    MFrec RFID;

    RFID.initCom();// blocking

    int lockedSectors[16];
    int nrOflockedSectors = 0;
    
    for( int i = 0; i<16; i++)
    {
    }

    return 0;

}

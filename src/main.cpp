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
#include "LCD/hdmiLCD.h"
#include "network/standardsocket.h"
#include "network/httpPostMaker.h"
#include "network/serverClientCom.h"
#include "keywords.h"

/*#############################################################################################################

Section:                                          ~forward declarations

#############################################################################################################*/

void sendSector( MFrec *rfid, int sector, HttpPostMaker *post, bool locked );

/*#############################################################################################################

Section:                                          ~main

#############################################################################################################*/

int main()
{
    /*-------------------------------------- setup LCD  ---------------------------------------*/

    HdmiLCD LCD( 0x27 );
    LCD.init(); 
    
    /*-------------------------------------- setup network COM ---------------------------------------*/

    std::string serverName = "folk.ntnu.no";
    std::string domain = "/haakonhy/php/requestHandling.php";
    
    StandardSocket socketToServer( &serverName, 80, STREAM );
    
    socketToServer.connectToServer();// blocking

    int clientSocketId = socketToServer.getSocketIdentity();

    if( clientSocketId <0 )
    {
	std::cerr << "no clientSocket\n";
	LCD.print( "Failed connection" );
	return -1;
    }

    HttpPostMaker REQ = HttpPostMaker( clientSocketId, domain ); 

    /*-------------------------------------- try default keys  ---------------------------------------*/

    MFrec RFID;

    RFID.initCom();// blocking


    int lockedSectors[16];
    int nrOfLockedSectors = 0;
    int unlockedSector = 0;

    for( int sector = 0; sector<16; sector++)
    {
	
	LCD.print( "Sector " + std::to_string( sector ) );

	if( !RFID.authenticateOnChip( AUTHENT_A, sector*4 ) )// takes blockAddr, not sector..
	{
	    lockedSectors[ nrOfLockedSectors++ ] = sector;
	    sendSector( &RFID, sector, &REQ, true );

	}
	else
	{
	    unlockedSector = sector;
	    sendSector( &RFID, sector, &REQ, false );
	}

    }// for each sector

    if( nrOfLockedSectors>15 )
    {
	LCD.print( "No default" );
	std::cout << "No sectors use a default key\n";
	return 0;
    }

    

    return 0;

}


/*=============================================================================================================

NAME:                                             ~sendSector
ARGUMENT(S):
DESCRIPTION: read a sector from RFID card and sends it to server
RETURN:

==============================================================================================================*/
void sendSector( MFrec *rfid, int sector, HttpPostMaker *post, bool locked )
{
    unsigned char block[18]; 

    for( int blk = 0; blk<4; blk++ )
    {
	// reset block
	for( int i = 0; i<16; i++) block[i] = 'X';

	if( !locked )
	    rfid->readBlock( (sector*4) + blk, block, 18 );// 16 data bytes + 2 checksum bytes

	/* POST variables are hidden since this is public */
	post->addToBody( var1 + "=" + std::to_string( (sector*4)+blk ) );

	
	// convert to ascii if possible
	std::string asciiRep="";
	    
	for( int i = 0; i<16; i++ )
	{
	    post->addToBody( var2 + std::to_string(i) + "=" + std::to_string( (int)block[i] ) );
        
	    if( block[i] >=32 && block[i]<127 )// readable char
	    {
		asciiRep += std::to_string( (int)block[i] );// assuring no truncation with to_string
	    }
	    else
	    {
		asciiRep += ".";
	    }
	}

	post->addToBody( var3 + "=" + asciiRep );
    }// for each block

    post->send();
}


    

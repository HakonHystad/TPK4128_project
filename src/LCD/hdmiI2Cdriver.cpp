/*27-03-17*****************************************************************************************************
***************************************************************************************************************
**                                                                                                           **
**                                             hdmiI2Cdriver.cpp                                             **
**                                                                                                           **
***************************************************************************************************************
**************************************************************************************************************/

#include "hdmiI2Cdriver.h"


/*=============================================================================================================

NAME:                                             ~HdmmiI2C(default constructor)
ARGUMENT(S):
DESCRIPTION: default constructr, marks slaveAddress as unvalid
RETURN:

==============================================================================================================*/
HdmiI2C::HdmiI2C()
    : HdmiI2C( 0 )
{
    // empty
}


/*=============================================================================================================

NAME:                                             ~HdmiI2C(constructor)
ARGUMENT(S):
DESCRIPTION: constructor, initializes slaveAddr
RETURN:

==============================================================================================================*/

HdmiI2C::HdmiI2C( uint8_t slaveAddr )
    : m_sAddr( slaveAddr ),
      m_p( new Peripheral ),
      m_gpio( new Peripheral )
{
    m_p->physical_addr = I2C_BASE;
    m_gpio->physical_addr = GPIO_BASE;
    
    if( m_sAddr > 0 && m_sAddr <=0x7F )
	setup();
}

/*=============================================================================================================

NAME:                                             ~HdmiI2C(destructor)
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/

HdmiI2C::~HdmiI2C()
{
    munmap(m_p->map, _SC_PAGE_SIZE);
    close(m_p->fd);
    delete m_p;
}

/*=============================================================================================================

NAME:                                             ~setup
ARGUMENT(S):
DESCRIPTION: finds the register addresses as well as setting the slave address
RETURN: 0 on success, -1 on failure

==============================================================================================================*/

int HdmiI2C::setup()
{
    
    // map BSC
    if( mapPeripheral( m_p ) <0 ) return -1;

    // map GPIO
    if( mapPeripheral( m_gpio ) <0 ) return -1;

    //setAltFunc();
    
    // set slave address
    if( m_sAddr>0 && m_sAddr<=0x7F )
    {
	*(m_p->addr + I2C_A) =((int)m_sAddr) & 0xff;
	return 0;
    }

    std::cerr << "No valid slave address set\n";
    return -1;
}

/*=============================================================================================================

NAME:                                             ~write
ARGUMENT(S):
DESCRIPTION: writes a byte over the I2C bus
RETURN:

==============================================================================================================*/

void HdmiI2C::write( uint8_t msg )
{
    std::cout << "writeI2C: " << std::hex << (int)msg << std::dec << std::endl;
    
    *(m_p->addr + I2C_DLEN) = 1;// 1 byte

    *(m_p->addr + I2C_FIFO) = ((int)msg) & 0xff;

    *(m_p->addr + I2C_S) = CLEAR_STATUS;
    

    std::cout << "C: " << std::hex << *(m_p->addr + I2C_C)
	      << "\nS: " << *(m_p->addr + I2C_S)
	      << "\nDLEN: " << *(m_p->addr + I2C_DLEN)
	      << "\nA: " << *(m_p->addr + I2C_A)
	      << "\nFIFO: " << *(m_p->addr + I2C_FIFO) << std::dec << std::endl;
    *(m_p->addr + I2C_C) = START_WRITE;

    wait();
}

/*=============================================================================================================

NAME:                                             ~setSlaveAddr
ARGUMENT(S):
DESCRIPTION: setter
RETURN:

==============================================================================================================*/

void HdmiI2C::setSlaveAddr( uint8_t slaveAddr )
{
    // I2C uses 7-bit addressing
    if( slaveAddr > 0x7F )
    {
	std::cerr << "Invalid I2C slave address\n";
	return;
    }

    m_sAddr = slaveAddr;
}

/*=============================================================================================================

NAME:                                             ~mapPeripheral
ARGUMENT(S):
DESCRIPTION: maps virtual memory to physical
RETURN: -1 if error

==============================================================================================================*/

int HdmiI2C::mapPeripheral( Peripheral *p )
{
        
    // Obtain handle to physical memory
    if(( p->fd = open("/dev/mem", O_RDWR | O_SYNC) )<0 )

    {
	std::cerr << "Unable to open /dev/mem\n";
	return -1;
    }


    // map physical memory to virtual memory

    p->map = mmap( NULL, _SC_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, p->fd, p->physical_addr );


    if( p->map == MAP_FAILED )
    {
	perror( "mmap" );
	return -1;
    }

    p->addr = (volatile uint32_t *)p->map;

    return p->fd;
    
}

/*=============================================================================================================

NAME:                                             ~setAltFunc
ARGUMENT(S):
DESCRIPTION: sets the gpio pin to use the alternative function I2C
RETURN:

==============================================================================================================*/

void HdmiI2C::setAltFunc()
{
    *(m_gpio->addr) &= ~0xfc0;// null bits 6-11 of FSEL0
    *(m_gpio->addr) |= 1<<11;// SCL
    *(m_gpio->addr) |= 1<<8;// SDA

//    std::cout << "Set alt function: " << *(m_gpio->addr) << std::endl;

}


/*=============================================================================================================

NAME:                                             ~wait
ARGUMENT(S):
DESCRIPTION: waits for the status register to clear
RETURN:

==============================================================================================================*/

void HdmiI2C::wait()
{
     //Wait till done, let's use a timeout just in case
    int timeout = 50;
    while( !(*(m_p->addr + I2C_S) & S_DONE) && --timeout )
    {
	usleep(1000);
    }

    if(timeout == 0) std::cerr << "i2c timeout. Something went wrong.\n";
	
}

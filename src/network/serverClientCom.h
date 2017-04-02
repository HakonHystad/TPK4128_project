/*A class for communicating as a server or client*/

#ifndef SERVERCLIENTCOM_H// header guard

#define SERVERCLIENTCOM_H


/*#############################################################################################################

Section:                                          ~includes

#############################################################################################################*/

#include "standardsocket.h"
#include <climits> // for INT_MAX
#include <pthread.h> // for threading in duplex mode
#include <csignal> // for SIGTERM
#include <string>

/*#############################################################################################################

Section:                                          ~defines

#############################################################################################################*/

#define BUFFERSIZE 0xFFFF // size of the send/receive buffer

/*#############################################################################################################

Section:                                          ~Class ServerClientCom

#############################################################################################################*/

class ServerClientCom
{
public:
	ServerClientCom(int socketId);// constructor
	~ServerClientCom();

	int acceptClient();
	int sendMsg( char *message = NULL );
	int sendMsg( std::string msg );
	int recvMsg( char *buffer=NULL, int bufferSize=0 );
	void duplex();
	void displayBuffer();

private:
	char m_buffer[ BUFFERSIZE ];
	int m_socketId;
	int m_IOsocketId;
	struct sockaddr_in m_clientAddrIn;

	
	/*-------------------------------------- threading  ---------------------------------------*/
	pthread_t t1;
	pthread_t t2;
	
	static void *t_sendDuplex( void *p_obj );
	static void *t_recvDuplex( void *p_obj );

};
	
#endif// header guard

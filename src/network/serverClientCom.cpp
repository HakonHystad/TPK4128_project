#include "serverClientCom.h"

/*=============================================================================================================

NAME:                                             ~ServerClientCom(default constructor)
ARGUMENT(S):
DESCRIPTION:
RETURN:

==============================================================================================================*/
ServerClientCom::ServerClientCom()
{
    //empty
}

/*=============================================================================================================

NAME:                                             ~ServerClientCom
ARGUMENT(S): the file desrictor to a socket (listening server or client)
DESCRIPTION: initialize the socket Id
RETURN:

==============================================================================================================*/

ServerClientCom::ServerClientCom( int socketId )
    : m_socketId( socketId ), m_IOsocketId( socketId )
{
    // empty

}

/*=============================================================================================================

NAME:                                             ~ServerClientCom (desctructor)
ARGUMENT(S):
DESCRIPTION: closes the IO file
RETURN:

==============================================================================================================*/

ServerClientCom::~ServerClientCom()
{
    close( m_IOsocketId );
}

/*=============================================================================================================

NAME:                                             ~acceptClient
ARGUMENT(S): 
DESCRIPTION: waits for a connecting client and accepts it
RETURN: a new file descriptor for IO between server and client, -1 if failed

==============================================================================================================*/

int ServerClientCom::acceptClient()
{
    socklen_t addrSize = sizeof( m_clientAddrIn );
    m_IOsocketId = accept( m_socketId, (struct sockaddr *)&m_clientAddrIn, &addrSize);

    if ( m_IOsocketId == -1 )
    {
	std::cout<<"Could not accept\n";
	perror("accept(..)");
    }
    else
    {
	std::cout<<"Connected to: "<<m_clientAddrIn.sin_family<<" at port "
		 <<ntohs( m_clientAddrIn.sin_port )<<std::endl;
    }

    return m_IOsocketId;
}

/*=============================================================================================================

NAME:                                             ~sendMsg
ARGUMENT(S): optinal c-string array
DESCRIPTION: sends a c-string, either from argument message or user input
RETURN: nr of bytes sent, -1 if error

==============================================================================================================*/

int ServerClientCom::sendMsg( char *message /*= NULL*/ )
{
    size_t length;
    
    if ( message == NULL )// take user input as message
    {
	do
	{
	    std::cout<<"<Send> ";
	    std::cin.clear();
	    std::cin.ignore( INT_MAX, '\n' );// flush stream, get rid of everything until newline (left by cin>>)
	    std::cin.getline( m_buffer, BUFFERSIZE-1 );

	    message = (char *)&m_buffer;

	    length = strlen( message );
	}while( (int)length == 0 );// get good input

    }
    else
    {
	length = strlen( message );
    }

    int bytesSent = send( m_IOsocketId, message, length, 0 );
    
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

NAME:                                             ~sendMsg
ARGUMENT(S): string to send
DESCRIPTION: 
RETURN: nr of bytes sent, -1 if error

==============================================================================================================*/

int ServerClientCom::sendMsg( std::string msg )
{

    const char* message = msg.c_str();
    size_t length = strlen( message);
    
    int bytesSent = send( m_IOsocketId, message, length, 0 );
    
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

NAME:                                             ~recvMsg
ARGUMENT(S): optional storage buffer and its size
DESCRIPTION: if buffer is passed it will receive the raw date, if not it will be stored and displayed as a c-string
RETURN: number of bytes received, -1 if error

==============================================================================================================*/

int ServerClientCom::recvMsg( char *buffer/*=NULL*/, int bufferSize/*=0*/ )
{
    bool disp = false;
    
    if ( buffer == NULL )// display and store locally
    {
	buffer = (char *)&m_buffer;
	bufferSize = BUFFERSIZE;
	
	disp = true;
    }

    int bytesRecvd = recv( m_IOsocketId, buffer, bufferSize-1,0 );

    if ( bytesRecvd == -1 )// return
    {
	std::cout<<"Could not receive\n";
	perror("recv(..)");
	return bytesRecvd;
    }
    else if ( bytesRecvd == 0 )
    {
	std::cout<<"Other end closed connection\n";
	return bytesRecvd;
    }
    
    if ( disp && bytesRecvd>0 )// append 0 terminating char and display
    {
	buffer[ bytesRecvd ] = '\0';

	displayBuffer();
    }

    return bytesRecvd;
}// end recvMsg(..)


/*=============================================================================================================

NAME:                                             ~duplex
ARGUMENT(S):
DESCRIPTION: 2-way communication through user input using multithreading
RETURN:

==============================================================================================================*/

void ServerClientCom::duplex()
{
    std::cout<<"(Press enter) Starting convorsation ('q' to quit) \n";
    // TODO: close both threads when 1 returns
 
    pthread_create( &t1, NULL, t_sendDuplex, this );// create thread pass obj 
    pthread_create( &t2, NULL, t_recvDuplex, this );
  
    pthread_join( t1, NULL );// wait for thread to finish, then resume here
    pthread_join( t2, NULL );
    
}// end duplex(..)


/*=============================================================================================================

NAME:                                             ~displayBuffer
ARGUMENT(S):
DESCRIPTION: prints buffer to screen
RETURN:

==============================================================================================================*/

void ServerClientCom::displayBuffer()
{
    m_buffer[BUFFERSIZE] = '\0'; // safety precaution
    printf( "<Receive> '%s'\n", m_buffer );
}

/*#############################################################################################################

Section:                                          ~Private

#############################################################################################################*/


/*=============================================================================================================

NAME:                                             ~sendDuplex
ARGUMENT(S):
DESCRIPTION: Sends a message from console as long as connection is open, made for threading
RETURN:

==============================================================================================================*/

void* ServerClientCom::t_sendDuplex( void *p_obj )
{
     ServerClientCom obj = *(ServerClientCom *)p_obj;
    
    size_t length;
    char tmpBuffer[ BUFFERSIZE ];

     std::cin.clear();
     std::cin.ignore( INT_MAX, '\n' );// flush stream, get rid of everything until newline (left by cin>>)

     std::cout<<"Ready to send\n";
     
    while(1)
    {	
	do
	{
	    //printf("<Send> ");
	    //std::cout<<"<Send> ";
	    std::cin.getline( tmpBuffer, BUFFERSIZE-1 );
	    std::cin.clear();

	    length = strlen( tmpBuffer );
	
	}while( (int)length == 0 );// get good input

	if ( (length == 1 && tmpBuffer[0] == 'q')  )// quit if input q
	{
	    std::cout<<"\nQuitting\n";

	    pthread_cancel( obj.t2 );// terminate the recv thread as well
	    // return (void*)1;
	    pthread_exit( NULL );
	}

	int bytesSent = send( obj.m_IOsocketId, (char *)&tmpBuffer, length, 0 );
    
	if ( bytesSent == -1 )
	{
	    std::cout<<"Could not send\n";
	    perror("send(..)");

	    pthread_cancel( obj.t2 );// terminate the recv thread as well
	    // return (void*)1;
	    pthread_exit( NULL );
	}
	else if ( bytesSent == 0 )
	{
	    std::cout<<"Other end closed connection\n";

	    pthread_cancel( obj.t2 );// terminate the recv thread as well
	    //return (void*)1;
	    pthread_exit( NULL );
	}
    }// while

}// end t_sendDuplex()

/*=============================================================================================================

NAME:                                             ~t_recvDuplex
ARGUMENT(S):
DESCRIPTION: receives and prints to terminal, made for threading
RETURN:

==============================================================================================================*/

void* ServerClientCom::t_recvDuplex( void *p_obj )
{
    ServerClientCom obj = *(ServerClientCom *)p_obj;
    
    char tmpBuffer[ BUFFERSIZE ];

    while (1)
    {
	int bytesRecvd = recv( obj.m_IOsocketId, (char *)&tmpBuffer, BUFFERSIZE-1,0 );

	if ( bytesRecvd == -1 )// return
	{
	    std::cout<<"Could not receive\n";
	    perror("recv(..)");

	    pthread_cancel( obj.t1 );// terminate the send thread as well
	    //return (void*)1;
	    pthread_exit( NULL );
	}
	else if ( bytesRecvd == 0 )
	{
	    std::cout<<"Other end closed connection\n";
	    
	    pthread_cancel( obj.t1 );// terminate the send thread as well
	    //return (void*)1;
	    pthread_exit( NULL );
	}

	tmpBuffer[ bytesRecvd ] = '\0';

	std::cout<<"\r<Received> '"<<tmpBuffer<<"'\n";
	//printf( "\r<Received> '%s' \n<Send> ", tmpBuffer );
    }// while
    
}// end t_recvDuplex

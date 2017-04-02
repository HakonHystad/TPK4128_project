#include "standardsocket.h"



/*=============================================================================================================

NAME:                                             ~StandardSocket(constructor)
ARGUMENT(S):
	- name of host computer (string or 4 ints separated by periods)
	- port number used (registered ports 1024-49151)
	- type of socket wanted (1 for stream (established connection ex. TCP)),
		2 for datagram (connectionless ex.UDP))
DESCRIPTION: generates an address structure and makes the file descriptor
		m_socketId (int representing open file for IO)
RETURN: 

==============================================================================================================*/

StandardSocket::StandardSocket(const std::string *p_hostname, int port, int modus)
    :m_socketId(SOCKET_ERROR)//init as no socket to be made
{
    // safety: null out unused memory in address struct
    memset( &m_socketAddrIn,0,sizeof(m_socketAddrIn));

    // store hostname memory address (to a c-string)
    const char *hostPointerAddr = p_hostname->c_str();

    //-------------------------------------- get host info --------------------------------------
    
    if (struct hostent* p_host = gethostbyname( hostPointerAddr ) )// if hostname is valid p_host gets made
    {
	m_socketAddrIn.sin_family = AF_INET;//use IPV4
	m_socketAddrIn.sin_port = htons( port );//add port number in correct byte order (host to network short)

	//copy p_host's address into socketAddrIn
	memcpy( &m_socketAddrIn.sin_addr, p_host->h_addr,p_host->h_length);

	//-------------------------------------- different modes --------------------------------------
	if (modus == 1)//if stream
	{
	    // make id (int) as a ipv4, stream socket, using TCP protocol
	    m_socketId = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	}
	else if (modus == 2)//if datagram
	{
	    // make id (int) as a ipv4, datagram socket, using UDP protocol
	    m_socketId = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}// if modus

	// if no viable socket has been made
	if ( m_socketId == SOCKET_ERROR )
	{
	    std::cout<<"Cannot make socket\n";
	    perror("Socket(..)");
	}// if no socket

    }// if struct..
    else//no valid hostname
    {
	std::cout<<"Can't resolve target address: \"<<hostPointerAddr"<<".\n";
	perror( "gethostbyname(..)");
    }// else struct..

}//StandardSocket(..)


/*=============================================================================================================

NAME:                                             ~StandardSocket (destructor)
ARGUMENT(S):
DESCRIPTION: closes the socket file descriptor
RETURN:

==============================================================================================================*/

StandardSocket::~StandardSocket()
{
    if (m_socketId != -1 )//socket made
    {
	close( m_socketId );
    }
}// ~StandardSocket

/*=============================================================================================================

NAME:                                             ~getSocketIdentity
ARGUMENT(S): 
DESCRIPTION: 
RETURN: socket identity nr (int) which is a file descriptor

==============================================================================================================*/

int StandardSocket::getSocketIdentity() const
{
    return m_socketId;
}

/*=============================================================================================================

NAME:                                             ~p_sockAddrIn
ARGUMENT(S):
DESCRIPTION:
RETURN: pointer to socket address structure

==============================================================================================================*/

const struct sockaddr_in* StandardSocket::p_sockAddrIn() const
{
    return &m_socketAddrIn;
}

/*=============================================================================================================

NAME:                                             ~setOption
ARGUMENT(S):
DESCRIPTION: whipe the kernel for socket fragments to reuse address (server side)
RETURN: 0 if good, -1 if not

==============================================================================================================*/

int StandardSocket::setOption() const
{
    int yes = 1;// put 1 in memory
    //try to "reset" socket if not
    if ( setsockopt( m_socketId, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == -1 )
    {
	std::cout<<"Can't set option.\n";
	perror( "setsockopt" );
	return -1;
    }
    return 0;
}

/*=============================================================================================================

NAME:                                             ~connectToServer
ARGUMENT(S):
DESCRIPTION: set up client side connection to a server
RETURN: 0 if good, -1 if not

==============================================================================================================*/

int StandardSocket::connectToServer() const
{
    //The OS chooses a !local! port nr (49152-65535) for us to go !from! which
    //the server we're connecting to receives => no need to bind
    // m_socktId is the file descriptor, m_socketAddrIn has the connection info
    if ( connect( m_socketId,
		  reinterpret_cast<const struct sockaddr*>( &m_socketAddrIn), sizeof( m_socketAddrIn )
	     	) == SOCKET_ERROR
	)
    {
	std::cout<<"Can't connect to server.\n";
	return -1;
    }// if connect(..)
    
    return 0;
}// connectToServer

/*=============================================================================================================

NAME:                                             ~bindToPort
ARGUMENT(S):
DESCRIPTION: server side connection to port which we can give to client
RETURN: 0 if good, -1 if not

==============================================================================================================*/

int StandardSocket::bindToPort() const
{
    // connect to a port using m_socketId as descriptor and m_socketAddrIn's connection info
    if ( bind( m_socketId,
	       reinterpret_cast<const struct sockaddr*>( &m_socketAddrIn ), sizeof( m_socketAddrIn )
	     ) == SOCKET_ERROR
	)
    {
	std::cout<<"Can't bind to port.\n";
	return -1;
    }// if bind(..)

    return 0;

}// bindToPort

/*=============================================================================================================

NAME:                                             ~listenQue
ARGUMENT(S): max nr of clients in que
DESCRIPTION: server listens for incomming connections on the binded port  and puts them in que until accepted
RETURN: 0 if good, -1 if not

==============================================================================================================*/

int StandardSocket::listenQue( int length ) const
{
    if ( listen( m_socketId, length ) == SOCKET_ERROR )
    {
	std::cout<<"Cannot listen.\n";
	return -1;
    }
    return 0;
}

/*=============================================================================================================

NAME:                                             ~shut
ARGUMENT(S):
DESCRIPTION: close the socket by closing the file descriptor
RETURN: 0 if good, -1 if not

==============================================================================================================*/

int StandardSocket::shut() const
{
    if ( m_socketId != -1 )//if open socket
    {
	if ( close( m_socketId ) == SOCKET_ERROR )
	{
	    std::cout<<"Can't close socket.\n";
	    return -1;
	}
    }// if m_socketId

    return 0;
}// shut()

/*=============================================================================================================

NAME:                                             ~printDetails
ARGUMENT(S):
DESCRIPTION: prints out the socket info
RETURN:

==============================================================================================================*/

void StandardSocket::printDetails() const
{
    std::cout<<"Opened socket Id "<<m_socketId<<" on port "<<ntohs( m_socketAddrIn.sin_port )<<".\n";

    std::cout<<"Server host:\n";
    std::cout<<"\tfamily = "<<m_socketAddrIn.sin_family<<std::endl;
    std::cout<<"\tport = "<<ntohs( m_socketAddrIn.sin_port )<<std::endl;
}

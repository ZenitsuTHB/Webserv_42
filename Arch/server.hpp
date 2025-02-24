#pragma once

#include <iostream>
#include <netinet/in.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
#endif

class SimpleSocket
{
    private:
        int connection;
        int sock;
        struct sockaddr_in address;

    public:
        SimpleSocket(int domain, int type, int protocol, int port, u_long interface);

        virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;

        void testConnection(int );

        struct sockaddr_in getAddress() { return address; }
        int getSocket() { return sock; }
        int getConnection() { return connection; }

	void	setConnection(int con)
	{
		connection = con;
	}

        ~SimpleSocket();
};

class BindingSocket : public SimpleSocket
{
	public: 
		BindingSocket(int domain, int service, int protocol, int port, u_long interface) : 
			SimpleSocket(domain, service, protocol, port, interface)
		{
			setConnection(connectToNetwork(getSocket(), getAddress()));
			testConnection(getConnection());
		}

		int connectToNetwork(int sock, struct sockaddr_in address)
		{
			return bind(sock, (struct sockaddr *)&address, sizeof(address));
		}

};

class ConnectingSocket : public SimpleSocket
{
	public:
		ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : 
			SimpleSocket(domain, service, protocol, port, interface)
		{
			setConnection(connectToNetwork(getSocket(), getAddress()));
			testConnection(getConnection());
		}

		int connectToNetwork(int sock, struct sockaddr_in address)
		{
			return bind(sock, (struct sockaddr *)&address, sizeof(address));
		}
};

class	ListeningSocket : public BindingSocket
{
	private:
		int backlog;
		int	listening;

	public:
		ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklg) :
			BindingSocket(domain, service, protocol, port, interface)
		{
			backlog = bklg;
		}

		void	start_listening()
		{
			listening = listen(getConnection(), backlog);
		}
};

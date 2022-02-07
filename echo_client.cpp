#include "config.hpp"
#include <iostream>
#include <boost/asio.hpp>

void clean(char * data)
{
	for(char *p = data; *p; p++)
	{
		*p = '\0';
	}
}

void session(boost::asio::ip::tcp::socket sock)
{
	char request[DATA_MAX_LENGTH];
	char reply[DATA_MAX_LENGTH];
	size_t request_length;
	size_t reply_length;

	for(;;)
	{
		std::cout << "Enter Message : ";
		std::cin.getline(request, DATA_MAX_LENGTH);
		request_length = std::strlen(request);
		
		boost::asio::write(sock, boost::asio::buffer(request, request_length));

		reply_length = boost::asio::read(sock,
			boost::asio::buffer(reply, request_length));

		std::cout << "Reply is : " << reply << std::endl;

		clean(reply);
	}	
}

void client(boost::asio::io_service& io_service, std::string ip, unsigned short port)
{
	using namespace boost::asio;
	using ip::tcp;

	tcp::socket sock(io_service);
	sock.connect(ip::tcp::endpoint(ip::address::from_string(ip), port));

	session(std::move(sock));
}

int main(int argc, char const *argv[])
{
	boost::asio::io_service io_service;

	client(io_service, IP, PORT);

	return 0;
}
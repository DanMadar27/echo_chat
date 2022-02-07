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
	char data[DATA_MAX_LENGTH];
	size_t length;
	boost::system::error_code ec;

	for (;;)
	{
		length = sock.read_some(boost::asio::buffer(data), ec);

		if (ec == boost::asio::error::eof)
		{
			std::cout << "Connection has been closed by client" << std::endl;
			break;
		}

		else if (ec)
		{
			throw boost::system::system_error(ec);
		}

		boost::asio::write(sock, boost::asio::buffer(data, length));
      	std::cout << "data received is : " << data << std::endl;

      	clean(data);
	}
}

void server(boost::asio::io_service& io_service, std::string ip, unsigned short port)
{
	using namespace boost::asio;

	ip::tcp::acceptor acceptor(io_service,
		ip::tcp::endpoint(ip::address::from_string(ip), port));

	ip::tcp::socket sock(io_service);
	std::cout << "Waiting for incoming connection\n";
	acceptor.accept(sock);
	std::cout << "Connection has been established\n";

	session(std::move(sock));
}


int main(int argc, char const *argv[])
{
	boost::asio::io_service io_service;

	server(io_service, IP, PORT);

	return 0;
}
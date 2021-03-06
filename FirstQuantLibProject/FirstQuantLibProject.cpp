// FirstQuantLibProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include<boost/beast/core.hpp>
#include<boost/beast/http.hpp>
#include<boost/beast/version.hpp>
#include<boost/asio/connect.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<cstdlib>
#include<string.h>
#include<iostream>
#include<vector>


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;


int main(int argc, char** argv)
{
	try
	{
	    
		// Check command line arguments.
		if (argc != 4 && argc != 5)
		{
			std::cerr <<
				"Usage: http-client-sync <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]\n" <<
				"Example:\n" <<
				"    http-client-sync www.example.com 80 /\n" <<
				"    http-client-sync www.example.com 80 / 1.0\n";
			return EXIT_FAILURE;
		}
		auto const host = argv[1];
		auto const port = argv[2];
		auto const target = argv[3];
		int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

		std::vector<double> doubleVector(10.0,5.0);
		for (auto it = doubleVector.begin(); it != doubleVector.end(); ++it)
		{
			std::cout << it << std::endl;
		}
		


		net::io_context ioc;

		tcp::resolver resolver(ioc);
		beast::tcp_stream stream(ioc);

		// Look pu the domain name
		auto const results = resolver.resolve(host, port);

		stream.connect(results);

		// Set up an HTTP GET request message
		http::request<http::string_body> req{ http::verb::get, target, version };
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		//Send the HTTP request to the remote host
		http::write(stream, req);

		// this buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// recieve the http response
		http::read(stream, buffer, res);

		// write the message to standard out
		std::cout << res << std::endl;

		// Gracefully close the socket
		beast::error_code ec;
		stream.socket().shutdown(tcp::socket::shutdown_both, ec);

		if (ec && ec != beast::errc::not_connected)
			throw beast::system_error{ ec };

		// if we get here then the connect is closed gracefully.

	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Hi !~" << std::endl;
    return EXIT_SUCCESS;
}

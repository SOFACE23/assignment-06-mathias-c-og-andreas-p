//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) 
{
  try
  {
    if (argc != 2) //Check the number of arguments
    {
      std::cerr << "Usage: client <host>" << std::endl; //Report the error
      return 1;
    }

    boost::asio::io_context io_context; // All I/O objects are required to have an associated io_context object

    tcp::resolver resolver(io_context); //Create a resolver object
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "daytime"); //Resolve the host name to an IP address

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints); //Connect to the server

    while(true) //Read the data from the socket
    {
      boost::array<char, 128> buf; //Buffer to hold the data
      boost::system::error_code error; //Boost::system::error_code is a type used to report errors

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof) //Check if the connection was closed cleanly by the peer
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e) //Handle any exceptions that may have been thrown
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
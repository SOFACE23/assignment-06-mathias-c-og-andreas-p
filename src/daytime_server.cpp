//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() //Create a string containing the current date and time
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    boost::asio::io_context io_context; // All I/O objects are required to have an associated io_context object

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13)); //Create an acceptor object and open the acceptor

    while (true)
    {
      tcp::socket socket(io_context); //Create a socket object
      acceptor.accept(socket); //Wait for a connection

      std::string message = make_daytime_string(); //Create the message to be sent back to the client

      boost::system::error_code ignored_error; //Boost::system::error_code is a type used to report errors
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error); //Write the message to the socket
    }
  }
  catch (std::exception &e) //Handle any exceptions that may have been thrown
  {
    std::cerr << e.what() << std::endl; //Report the error
  }

  return 0;
}
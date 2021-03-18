#include <iostream>

#include <boost/asio/io_context.hpp>

#include "client.h"

int main (int argc, char **argv)
{
  std::string port;
  std::string sender_port;

  if (argc == 1) {
    std::cout << "No arguments provided." << std::endl;
    return 1;
  } else if (argc > 3) {
    std::cout << "Too many arguments provided, taking only the first two.";
    std::cout << std::endl;
  }

  port = argv[1];
  sender_port = argv[2];
  boost::asio::io_context io_context;

  Client client (port, sender_port, io_context);
  io_context.run ();
  client.start ();

  std::cout << "Client closed." << std::endl;
  return 0;
}
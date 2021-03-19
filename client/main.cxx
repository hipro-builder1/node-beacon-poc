#include <iostream>

#include <boost/asio/io_context.hpp>

#include "client.h"

int main (int argc, char **argv)
{
  std::string port;

  if (argc != 2) {
    std::cout << "Usage: ./node-beacon-poc <port>" << std::endl;
    return 1;
  }

  port = argv[1];
  boost::asio::io_context io_context;

  Client client (port, io_context);
  io_context.run ();
  client.start ();

  std::cout << "Client closed." << std::endl;
  return 0;
}
#include <iostream>

#include <boost/asio/io_context.hpp>

#include "client.h"

int main (int argc, char **argv)
{

  if (argc != 2) {
    std::cout << argv[0] << "Usage Error, <udp_port_num>" << std::endl;
    return 1;
  }

//  boost::asio::io_context io_context;

  Client client;
  client.start(atoi(argv[1]));

  std::cout << "Client closed." << std::endl;
  return 0;
}

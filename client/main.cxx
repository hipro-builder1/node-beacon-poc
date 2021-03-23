#include <iostream>
#include <memory>

#include <boost/asio/io_context.hpp>

#include "client.h"

void sigint_handler (
  const bsys::error_code &ec, int sig, asio::io_context *io, Client *client)
{
  std::cout << "Got keyboard interrupt. Exiting..." << std::endl;
  client->close ();
  io->stop ();
}

int main (int argc, char **argv)
{
  std::string port;

  if (argc != 2) {
    std::cout << "Usage: ./node-beacon-poc <port>" << std::endl;
    return 1;
  }

  port = argv[1];
  uint16_t port_no;

  try {
    port_no = std::stoi (port);
  } catch (std::invalid_argument ia) {
    std::cerr << "ERROR : Invalid <port> format." << std::endl;
    return 1;
  } catch (std::out_of_range oor) {
    std::cerr << "ERROR : Invalid <port> value." << std::endl;
    return 1;
  } catch (std::exception ex) {
    std::cerr << "ERROR : Invalid <port> provided." << std::endl;
    return 1;
  }

  boost::asio::io_context io_context;

  auto client = std::make_unique<Client> (io_context, port_no);
  client->build ();
  client->start ();

  asio::signal_set signals (io_context, SIGINT, SIGTERM);
  signals.async_wait (
    std::bind (
      sigint_handler, std::placeholders::_1, std::placeholders::_2,
      &io_context, client.get ()));

  io_context.run ();
  return 0;
}

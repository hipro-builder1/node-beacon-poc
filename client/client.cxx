#include <iostream>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "client.h"

#define LOCALHOST "127.0.0.1"
#define IP_INDEX 0
#define PORT_INDEX 1

Client::Client (
  std::string &port, asio::io_context &io_context)
  : m_port (port),
    m_io_context (io_context),
    m_tcp_socket (m_tcp_context)
{}

bool
Client::start ()
{
  unsigned short port_no;
  port_no = std::stoi (m_port);

  std::cout << "Client started at " << port_no << std::endl;

  asio::ip::udp::endpoint endpoint;
  asio::ip::udp::socket socket (
    m_io_context, asio::ip::udp::endpoint {asio::ip::udp::v4 (), port_no});

  char recv_data [1024] = {};
  while (true)
  {
    std::cout << "Waiting for broadcast..." << std::endl;
    socket.receive_from (
      asio::buffer (recv_data), endpoint);

    auto res = receive_handler (recv_data);
    if (!res) {
      return false;
    }
  }
}

bool
Client::receive_handler (std::string recv_data)
{
  std::cout << recv_data << std::endl;

  if (recv_data.size () == 0) {
    std::cout << "Recieved no data. exiting." << std::endl;
    return false;
  }

  std::vector<std::string> data;
  boost::split (data, recv_data, boost::is_any_of (":"));

  send_ping (data[IP_INDEX], data[PORT_INDEX]);

  return true;
}

void
Client::send_ping (std::string server_ip, std::string server_port)
{
  std::cout << "Received server : " << server_ip << ":" << server_port
    << std::endl;

  unsigned short server_port_no;
  server_port_no = std::stoi (server_port);

  m_tcp_context.run ();
  asio::ip::tcp::endpoint endpoint (
    asio::ip::make_address_v4 (server_ip), server_port_no);
  m_tcp_socket.close ();
  m_tcp_socket.async_connect (
    endpoint, std::bind (&Client::handle_connect, this, std::placeholders::_1));
}

void
Client::handle_connect (const boost::system::error_code &error)
{
  std::cout << "Connected to server." << std::endl;

  m_tcp_socket.async_write_some (
    asio::buffer ("ping", 4),
    std::bind (
      &Client::handle_write, this, std::placeholders::_1,
      std::placeholders::_2));
}

void
Client::handle_write (
  const boost::system::error_code &error, std::size_t bytes_transferred)
{
  std::cout << "Sent ping." << std::endl;

  m_tcp_socket.async_read_some (
    asio::buffer (m_server_pong, 4),
    std::bind (
      &Client::handle_pong, this, std::placeholders::_1,
      std::placeholders::_2));
}

void
Client::handle_pong (
  const boost::system::error_code &error, std::size_t bytes_transferred)
{
  if (bytes_transferred == 0) {
    std::cout << "Didn't recieve pong" << std::endl;
    std::cout << error.message ();
    return;
  }

  if (m_server_pong == "pong") {
    std::cout << "recieved pong" << std::endl;
  }
}

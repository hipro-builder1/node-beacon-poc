#include <iostream>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "client.h"

#define LOCALHOST "127.0.0.1"
#define IP_INDEX 0
#define PORT_INDEX 1

struct tcp_ip_port_s
{
	boost::asio::ip::address_v4::bytes_type tcp_ip;
	uint16_t tcp_port;
} __attribute__((packed));

Client::Client (asio::io_context &io_context, int port)
  : m_io_context (io_context),
    m_port (port)
{}

void
Client::build ()
{
  std::cout << "Building Client" << std::endl;
  m_ep_udp_server = std::make_unique <ip::udp::endpoint> (
    ip::make_address_v4 (LOCALHOST), m_port);
  m_ep_udp = std::make_unique <ip::udp::endpoint> ();
  m_sock_udp = std::make_unique <ip::udp::socket> (
    m_io_context, ip::udp::endpoint (ip::udp::v4 (), m_port));
}

void
Client::start ()
{
  m_sock_udp->async_receive_from (
    asio::buffer (m_udp_buff), *m_ep_udp,
    std::bind (
      &Client::handle_receive, this, std::placeholders::_1,
      std::placeholders::_2));

  std::cout << "Client started at " << m_port << std::endl;
}

void
Client::handle_receive (const bsys::error_code &ec, std::size_t bytes)
{
  if (ec) {
    std::cout << "ERROR: handle_receive: " << ec.message ();
    return;
  }

  tcp_ip_port_s addr;
  ip::address_v4 server_addr;
  try {
    memcpy (&addr, m_udp_buff, sizeof (addr));

    bsys::error_code err;
    server_addr = ip::make_address_v4 (addr.tcp_ip);
  } catch (std::exception e) {
    std::cout << "Got corrupted ip from server: " << e.what () << std::endl;
    return;
  }

  m_sock_tcp = std::make_unique<ip::tcp::socket> (m_io_context);
  auto server_ep =  ip::tcp::endpoint (server_addr, addr.tcp_port);
  m_sock_tcp->async_connect (
    server_ep,
    std::bind (&Client::handle_connect, this, std::placeholders::_1));

  // wait for next broadcast
  start ();
}

void
Client::handle_connect (const bsys::error_code &ec)
{
  if (ec) {
    std::cout << "ERROR: handle_connect: " << ec.message ();
    return;
  }

  std::cout << "Connected to server" << std::endl;

  m_sock_tcp->async_write_some (
    asio::buffer ("ping", 4),
    std::bind (
      &Client::handle_write, this, std::placeholders::_1,
      std::placeholders::_2));
}

void
Client::handle_write (
  const boost::system::error_code &ec, std::size_t bytes)
{
  if (ec) {
    std::cout << "ERROR: handle_write: " << ec.message ();
    return;
  }
  std::cout << "Sent ping." << std::endl;

  m_sock_tcp->async_read_some (
    asio::buffer (m_tcp_buff, 4),
    std::bind (
      &Client::handle_pong, this, std::placeholders::_1,
      std::placeholders::_2));
}

void
Client::handle_pong (
  const boost::system::error_code &ec, std::size_t bytes)
{
  if (ec) {
    std::cout << "ERROR: handle_pong: " << ec.message ();
    return;
  }

  if (bytes == 0) {
    std::cout << "Didn't recieve pong" << std::endl;
    return;
  }

  std::string pong (m_tcp_buff);
  if (pong != "ping") {
    std::cout << "recieved " << m_tcp_buff << std::endl;
    return;
  }

  std::cout << "recieved ping" << std::endl;
}

void
Client::close ()
{
  m_sock_udp->close ();
  if (m_sock_udp->is_open ()) {
    m_sock_tcp->close ();
  }
}

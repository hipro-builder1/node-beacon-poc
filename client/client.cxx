#include <iostream>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "client.h"

#define LOCALHOST "127.0.0.1"
#define IP_INDEX 0
#define PORT_INDEX 1

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
    asio::buffer (m_buff), *m_ep_udp,
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

  std::string str_data(m_buff, bytes);
  std::cout << "Recieved from server : " << str_data << std::endl;

  std::vector<std::string> data;
  boost::split (data, str_data, boost::is_any_of (":"));

  bsys::error_code err;
  auto server_addr = ip::make_address_v4 (data[IP_INDEX], err);
  if (err) {
    std::cout << "Recieved corrupted IP from server, aborting..." << std::endl;
    std::cout << "ERROR : " << err.message () << std::endl;
    return;
  }

  uint16_t tcp_port_no;

  try {
    tcp_port_no = std::stoi (data[PORT_INDEX]);
  } catch (std::invalid_argument ia) {
    std::cerr << "Corrupted port format: " << ia.what () << std::endl;
    return;
  } catch (std::out_of_range oor) {
    std::cerr << "Corrupted port format: " << oor.what () << std::endl;
    return;
  } catch (std::exception ex) {
    std::cerr << "Corrupted port format: " << ex.what () << std::endl;
    return;
  }

  m_sock_tcp = std::make_unique<ip::tcp::socket> (m_io_context);
  auto server_ep =  ip::tcp::endpoint (
    ip::make_address_v4 (data[IP_INDEX]), tcp_port_no);
  m_sock_tcp->async_connect (
    server_ep,
    std::bind (&Client::handle_connect, this, std::placeholders::_1));
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
    asio::buffer ("ping"),
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
    asio::buffer (m_buff, 4),
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

  std::string pong (m_buff);
  if (pong == "ping") {
    std::cout << "recieved pong" << std::endl;
  }
}

void
Client::close ()
{
  m_sock_udp->close ();
  if (m_sock_udp->is_open ()) {
    m_sock_tcp->close ();
  }
}

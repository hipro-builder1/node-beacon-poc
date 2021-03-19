#include <iostream>
#include <vector>

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "client.h"

#define LOCALHOST "127.0.0.1"
#define IP_INDEX 0
#define PORT_INDEX 1

bool
Client::start (short unsigned port)
{
  std::cout << "Client started." << std::endl;
  asio::ip::udp::socket socket (m_io_context,
		  boost::asio::ip::udp::endpoint{boost::asio::ip::udp::v4(),port});

 // socket.open (asio::ip::udp::v4 ());

//  unsigned short port_no;
//  unsigned short sender_port_no;
//  std::stringstream conv;
//  conv << m_port;
//  conv >> port_no;
//  conv.str ("");
//  conv.clear();
//  conv << m_sender_port;
//  conv >> sender_port_no;


  char recv_data[1024] = {0};

  std::cout << "Listening...";
  while (true)
  {
    socket.receive_from (asio::buffer (recv_data), m_client);
    std::cout << m_client << ": "<< recv_data << std::endl;
    
  }
}

bool
Client::receive_handler (std::string recv_data)
{
  std::cout << "Handler called." << std::endl;

  if (recv_data.size () == 0) {
    std::cout << "Recieved no data. exiting.";
    return false;
  }

  std::vector<std::string> data;
  boost::split (data, recv_data, boost::is_any_of ("|"));

  send_ping (data[IP_INDEX], data[PORT_INDEX]);

  return true;
}

void
Client::send_ping (std::string server_ip, std::string server_port)
{
  std::cout << "Received server : " << server_ip << ":" << server_port;
  std::cout << std::endl;

  std::stringstream conv (server_port);
  unsigned short server_port_no;
  conv >> server_port_no;

  asio::ip::tcp::socket socket {m_io_context};
  asio::ip::tcp::endpoint server_endpoint (
    asio::ip::make_address_v4 (server_ip), server_port_no);
  socket.connect (server_endpoint);
  socket.write_some (boost::asio::buffer ("ping"));

  socket.async_read_some (
    asio::buffer(m_server_pong),
    std::bind(
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
    std::cout << "recieved pong";
  }
}

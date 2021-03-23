/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#include <cstdint>

#include <iostream>

#include "udp-server.h"
#include "protocol.h"

UdpServer::UdpServer(
  uint16_t port, std::string tcp_ip,uint16_t tcp_port)
  : m_socket(m_io_service), m_tcp_ip_port(tcp_ip), m_thread_run(true)
{
  m_socket.open(boost::asio::ip::udp::v4());
  boost::asio::socket_base::broadcast option(true);
  m_socket.set_option(option);
  m_remote_endpoint = boost::asio::ip::udp::endpoint(
    boost::asio::ip::address::from_string(broadcast_ip.data()),
    port);
  m_broadcast_msg_data = malloc(sizeof(struct broadcast_tcp_ip_port_s));
  if(m_broadcast_msg_data== nullptr) {
  	std::runtime_error("malloc error"); //TODO std::strerror(errno);
  }
  struct broadcast_tcp_ip_port_s* broadcast_tcp_ip_port = static_cast<struct broadcast_tcp_ip_port_s*>(m_broadcast_msg_data);
  struct in_addr ip; 
  int ret = inet_aton(tcp_ip.c_str(),&ip);
  if(ret != 1) {
  	std::runtime_error("Invalid IP");
  }
  memcpy(&broadcast_tcp_ip_port->tcp_ip,&ip,sizeof(struct in_addr));

  broadcast_tcp_ip_port->tcp_port = tcp_port;
}

void
UdpServer::broadcast_data()
{
  try {
    while (m_thread_run) {
      std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
      m_socket.send_to(
        boost::asio::buffer(m_broadcast_msg_data, sizeof(struct broadcast_tcp_ip_port_s)),
        m_remote_endpoint, 0, m_ignored_error);
    }
  } catch (const boost::system::system_error& ex) {
    std::cout << "udp broadcast error " << ex.what() << std::endl;
  }
}

void
UdpServer::stop()
{
  m_thread_run = false;
}

/*
  Local Variables:
  mode: c++
  End:
*/

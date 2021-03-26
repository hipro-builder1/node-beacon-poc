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

UdpServer::UdpServer(uint16_t port, std::string tcp_ip, uint16_t tcp_port)
  : m_socket(m_io_service), m_tcp_ip_port(tcp_ip), m_thread_run(true)
{
  m_socket.open(boost::asio::ip::udp::v4());
  boost::asio::socket_base::broadcast option(true);
  m_socket.set_option(option);
  m_remote_endpoint = boost::asio::ip::udp::endpoint(
    boost::asio::ip::address::from_string(broadcast_ip.data()), port);

  boost::asio::ip::address_v4 ip = boost::asio::ip::make_address_v4(tcp_ip);

  m_broadcast_data_packet.tcp_ip = ip.to_bytes();
  m_broadcast_data_packet.tcp_port = tcp_port;
}

void
UdpServer::broadcast_data()
{
  boost::system::error_code ignored_error;
  while (m_thread_run) {
    std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    m_socket.send_to(
      boost::asio::buffer(
        (void*)&m_broadcast_data_packet,
        sizeof(struct broadcast_tcp_ip_port_s)),
      m_remote_endpoint, 0, ignored_error);
    if (ignored_error) {
      std::cout << "Udp broadcast Error: " << ignored_error.category().name()
                << std::endl;
    }
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

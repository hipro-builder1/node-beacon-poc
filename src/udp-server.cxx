/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#include <iostream>

#include "udp-server.h"

UdpServer::UdpServer(
  std::string ip, std::string port, std::string tcp_ip_port)
  : m_socket(m_io_service), m_tcp_ip_port(tcp_ip_port), m_thread_run(true)
{
  m_socket.open(boost::asio::ip::udp::v4());
  boost::asio::socket_base::broadcast option(true);
  m_socket.set_option(option);
  m_remote_endpoint = boost::asio::ip::udp::endpoint(
    boost::asio::ip::address::from_string(ip),
    boost::lexical_cast<int>(port));
}

void
UdpServer::broadcast_data()
{
  try {
    while (m_thread_run) {
      std::this_thread::sleep_for(std::chrono::seconds(5));
      m_socket.send_to(
        boost::asio::buffer(m_tcp_ip_port.c_str(), m_tcp_ip_port.length()),
        m_remote_endpoint, 0, m_ignored_error);
    }
  } catch (const boost::system::system_error& ex) {
    std::cout << "udp broadcast error";
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

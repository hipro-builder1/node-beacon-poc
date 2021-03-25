/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#ifndef HIPRO__b6f033ae_87a7_11eb_8e3f_b9b5ee5d4b95
#define HIPRO__b6f033ae_87a7_11eb_8e3f_b9b5ee5d4b95

#include <cstdlib>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/signal_set.hpp>

#include "tcp-session.h"
#include "udp-server.h"

class TcpServerApp
{
public:
  TcpServerApp(uint16_t port)
    : m_port(port),
      m_acceptor(
        m_io_service,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
  {
    start_accept();
  }
  ~TcpServerApp();
  void run(uint16_t broadcast_port, std::string tcp_ip);

private:
  void sigint_handler(const boost::system::error_code& error, int signal_num);
  void start_accept();
  void handle_accept(
    TcpSession* tcp_session, const boost::system::error_code& error);
  void stop();

private:
  std::vector<std::unique_ptr<TcpSession>> m_vector_tcp_session;
  std::unique_ptr<UdpServer> m_udp_broadcast_server;
  const uint16_t m_port;
  std::thread m_th_udp_server;
  boost::asio::io_service m_io_service;
  boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif // HIPRO__b6f033ae_87a7_11eb_8e3f_b9b5ee5d4b95

/*
  Local Variables:
  mode: c++
  End:
*/

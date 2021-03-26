/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#ifndef HIPRO__ed1b19e2_7bd3_11eb_baf6_69de8c48ade4
#define HIPRO__ed1b19e2_7bd3_11eb_baf6_69de8c48ade4

#include <string>
#include <memory>
#include <queue>
#include <mutex>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "protocol.h"

class UdpServer
{
public:
  UdpServer(uint16_t port, std::string tcp_ip, uint16_t tcp_port);
  void broadcast_data();
  void stop();

private:
  std::string m_tcp_ip_port;
  struct broadcast_tcp_ip_port_s m_broadcast_data_packet;
  const std::string broadcast_ip = "255.255.255.255";
  const int sleep_time = 5;
  bool m_thread_run;
  boost::asio::io_service m_io_service;
  boost::asio::ip::udp::socket m_socket;
  boost::asio::ip::udp::endpoint m_remote_endpoint;
};

#endif // HIPRO__ed1b19e2_7bd3_11eb_baf6_69de8c48ade4

/*
  Local Variables:
  mode: c++
  End:
*/

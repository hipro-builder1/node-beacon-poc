/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#include "tcp-server-app.h"
#include "tcp-session.h"

void
TcpServerApp::start_accept()
{
  m_acceptor.async_accept(
    m_socket,
    boost::bind(
      &TcpServerApp::handle_accept, this, boost::asio::placeholders::error));
}

void
TcpServerApp::start() 
{
  start_accept();
}

void
TcpServerApp::run(uint16_t broadcast_port, std::string tcp_ip)
{
  m_udp_broadcast_server =
    std::make_unique<UdpServer>(broadcast_port, tcp_ip, m_port);

  m_th_udp_server = std::move(
    std::thread(&UdpServer::broadcast_data, m_udp_broadcast_server.get()));

  boost::asio::signal_set signals(m_io_service, SIGINT, SIGTERM);

  signals.async_wait(std::bind(
    &TcpServerApp::sigint_handler, this, std::placeholders::_1,
    std::placeholders::_2));

  m_io_service.run();
}

void
TcpServerApp::handle_accept(const boost::system::error_code& error)
{
  if (error) {
    std::cerr << "Error in async_accept " << error.category().name()
              << std::endl;
    start_accept();
    return;
  }
  auto tcp_session = std::make_unique<TcpSession>(
    std::move(m_socket),
    std::bind(&TcpServerApp::destroy_session, this, std::placeholders::_1));
  tcp_session->start();
  m_vector_tcp_session.push_back(std::move(tcp_session));
  start_accept();
}

void
TcpServerApp::sigint_handler(
  const boost::system::error_code& error, int signal_num)
{
  std::cout << "Got SIGINT. Terminating." << std::endl;
  stop();
}

void
TcpServerApp::destroy_session(TcpSession* tcp_session)
{
  std::cout << " ********************************Removing "
               "tcp session object**************************"
            << std::endl;
  auto itr = std::remove_if(
    m_vector_tcp_session.begin(), m_vector_tcp_session.end(),
    [tcp_session](auto const& ptr) { return ptr.get() == tcp_session; });
  // TODO itr
}

void
TcpServerApp::stop()
{
  m_udp_broadcast_server->stop();
  m_th_udp_server.join();
  m_io_service.stop();
}

/*
  Local Variables:
  mode: c++
  End:
*/

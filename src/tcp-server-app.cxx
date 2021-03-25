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


TcpServerApp::~TcpServerApp()
{
	for( int i = 0; i < m_vector_tcp_session.size(); i++) {
		m_vector_tcp_session[i].reset(nullptr);
	}
	m_vector_tcp_session.clear();
}

void
TcpServerApp::start_accept()
{

  m_vector_tcp_session.emplace_back(std::make_unique<TcpSession>(m_io_service));
  m_acceptor.async_accept(
    (m_vector_tcp_session.back())->socket(),
    boost::bind(
      &TcpServerApp::handle_accept, this, (m_vector_tcp_session.back()).get(),
      boost::asio::placeholders::error));
}
void
TcpServerApp::run(uint16_t broadcast_port,std::string tcp_ip)
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
TcpServerApp::handle_accept(
  TcpSession* tcp_session, const boost::system::error_code& error)
{
  if (!error) {
    tcp_session->start();
  } else {
	  std::cerr << "Error in async_accept "<< error.value() << std::endl; 
	  tcp_session = nullptr;
  }
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
TcpServerApp::stop()
{
  m_udp_broadcast_server->stop();
  m_th_udp_server.join();
  m_udp_broadcast_server.reset(nullptr);
  m_io_service.stop();
}

/*
  Local Variables:
  mode: c++
  End:
*/

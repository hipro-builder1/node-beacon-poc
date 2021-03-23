/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#include "tcp-session.h"

void
TcpSession::start()
{
  m_socket.async_read_some(
    boost::asio::buffer(m_data, MAX_LENGTH),
    boost::bind(
      &TcpSession::handle_read, this, boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void
TcpSession::handle_read(
  const boost::system::error_code& error, size_t bytes_transferred)
{
  if (!error) {
    boost::asio::async_write(
      m_socket, boost::asio::buffer(m_data, bytes_transferred),
      boost::asio::transfer_all(),
      std::bind(&TcpSession::handle_write, this, std::placeholders::_1));
    std::cout << "Pong Send" << std::endl;
  } else {
    std::cerr << "async_read error: connection droped";
    m_socket.close();
  }
}

void
TcpSession::handle_write(const boost::system::error_code& error)
{
  if (!error) {
    m_socket.async_read_some(
      boost::asio::buffer(m_data, MAX_LENGTH),
      std::bind(
        &TcpSession::handle_read, this, std::placeholders::_1,
        std::placeholders::_2));
    std::cout << "Got Ping" << std::endl;
  } else {
    std::cerr << "async_write error: connection droped";
    m_socket.close();
  }
}

/*
  Local Variables:
  mode: c++
  End:
*/

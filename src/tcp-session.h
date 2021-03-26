/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#ifndef HIPRO__bcb24458_87a7_11eb_8e3f_b9b5ee5d4b95
#define HIPRO__bcb24458_87a7_11eb_8e3f_b9b5ee5d4b95

#include <cstdlib>
#include <iostream>
#include <functional>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

class TcpSession
{
public:
  TcpSession(
    boost::asio::ip::tcp::socket&& socket,
    std::function<void(TcpSession*)> callback_function)
    : m_socket(std::move(socket)), m_do_callback(callback_function)
  {}

  void start();

private:
  void handle_read(
    const boost::system::error_code& error, size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);

private:
  boost::asio::ip::tcp::socket m_socket;
  std::function<void(TcpSession*)> m_do_callback;
  enum
  {
    MAX_LENGTH = 4
  };
  char m_data[MAX_LENGTH];
};

#endif // HIPRO__bcb24458_87a7_11eb_8e3f_b9b5ee5d4b95

/*
  Local Variables:
  mode: c++
  End:
*/

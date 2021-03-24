/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#ifndef HIPRO__28d548ec_8b8b_11eb_a950_f921648db99a
#define HIPRO__28d548ec_8b8b_11eb_a950_f921648db99a

#include <stdint.h>

#include <boost/asio/ip/address_v4.hpp>

struct broadcast_tcp_ip_port_s
{
	boost::asio::ip::address_v4::bytes_type tcp_ip;
	uint16_t tcp_port;
} __attribute__((packed));

#endif // HIPRO__28d548ec_8b8b_11eb_a950_f921648db99a

/*
  Local Variables:
  mode: c++
  End:
*/

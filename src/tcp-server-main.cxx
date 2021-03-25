/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#include <cstdlib>
#include <cstdint>
#include <arpa/inet.h>

#include "tcp-server-app.h"

int
main(int argc, char* argv[])
{
  try {
    if (argc != 4) {
      std::cerr << "Usage error: " << argv[0]
                << " <broadcast_port> <tcp_port> <tcp_ip>" << std::endl;
      exit(EXIT_FAILURE);
    }
    int tcp_port_int = std::stoi(argv[2]);
    int broadcast_port_int = std::stoi(argv[1]);
    std::string tcp_ip = argv[3];

    if (
      (tcp_port_int <= static_cast<int>(UINT16_MAX) && tcp_port_int >= 0) &&
      (broadcast_port_int <= static_cast<int>(UINT16_MAX) &&
       broadcast_port_int >= 0)) {

      uint16_t tcp_port_short = static_cast<uint16_t>(tcp_port_int);
      uint16_t broadcast_port = static_cast<uint16_t>(broadcast_port_int);

      TcpServerApp tcp_server_app(tcp_port_short);
      tcp_server_app.run(broadcast_port, tcp_ip);

    } else {
      std::runtime_error("Port must be less than 65535");
    }

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

/*
 Local Variables:
 mode: c++
 End:
*/

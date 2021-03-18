/* Copyright (C) 2008-2021 HiPro IT Solutions Private Limited,
 * Chennai. All rights reserved.
 *
 * This program and the accompanying materials are made available
 * under the terms described in the LICENSE file which accompanies
 * this distribution. If the LICENSE file was not attached to this
 * distribution or for further clarifications, please contact
 * legal@hipro.co.in. */

#include <cstdlib>

#include "tcp-server-app.h"

int
main(int argc, char* argv[])
{
  try {
    if (argc != 3) {
      std::cerr
        << "Usage error: async_tcp_ping_pong_server <port> <tcp_ip:port>\n";
      exit(EXIT_FAILURE);
    }
    TcpServerApp tcp_server_app(atoi(argv[1]));

    tcp_server_app.run(argv[2]);

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

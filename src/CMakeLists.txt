# Copyright (C) 2016-2021 NSEIT Limited, Mumbai, India. All rights
# reserved.
#
# This program and the accompanying materials are made available under
# the terms described in the LICENSE file which accompanies this
# distribution. If the LICENSE file was not attached to this
# distribution or for further clarifications, please contact
# legal@hipro.co.in.

# tcp-server
add_library (tcp-server-lib
 	tcp-server-app.cxx
 	tcp-server-app.h
	tcp-session.cxx
	tcp-session.h
	udp-server.cxx
	udp-server.h
	)

set (Boost_USE_STATIC_LIBS ON)
find_package (Boost REQUIRED COMPONENTS
  log thread system)

target_link_libraries(tcp-server-lib
	${Boost_LIBRARIES}
	)

add_executable (tcp-server
	tcp-server-main.cxx
	)

target_link_libraries(tcp-server
	tcp-server-lib
	)

install (TARGETS tcp-server DESTINATION bin)

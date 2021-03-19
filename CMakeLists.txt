# Copyright (C) 2008-2020 HiPro IT Solutions Private Limited,
# Chennai. All rights reserved.
#
# This program and the accompanying materials are made available under
# the terms described in the LICENSE file which accompanies this
# distribution. If the LICENSE file was not attached to this
# distribution or for further clarifications, please contact
# legal@hipro.co.in.

cmake_minimum_required (VERSION 3.13)
project (tcp-server VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


set (Boost_USE_STATIC_LIBS ON)
find_package (Boost REQUIRED COMPONENTS
   thread date_time system filesystem program_options)

add_subdirectory (src)

cmake_minimum_required(VERSION 3.1.0)

project (node-beacon-poc)

set (Boost_USE_STATIC_LIBS ON)
find_package (Boost REQUIRED COMPONENTS
  log thread system)

add_executable(node-beacon-poc
  client.cxx
  main.cxx
)

target_link_libraries (node-beacon-poc
  ${Threads}
  ${Boost_LIBRARIES}
  )

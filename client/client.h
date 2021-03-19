#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

namespace asio = ::boost::asio;

class Client {
public:
  Client() {}
  bool start (short unsigned port);

private:
  bool receive_handler (std::string recv_data);
  void send_ping (std::string server_ip, std::string server_port);
  void handle_pong (
    const boost::system::error_code &error, std::size_t bytes_transferred);

private:
  boost::asio::io_context m_io_context;
   boost::asio::ip::udp::endpoint m_client;
  std::string m_server_pong;
};

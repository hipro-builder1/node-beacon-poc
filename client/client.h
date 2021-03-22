#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

namespace asio = ::boost::asio;

class Client {
public:
  Client (std::string &port, asio::io_context &io_context);
  bool start ();

private:
  bool receive_handler (std::string recv_data);
  void send_ping (std::string server_ip, std::string server_port);
  void handle_connect (const boost::system::error_code &error);
  void handle_write (
    const boost::system::error_code &error, std::size_t bytes_transferred);
  void handle_pong (
    const boost::system::error_code &error, std::size_t bytes_transferred);

private:
  std::string &m_port;
  asio::io_context &m_io_context;

  std::string m_server_pong;
  asio::io_context m_tcp_context;
  asio::ip::tcp::socket m_tcp_socket;
};

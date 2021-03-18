#include <string>

#include <boost/asio/io_context.hpp>

namespace asio = ::boost::asio;

class Client {
public:
  Client (
    std::string &port, std::string &sender_port, asio::io_context &io_context);
  bool start ();

private:
  bool receive_handler (std::string recv_data);
  void send_ping (std::string server_ip, std::string server_port);
  void handle_pong (
    const boost::system::error_code &error, std::size_t bytes_transferred);

private:
  std::string &m_port;
  std::string &m_sender_port;
  asio::io_context &m_io_context;

  std::string m_server_pong;
};
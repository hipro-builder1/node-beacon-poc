#include <string>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

namespace asio = ::boost::asio;
namespace ip = ::boost::asio::ip;
namespace bsys = ::boost::system;

class Client {
public:
  Client (asio::io_context &io_context, int port);
  void build ();
  void start ();
  void close ();

private:
  void handle_connect (const bsys::error_code &ec);
  void handle_receive (const bsys::error_code &ec, std::size_t bytes);
  void handle_write (const boost::system::error_code &ec, std::size_t bytes);
  void handle_pong (const boost::system::error_code &ec, std::size_t bytes);

private:
  int m_port;
  asio::io_context &m_io_context;

  std::unique_ptr <ip::udp::endpoint> m_ep_udp_server;
  std::unique_ptr <ip::udp::endpoint> m_ep_udp;
  std::unique_ptr <ip::udp::socket> m_sock_udp;
  std::unique_ptr <ip::tcp::socket> m_sock_tcp;

  char m_buff[1024];
};

#ifndef SRC_TCP_SERVER_HPP_
#define SRC_TCP_SERVER_HPP_

#include <functional>

class TCPServer {
 public:
  TCPServer(
    int port,
    std::function<void(const size_t size, const void* data, std::function<void(const size_t size, const void* data)> reply)> on_message
  );
};

#endif  // SRC_TCP_SERVER_HPP_

#ifndef SRC_TCP_SERVER_HPP_
#define SRC_TCP_SERVER_HPP_

#include <functional>

class TCPSever {
 public:
  TCPSever(
    int port,
    std::function<void(size_t size, void* data, std::function<void(size_t size, void* data)> reply)> on_message
  );
};

#endif  // SRC_TCP_SERVER_HPP_

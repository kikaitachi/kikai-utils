#include "logger.hpp"
#include "tcp_server.hpp"
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

TCPServer::TCPServer(
  int port,
  std::function<void(size_t size, void* data, std::function<void(size_t size, void* data)> reply)> on_message
) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    throw std::runtime_error("Failed to create HTTP server socket");
  }
  int sock_option = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &sock_option, sizeof(sock_option)) == -1) {
    throw std::runtime_error("Can't enable SO_REUSEADDR option for HTTP server socket");
  }
  struct sockaddr_in addr;
  std::memset(&addr, '0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    throw std::runtime_error("Failed to bind HTTP server socket");
  }
  if (listen(server_fd, 10) == -1) {
    throw std::runtime_error("Failed to listen to HTTP server socket");
  }
  for ( ; ; ) {
    struct sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    int client_fd = accept(server_fd, &addr, &addr_len);
    if (client_fd == -1) {
      throw std::runtime_error("Failed to accept connection on server socket");
    }
    uint32_t len;
    ssize_t result = read(client_fd, &len, 4);
    if (result != 4) {
      throw std::runtime_error("Failed to read length of audio data");
    }
    char *buffer[len];
    size_t offset = 0;
    while (offset < len) {
      ssize_t result = read(client_fd, buffer + offset, len - offset);
      if (result < 0) {
        throw std::runtime_error("Failed to read audio data");
      }
      offset += result;
    }
    on_message(len, buffer, [client_fd](size_t size, void* data) {
      if (write(client_fd, data, size) != size) {
        throw std::runtime_error("Failed to write message size");
      }
      size_t offset = 0;
      while (offset < size) {
        ssize_t result = write(client_fd, ((char*)data) + offset, size - offset);
        if (result < 0) {
          throw std::runtime_error("Failed to write message");
        }
        offset += result;
      }
    });
    close(client_fd);
  }
}

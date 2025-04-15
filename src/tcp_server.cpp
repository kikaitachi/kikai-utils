#include "logger.hpp"
#include "tcp_server.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

TCPServer::TCPServer(
  int port,
  std::function<void(const size_t size, const void* data, std::function<void(const size_t size, const void* data)> reply)> on_message
) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    logger::last("Failed to create HTTP server socket");
    return;
  }
  int sock_option = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &sock_option, sizeof(sock_option)) == -1) {
    logger::last("Can't enable SO_REUSEADDR option for HTTP server socket");
    return;
  }
  struct sockaddr_in addr;
  std::memset(&addr, '0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    logger::last("Failed to bind HTTP server socket");
    return;
  }
  if (listen(server_fd, 10) == -1) {
    logger::last("Failed to listen to HTTP server socket");
    return;
  }
  for ( ; ; ) {
    struct sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    int client_fd = accept(server_fd, &addr, &addr_len);
    if (client_fd == -1) {
      logger::last("Failed to accept connection on server socket");
      continue;
    }
    uint32_t len;
    ssize_t result = read(client_fd, &len, 4);
    if (result != 4) {
      logger::last("Failed to read length of message");
      close(client_fd);
      continue;
    }
    char *buffer[len];
    size_t offset = 0;
    while (offset < len) {
      ssize_t result = read(client_fd, buffer + offset, len - offset);
      if (result < 0) {
        logger::last("Failed to read message");
        close(client_fd);
        continue;
      }
      offset += result;
    }
    on_message(len, buffer, [client_fd](size_t size, void* data) {
      if (write(client_fd, data, size) != size) {
        logger::last("Failed to write message size");
        close(client_fd);
        return;
      }
      size_t offset = 0;
      while (offset < size) {
        ssize_t result = write(client_fd, ((char*)data) + offset, size - offset);
        if (result < 0) {
          logger::last("Failed to write message");
          close(client_fd);
          return;
        }
        offset += result;
      }
    });
    close(client_fd);
  }
}

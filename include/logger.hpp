#ifndef SRC_LOGGER_HPP_
#define SRC_LOGGER_HPP_

#include <format>
#include <memory>
#include <string>

namespace logger {
  void log_level(const std::string_view level, std::string_view fmt, std::format_args args);

  /**
   * Log a message with a debug level.
   */
  template< class... Args >
  void debug(std::format_string<Args...> fmt, Args&&... args) {
    log_level("⚫", fmt.get(), std::make_format_args(args...));
  }

  /**
   * Log a message with a info level.
   */
  template< class... Args >
  void info(std::format_string<Args...> fmt, Args&&... args) {
    log_level("🟢", fmt.get(), std::make_format_args(args...));
  }

  /**
   * Log a message with a warning level.
   */
  template< class... Args >
  void warn(std::format_string<Args...> fmt, Args&&... args) {
    log_level("🟠", fmt.get(), std::make_format_args(args...));
  }

  /**
   * Log a message with a error level.
   */
  template< class... Args >
  void error(std::format_string<Args...> fmt, Args&&... args) {
    log_level("🔴", fmt.get(), std::make_format_args(args...));
  }

  /**
   * Log a message with a error level and append error from the last call.
   */
  template< class... Args >
  void last(std::format_string<Args...> fmt, Args&&... args) {
    log_level("🟣", fmt.get(), std::make_format_args(args...));
  }
}

#endif  // SRC_LOGGER_HPP_

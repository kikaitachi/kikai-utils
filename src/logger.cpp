#include "logger.hpp"

#include <errno.h>
#include <format>
#include <iostream>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <string>

namespace logger {

void log_level(const std::string_view level, std::string_view fmt, std::format_args args) {
  std::string last_error = "";
  if (level == "🟣") {
    std::string error(strerror(errno));
    last_error += ": " + error;
  }
  timespec timestamp;
  clock_gettime(CLOCK_REALTIME, &timestamp);
  struct tm time;
  localtime_r(&timestamp.tv_sec, &time);
  std::cerr << std::format(
    "{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}.{:09d} {} {}{}\n",
    time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
    time.tm_hour, time.tm_min, time.tm_sec,
    timestamp.tv_nsec, level, std::vformat(fmt, args), last_error
  );
}

}  // namespace logger

#include <atomic>
#include <csignal>
#include "runtime.hpp"

static std::atomic<bool> running = true;
static std::mutex mutex;
static std::condition_variable condition;

static void handler(int signum) {
  if (signum == SIGTERM) {
    running = false;
    condition.notify_all();
  }
}

Runtime::Runtime() {
  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &sa, nullptr);
}

void Runtime::wait_for_termination() {
  std::unique_lock<std::mutex> lock(mutex);
  condition.wait(lock, [this]() { return !running; });
  exit(0);
}

bool Runtime::not_terminated() {
  return running;
}

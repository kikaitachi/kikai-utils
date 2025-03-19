#ifndef SRC_RUNTIME_HPP_
#define SRC_RUNTIME_HPP_

#include <condition_variable>
#include <mutex>

class Runtime {
 public:
  Runtime();
  void wait_for_termination();
  bool not_terminated();
};

#endif  // SRC_RUNTIME_HPP_

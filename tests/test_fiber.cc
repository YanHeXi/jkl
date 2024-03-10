#include "yhx/yhx.h"

yhx::Logger::ptr g_logger = YHX_LOG_ROOT();

void run_in_fiber() {
  YHX_LOG_INFO(g_logger) << "run_in_fiber begin";
  // yhx::Fiber::GetThis()->swapOut();
  yhx::Fiber::YieldToHold();
  YHX_LOG_INFO(g_logger) << "run_in_fiber end";
  yhx::Fiber::YieldToHold();
}

void test_fiber() {
  YHX_LOG_INFO(g_logger) << "main begin -1";
  {
    yhx::Fiber::GetThis();
    YHX_LOG_INFO(g_logger) << "main begin";
    yhx::Fiber::ptr fiber(new yhx::Fiber(run_in_fiber));
    fiber->swapIn();
    YHX_LOG_INFO(g_logger) << "main after swapIn";
    fiber->swapIn();
    YHX_LOG_INFO(g_logger) << "main after end";
    fiber->swapIn();
  }
  YHX_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char **argv) {
  yhx::Thread::SetName("main");
  // test_fiber();

  std::vector<yhx::Thread::ptr> thrs;
  for (int i = 0; i < 3; ++i) {
    thrs.push_back(yhx::Thread::ptr(
        new yhx::Thread(&test_fiber, "name_" + std::to_string(i))));
  }
  for (auto i : thrs) {
    i->join();
  }
  return 0;
}

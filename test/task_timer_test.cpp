#include <string>
#include "../src/task_timer.hpp"
#include "gtest/gtest.h"

TEST(task_timer, times_tasks) {
  task_timer timer("Testing");

  sleep(0.1);

  ASSERT_GT(timer.elapsed(), 0); 
}

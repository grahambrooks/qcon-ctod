#include <string>
#include "../src/task_timer.hpp"
#include "gtest/gtest.h"

TEST(task_timer, times_tasks) {
  task_timer timer("Testing");
  printf("This is timed\n");
  printf("This is timed\n");
  printf("This is timed\n");
  printf("This is timed\n");
  printf("This is timed\n");
  printf("This is timed\n");
  printf("This is timed\n");
}

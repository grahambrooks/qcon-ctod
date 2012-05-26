#ifndef TASK_TIMER_DEFINED
#define TASK_TIMER_DEFINED

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

class task_timer {
protected:
  long start;
  std::string activity_name;
  long usec_time() {
    timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000000 + t.tv_usec;
  }
public:
  task_timer(const char* activity) {
    start = usec_time();
    activity_name = activity;
  }
  
  ~task_timer() {
    auto end = usec_time();

    auto elapsed = end - start;

    
    auto seconds = elapsed / 1000000;
    elapsed = elapsed % 1000000;
    printf("%s took %ld.", activity_name.c_str(), seconds);

    for (auto i = 100000; i > 0; i /= 10) {
      printf("%ld", elapsed / i);
      elapsed = elapsed % i;
    }
    printf("s\n");
  }
};

#endif

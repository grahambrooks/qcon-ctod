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

    auto e = elapsed();

    
    auto seconds = e / 1000000;
    e = e % 1000000;
    printf("%s took %ld.", activity_name.c_str(), seconds);

    for (auto i = 100000; i > 0; i /= 10) {
      printf("%ld", e / i);
      e = e % i;
    }
    printf("s\n");
  }

  long elapsed() {
    auto end = usec_time();
    
    return end - start;
  }
};

#endif

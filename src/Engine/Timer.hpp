#ifndef TIMER_H
#define TIMER_H

class Timer {
  public:
    virtual ~Timer(){};
    virtual double getLastTickTime() = 0;
    virtual double timePerformance() = 0;
};

#endif
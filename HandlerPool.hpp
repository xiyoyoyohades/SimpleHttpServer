#pragma once
using namespace std;

class HandlerPool
{
  private:
    SafeQueue <MySocket> pendingConns;
    
    volatile boolean running;
    static const int num_threads = 1;

  public:
    HandlerPool();
    void start();
    void run(int tid);
    void addConn(MySocket commingConn);
    void stop();
    ~HandlerPool();
};

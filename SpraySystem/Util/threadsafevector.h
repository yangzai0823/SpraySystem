#ifndef THREADSAFEVECTOR_H
#define THREADSAFEVECTOR_H
#include <mutex>
#include <vector>
template <typename T>
class ThreadSafeVector
{
public:
std::mutex VectorMutex;
 void push_back(T val)
 {
      VectorMutex.lock();
      safevector.push_back(val);
      VectorMutex.unlock();
 }
 std::vector<T> & getVector(){return safevector;}
private:
   std::vector<T> safevector;
};

#endif
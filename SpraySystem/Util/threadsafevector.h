#ifndef THREADSAFEVECTOR_H
#define THREADSAFEVECTOR_H
#include <mutex>
#include <vector>
template <typename T>
class ThreadSafeVector {
public:
    std::mutex VectorMutex;
    void push_back(T val) {
        VectorMutex.lock();
        safevector.push_back(val);
        VectorMutex.unlock();
    }
    void removeSome(int64_t current_encoder, int64_t expire_range) {
        VectorMutex.lock();
        int N = safevector.size();
        std::vector<int> ind;
        for (int i = 0; i < N; i++) {
            if (llabs(safevector[i].encoder - current_encoder) > expire_range) {
                ind.push_back(i);
            }
        }

        for (int i = ind.size() - 1; i >= 0; i--) {
            safevector.erase(safevector.begin() + ind[i]);
        }
        VectorMutex.unlock();
    }
    std::vector<T>& getVector() { return safevector; }

private:
    std::vector<T> safevector;
};

#endif

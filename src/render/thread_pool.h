#ifndef RADIANT_THREAD_POOL_H
#define RADIANT_THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>

template<typename T>
class thread_pool {
private:
    bool should_terminate = false;
    std::mutex queue_mutex;
    std::condition_variable mutex_condition;
    std::vector<std::thread> threads;
    std::queue<T> jobs;

    void thread_loop() {
        while (true) {
            T job;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                mutex_condition.wait(lock, [this] {
                    return !jobs.empty() || should_terminate;
                });
                if (should_terminate) {
                    return;
                }
                job = jobs.front();
                jobs.pop();
                std::cout << "\r[info] " << jobs.size() << " jobs left "
                          << std::flush;
            }
            job.run();
        }
    }

public:
    void start(const unsigned int &num_threads) {
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(&thread_pool::thread_loop, this);
        }
    }

    void queue_job(T job) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            jobs.push(job);
        }
        mutex_condition.notify_one();
    }

    bool is_done() {
        bool is_pool_done;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            is_pool_done = jobs.empty();
        }
        return is_pool_done;
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            should_terminate = true;
        }
        mutex_condition.notify_all();
        for (std::thread &active_thread: threads) {
            active_thread.join();
        }
        threads.clear();
    }

    ~thread_pool() { stop(); }
};

#endif //RADIANT_THREAD_POOL_H

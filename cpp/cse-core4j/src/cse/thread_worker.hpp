
#ifndef CSECOREJNI_THREAD_WORKER_HPP
#define CSECOREJNI_THREAD_WORKER_HPP


#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

class thread_worker
{

public:
    thread_worker();

    void work(std::function<void()>);

    ~thread_worker();

private:
    bool stop_ = false;
    bool ready_ = false;
    bool processed_ = false;

    std::mutex m_;
    std::thread worker_;
    std::condition_variable cv_;

    std::function<void()> f_;

    void listen();
};

#endif

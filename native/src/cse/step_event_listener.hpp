
#ifndef CSECOREJNI_STEP_EVENT_LISTENER_HPP
#define CSECOREJNI_STEP_EVENT_LISTENER_HPP

#include <cse/observer.hpp>

#include <condition_variable>
#include <jni.h>
#include <mutex>
#include <thread>

namespace cse
{

class step_event_listener : public observer
{

public:
    step_event_listener(JNIEnv* env, jobject listener);

    void simulator_added(simulator_index index, observable* observable, time_point point) override;
    void simulator_removed(simulator_index index, time_point point) override;
    void variables_connected(variable_id output, variable_id input, time_point point) override;
    void variable_disconnected(variable_id input, time_point point) override;
    void step_complete(step_number lastStep, duration lastStepSize, time_point currentTime) override;
    void simulator_step_complete(simulator_index index, step_number lastStep, duration lastStepSize, time_point currentTime) override;

    ~step_event_listener() override;

private:
    JavaVM* jvm_;
    jmethodID mid_;
    jobject listener_;

    bool stop_ = false;
    bool ready_ = false;
    bool processed_ = false;

    std::mutex m_;
    std::thread worker_;
    std::condition_variable cv_;

    void listen();
    void callback();
};

} // namespace cse


#endif //CSECOREJNI_STEP_EVENT_LISTENER_HPP

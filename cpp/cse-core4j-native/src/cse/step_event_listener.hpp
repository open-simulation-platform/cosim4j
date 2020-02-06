
#ifndef CSECOREJNI_STEP_EVENT_LISTENER_HPP
#define CSECOREJNI_STEP_EVENT_LISTENER_HPP

#include <cse/observer.hpp>
#include <cse/thread_worker.hpp>

#include <jni.h>


namespace cse
{

class step_event_listener : public observer
{

public:
    step_event_listener(JNIEnv* env, jobject listener);

    void simulation_initialized(step_number firstStep, time_point startTime) override;
    void simulator_added(simulator_index index, observable* observable, time_point point) override;
    void simulator_removed(simulator_index index, time_point point) override;
    void variables_connected(variable_id, variable_id, time_point point) override;
    void variable_disconnected(variable_id input, time_point point) override;
    void step_complete(step_number lastStep, duration lastStepSize, time_point currentTime) override;
    void simulator_step_complete(simulator_index index, step_number lastStep, duration lastStepSize, time_point currentTime) override;

    ~step_event_listener() override;

private:
    JavaVM* jvm_;
    jobject listener_;
    jmethodID onStepCompletedId_;
    jmethodID onSimulationInitializedId_;

    thread_worker worker_;
};

inline void step_event_listener::simulator_added(simulator_index, observable*, time_point)
{
}
inline void step_event_listener::simulator_removed(simulator_index, time_point)
{
}
inline void step_event_listener::variables_connected(variable_id, variable_id, time_point)
{
}
inline void step_event_listener::variable_disconnected(variable_id, time_point)
{
}
inline void step_event_listener::simulator_step_complete(simulator_index, step_number, duration, time_point)
{
}

} // namespace cse


#endif //CSECOREJNI_STEP_EVENT_LISTENER_HPP

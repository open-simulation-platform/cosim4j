
#include <cse/jni_helper.hpp>
#include <cse/step_event_listener.hpp>

namespace cse
{

step_event_listener::step_event_listener(JNIEnv* env, jobject listener)
{
    env->GetJavaVM(&jvm_);
    jclass cls = env->GetObjectClass(listener);
    onStepCompletedId_ = env->GetMethodID(cls, "onStepComplete", "(JDD)V");
    onSimulationInitializedId_ = env->GetMethodID(cls, "onSimulationInitialized", "(D)V");
    listener_ = env->NewGlobalRef(listener);
}

void step_event_listener::simulation_initialized(step_number, time_point currentTime)
{
    worker_.work([this, currentTime]() {
        jvm_invoke(jvm_, [this, currentTime](JNIEnv* env) {
            auto jCurrentTime = to_double_time_point(currentTime);
            env->CallVoidMethod(listener_, onSimulationInitializedId_, jCurrentTime);
        });
    });
}

void step_event_listener::step_complete(step_number lastStep, duration lastStepSize, time_point currentTime)
{
    worker_.work([this, lastStep, lastStepSize, currentTime]() {
        jvm_invoke(jvm_, [this, lastStep, lastStepSize, currentTime](JNIEnv* env) {
            auto jLastStep = static_cast<jlong>(lastStep);
            auto jLastStepSize = to_double_duration(lastStepSize, {});
            auto jCurrentTime = to_double_time_point(currentTime);
            env->CallVoidMethod(listener_, onStepCompletedId_, jLastStep, jCurrentTime, jLastStepSize);
        });
    });
}

step_event_listener::~step_event_listener()
{
    jvm_invoke(jvm_, [this](JNIEnv* env) {
        env->DeleteGlobalRef(listener_);
    });
}

}


#include <cse/jni_helper.hpp>
#include <cse/step_event_listener.hpp>

cse::step_event_listener::step_event_listener(JNIEnv* env, jobject listener)
{
    env->GetJavaVM(&jvm_);
    jclass cls = env->GetObjectClass(listener);
    mid_ = env->GetMethodID(cls, "onStepComplete", "(JDD)V");
    listener_ = env->NewGlobalRef(listener);
}

void cse::step_event_listener::step_complete(step_number lastStep, duration lastStepSize, time_point currentTime)
{
    worker_.work([this, lastStep, lastStepSize, currentTime]() {
        jvm_invoke(jvm_, [this, lastStep, lastStepSize, currentTime](JNIEnv* env) {
            auto jLastStep = static_cast<jlong>(lastStep);
            auto jLastStepSize = to_double_duration(lastStepSize, {});
            auto jCurrentTime = to_double_time_point(currentTime);
            env->CallVoidMethod(listener_, mid_, jLastStep, jCurrentTime, jLastStepSize);
        });
    });
}

cse::step_event_listener::~step_event_listener()
{
    jvm_invoke(jvm_, [this](JNIEnv* env) {
        env->DeleteGlobalRef(listener_);
    });
}

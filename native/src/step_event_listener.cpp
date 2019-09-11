
#include <jni.h>
#include "cse/step_event_listener.hpp"

#include <iostream>

cse::step_event_listener::step_event_listener(JNIEnv* env, jobject listener)
{

    jint rs = env->GetJavaVM(&jvm_);
    assert (rs == JNI_OK);

    jclass cls = env->GetObjectClass(listener);
    mid_ = env->GetMethodID(cls, "post", "()V");
    listener_ = env->NewGlobalRef(listener);

}

void cse::step_event_listener::step_complete(cse::step_number lastStep, cse::duration lastStepSize, cse::time_point currentTime)
{
    JNIEnv* env;
    int getEnvStat = jvm_->GetEnv((void **)&env, JNI_VERSION_1_8);
    if (getEnvStat == JNI_EDETACHED) {
        jvm_->AttachCurrentThread((void **)&env, nullptr);
    }
    env->CallVoidMethod(listener_, mid_);

    jvm_->DetachCurrentThread();

}

cse::step_event_listener::~step_event_listener()
{

    JNIEnv* env;
    int getEnvStat = jvm_->GetEnv((void **)&env, JNI_VERSION_1_8);
    if (getEnvStat == JNI_EDETACHED) {
        jvm_->AttachCurrentThread((void **)&env, nullptr);
    }

    env->DeleteGlobalRef(listener_);

    jvm_->DetachCurrentThread();
    
}



////

void cse::step_event_listener::simulator_added(cse::simulator_index index, cse::observable* observable, cse::time_point point)
{
}
void cse::step_event_listener::simulator_removed(cse::simulator_index index, cse::time_point point)
{
}
void cse::step_event_listener::variables_connected(cse::variable_id output, cse::variable_id input, cse::time_point point)
{
}
void cse::step_event_listener::variable_disconnected(cse::variable_id input, cse::time_point point)
{
}
void cse::step_event_listener::simulator_step_complete(cse::simulator_index index, cse::step_number lastStep, cse::duration lastStepSize, cse::time_point currentTime)
{
}




#include <cse/jni_helper.hpp>
#include <cse/step_event_listener.hpp>

cse::step_event_listener::step_event_listener(JNIEnv* env, jobject listener)
{
    env->GetJavaVM(&jvm_);
    jclass cls = env->GetObjectClass(listener);
    mid_ = env->GetMethodID(cls, "post", "()V");
    listener_ = env->NewGlobalRef(listener);
}

void cse::step_event_listener::step_complete(cse::step_number, cse::duration, cse::time_point)
{
    worker_.work([this]() {
        jvm_invoke(jvm_, [this](JNIEnv* env){
            env->CallVoidMethod(listener_, mid_);
        });
    });
}

cse::step_event_listener::~step_event_listener()
{
    jvm_invoke(jvm_, [this](JNIEnv* env){
        env->DeleteGlobalRef(listener_);
    });
}

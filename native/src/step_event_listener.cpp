
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
        JNIEnv* env;
        bool attach = false;
        int getEnvStat = jvm_->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        if (getEnvStat == JNI_EDETACHED) {
            attach = true;
            jvm_->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
        }
        env->CallVoidMethod(listener_, mid_);
        if (attach) {
            jvm_->DetachCurrentThread();
        }
    });
}

cse::step_event_listener::~step_event_listener()
{
    JNIEnv* env;
    bool attach = false;
    int getEnvStat = jvm_->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        attach = true;
        jvm_->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
    }

    env->DeleteGlobalRef(listener_);

    if (attach) {
        jvm_->DetachCurrentThread();
    }
}

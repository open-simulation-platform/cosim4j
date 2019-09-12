
#include "cse/step_event_listener.hpp"

cse::step_event_listener::step_event_listener(JNIEnv* env, jobject listener)
{
    env->GetJavaVM(&jvm_);
    jclass cls = env->GetObjectClass(listener);
    mid_ = env->GetMethodID(cls, "post", "()V");
    listener_ = env->NewGlobalRef(listener);
    worker_ = std::thread(&cse::step_event_listener::listen, this);
}

void cse::step_event_listener::step_complete(cse::step_number lastStep, cse::duration lastStepSize, cse::time_point currentTime)
{

    {
        std::lock_guard<std::mutex> lk(m_);
        ready_ = true;
        processed_ = false;
    }

    cv_.notify_one();

    {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this] {
            return processed_;
        });
    }

    //        std::thread t(&cse::step_event_listener::callback, this);
    //        t.join();
}

void cse::step_event_listener::listen()
{

    while (!stop_) {

        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this] {
            return ready_;
        });

        if (stop_) break;

        callback();

        ready_ = false;
        processed_ = true;

        lk.unlock();
        cv_.notify_one();
    }
}

void cse::step_event_listener::callback()
{

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
}

cse::step_event_listener::~step_event_listener()
{

    stop_ = true;
    {
        std::lock_guard<std::mutex> lk(m_);
        ready_ = true;
    }

    cv_.notify_one();
    worker_.join();

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


#ifndef CSECOREJNI_JNI_HELPER_HPP
#define CSECOREJNI_JNI_HELPER_HPP

#include <functional>
#include <iostream>
#include <jni.h>
#include <iostream>

namespace
{

void jvm_invoke(JavaVM* jvm, const std::function<void(JNIEnv*)>& f)
{

    JNIEnv* env;
    bool attach = false;
    int getEnvStat = jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        attach = true;
        jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
    }

    f(env);

    if (attach) {
        jvm->DetachCurrentThread();
    }
}

std::string invoke_string_getter(JNIEnv* env, jobject slave, jmethodID mid)
{
    auto jname = reinterpret_cast<jstring>(env->CallObjectMethod(slave, mid));
    if (jname == nullptr) return "";
    auto cname = env->GetStringUTFChars(jname, nullptr);
    auto name = std::string(cname);
    env->ReleaseStringUTFChars(jname, cname);
    return name;
}

inline jmethodID GetMethodID(JNIEnv* env, jclass cls, const char* name, const char* sig)
{
    jmethodID id = env->GetMethodID(cls, name, sig);
    if (id == nullptr) {
        std::cerr << "Unable to locate method '" << name << "'!";
    }
    return id;
}

inline jclass FindClass(JNIEnv* env, const char* name)
{
    jclass cls = env->FindClass(name);
    if (cls == nullptr) {
        std::cerr << "Unable to find class '" << name << "'!" << std::endl;
    }
    return cls;
}

} // namespace

#endif

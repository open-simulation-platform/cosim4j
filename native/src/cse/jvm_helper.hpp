
#ifndef CSECOREJNI_JVM_HELPER_HPP
#define CSECOREJNI_JVM_HELPER_HPP

#include <functional>
#include <iostream>
#include <jni.h>


namespace
{

void jvm_invoke(JavaVM* jvm, std::function<void(JNIEnv*)> f)
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
    auto cname = env->GetStringUTFChars(jname, nullptr);
    env->ReleaseStringUTFChars(jname, cname);
    return cname;
}

} // namespace

#endif


#ifndef CSECOREJNI_JVM_HELPER_HPP
#define CSECOREJNI_JVM_HELPER_HPP

#include <jni.h>

#include <functional>

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

} // namespace

#endif

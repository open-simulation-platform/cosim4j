
#ifndef CSECOREJNI_ERROR_HELPER_HPP
#define CSECOREJNI_ERROR_HELPER_HPP

#include <jni.h>

namespace
{

jobject create_error(JNIEnv* env)
{

    jclass cls = env->FindClass("org/osp/cse/CseError");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;)V");

    return env->NewObject(
        cls,
        ctor,
        cse_last_error_code(),
        env->NewStringUTF(cse_last_error_message()));
}

} // namespace

#endif //CSECOREJNI_ERROR_HELPER_HPP

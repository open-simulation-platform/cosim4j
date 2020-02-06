
#include <cse.h>

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getLastError(JNIEnv* env, jobject)
{
    jclass cls = env->FindClass("org/osp/cse/CseError");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;)V");

    return env->NewObject(
        cls,
        ctor,
        cse_last_error_code(),
        env->NewStringUTF(cse_last_error_message()));
}

#ifdef __cplusplus
}
#endif

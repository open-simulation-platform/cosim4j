
#include <cosim.h>

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jobject JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_getLastError(JNIEnv* env, jobject)
{
    jclass cls = env->FindClass("com/opensimulationplatform/cosim/CosimError");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;)V");

    return env->NewObject(
        cls,
        ctor,
        cosim_last_error_code(),
        env->NewStringUTF(cosim_last_error_message()));
}

#ifdef __cplusplus
}
#endif


#include <cse/error_helper.hpp>

#include <jni.h>


#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getLastError(JNIEnv* env, jobject obj)
{
    return create_error(env);
}

#ifdef __cplusplus
}
#endif

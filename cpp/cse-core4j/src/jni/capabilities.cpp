
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_hasPython(JNIEnv*, jobject)
{
#ifdef HAS_PYTHON
    return true;
#else
return false;
#endif
}

#ifdef __cplusplus
}
#endif
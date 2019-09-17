
#include <cse.h>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectIntegers(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_connect_integer_variables(reinterpret_cast<cse_execution*>(executionPtr), outputSlaveIndex, (cse_value_reference)outputSlaveIndex, inputSlaveIndex, (cse_value_reference)inputSlaveIndex);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectReals(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_connect_real_variables(reinterpret_cast<cse_execution*>(executionPtr), outputSlaveIndex, (cse_value_reference)outputSlaveIndex, inputSlaveIndex, (cse_value_reference)inputSlaveIndex);
}


#ifdef __cplusplus
}
#endif

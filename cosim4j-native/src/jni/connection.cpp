
#include <cosim.h>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_connectIntegers(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cosim_execution_connect_integer_variables(reinterpret_cast<cosim_execution*>(executionPtr), outputSlaveIndex, (cosim_value_reference)outputSlaveIndex, inputSlaveIndex, (cosim_value_reference)inputSlaveIndex);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_connectReals(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cosim_execution_connect_real_variables(reinterpret_cast<cosim_execution*>(executionPtr), outputSlaveIndex, (cosim_value_reference)outputSlaveIndex, inputSlaveIndex, (cosim_value_reference)inputSlaveIndex);
}


#ifdef __cplusplus
}
#endif

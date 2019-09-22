
#include <cse.h>
#include <cse/model_description_helper.hpp>
#include <cse/slave_infos_helper.hpp>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getModelDescription(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }
    return create_model_description(env, executionPtr, slaveIndex);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSlave(JNIEnv* env, jobject obj, jstring fmuPath)
{
    const char* fmuPath_ = env->GetStringUTFChars(fmuPath, nullptr);
    cse_slave* slave = cse_local_slave_create(fmuPath_);
    env->ReleaseStringUTFChars(fmuPath, fmuPath_);
    return reinterpret_cast<jlong>(slave);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroySlave(JNIEnv* env, jobject obj, jlong slave)
{
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
        return false;
    }
    return cse_local_slave_destroy(reinterpret_cast<cse_slave*>(slave)) == 0;
}

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_addSlave(JNIEnv* env, jobject obj, jlong executionPtr, jlong slave)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
        return false;
    }

    return static_cast<jint>(cse_execution_add_slave(reinterpret_cast<cse_execution*>(executionPtr), reinterpret_cast<cse_slave*>(slave)));
}

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumSlaves(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return static_cast<jint>(cse_execution_get_num_slaves(reinterpret_cast<cse_execution*>(executionPtr)));
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getSlaveInfos(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }

    return create_slave_infos(env, executionPtr);
}


#ifdef __cplusplus
}
#endif
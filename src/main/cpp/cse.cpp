
#include <jni.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "cse.h"

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jstring JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorMessage(JNIEnv *env, jobject obj) {
    const char* msg = cse_last_error_message();
    return env->NewStringUTF(msg);
}


JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv *env, jobject obj, jdouble startTime, jdouble stepSize) {
    auto execution = cse_execution_create((cse_time_point) (1000000000.0 * startTime), (cse_duration) (1000000000.0 * stepSize));
    if (execution == 0) {
        std::cerr << "[JNI-wrapper]" << cse_last_error_message() << std::endl;
    }
    return (jlong) execution;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
    }
    return cse_execution_destroy((cse_execution*) execution) == 0;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createLocalSlave(JNIEnv *env, jobject obj, jstring fmuPath) {
    const char* _fmuPath = env->GetStringUTFChars(fmuPath, 0);
    cse_slave* slave = cse_local_slave_create(_fmuPath);
    env->ReleaseStringUTFChars(fmuPath, _fmuPath);
    return (jlong) slave;
}


JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_addSlave(JNIEnv *env, jobject obj, jlong execution, jlong slave) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
    }
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] slave is NULL" << std::endl;
    }

    return (jint) cse_execution_add_slave((cse_execution*) execution, (cse_slave*) slave);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_step(JNIEnv *env, jobject obj, jlong execution, jlong numSteps) {
    return cse_execution_step((cse_execution*) execution, numSteps) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_start(JNIEnv *env, jobject obj, jlong execution) {
    return cse_execution_start((cse_execution*) execution) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stop(JNIEnv *env, jobject obj, jlong execution) {
    return cse_execution_stop((cse_execution*) execution) == 0;
}

#ifdef __cplusplus
}
#endif
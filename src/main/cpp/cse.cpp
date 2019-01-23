
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "cse.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv *env, jobject obj, jlong startTime, jlong stepSize) {
    return (jlong) cse_execution_create(startTime, stepSize);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv *env, jobject obj, jlong execution) {
    return cse_execution_destroy((cse_execution*) execution) == 0;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createLocalSlave(JNIEnv *env, jobject obj, jstring fmuPath) {
    const char* _fmuPath = env->GetStringUTFChars(fmuPath, 0);
    cse_slave* slave = cse_local_slave_create(_fmuPath);
    env->ReleaseStringUTFChars(fmuPath, _fmuPath);
    return (jlong) slave;
}


JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_addSlave(JNIEnv *env, jobject obj, jlong execution, jlong slave) {
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
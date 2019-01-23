
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
        return 0;
    }
    return (jlong) execution;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
        return false;
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
       return false;
    }
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] slave is NULL" << std::endl;
        return false;
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

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_enableRealTimeSimulation(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
        return false;
    }
    return cse_execution_enable_real_time_simulation((cse_execution*) execution) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_disableRealTimeSimulation(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
        return false;
    }
    return cse_execution_disable_real_time_simulation((cse_execution*) execution) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getInteger(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlongArray vr, jintArray ref) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] observer is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);

    int* _ref = (int*) malloc(sizeof(int) * size);

    std::vector<cse_variable_index> __vr(size);
    for (int i = 0; i < size; i++) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_observer_slave_get_integer((cse_observer*) observer, slaveIndex, __vr.data(), size, _ref) == 0;

    env->SetIntArrayRegion(ref, 0, size, _ref);

    free(_ref);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getReal(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlongArray vr, jdoubleArray ref) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] observer is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);

    double* _ref = (double*) malloc(sizeof(double) * size);

    std::vector<cse_variable_index> __vr(size);
    for (int i = 0; i < size; i++) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_observer_slave_get_real((cse_observer*) observer, slaveIndex, __vr.data(), size, _ref) == 0;

    env->SetDoubleArrayRegion(ref, 0, size, _ref);

    free(_ref);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;

}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInteger(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex, jlongArray vr, jintArray values) {

    if (execution == 0) {
       std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);
    jint *_values = env->GetIntArrayElements(values, 0);

    std::vector<cse_variable_index> __vr(size);
    for (int i = 0; i < size; i++) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_execution_slave_set_integer((cse_execution*) execution, slaveIndex, __vr.data(), size, _values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseIntArrayElements(values, _values, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setReal(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex, jlongArray vr, jdoubleArray values) {

    if (execution == 0) {
       std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);
    jdouble *_values = env->GetDoubleArrayElements(values, 0);

    std::vector<cse_variable_index> __vr(size);
    for (int i = 0; i < size; i++) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_execution_slave_set_real((cse_execution*) execution, slaveIndex, __vr.data(), size, _values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseDoubleArrayElements(values, _values, 0);

    return status;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createMembufferObserver(JNIEnv *env, jobject obj) {
    auto observer = cse_membuffer_observer_create();
    if (observer == 0) {
        std::cerr << "[JNI-wrapper]" << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) observer;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserver(JNIEnv *env, jobject obj, jstring logDir) {
    const char* _logDir = env->GetStringUTFChars(logDir, 0);
    auto observer = cse_file_observer_create(_logDir);
    env->ReleaseStringUTFChars(logDir, _logDir);
    if (observer == 0) {
        std::cerr << "[JNI-wrapper]" << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) observer;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyObserver(JNIEnv *env, jobject obj, jlong observer) {
    if (observer == 0) {
        std::cerr << "[JNI-wrapper] observer is NULL" << std::endl;
        return false;
    }
    return cse_observer_destroy((cse_observer*) observer) == 0;
}

#ifdef __cplusplus
}
#endif
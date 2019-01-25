
#include <jni.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "cse.h"

#ifdef __cplusplus
extern "C" {
#endif


namespace {

    const double sec2nano = 1e9;

    cse_time_point to_cse_time_point(jdouble time_point) {
        return time_point * sec2nano;
    }

    cse_duration to_cse_duration(jdouble duration) {
        return duration * sec2nano;
    }

}

JNIEXPORT jstring JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorMessage(JNIEnv *env, jobject obj) {
    const char* msg = cse_last_error_message();
    return env->NewStringUTF(msg);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv *env, jobject obj, jdouble startTime, jdouble stepSize) {
    cse_execution* execution = cse_execution_create(to_cse_time_point(startTime), to_cse_duration(stepSize));
    if (execution == 0) {
        std::cerr << "[JNI-wrapper]" << "Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) execution;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecutionFromSsp(JNIEnv *env, jobject obj, jstring sspDir, jdouble startTime) {
    const char* _sspDir = env->GetStringUTFChars(sspDir, 0);
    cse_execution* execution = cse_ssp_execution_create(_sspDir, to_cse_time_point(startTime));
    env->ReleaseStringUTFChars(sspDir, _sspDir);
    if (execution == 0) {
        std::cerr << "[JNI-wrapper]" << "Failed to create execution: " << cse_last_error_message() << std::endl;
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

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_start(JNIEnv *env, jobject obj, jlong execution) {
    return cse_execution_start((cse_execution*) execution) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_step(JNIEnv *env, jobject obj, jlong execution, jlong numSteps) {
    return cse_execution_step((cse_execution*) execution, numSteps) == 0;
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

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStatus(JNIEnv *env, jobject obj, jlong execution, jobject status) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
        return false;
    }

    const char* className = "org/osp/cse/CseExecutionStatusImpl";
    jclass cls = env->FindClass(className);

    if (cls == 0) {
        std::cerr << "[JNI-wrapper] Fatal: Could not locate '" << className << "'" << std::endl;
        return false;
    }

    jfieldID currentTimeId = env->GetFieldID(cls, "currentTime", "D");
    jfieldID stateId = env->GetFieldID(cls, "stateId", "I");
    jfieldID errorId = env->GetFieldID(cls, "errorCodeId", "I");
    jfieldID realTimeFactorId = env->GetFieldID(cls, "realTimeFactor", "D");
    jfieldID realTimeSimulationId = env->GetFieldID(cls, "realTimeSimulation", "Z");

    cse_execution_status _status;
    jboolean success = cse_execution_get_status((cse_execution*) execution, &_status);

    env->SetDoubleField(status, currentTimeId, ((double) _status.current_time) / 1e9);
    env->SetDoubleField(status, realTimeFactorId, _status.real_time_factor);
    env->SetIntField(status, stateId, _status.state);
    env->SetIntField(status, errorId, _status.error_code);
    env->SetBooleanField(status, realTimeSimulationId, _status.is_real_time_simulation == 0);

    return success;
}

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumSlaves(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
       return false;
    }
    return (jint) cse_execution_get_num_slaves((cse_execution*) execution);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getSlaveInfos(JNIEnv *env, jobject obj, jlong execution, jobjectArray infos) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(infos);

    cse_slave_info* _infos = (cse_slave_info*) malloc(sizeof(cse_slave_info)*size);
    jboolean status = cse_execution_get_slave_infos((cse_execution*) execution, _infos, size) == 0;

    const char* className = "org/osp/cse/CseSlaveInfo";
    jclass cls = env->FindClass(className);

    if (cls == 0) {
        std::cerr << "[JNI-wrapper] Fatal: Could not locate '" << className << "'" << std::endl;
        return false;
    }

    jfieldID nameId = env->GetFieldID(cls, "name_", "Ljava/lang/String;");
    jfieldID sourceId = env->GetFieldID(cls, "source_", "Ljava/lang/String;");
    jfieldID indexId = env->GetFieldID(cls, "index", "I");

    for (int i = 0; i < size; i++) {
        jobject info = env->GetObjectArrayElement(infos, i);
        env->SetObjectField(info, nameId, env->NewStringUTF(_infos[i].name));
        env->SetObjectField(info, sourceId, env->NewStringUTF(_infos[i].source));
        env->SetIntField(info, indexId, _infos[i].index);
    }

    free(_infos);

    return status;
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

    env->SetIntArrayRegion(ref, 0, size, (jint*)_ref);

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

    jboolean status = cse_execution_slave_set_integer((cse_execution*) execution, slaveIndex, __vr.data(), size, (int*)_values) == 0;

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

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbersForDuration(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jdouble duration, jlongArray steps) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] observer is NULL" << std::endl;
       return false;
    }

     const size_t size = 2;
     cse_step_number* _steps = (cse_step_number*) malloc(sizeof(cse_step_number) * size);

    jboolean status = cse_observer_get_step_numbers_for_duration((cse_observer*) observer, slaveIndex, to_cse_duration(duration), _steps);

    for (int i = 0; i < size; i++) {
        jlong step = (jlong) _steps[i];
        env->SetLongArrayRegion(steps, i, 1, &step);
    }

    free(_steps);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbers(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jdouble begin, jdouble end, jlongArray steps) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] observer is NULL" << std::endl;
       return false;
    }

    const size_t size = 2;
    cse_step_number* _steps = (cse_step_number*) malloc(sizeof(cse_step_number) * size);

    jboolean status = cse_observer_get_step_numbers((cse_observer*) observer, slaveIndex, to_cse_time_point(begin), to_cse_time_point(end), _steps);

    for (int i = 0; i < size; i++) {
        jlong step = (jlong) _steps[i];
        env->SetLongArrayRegion(steps, i, 1, &step);
    }

    free(_steps);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectIntegers(JNIEnv *env, jobject obj, jlong execution, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
       return false;
    }
    return cse_execution_connect_integer_variables((cse_execution*) execution, outputSlaveIndex, (cse_variable_index) outputSlaveIndex, inputSlaveIndex, (cse_variable_index) inputSlaveIndex);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectReals(JNIEnv *env, jobject obj, jlong execution, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
       return false;
    }
    return cse_execution_connect_real_variables((cse_execution*) execution, outputSlaveIndex, (cse_variable_index) outputSlaveIndex, inputSlaveIndex, (cse_variable_index) inputSlaveIndex);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createMembufferObserver(JNIEnv *env, jobject obj) {
    auto observer = cse_membuffer_observer_create();
    if (observer == 0) {
        std::cerr << "[JNI-wrapper]" << "Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) observer;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserver(JNIEnv *env, jobject obj, jstring logDir) {
    const char* _logDir = env->GetStringUTFChars(logDir, 0);
    cse_observer* observer = cse_file_observer_create(_logDir);
    env->ReleaseStringUTFChars(logDir, _logDir);
    if (observer == 0) {
        std::cerr << "[JNI-wrapper]" << "Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) observer;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addObserver(JNIEnv *env, jobject obj, jlong execution, jlong observer) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] execution is NULL" << std::endl;
        return false;
    }

    if (observer == 0) {
        std::cerr << "[JNI-wrapper] observer is NULL" << std::endl;
        return false;
    }
    return cse_execution_add_observer((cse_execution*) execution, (cse_observer*) observer) == 0;
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
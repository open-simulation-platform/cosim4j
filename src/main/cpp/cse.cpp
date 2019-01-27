
#include <jni.h>
#include <stdlib.h>
#include <iostream>

#include "cse.h"
#include "CseSamplesFields.hpp"
#include "CseSamplesDirectFields.hpp"
#include "CseExecutionStatusFields.hpp"


#ifdef __cplusplus
extern "C" {
#endif


namespace {

    const double sec2nano = 1e9;
    const double nano2sec = 1.0/sec2nano;

    cse_time_point to_cse_time_point(jdouble time_point) {
        return time_point * sec2nano;
    }

    cse_duration to_cse_duration(jdouble duration) {
        return duration * sec2nano;
    }

    jdouble to_seconds(int64_t duration) {
        return ((double) duration) * nano2sec;
    }

}

JNIEXPORT jstring JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorMessage(JNIEnv *env, jobject obj) {
    const char* msg = cse_last_error_message();
    return env->NewStringUTF(msg);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv *env, jobject obj, jdouble startTime, jdouble stepSize) {
    cse_execution* execution = cse_execution_create(to_cse_time_point(startTime), to_cse_duration(stepSize));
    if (execution == 0) {
        std::cerr << "[JNI-wrapper]" << "Error: Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) execution;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecutionFromSsp(JNIEnv *env, jobject obj, jstring sspDir, jdouble startTime) {
    const char* _sspDir = env->GetStringUTFChars(sspDir, 0);
    cse_execution* execution = cse_ssp_execution_create(_sspDir, to_cse_time_point(startTime));
    env->ReleaseStringUTFChars(sspDir, _sspDir);
    if (execution == 0) {
        std::cerr << "[JNI-wrapper]" << "Error: Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) execution;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
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
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
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
        std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
        return false;
    }
    return cse_execution_enable_real_time_simulation((cse_execution*) execution) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_disableRealTimeSimulation(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
        return false;
    }
    return cse_execution_disable_real_time_simulation((cse_execution*) execution) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStatus(JNIEnv *env, jobject obj, jlong execution, jobject status) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
        return false;
    }

    cse_execution_status _status;
    jboolean success = cse_execution_get_status((cse_execution*) execution, &_status);

    initCseExecutionStatusFields(env);

    env->SetIntField(status, cseExecutionStatusFields.stateId, _status.state);
    env->SetIntField(status, cseExecutionStatusFields.errorId, _status.error_code);
    env->SetDoubleField(status, cseExecutionStatusFields.realTimeFactorId, _status.real_time_factor);
    env->SetDoubleField(status, cseExecutionStatusFields.currentTimeId, to_seconds(_status.current_time));
    env->SetBooleanField(status, cseExecutionStatusFields.realTimeSimulationId, _status.is_real_time_simulation == 0);

    return success;
}

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumSlaves(JNIEnv *env, jobject obj, jlong execution) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }
    return (jint) cse_execution_get_num_slaves((cse_execution*) execution);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getSlaveInfos(JNIEnv *env, jobject obj, jlong execution, jobjectArray infos) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(infos);

    cse_slave_info* _infos = (cse_slave_info*) malloc(sizeof(cse_slave_info)*size);
    jboolean status = cse_execution_get_slave_infos((cse_execution*) execution, _infos, size) == 0;

    const char* className = "org/osp/cse/CseSlaveInfo";
    jclass cls = env->FindClass(className);

    if (cls == 0) {
        std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'" << std::endl;
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

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setReal(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex, jlongArray vr, jdoubleArray values) {

    if (execution == 0) {
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);
    jdouble *_values = env->GetDoubleArrayElements(values, 0);

    jboolean status = cse_execution_slave_set_real((cse_execution*) execution, slaveIndex, (cse_variable_index*) _vr, size, _values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseDoubleArrayElements(values, _values, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getReal(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlongArray vr, jdoubleArray ref) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);

    double* _ref = (double*) malloc(sizeof(double) * size);

    jboolean status = cse_observer_slave_get_real((cse_observer*) observer, slaveIndex, (cse_variable_index*) _vr, size, _ref) == 0;

    env->SetDoubleArrayRegion(ref, 0, size, _ref);

    free(_ref);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamples(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return NULL;
    }

    double* values = (double*) malloc(sizeof(double) * nSamples);
    cse_step_number* steps = (cse_step_number*) malloc(sizeof(cse_step_number*) * nSamples);
    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint) cse_observer_slave_get_real_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);

    jdoubleArray _values = env->NewDoubleArray(numSamplesRead);
    jdoubleArray _times = env->NewDoubleArray(numSamplesRead);
    jlongArray _steps = env->NewLongArray(numSamplesRead);

    env->SetDoubleArrayRegion(_values, 0, numSamplesRead, values);

    for (int i = 0; i < numSamplesRead; i++) {
        jlong step = (jlong) steps[i];
        double time = to_seconds(times[i]);

        env->SetDoubleArrayRegion(_times, i, 1, &time);
        env->SetLongArrayRegion(_steps, i, 1, &step);
    }


    initCseRealSamplesFields(env);
    env->SetIntField(samples, cseRealSamplesFields.sizeId, numSamplesRead);
    env->SetObjectField(samples, cseRealSamplesFields.valuesId, _values);
    env->SetObjectField(samples, cseRealSamplesFields.stepsId, _steps);
    env->SetObjectField(samples, cseRealSamplesFields.timesId, _times);

    free(values);
    free(steps);
    free(times);

    return samples;
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamplesDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return NULL;
    }

    double* values = (double*) malloc(sizeof(double) * nSamples);
    cse_step_number* steps = (cse_step_number*) malloc(sizeof(cse_step_number*) * nSamples);
    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint) cse_observer_slave_get_real_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);

    double* times_ = (double*) malloc(sizeof(double)* numSamplesRead);
    for (int i = 0; i < numSamplesRead; i++) {
        times_[i] = to_seconds(times[i]);
    }
    free(times);



    jobject valueBuffer = env->NewDirectByteBuffer(values, 8 * numSamplesRead);
    jobject timeBuffer = env->NewDirectByteBuffer(times_, 8 * numSamplesRead);
    jobject stepBuffer = env->NewDirectByteBuffer(steps, 8 * numSamplesRead);

    initCseRealSamplesDirectFields(env);
    env->SetIntField(samples, cseRealSamplesDirectFields.sizeId, numSamplesRead);
    env->SetObjectField(samples, cseRealSamplesDirectFields.valuesId, valueBuffer);
    env->SetObjectField(samples, cseRealSamplesDirectFields.timesId, timeBuffer);
    env->SetObjectField(samples, cseRealSamplesDirectFields.stepsId, stepBuffer);

    return samples;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInteger(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex, jlongArray vr, jintArray values) {

    if (execution == 0) {
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);
    jint *_values = env->GetIntArrayElements(values, 0);

    jboolean status = cse_execution_slave_set_integer((cse_execution*) execution, slaveIndex, (cse_variable_index*) _vr, size, (int*)_values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseIntArrayElements(values, _values, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getInteger(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlongArray vr, jintArray ref) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);

    int* _ref = (int*) malloc(sizeof(int) * size);

    jboolean status = cse_observer_slave_get_integer((cse_observer*) observer, slaveIndex, (cse_variable_index*) _vr, size, _ref) == 0;

    env->SetIntArrayRegion(ref, 0, size, (jint*)_ref);

    free(_ref);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamples(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return NULL;
    }

    const char* className = "org/osp/cse/CseIntegerSamples";
    jclass cls = env->FindClass(className);

    if (cls == 0) {
        std::cerr << "[JNI-wrapper] Fatal: Could not locate '" << className << "'" << std::endl;
        return NULL;
    }

    jfieldID sizeId = env->GetFieldID(cls, "size", "I");
    jfieldID valuesId = env->GetFieldID(cls, "values", "[I");
    jfieldID stepsId = env->GetFieldID(cls, "steps", "[J");
    jfieldID timesId = env->GetFieldID(cls, "times", "[D");

    jmethodID constructor = env->GetMethodID(cls, "<init>", "()V");
    jobject samples = env->NewObject(cls, constructor);

    int* values = (int*) malloc(sizeof(int) * nSamples);
    cse_step_number* steps = (cse_step_number*) malloc(sizeof(cse_step_number*) * nSamples);
    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint) cse_observer_slave_get_integer_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);

    jintArray _values = env->NewIntArray(numSamplesRead);
    jdoubleArray _times = env->NewDoubleArray(numSamplesRead);
    jlongArray _steps = env->NewLongArray(numSamplesRead);

    env->SetIntArrayRegion(_values, 0, numSamplesRead, values);

    for (int i = 0; i < numSamplesRead; i++) {
        jlong step = (jlong) steps[i];
        double time = to_seconds(times[i]);

        env->SetDoubleArrayRegion(_times, i, 1, &time);
        env->SetLongArrayRegion(_steps, i, 1, &step);
    }

    env->SetIntField(samples, sizeId, numSamplesRead);
    env->SetObjectField(samples, valuesId, _values);
    env->SetObjectField(samples, stepsId, _steps);
    env->SetObjectField(samples, timesId, _times);

    free(values);
    free(steps);
    free(times);

    return samples;
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamplesDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return NULL;
    }

    int* values = (int*) malloc(sizeof(int) * nSamples);
    cse_step_number* steps = (cse_step_number*) malloc(sizeof(cse_step_number*) * nSamples);
    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint) cse_observer_slave_get_integer_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);

    double* times_ = (double*) malloc(sizeof(double)* numSamplesRead);
    for (int i = 0; i < numSamplesRead; i++) {
        times_[i] = to_seconds(times[i]);
    }
    free(times);

    const char* className = "org/osp/cse/CseRealSamplesDirect";
    jclass cls = env->FindClass(className);
    if (cls == 0) {
        std::cerr << "[JNI-wrapper] Error: Fatal: Could not locate '" << className << "'" << std::endl;
        return NULL;
    }

    jfieldID sizeId = env->GetFieldID(cls, "size", "I");
    jfieldID valuesId = env->GetFieldID(cls, "valueBuffer", "Ljava/nio/ByteBuffer;");
    jfieldID timesId = env->GetFieldID(cls, "timeBuffer",  "Ljava/nio/ByteBuffer;");
    jfieldID stepsId = env->GetFieldID(cls, "stepBuffer", "Ljava/nio/ByteBuffer;");

    jmethodID constructor = env->GetMethodID(cls, "<init>", "()V");
    jobject samples = env->NewObject(cls, constructor);

    jobject valueBuffer = env->NewDirectByteBuffer(values, 4 * numSamplesRead);
    jobject timeBuffer = env->NewDirectByteBuffer(times_, 8 * numSamplesRead);
    jobject stepBuffer = env->NewDirectByteBuffer(steps, 8 * numSamplesRead);

    env->SetIntField(samples, sizeId, numSamplesRead);
    env->SetObjectField(samples, valuesId, valueBuffer);
    env->SetObjectField(samples, timesId, timeBuffer);
    env->SetObjectField(samples, stepsId, stepBuffer);

    return samples;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbersForDuration(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jdouble duration, jlongArray steps) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
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
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
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
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }
    return cse_execution_connect_integer_variables((cse_execution*) execution, outputSlaveIndex, (cse_variable_index) outputSlaveIndex, inputSlaveIndex, (cse_variable_index) inputSlaveIndex);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectReals(JNIEnv *env, jobject obj, jlong execution, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }
    return cse_execution_connect_real_variables((cse_execution*) execution, outputSlaveIndex, (cse_variable_index) outputSlaveIndex, inputSlaveIndex, (cse_variable_index) inputSlaveIndex);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createMembufferObserver(JNIEnv *env, jobject obj) {
    auto observer = cse_membuffer_observer_create();
    if (observer == 0) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) observer;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserver(JNIEnv *env, jobject obj, jstring logDir) {
    const char* _logDir = env->GetStringUTFChars(logDir, 0);
    cse_observer* observer = cse_file_observer_create(_logDir);
    env->ReleaseStringUTFChars(logDir, _logDir);
    if (observer == 0) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) observer;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addObserver(JNIEnv *env, jobject obj, jlong execution, jlong observer) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
        return false;
    }

    if (observer == 0) {
        std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
        return false;
    }
    return cse_execution_add_observer((cse_execution*) execution, (cse_observer*) observer) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyObserver(JNIEnv *env, jobject obj, jlong observer) {
    if (observer == 0) {
        std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
        return false;
    }
    return cse_observer_destroy((cse_observer*) observer) == 0;
}

#ifdef __cplusplus
}
#endif
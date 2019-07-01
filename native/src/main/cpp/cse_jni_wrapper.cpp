
#include <string>
#include <iostream>

#include "cse_jni_wrapper.hpp"

#include "cse_samples_fields.hpp"
#include "cse_samples_direct_fields.hpp"
#include "cse_execution_status_fields.hpp"


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

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorCode_(JNIEnv *env, jobject obj) {
    return cse_last_error_code();
}

JNIEXPORT jstring JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorMessage(JNIEnv *env, jobject obj) {
    const char* msg = cse_last_error_message();
    return env->NewStringUTF(msg);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv *env, jobject obj, jdouble startTime, jdouble stepSize) {
    cse_execution* execution = cse_execution_create(to_cse_time_point(startTime), to_cse_duration(stepSize));
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]" << "Error: Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) execution;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSspExecution(JNIEnv *env, jobject obj, jstring sspDir, jdouble startTime) {
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

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSlave(JNIEnv *env, jobject obj, jstring fmuPath) {
    const char* _fmuPath = env->GetStringUTFChars(fmuPath, 0);
    cse_slave* slave = cse_local_slave_create(_fmuPath);
    env->ReleaseStringUTFChars(fmuPath, _fmuPath);
    return (jlong) slave;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroySlave(JNIEnv *env, jobject obj, jlong slave) {
     if (slave == 0) {
       std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
       return false;
    }
    return cse_local_slave_destroy((cse_slave*) slave) == 0;
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

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setRealTimeFactorTarget(JNIEnv *env, jobject obj, jlong execution, jdouble realTimeFactor) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
        return false;
    }
    return cse_execution_set_real_time_factor_target((cse_execution*) execution, realTimeFactor) == 0;
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

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumVariables(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }
    return (jint) cse_slave_get_num_variables((cse_execution*) execution, slaveIndex);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getVariables(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex, jobjectArray vars) {
    if (execution == 0) {
       std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
       return false;
    }

    const char* className = "org/osp/cse/CseVariableDescription";
    jclass cls = env->FindClass(className);

    if (cls == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vars);
    cse_variable_description* _vars = (cse_variable_description*) malloc(sizeof(cse_variable_description)*size);
    jboolean status = cse_slave_get_variables((cse_execution*) execution, slaveIndex, _vars, size) != -1;

    if (status) {

        jfieldID vrId = env->GetFieldID(cls, "valueReference", "J");
        jfieldID nameId = env->GetFieldID(cls, "name", "Ljava/lang/String;");
        jfieldID typeId = env->GetFieldID(cls, "variableType", "I");
        jfieldID causalityId = env->GetFieldID(cls, "variableCausality", "I");
        jfieldID variabilityId = env->GetFieldID(cls, "variableVariability", "I");

        for (int i = 0; i < size; i++) {
            jobject var = env->GetObjectArrayElement(vars, i);
            env->SetLongField(var, vrId, (jlong) _vars[i].index);
            env->SetObjectField(var, nameId, env->NewStringUTF(_vars[i].name));
            env->SetIntField(var, typeId, (jint) _vars[i].type);
            env->SetIntField(var, causalityId, (jint) _vars[i].causality);
            env->SetIntField(var, variabilityId, (jint) _vars[i].variability);
        }

    } else {
        std::cerr << "Error:" << cse_last_error_message() << std::endl;
    }

    free(_vars);

    return status == 0;
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

    const char* className = "org/osp/cse/CseSlaveInfo";
    jclass cls = env->FindClass(className);

    if (cls == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(infos);
    cse_slave_info* _infos = (cse_slave_info*) malloc(sizeof(cse_slave_info)*size);
    jboolean status = cse_execution_get_slave_infos((cse_execution*) execution, _infos, size) == 0;

    if (status) {

        jfieldID indexId = env->GetFieldID(cls, "index", "I");
        jfieldID nameId = env->GetFieldID(cls, "name", "Ljava/lang/String;");
        jfieldID sourceId = env->GetFieldID(cls, "source", "Ljava/lang/String;");

        for (int i = 0; i < size; i++) {
            jobject info = env->GetObjectArrayElement(infos, i);
            env->SetIntField(info, indexId, _infos[i].index);
            env->SetObjectField(info, nameId, env->NewStringUTF(_infos[i].name));
            env->SetObjectField(info, sourceId, env->NewStringUTF(_infos[i].source));
        }

    } else {
      std::cerr << "Error:" << cse_last_error_message() << std::endl;
    }

    free(_infos);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setReal(JNIEnv *env, jobject obj, jlong manipulator, jint slaveIndex, jlongArray vr, jdoubleArray values) {
     if (manipulator == 0) {
       std::cerr << "[JNI-wrapper] Error: manipulator is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);
    jdouble *_values = env->GetDoubleArrayElements(values, 0);

    cse_variable_index* __vr = (cse_variable_index*) malloc(sizeof(cse_variable_index) * size);
    for (unsigned int i = 0; i < size; ++i) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_manipulator_slave_set_real((cse_manipulator*) manipulator, slaveIndex, __vr, size, _values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseDoubleArrayElements(values, _values, 0);

    free(__vr);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInteger(JNIEnv *env, jobject obj, jlong manipulator, jint slaveIndex, jlongArray vr, jintArray values) {
    if (manipulator == 0) {
       std::cerr << "[JNI-wrapper] Error: manipulator is NULL" << std::endl;
       return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong *_vr = env->GetLongArrayElements(vr, 0);
    jint *_values = env->GetIntArrayElements(values, 0);

    cse_variable_index* __vr = (cse_variable_index*) malloc(sizeof(cse_variable_index) * size);
    for (unsigned int i = 0; i < size; ++i) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_manipulator_slave_set_integer((cse_manipulator*) manipulator, slaveIndex, __vr, size, (int*)_values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseIntArrayElements(values, _values, 0);

    free(__vr);

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

    cse_variable_index* __vr = (cse_variable_index*) malloc(sizeof(cse_variable_index) * size);
    for (unsigned int i = 0; i < size; ++i) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_observer_slave_get_real((cse_observer*) observer, slaveIndex, __vr, size, _ref) == 0;

    env->SetDoubleArrayRegion(ref, 0, size, _ref);

    free(_ref);
    free(__vr);
    env->ReleaseLongArrayElements(vr, _vr, 0);

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

    cse_variable_index* __vr = (cse_variable_index*) malloc(sizeof(cse_variable_index) * size);
    for (unsigned int i = 0; i < size; ++i) {
        __vr[i] = (cse_variable_index) _vr[i];
    }

    jboolean status = cse_observer_slave_get_integer((cse_observer*) observer, slaveIndex, __vr, size, _ref) == 0;

    env->SetIntArrayRegion(ref, 0, size, (jint*)_ref);

    free(_ref);
    free(__vr);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jobject vr, jint nvr, jobject ref) {
//    if (observer == 0) {
//       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
//       return false;
//    }
//
//    jlong *_vr = (jlong*) env->GetDirectBufferAddress(vr);
//    int* _ref = (int*) env->GetDirectBufferAddress(ref);
//
//    return cse_observer_slave_get_integer((cse_observer*) observer, slaveIndex, (cse_variable_index*) _vr, nvr, _ref) == 0;
//}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamples(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return NULL;
    }

std::cerr << "hello" << std::endl;

    double* values = (double*) malloc(sizeof(double) * nSamples);
    cse_step_number* steps = (cse_step_number*) malloc(sizeof(cse_step_number*) * nSamples);
    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint) cse_observer_slave_get_real_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);
    jboolean success = numSamplesRead != -1;

    if (success) {

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

    }

    free(values);
    free(steps);
    free(times);

    return success;
}

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamplesDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples) {
//    if (observer == 0) {
//       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
//       return false;
//    }
//
//    initCseRealSamplesDirectFields(env);
//    jobject valueBuffer = env->GetObjectField(samples, cseRealSamplesDirectFields.valuesId);
//    jobject timeBuffer = env->GetObjectField(samples, cseRealSamplesDirectFields.timesId);
//    jobject stepBuffer = env->GetObjectField(samples, cseRealSamplesDirectFields.stepsId);
//
//    double* values = (double*) env->GetDirectBufferAddress(valueBuffer);
//    cse_step_number* steps = (cse_step_number*) env->GetDirectBufferAddress(stepBuffer);
//    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point)*nSamples);
//
//    jint numSamplesRead = (jint) cse_observer_slave_get_real_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);
//
//    double* times_ = (double*) env->GetDirectBufferAddress(timeBuffer);
//    for (int i = 0; i < numSamplesRead; i++) {
//        times_[i] = to_seconds(times[i]);
//    }
//    free(times);
//
//    env->SetIntField(samples, cseRealSamplesDirectFields.sizeId, numSamplesRead);
//
//    return true;
//}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamples(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return NULL;
    }

    int* values = (int*) malloc(sizeof(int) * nSamples);
    cse_step_number* steps = (cse_step_number*) malloc(sizeof(cse_step_number*) * nSamples);
    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint) cse_observer_slave_get_integer_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);
    jboolean success = numSamplesRead != -1;

    if (success) {

        jintArray _values = env->NewIntArray(numSamplesRead);
        jdoubleArray _times = env->NewDoubleArray(numSamplesRead);
        jlongArray _steps = env->NewLongArray(numSamplesRead);

        env->SetIntArrayRegion(_values, 0, numSamplesRead, (jint*) values);

        for (int i = 0; i < numSamplesRead; i++) {
            jlong step = (jlong) steps[i];
            double time = to_seconds(times[i]);

            env->SetDoubleArrayRegion(_times, i, 1, &time);
            env->SetLongArrayRegion(_steps, i, 1, &step);
        }

        initCseIntegerSamplesFields(env);
        env->SetIntField(samples, cseIntegerSamplesFields.sizeId, numSamplesRead);
        env->SetObjectField(samples, cseIntegerSamplesFields.valuesId, _values);
        env->SetObjectField(samples, cseIntegerSamplesFields.stepsId, _steps);
        env->SetObjectField(samples, cseIntegerSamplesFields.timesId, _times);
    }
    free(values);
    free(steps);
    free(times);

    return success;
}

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamplesDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples) {
//    if (observer == 0) {
//       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
//       return false;
//    }
//
//    initCseRealSamplesDirectFields(env);
//    jobject valueBuffer = env->GetObjectField(samples, cseRealSamplesDirectFields.valuesId);
//    jobject timeBuffer = env->GetObjectField(samples, cseRealSamplesDirectFields.timesId);
//    jobject stepBuffer = env->GetObjectField(samples, cseRealSamplesDirectFields.stepsId);
//
//    int* values = (int*) env->GetDirectBufferAddress(valueBuffer);
//    cse_step_number* steps = (cse_step_number*) env->GetDirectBufferAddress(stepBuffer);
//    cse_time_point* times = (cse_time_point*) malloc(sizeof(cse_time_point)*nSamples);
//
//    jint numSamplesRead = (jint) cse_observer_slave_get_integer_samples((cse_observer*) observer, slaveIndex, (cse_variable_index) vr, (cse_step_number) fromStep, nSamples, values, steps, times);
//
//    double* times_ = (double*) env->GetDirectBufferAddress(timeBuffer);
//    for (int i = 0; i < numSamplesRead; i++) {
//        times_[i] = to_seconds(times[i]);
//    }
//    free(times);
//
//    env->SetIntField(samples, cseRealSamplesDirectFields.sizeId, numSamplesRead);
//
//    return true;
//}

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

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserverFromCfg(JNIEnv *env, jobject obj, jstring logDir, jstring cfgPath) {
    const char* _logDir = env->GetStringUTFChars(logDir, 0);
    const char* _cfgPath = env->GetStringUTFChars(cfgPath, 0);
    cse_observer* observer = cse_file_observer_create_from_cfg(_logDir, _cfgPath);
    env->ReleaseStringUTFChars(logDir, _logDir);
    env->ReleaseStringUTFChars(cfgPath, _cfgPath);
    if (observer == 0) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong) observer;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createTimeSeriesObserver(JNIEnv *env, jobject obj) {
    return (jlong) cse_time_series_observer_create;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createBufferedTimeSeriesObserver(JNIEnv *env, jobject obj, jint bufferSize) {
    return (jlong) cse_buffered_time_series_observer_create(static_cast<size_t>(bufferSize));
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_startObserving(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jint variableType, jlong vr) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return false;
    }
    return cse_observer_start_observing((cse_observer*) observer, slaveIndex, (cse_variable_type) variableType, (cse_variable_index) vr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stopObserving(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jint variableType, jlong vr) {
    if (observer == 0) {
       std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
       return false;
    }
    return cse_observer_stop_observing((cse_observer*) observer, slaveIndex, (cse_variable_type) variableType, (cse_variable_index) vr) == 0;
}


JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyObserver(JNIEnv *env, jobject obj, jlong observer) {
    if (observer == 0) {
        std::cerr << "[JNI-wrapper] Error: observer is NULL" << std::endl;
        return false;
    }
    return cse_observer_destroy((cse_observer*) observer) == 0;
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

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createOverrideManipulator(JNIEnv *env, jobject obj) {
    return (jlong) cse_override_manipulator_create();
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addManipulator(JNIEnv *env, jobject obj, jlong execution, jlong manipulator) {
    if (execution == 0) {
        std::cerr << "[JNI-wrapper] Error: execution is NULL" << std::endl;
        return false;
    }
    if (manipulator == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulator is NULL" << std::endl;
        return false;
    }
    return cse_execution_add_manipulator((cse_execution*) execution, (cse_manipulator*) manipulator) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyManipulator(JNIEnv *env, jobject obj, jlong manipulator) {
    if (manipulator == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulator is NULL" << std::endl;
        return false;
    }
    return cse_manipulator_destroy((cse_manipulator*) manipulator) == 0;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createScenarioManager(JNIEnv *env, jobject obj) {
    return (jlong) cse_scenario_manager_create();
}

#ifdef __cplusplus
}
#endif
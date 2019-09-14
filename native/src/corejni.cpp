
#include <cse/error_helper.hpp>
#include <cse/execution.hpp>
#include <cse/execution_status_helper.hpp>
#include <cse/model.hpp>
#include <cse/model_description_helper.hpp>
#include <cse/samples_fields.hpp>
#include <cse/slave_infos_helper.hpp>
#include <cse/step_event_listener.hpp>
#include <cse/unit_conversion.hpp>

#include <atomic>
#include <iostream>
#include <memory>
#include <vector>


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getLastError(JNIEnv* env, jobject obj)
{
    return create_error(env);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv* env, jobject obj, jdouble startTime, jdouble stepSize)
{
    cse_execution* execution = cse_execution_create(to_cse_time_point(startTime), to_cse_duration(stepSize));
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong)execution;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSspExecution(JNIEnv* env, jobject obj, jstring sspDir, jdouble startTime)
{
    const char* _sspDir = env->GetStringUTFChars(sspDir, nullptr);
    cse_execution* execution = cse_ssp_execution_create(_sspDir, to_cse_time_point(startTime));
    env->ReleaseStringUTFChars(sspDir, _sspDir);
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong)execution;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_destroy((cse_execution*)executionPtr) == 0;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSlave(JNIEnv* env, jobject obj, jstring fmuPath)
{
    const char* _fmuPath = env->GetStringUTFChars(fmuPath, 0);
    cse_slave* slave = cse_local_slave_create(_fmuPath);
    env->ReleaseStringUTFChars(fmuPath, _fmuPath);
    return (jlong)slave;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroySlave(JNIEnv* env, jobject obj, jlong slave)
{
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
        return false;
    }
    return cse_local_slave_destroy((cse_slave*)slave) == 0;
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

    return (jint)cse_execution_add_slave((cse_execution*)executionPtr, (cse_slave*)slave);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_start(JNIEnv* env, jobject obj, jlong executionPtr)
{
    return cse_execution_start((cse_execution*)executionPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_step(JNIEnv* env, jobject obj, jlong executionPtr, jlong numSteps)
{
    return cse_execution_step((cse_execution*)executionPtr, numSteps) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_simulateUntil(JNIEnv* env, jobject obj, jlong executionPtr, jdouble targetTime)
{
    return cse_execution_simulate_until((cse_execution*)executionPtr, to_cse_time_point(targetTime)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stop(JNIEnv* env, jobject obj, jlong executionPtr)
{
    return cse_execution_stop((cse_execution*)executionPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_enableRealTimeSimulation(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_enable_real_time_simulation((cse_execution*)executionPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_disableRealTimeSimulation(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_disable_real_time_simulation((cse_execution*)executionPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setRealTimeFactorTarget(JNIEnv* env, jobject obj, jlong executionPtr, jdouble realTimeFactor)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_set_real_time_factor_target((cse_execution*)executionPtr, realTimeFactor) == 0;
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getStatus(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }
    return create_execution_status(env, executionPtr);
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getModelDescription(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }
    return create_model_description(env, executionPtr, slaveIndex);
}

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumSlaves(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return (jint)cse_execution_get_num_slaves((cse_execution*)executionPtr);
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getSlaveInfos(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }

    return create_slave_infos(env, executionPtr);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setReal(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jdoubleArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);
    jdouble* _values = env->GetDoubleArrayElements(values, nullptr);

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_manipulator_slave_set_real((cse_manipulator*)manipulatorPtr, slaveIndex, __vr, size, _values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseDoubleArrayElements(values, _values, 0);

    free(__vr);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialRealValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jdouble value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto& cpp_execution = ((cse_execution*)executionPtr)->cpp_execution;
    cpp_execution->set_real_initial_value(slaveIndex, static_cast<cse::value_reference>(vr), value);

    return true;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInteger(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jintArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);
    jint* _values = env->GetIntArrayElements(values, nullptr);

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_manipulator_slave_set_integer((cse_manipulator*)manipulatorPtr, slaveIndex, __vr, size, (int*)_values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseIntArrayElements(values, _values, 0);

    free(__vr);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialIntegerValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jint value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto& cpp_execution = ((cse_execution*)executionPtr)->cpp_execution;
    cpp_execution->set_integer_initial_value(slaveIndex, static_cast<cse::value_reference>(vr), value);

    return true;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setBoolean(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jbooleanArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);
    jboolean* _values = env->GetBooleanArrayElements(values, nullptr);

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_manipulator_slave_set_boolean((cse_manipulator*)manipulatorPtr, slaveIndex, __vr, size, (bool*)_values) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);
    env->ReleaseBooleanArrayElements(values, _values, 0);

    free(__vr);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialBooleanValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jboolean value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto& cpp_execution = ((cse_execution*)executionPtr)->cpp_execution;
    cpp_execution->set_boolean_initial_value(slaveIndex, static_cast<cse::value_reference>(vr), value);

    return true;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setString(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jobjectArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);

    std::vector<const char*> _values(size);
    for (int i = 0; i < size; i++) {
        auto str = (jstring)env->GetObjectArrayElement(values, i);
        _values[i] = env->GetStringUTFChars(str, nullptr);
    }

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_manipulator_slave_set_string((cse_manipulator*)manipulatorPtr, slaveIndex, __vr, size, _values.data()) == 0;

    env->ReleaseLongArrayElements(vr, _vr, 0);

    free(__vr);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialStringValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jstring value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto value_ = env->GetStringUTFChars(value, nullptr);
    auto& cpp_execution = ((cse_execution*)executionPtr)->cpp_execution;
    cpp_execution->set_string_initial_value(slaveIndex, static_cast<cse::value_reference>(vr), value_);
    env->ReleaseStringUTFChars(value, value_);

    return true;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getReal(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jdoubleArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);
    auto* _ref = (double*)malloc(sizeof(double) * size);

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_observer_slave_get_real((cse_observer*)observerPtr, slaveIndex, __vr, size, _ref) == 0;

    env->SetDoubleArrayRegion(ref, 0, size, _ref);

    free(_ref);
    free(__vr);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getInteger(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jintArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);
    auto* _ref = (int*)malloc(sizeof(int) * size);

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_observer_slave_get_integer((cse_observer*)observerPtr, slaveIndex, __vr, size, _ref) == 0;

    env->SetIntArrayRegion(ref, 0, size, (jint*)_ref);

    free(_ref);
    free(__vr);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getBoolean(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jbooleanArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);
    auto _ref = (bool*)malloc(sizeof(bool) * size);

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_observer_slave_get_boolean((cse_observer*)observerPtr, slaveIndex, __vr, size, _ref) == 0;

    env->SetBooleanArrayRegion(ref, 0, size, (jboolean*)_ref);

    free(_ref);
    free(__vr);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getString(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jobjectArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* _vr = env->GetLongArrayElements(vr, nullptr);

    std::vector<const char*> _ref(size);
    for (int i = 0; i < size; i++) {
        auto str = (jstring)env->GetObjectArrayElement(ref, i);
        _ref[i] = env->GetStringUTFChars(str, nullptr);
    }

    auto* __vr = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        __vr[i] = (cse_value_reference)_vr[i];
    }

    jboolean status = cse_observer_slave_get_string((cse_observer*)observerPtr, slaveIndex, __vr, size, _ref.data()) == 0;

    for (int i = 0; i < size; i++) {
        jstring value = env->NewStringUTF(_ref[i]);
        env->SetObjectArrayElement(ref, i, value);
    }

    free(__vr);
    env->ReleaseLongArrayElements(vr, _vr, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamples(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    auto* values = (double*)malloc(sizeof(double) * nSamples);
    auto* steps = (cse_step_number*)malloc(sizeof(cse_step_number*) * nSamples);
    auto* times = (cse_time_point*)malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint)cse_observer_slave_get_real_samples((cse_observer*)observerPtr, slaveIndex, (cse_value_reference)vr, (cse_step_number)fromStep, nSamples, values, steps, times);
    jboolean success = numSamplesRead != -1;

    if (success) {

        jdoubleArray _values = env->NewDoubleArray(numSamplesRead);
        jdoubleArray _times = env->NewDoubleArray(numSamplesRead);
        jlongArray _steps = env->NewLongArray(numSamplesRead);

        env->SetDoubleArrayRegion(_values, 0, numSamplesRead, values);

        for (int i = 0; i < numSamplesRead; i++) {
            auto step = (jlong)steps[i];
            double time = to_seconds(times[i]);

            env->SetDoubleArrayRegion(_times, i, 1, &time);
            env->SetLongArrayRegion(_steps, i, 1, &step);
        }

        init_real_samples_fields(env);
        env->SetIntField(samples, realSamplesFields.sizeId, numSamplesRead);
        env->SetObjectField(samples, realSamplesFields.valuesId, _values);
        env->SetObjectField(samples, realSamplesFields.stepsId, _steps);
        env->SetObjectField(samples, realSamplesFields.timesId, _times);
    }

    free(values);
    free(steps);
    free(times);

    return success;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamples(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    int* values = (int*)malloc(sizeof(int) * nSamples);
    auto* steps = (cse_step_number*)malloc(sizeof(cse_step_number*) * nSamples);
    auto* times = (cse_time_point*)malloc(sizeof(cse_time_point*) * nSamples);

    jint numSamplesRead = (jint)cse_observer_slave_get_integer_samples((cse_observer*)observerPtr, slaveIndex, (cse_value_reference)vr, (cse_step_number)fromStep, nSamples, values, steps, times);
    jboolean success = numSamplesRead != -1;

    if (success) {

        jintArray _values = env->NewIntArray(numSamplesRead);
        jdoubleArray _times = env->NewDoubleArray(numSamplesRead);
        jlongArray _steps = env->NewLongArray(numSamplesRead);

        env->SetIntArrayRegion(_values, 0, numSamplesRead, (jint*)values);

        for (int i = 0; i < numSamplesRead; i++) {
            auto step = (jlong)steps[i];
            double time = to_seconds(times[i]);

            env->SetDoubleArrayRegion(_times, i, 1, &time);
            env->SetLongArrayRegion(_steps, i, 1, &step);
        }

        init_integer_samples_fields(env);
        env->SetIntField(samples, integerSamplesFields.sizeId, numSamplesRead);
        env->SetObjectField(samples, integerSamplesFields.valuesId, _values);
        env->SetObjectField(samples, integerSamplesFields.stepsId, _steps);
        env->SetObjectField(samples, integerSamplesFields.timesId, _times);
    }
    free(values);
    free(steps);
    free(times);

    return success;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbersForDuration(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jdouble duration, jlongArray steps)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const size_t size = 2;
    auto* _steps = (cse_step_number*)malloc(sizeof(cse_step_number) * size);

    jboolean status = cse_observer_get_step_numbers_for_duration((cse_observer*)observerPtr, slaveIndex, to_cse_duration(duration), _steps);

    for (int i = 0; i < size; i++) {
        auto step = (jlong)_steps[i];
        env->SetLongArrayRegion(steps, i, 1, &step);
    }

    free(_steps);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbers(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jdouble begin, jdouble end, jlongArray steps)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const size_t size = 2;
    auto* _steps = (cse_step_number*)malloc(sizeof(cse_step_number) * size);

    jboolean status = cse_observer_get_step_numbers((cse_observer*)observerPtr, slaveIndex, to_cse_time_point(begin), to_cse_time_point(end), _steps);

    for (int i = 0; i < size; i++) {
        auto step = (jlong)_steps[i];
        env->SetLongArrayRegion(steps, i, 1, &step);
    }

    free(_steps);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectIntegers(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_connect_integer_variables((cse_execution*)executionPtr, outputSlaveIndex, (cse_value_reference)outputSlaveIndex, inputSlaveIndex, (cse_value_reference)inputSlaveIndex);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectReals(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_connect_real_variables((cse_execution*)executionPtr, outputSlaveIndex, (cse_value_reference)outputSlaveIndex, inputSlaveIndex, (cse_value_reference)inputSlaveIndex);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createLastValueObserver(JNIEnv* env, jobject obj)
{
    return (jlong)cse_last_value_observer_create();
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserver(JNIEnv* env, jobject obj, jstring logDir)
{
    const char* _logDir = env->GetStringUTFChars(logDir, nullptr);
    cse_observer* observer = cse_file_observer_create(_logDir);
    env->ReleaseStringUTFChars(logDir, _logDir);
    if (observer == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong)observer;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserverFromCfg(JNIEnv* env, jobject obj, jstring logDir, jstring cfgPath)
{
    const char* _logDir = env->GetStringUTFChars(logDir, nullptr);
    const char* _cfgPath = env->GetStringUTFChars(cfgPath, nullptr);
    cse_observer* observer = cse_file_observer_create_from_cfg(_logDir, _cfgPath);
    env->ReleaseStringUTFChars(logDir, _logDir);
    env->ReleaseStringUTFChars(cfgPath, _cfgPath);
    if (observer == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return (jlong)observer;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createTimeSeriesObserver(JNIEnv* env, jobject obj)
{
    return (jlong)cse_time_series_observer_create;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createBufferedTimeSeriesObserver(JNIEnv* env, jobject obj, jint bufferSize)
{
    return (jlong)cse_buffered_time_series_observer_create(static_cast<size_t>(bufferSize));
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_startObserving(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_observer_start_observing((cse_observer*)observerPtr, slaveIndex, (cse_variable_type)variableType, (cse_value_reference)vr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stopObserving(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_observer_stop_observing((cse_observer*)observerPtr, slaveIndex, (cse_variable_type)variableType, (cse_value_reference)vr) == 0;
}


JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyObserver(JNIEnv* env, jobject obj, jlong observerPtr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_observer_destroy((cse_observer*)observerPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addObserver(JNIEnv* env, jobject obj, jlong executionPtr, jlong observerPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_add_observer((cse_execution*)executionPtr, (cse_observer*)observerPtr) == 0;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createOverrideManipulator(JNIEnv* env, jobject obj)
{
    return (jlong)cse_override_manipulator_create();
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addManipulator(JNIEnv* env, jobject obj, jlong executionPtr, jlong manipulatorPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_add_manipulator((cse_execution*)executionPtr, (cse_manipulator*)manipulatorPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyManipulator(JNIEnv* env, jobject obj, jlong manipulatorPtr)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_manipulator_destroy((cse_manipulator*)manipulatorPtr) == 0;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createScenarioManager(JNIEnv* env, jobject obj)
{
    return (jlong)cse_scenario_manager_create();
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_loadScenario(JNIEnv* env, jobject obj, jlong executionPtr, jlong manipulatorPtr, jstring scenarioFile)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    const char* _scenarioFile = env->GetStringUTFChars(scenarioFile, nullptr);
    jboolean status = cse_execution_load_scenario((cse_execution*)executionPtr, (cse_manipulator*)manipulatorPtr, _scenarioFile) == 0;
    env->ReleaseStringUTFChars(scenarioFile, _scenarioFile);
    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_isScenarioRunning(JNIEnv* env, jobject obj, jlong manipulatorPtr)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_scenario_is_running((cse_manipulator*)manipulatorPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_abortScenario(JNIEnv* env, jobject obj, jlong manipulatorPtr)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_scenario_abort((cse_manipulator*)manipulatorPtr) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setupSimpleConsoleLogging(JNIEnv* env, jobject obj)
{
    return cse_log_setup_simple_console_logging() == 0;
}

JNIEXPORT void JNICALL Java_org_osp_cse_jni_CseLibrary_setLogLevel(JNIEnv* env, jobject obj, jint level)
{
    cse_log_set_output_level((cse_log_severity_level)level);
}


JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createStepEventListener(JNIEnv* env, jobject obj, jobject listener)
{
    auto observer = std::make_unique<cse_observer>();
    observer->cpp_observer = std::make_unique<cse::step_event_listener>(env, listener);
    return reinterpret_cast<jlong>(observer.release());
}

#ifdef __cplusplus
}
#endif


#include <cosim.h>
#include <cosim/execution_status_helper.hpp>
#include <cosim/unit_conversion.hpp>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_createExecution(JNIEnv* env, jobject, jdouble startTime, jdouble stepSize)
{
    cosim_execution* execution = cosim_execution_create(to_cosim_time_point(startTime), to_cosim_duration(stepSize));
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cosim_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(execution);
}

JNIEXPORT jlong JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_createSspExecution(JNIEnv* env, jobject, jstring sspPath, jboolean startTimeDefined, jdouble startTime)
{
    auto sspDir_ = env->GetStringUTFChars(sspPath, nullptr);
    cosim_execution* execution = cosim_ssp_execution_create(sspDir_, startTimeDefined, to_cosim_time_point(startTime));
    env->ReleaseStringUTFChars(sspPath, sspDir_);
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cosim_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(execution);
}

JNIEXPORT jlong JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_createOspConfigExecution(JNIEnv* env, jobject, jstring configPath, jboolean startTimeDefined, jdouble startTime)
{
    auto configDir_ = env->GetStringUTFChars(configPath, nullptr);
    cosim_execution* execution = cosim_osp_config_execution_create(configDir_, startTimeDefined, to_cosim_time_point(startTime));
    env->ReleaseStringUTFChars(configPath, configDir_);
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cosim_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(execution);
}

JNIEXPORT jlong JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_createFixedStepSspExecution(JNIEnv* env, jobject, jstring sspDir, jdouble stepSize, jboolean startTimeDefined, jdouble startTime)
{
    auto sspDir_ = env->GetStringUTFChars(sspDir, nullptr);
    cosim_execution* execution = cosim_ssp_fixed_step_execution_create(
        sspDir_,
        startTimeDefined,
        to_cosim_time_point(startTime),
        to_cosim_duration(stepSize));
    env->ReleaseStringUTFChars(sspDir, sspDir_);
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cosim_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(execution);
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_destroyExecution(JNIEnv* env, jobject, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cosim_execution_destroy(reinterpret_cast<cosim_execution*>(executionPtr)) == 0;
}


JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_start(JNIEnv* env, jobject, jlong executionPtr)
{
    return cosim_execution_start(reinterpret_cast<cosim_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_step(JNIEnv* env, jobject, jlong executionPtr, jlong numSteps)
{
    return cosim_execution_step(reinterpret_cast<cosim_execution*>(executionPtr), numSteps) == 0;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_simulateUntil(JNIEnv* env, jobject, jlong executionPtr, jdouble targetTime)
{
    return cosim_execution_simulate_until(reinterpret_cast<cosim_execution*>(executionPtr), to_cosim_time_point(targetTime)) == 0;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_stop(JNIEnv* env, jobject, jlong executionPtr)
{
    return cosim_execution_stop(reinterpret_cast<cosim_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_enableRealTimeSimulation(JNIEnv* env, jobject, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cosim_execution_enable_real_time_simulation(reinterpret_cast<cosim_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_disableRealTimeSimulation(JNIEnv* env, jobject, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cosim_execution_disable_real_time_simulation(reinterpret_cast<cosim_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_setRealTimeFactorTarget(JNIEnv* env, jobject, jlong executionPtr, jdouble realTimeFactor)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cosim_execution_set_real_time_factor_target(reinterpret_cast<cosim_execution*>(executionPtr), realTimeFactor) == 0;
}

JNIEXPORT jobject JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_getStatus(JNIEnv* env, jobject, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }
    return create_execution_status(env, executionPtr);
}


#ifdef __cplusplus
}
#endif

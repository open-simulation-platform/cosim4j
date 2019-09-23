
#include <cse.h>


#include <cse/unit_conversion.hpp>
#include <cse/execution_status_helper.hpp>

#include <jni.h>

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv* env, jobject obj, jdouble startTime, jdouble stepSize)
{
    cse_execution* execution = cse_execution_create(to_cse_time_point(startTime), to_cse_duration(stepSize));
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(execution);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSspExecution(JNIEnv* env, jobject obj, jstring sspDir, jboolean startTimeDefined, jdouble startTime)
{
    auto sspDir_ = env->GetStringUTFChars(sspDir, nullptr);
    cse_execution* execution = cse_ssp_execution_create(sspDir_, startTimeDefined, to_cse_time_point(startTime));
    env->ReleaseStringUTFChars(sspDir, sspDir_);
    if (execution == nullptr) {
        std::cerr << "[JNI-wrapper]"
                  << "Error: Failed to create execution: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(execution);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_destroy(reinterpret_cast<cse_execution*>(executionPtr)) == 0;
}


JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_start(JNIEnv* env, jobject obj, jlong executionPtr)
{
    return cse_execution_start(reinterpret_cast<cse_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_step(JNIEnv* env, jobject obj, jlong executionPtr, jlong numSteps)
{
    return cse_execution_step(reinterpret_cast<cse_execution*>(executionPtr), numSteps) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_simulateUntil(JNIEnv* env, jobject obj, jlong executionPtr, jdouble targetTime)
{
    return cse_execution_simulate_until(reinterpret_cast<cse_execution*>(executionPtr), to_cse_time_point(targetTime)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stop(JNIEnv* env, jobject obj, jlong executionPtr)
{
    return cse_execution_stop(reinterpret_cast<cse_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_enableRealTimeSimulation(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_enable_real_time_simulation(reinterpret_cast<cse_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_disableRealTimeSimulation(JNIEnv* env, jobject obj, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_disable_real_time_simulation(reinterpret_cast<cse_execution*>(executionPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setRealTimeFactorTarget(JNIEnv* env, jobject obj, jlong executionPtr, jdouble realTimeFactor)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_set_real_time_factor_target(reinterpret_cast<cse_execution*>(executionPtr), realTimeFactor) == 0;
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getStatus(JNIEnv* env, jobject obj, jlong executionPtr)
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

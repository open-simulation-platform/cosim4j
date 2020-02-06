#include <cse.h>
#include <cse/samples_helper.hpp>
#include <cse/unit_conversion.hpp>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamples(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return nullptr;
    }

    return create_real_samples(env, observerPtr, slaveIndex, vr, fromStep, nSamples);
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamples(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return nullptr;
    }

    return create_integer_samples(env, observerPtr, slaveIndex, vr, fromStep, nSamples);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbersForDuration(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jdouble duration, jlongArray steps)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const size_t size = 2;
    auto steps_ = (cse_step_number*)malloc(sizeof(cse_step_number) * size);

    bool status = cse_observer_get_step_numbers_for_duration(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, to_cse_duration(duration), steps_);

    for (int i = 0; i < size; i++) {
        auto step = static_cast<jlong>(steps_[i]);
        env->SetLongArrayRegion(steps, i, 1, &step);
    }

    free(steps_);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbers(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jdouble begin, jdouble end, jlongArray steps)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const size_t size = 2;
    auto steps_ = (cse_step_number*)malloc(sizeof(cse_step_number) * size);

    bool status = cse_observer_get_step_numbers(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, to_cse_time_point(begin), to_cse_time_point(end), steps_);

    for (int i = 0; i < size; i++) {
        auto step = static_cast<jlong>(steps_[i]);
        env->SetLongArrayRegion(steps, i, 1, &step);
    }

    free(steps_);

    return status;
}

#ifdef __cplusplus
}
#endif

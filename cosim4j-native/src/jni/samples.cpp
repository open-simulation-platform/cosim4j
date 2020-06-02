#include <cosim.h>
#include <cosim/samples_helper.hpp>
#include <cosim/unit_conversion.hpp>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jobject JNICALL Java_org_osp_cosim_jni_CosimLibrary_getRealSamples(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return nullptr;
    }

    return create_real_samples(env, observerPtr, slaveIndex, vr, fromStep, nSamples);
}

JNIEXPORT jobject JNICALL Java_org_osp_cosim_jni_CosimLibrary_getIntegerSamples(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return nullptr;
    }

    return create_integer_samples(env, observerPtr, slaveIndex, vr, fromStep, nSamples);
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_getStepNumbersForDuration(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jdouble duration, jlongArray steps)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const size_t size = 2;
    auto steps_ = (cosim_step_number*)malloc(sizeof(cosim_step_number) * size);

    bool status = cosim_observer_get_step_numbers_for_duration(reinterpret_cast<cosim_observer*>(observerPtr), slaveIndex, to_cosim_duration(duration), steps_);

    for (int i = 0; i < size; i++) {
        auto step = static_cast<jlong>(steps_[i]);
        env->SetLongArrayRegion(steps, i, 1, &step);
    }

    free(steps_);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_getStepNumbers(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jdouble begin, jdouble end, jlongArray steps)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const size_t size = 2;
    auto steps_ = (cosim_step_number*)malloc(sizeof(cosim_step_number) * size);

    bool status = cosim_observer_get_step_numbers(reinterpret_cast<cosim_observer*>(observerPtr), slaveIndex, to_cosim_time_point(begin), to_cosim_time_point(end), steps_);

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

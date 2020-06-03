
#ifndef COSIMJNI_COSIM_SAMPLES_FIELDS_HPP
#define COSIMJNI_COSIM_SAMPLES_FIELDS_HPP

#include <cosim.h>
#include <cosim/unit_conversion.hpp>

#include <jni.h>
#include <memory>

namespace
{

jobject create_real_samples(JNIEnv* env, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples)
{
    jclass cls = env->FindClass("org/osp/cosim/CosimRealSamples");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(I[J[D[D)V");

    auto* values = (double*)malloc(sizeof(double) * nSamples);
    auto* steps = (cosim_step_number*)malloc(sizeof(cosim_step_number*) * nSamples);
    auto* times = (cosim_time_point*)malloc(sizeof(cosim_time_point*) * nSamples);

    auto numSamplesRead = (int)cosim_observer_slave_get_real_samples(
        reinterpret_cast<cosim_observer*>(observerPtr),
        slaveIndex,
        static_cast<cosim_value_reference>(vr),
        static_cast<cosim_step_number>(fromStep),
        nSamples,
        values,
        steps,
        times);

    if (numSamplesRead == -1) {
        return nullptr;
    }

    jdoubleArray values_ = env->NewDoubleArray(numSamplesRead);
    jdoubleArray times_ = env->NewDoubleArray(numSamplesRead);
    jlongArray steps_ = env->NewLongArray(numSamplesRead);

    env->SetDoubleArrayRegion(values_, 0, numSamplesRead, values);

    for (int i = 0; i < numSamplesRead; i++) {
        auto step = static_cast<jlong>(steps[i]);
        double time = to_seconds(times[i]);

        env->SetDoubleArrayRegion(times_, i, 1, &time);
        env->SetLongArrayRegion(steps_, i, 1, &step);
    }

    auto samples = env->NewObject(cls,
        ctor,
        numSamplesRead,
        steps_,
        times_,
        values_);

    free(values);
    free(steps);
    free(times);

    return samples;
}

jobject create_integer_samples(JNIEnv* env, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples)
{
    jclass cls = env->FindClass("org/osp/cosim/CosimIntegerSamples");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(I[J[D[D)V");

    auto* values = (int*)malloc(sizeof(int) * nSamples);
    auto* steps = (cosim_step_number*)malloc(sizeof(cosim_step_number*) * nSamples);
    auto* times = (cosim_time_point*)malloc(sizeof(cosim_time_point*) * nSamples);

    auto numSamplesRead = (int)cosim_observer_slave_get_integer_samples(
        reinterpret_cast<cosim_observer*>(observerPtr),
        slaveIndex,
        static_cast<cosim_value_reference>(vr),
        static_cast<cosim_step_number>(fromStep),
        nSamples,
        values,
        steps,
        times);

    if (numSamplesRead == -1) {
        return nullptr;
    }

    jintArray values_ = env->NewIntArray(numSamplesRead);
    jdoubleArray times_ = env->NewDoubleArray(numSamplesRead);
    jlongArray steps_ = env->NewLongArray(numSamplesRead);

    env->SetIntArrayRegion(values_, 0, numSamplesRead, reinterpret_cast<jint*>(values));

    for (int i = 0; i < numSamplesRead; i++) {
        auto step = static_cast<jlong>(steps[i]);
        double time = to_seconds(times[i]);

        env->SetDoubleArrayRegion(times_, i, 1, &time);
        env->SetLongArrayRegion(steps_, i, 1, &step);
    }

    auto samples = env->NewObject(cls,
        ctor,
        numSamplesRead,
        steps_,
        times_,
        values_);

    free(values);
    free(steps);
    free(times);

    return samples;
}

struct real_samples_fields
{
    jfieldID valuesId;
    jfieldID timesId;
    jfieldID stepsId;
    jfieldID sizeId;

    bool initialized = false;
};

real_samples_fields realSamplesFields;

inline void init_real_samples_fields(JNIEnv* env)
{

    if (!realSamplesFields.initialized) {

        const char* className = "org/osp/cosim/CosimRealSamples";
        jclass cls = env->FindClass(className);

        realSamplesFields.valuesId = env->GetFieldID(cls, "values", "[D");
        realSamplesFields.timesId = env->GetFieldID(cls, "times", "[D");
        realSamplesFields.stepsId = env->GetFieldID(cls, "steps", "[J");
        realSamplesFields.sizeId = env->GetFieldID(cls, "size", "I");

        realSamplesFields.initialized = true;
    }
}

struct integer_samples_fields
{
    jfieldID valuesId;
    jfieldID timesId;
    jfieldID stepsId;
    jfieldID sizeId;

    bool initialized = false;
};

integer_samples_fields integerSamplesFields;

inline void init_integer_samples_fields(JNIEnv* env)
{

    if (!integerSamplesFields.initialized) {

        const char* className = "org/osp/cosim/CosimIntegerSamples";
        jclass cls = env->FindClass(className);

        integerSamplesFields.valuesId = env->GetFieldID(cls, "values", "[I");
        integerSamplesFields.timesId = env->GetFieldID(cls, "times", "[D");
        integerSamplesFields.stepsId = env->GetFieldID(cls, "steps", "[J");
        integerSamplesFields.sizeId = env->GetFieldID(cls, "size", "I");

        integerSamplesFields.initialized = true;
    }
}

} // namespace

#endif

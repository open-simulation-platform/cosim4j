
#ifndef CSECOREJNI_SAMPLES_FIELDS_HPP
#define CSECOREJNI_SAMPLES_FIELDS_HPP

#include <jni.h>

namespace
{

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

        const char* className = "org/osp/cse/CseRealSamples";
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

        const char* className = "org/osp/cse/CseIntegerSamples";
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

#include <iostream>
#include <jni.h>

namespace
{

struct CseRealSamplesFields
{
    jfieldID valuesId;
    jfieldID timesId;
    jfieldID stepsId;
    jfieldID sizeId;

    bool initialized = false;
};

CseRealSamplesFields cseRealSamplesFields;

inline void initCseRealSamplesFields(JNIEnv* env)
{

    if (!cseRealSamplesFields.initialized) {

        const char* className = "org/osp/cse/CseRealSamples";
        jclass cls = env->FindClass(className);

        if (cls == 0) {
            std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'"
                      << std::endl;
        }

        cseRealSamplesFields.valuesId = env->GetFieldID(cls, "values", "[D");
        cseRealSamplesFields.timesId = env->GetFieldID(cls, "times", "[D");
        cseRealSamplesFields.stepsId = env->GetFieldID(cls, "steps", "[J");
        cseRealSamplesFields.sizeId = env->GetFieldID(cls, "size", "I");

        cseRealSamplesFields.initialized = true;
    }
}

struct CseIntegerSamplesFields
{
    jfieldID valuesId;
    jfieldID timesId;
    jfieldID stepsId;
    jfieldID sizeId;

    bool initialized = false;
};

CseIntegerSamplesFields cseIntegerSamplesFields;

inline void initCseIntegerSamplesFields(JNIEnv* env)
{

    if (!cseIntegerSamplesFields.initialized) {

        const char* className = "org/osp/cse/CseIntegerSamples";
        jclass cls = env->FindClass(className);

        if (cls == 0) {
            std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'"
                      << std::endl;
        }

        cseIntegerSamplesFields.valuesId = env->GetFieldID(cls, "values", "[I");
        cseIntegerSamplesFields.timesId = env->GetFieldID(cls, "times", "[D");
        cseIntegerSamplesFields.stepsId = env->GetFieldID(cls, "steps", "[J");
        cseIntegerSamplesFields.sizeId = env->GetFieldID(cls, "size", "I");

        cseRealSamplesFields.initialized = true;
    }
}

}
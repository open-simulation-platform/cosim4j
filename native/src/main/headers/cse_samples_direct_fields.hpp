#include <jni.h>
#include <iostream>

namespace {

    struct CseRealSamplesDirectFields {
        jfieldID valuesId;
        jfieldID timesId;
        jfieldID stepsId;
        jfieldID sizeId;

        bool initialized = false;

    };

    CseRealSamplesDirectFields cseRealSamplesDirectFields;

    inline void initCseRealSamplesDirectFields(JNIEnv *env) {

        if (!cseRealSamplesDirectFields.initialized) {

            const char* className = "org/osp/cse/CseRealSamplesDirect";
            jclass cls = env->FindClass(className);

            if (cls == 0) {
                std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'" << std::endl;
            }

            cseRealSamplesDirectFields.valuesId = env->GetFieldID(cls, "valueBuffer", "Ljava/nio/ByteBuffer;");
            cseRealSamplesDirectFields.timesId = env->GetFieldID(cls, "timeBuffer", "Ljava/nio/ByteBuffer;");
            cseRealSamplesDirectFields.stepsId = env->GetFieldID(cls, "stepBuffer", "Ljava/nio/ByteBuffer;");
            cseRealSamplesDirectFields.sizeId = env->GetFieldID(cls, "size", "I");

            cseRealSamplesDirectFields.initialized = true;

        }

    }


     struct CseIntegeramplesDirectFields {
            jfieldID valuesId;
            jfieldID timesId;
            jfieldID stepsId;
            jfieldID sizeId;

            bool initialized = false;

        };

        CseIntegeramplesDirectFields cseIntegerSamplesDirectFields;

        inline void initCseIntegerSamplesDirectFields(JNIEnv *env) {

            if (!cseIntegerSamplesDirectFields.initialized) {

                const char* className = "org/osp/cse/CseIntegerSamplesDirect";
                jclass cls = env->FindClass(className);

                if (cls == 0) {
                    std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'" << std::endl;
                }

                cseIntegerSamplesDirectFields.valuesId = env->GetFieldID(cls, "valueBuffer", "Ljava/nio/ByteBuffer;");
                cseIntegerSamplesDirectFields.timesId = env->GetFieldID(cls, "timeBuffer", "Ljava/nio/ByteBuffer;");
                cseIntegerSamplesDirectFields.stepsId = env->GetFieldID(cls, "stepBuffer", "Ljava/nio/ByteBuffer;");
                cseIntegerSamplesDirectFields.sizeId = env->GetFieldID(cls, "size", "I");

                cseIntegerSamplesDirectFields.initialized = true;

            }

        }

}
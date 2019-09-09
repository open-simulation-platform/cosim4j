
#include <iostream>
#include <jni.h>

namespace
{

struct CseExecutionStatusFields
{
    jfieldID stateId;
    jfieldID errorId;
    jfieldID currentTimeId;
    jfieldID realTimeFactorId;
    jfieldID realTimeSimulationId;

    bool initialized = false;
};

CseExecutionStatusFields cseExecutionStatusFields;

inline void initCseExecutionStatusFields(JNIEnv* env)
{

    if (!cseExecutionStatusFields.initialized) {

        const char* className = "org/osp/cse/CseExecutionStatus";
        jclass cls = env->FindClass(className);

        if (cls == 0) {
            std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'"
                      << std::endl;
        }

        cseExecutionStatusFields.stateId = env->GetFieldID(cls, "stateId", "I");
        cseExecutionStatusFields.errorId = env->GetFieldID(cls, "errorCodeId", "I");
        cseExecutionStatusFields.currentTimeId =
            env->GetFieldID(cls, "currentTime", "D");
        cseExecutionStatusFields.realTimeFactorId =
            env->GetFieldID(cls, "realTimeFactor", "D");
        cseExecutionStatusFields.realTimeSimulationId =
            env->GetFieldID(cls, "realTimeSimulation", "Z");

        cseExecutionStatusFields.initialized = true;
    }
}

}
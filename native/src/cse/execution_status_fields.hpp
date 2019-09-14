
#ifndef CSECOREJNI_EXECUTION_STATUS_FIELDS_HPP
#define CSECOREJNI_EXECUTION_STATUS_FIELDS_HPP

#    include <iostream>
#    include <jni.h>

namespace
{

struct execution_status_fields
{
    jfieldID stateId;
    jfieldID errorId;
    jfieldID currentTimeId;
    jfieldID realTimeFactorId;
    jfieldID realTimeSimulationId;

    bool initialized = false;
};

execution_status_fields executionStatusFields;

inline void init_execution_status_fields(JNIEnv* env)
{

    if (!executionStatusFields.initialized) {

        const char* className = "org/osp/cse/CseExecutionStatus";
        jclass cls = env->FindClass(className);

        if (cls == nullptr) {
            std::cerr << "[JNI-wrapper] Error: Could not locate '" << className << "'"
                      << std::endl;
        }

        executionStatusFields.stateId = env->GetFieldID(cls, "stateId", "I");
        executionStatusFields.errorId = env->GetFieldID(cls, "errorCodeId", "I");
        executionStatusFields.currentTimeId =
            env->GetFieldID(cls, "currentTime", "D");
        executionStatusFields.realTimeFactorId =
            env->GetFieldID(cls, "realTimeFactor", "D");
        executionStatusFields.realTimeSimulationId =
            env->GetFieldID(cls, "realTimeSimulation", "Z");

        executionStatusFields.initialized = true;
    }
}

} // namespace

#endif

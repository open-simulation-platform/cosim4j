
#ifndef CSECOREJNI_EXECUTION_STATUS_HELPER_HPP
#define CSECOREJNI_EXECUTION_STATUS_HELPER_HPP

#include <cse.h>
#include <cse/cse_execution_s.hpp>
#include <cse/unit_conversion.hpp>

#include <jni.h>

namespace
{

jobject create_execution_status(JNIEnv* env, jlong executionPtr)
{
    jclass cls = env->FindClass("org/osp/cse/CseExecutionStatus");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(DDDZII)V");

    cse_execution_status status;
    jboolean success = cse_execution_get_status(reinterpret_cast<cse_execution*>(executionPtr), &status) == 0;
    if (!success) {
        return nullptr;
    }
    return env->NewObject(cls,
        ctor,
        to_seconds(status.current_time),
        status.real_time_factor,
        status.real_time_factor_target,
        status.is_real_time_simulation,
        status.state,
        status.error_code);
}

} // namespace

#endif

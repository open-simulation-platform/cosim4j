
#ifndef COSIMJNI_COSIM_EXECUTION_STATUS_HELPER_HPP
#define COSIMJNI_COSIM_EXECUTION_STATUS_HELPER_HPP

#include <cosim.h>
#include <cosim/cosim_execution_s.hpp>
#include <cosim/unit_conversion.hpp>

#include <jni.h>

namespace
{

jobject create_execution_status(JNIEnv* env, jlong executionPtr)
{
    jclass cls = env->FindClass("org/osp/cosim/CosimExecutionStatus");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(DDDZII)V");

    cosim_execution_status status;
    jboolean success = cosim_execution_get_status(reinterpret_cast<cosim_execution*>(executionPtr), &status) == 0;
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

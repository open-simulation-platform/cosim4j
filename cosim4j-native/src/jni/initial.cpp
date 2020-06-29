
#include <cosim.h>
#include <cosim/cosim_execution_s.hpp>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_setInitialRealValue(JNIEnv* env, jobject, jlong executionPtr, jint slaveIndex, jlong vr, jdouble value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto& cpp_execution = reinterpret_cast<cosim_execution*>(executionPtr)->cpp_execution;
    cpp_execution->set_real_initial_value(slaveIndex, static_cast<cosim::value_reference>(vr), value);

    return true;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_setInitialIntegerValue(JNIEnv* env, jobject, jlong executionPtr, jint slaveIndex, jlong vr, jint value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto& cpp_execution = reinterpret_cast<cosim_execution*>(executionPtr)->cpp_execution;
    cpp_execution->set_integer_initial_value(slaveIndex, static_cast<cosim::value_reference>(vr), value);

    return true;
}


JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_setInitialBooleanValue(JNIEnv* env, jobject, jlong executionPtr, jint slaveIndex, jlong vr, jboolean value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto& cpp_execution = reinterpret_cast<cosim_execution*>(executionPtr)->cpp_execution;
    cpp_execution->set_boolean_initial_value(slaveIndex, static_cast<cosim::value_reference>(vr), value);

    return true;
}

JNIEXPORT jboolean JNICALL Java_com_opensimulationplatform_cosim_jni_CosimLibrary_setInitialStringValue(JNIEnv* env, jobject, jlong executionPtr, jint slaveIndex, jlong vr, jstring value)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }

    auto value_ = env->GetStringUTFChars(value, nullptr);
    auto& cpp_execution = reinterpret_cast<cosim_execution*>(executionPtr)->cpp_execution;
    cpp_execution->set_string_initial_value(slaveIndex, static_cast<cosim::value_reference>(vr), value_);
    env->ReleaseStringUTFChars(value, value_);

    return true;
}


#ifdef __cplusplus
}
#endif

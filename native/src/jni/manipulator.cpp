
#include <cse.h>

#include <iostream>
#include <jni.h>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createOverrideManipulator(JNIEnv* env, jobject obj)
{
    return reinterpret_cast<jlong>(cse_override_manipulator_create());
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addManipulator(JNIEnv* env, jobject obj, jlong executionPtr, jlong manipulatorPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_add_manipulator(reinterpret_cast<cse_execution*>(executionPtr), reinterpret_cast<cse_manipulator*>(manipulatorPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyManipulator(JNIEnv* env, jobject obj, jlong manipulatorPtr)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_manipulator_destroy(reinterpret_cast<cse_manipulator*>(manipulatorPtr)) == 0;
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createScenarioManager(JNIEnv* env, jobject obj)
{
    return (jlong)cse_scenario_manager_create();
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_loadScenario(JNIEnv* env, jobject obj, jlong executionPtr, jlong manipulatorPtr, jstring scenarioFile)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    auto scenarioFile_ = env->GetStringUTFChars(scenarioFile, nullptr);
    bool status = cse_execution_load_scenario(reinterpret_cast<cse_execution*>(executionPtr), reinterpret_cast<cse_manipulator*>(manipulatorPtr), scenarioFile_) == 0;
    env->ReleaseStringUTFChars(scenarioFile, scenarioFile_);
    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_isScenarioRunning(JNIEnv* env, jobject obj, jlong manipulatorPtr)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_scenario_is_running(reinterpret_cast<cse_manipulator*>(manipulatorPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_abortScenario(JNIEnv* env, jobject obj, jlong manipulatorPtr)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }
    return cse_scenario_abort(reinterpret_cast<cse_manipulator*>(manipulatorPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setReal(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jdoubleArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    jdouble* _values = env->GetDoubleArrayElements(values, nullptr);

    auto vr__ = reinterpret_cast<cse_value_reference*>(malloc(sizeof(cse_value_reference) * size));
    for (int i = 0; i < size; ++i) {
        vr__[i] = (cse_value_reference)vr_[i];
    }

    bool status = cse_manipulator_slave_set_real(reinterpret_cast<cse_manipulator*>(manipulatorPtr), slaveIndex, vr__, size, _values) == 0;

    env->ReleaseLongArrayElements(vr, vr_, 0);
    env->ReleaseDoubleArrayElements(values, _values, 0);

    free(vr__);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInteger(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jintArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    auto vr_ = env->GetLongArrayElements(vr, nullptr);
    auto values_ = env->GetIntArrayElements(values, nullptr);

    auto* vr__ = reinterpret_cast<cse_value_reference*>(malloc(sizeof(cse_value_reference) * size));
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cse_value_reference>(vr_[i]);
    }

    bool status = cse_manipulator_slave_set_integer(
                      reinterpret_cast<cse_manipulator*>(manipulatorPtr),
                      slaveIndex, vr__, size,
                      reinterpret_cast<int*>(values_)) == 0;

    env->ReleaseLongArrayElements(vr, vr_, 0);
    env->ReleaseIntArrayElements(values, values_, 0);

    free(vr__);

    return status;
}


JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setBoolean(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jbooleanArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    jboolean* values_ = env->GetBooleanArrayElements(values, nullptr);

    auto* vr__ = reinterpret_cast<cse_value_reference*>(malloc(sizeof(cse_value_reference) * size));
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cse_value_reference>(vr_[i]);
    }

    bool status = cse_manipulator_slave_set_boolean(reinterpret_cast<cse_manipulator*>(manipulatorPtr), slaveIndex, vr__, size, reinterpret_cast<bool*>(values_)) == 0;

    env->ReleaseLongArrayElements(vr, vr_, 0);
    env->ReleaseBooleanArrayElements(values, values_, 0);

    free(vr__);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setString(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jobjectArray values)
{
    if (manipulatorPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: manipulatorPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);

    std::vector<const char*> values_(size);
    for (int i = 0; i < size; i++) {
        auto value = reinterpret_cast<jstring>(env->GetObjectArrayElement(values, i));
        values_[i] = env->GetStringUTFChars(value, nullptr);
    }

    auto* vr__ = reinterpret_cast<cse_value_reference*>(malloc(sizeof(cse_value_reference) * size));
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cse_value_reference>(vr_[i]);
    }

    bool status = cse_manipulator_slave_set_string(reinterpret_cast<cse_manipulator*>(manipulatorPtr), slaveIndex, vr__, size, values_.data()) == 0;

    for (int i = 0; i < size; i++) {
        auto value = reinterpret_cast<jstring>(env->GetObjectArrayElement(values, i));
        env->ReleaseStringUTFChars(value, values_[i]);
    }

    env->ReleaseLongArrayElements(vr, vr_, 0);

    free(vr__);

    return status;
}


#ifdef __cplusplus
}
#endif

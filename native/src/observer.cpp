
#include <cse.h>

#include <iostream>
#include <jni.h>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createLastValueObserver(JNIEnv* env, jobject obj)
{
    return reinterpret_cast<jlong>(cse_last_value_observer_create());
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserver(JNIEnv* env, jobject obj, jstring logDir)
{
    const char* _logDir = env->GetStringUTFChars(logDir, nullptr);
    cse_observer* observer = cse_file_observer_create(_logDir);
    env->ReleaseStringUTFChars(logDir, _logDir);
    if (observer == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(observer);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserverFromCfg(JNIEnv* env, jobject obj, jstring logDir, jstring cfgPath)
{
    const char* logDir_ = env->GetStringUTFChars(logDir, nullptr);
    const char* cfgPath_ = env->GetStringUTFChars(cfgPath, nullptr);
    cse_observer* observer = cse_file_observer_create_from_cfg(logDir_, cfgPath_);
    env->ReleaseStringUTFChars(logDir, logDir_);
    env->ReleaseStringUTFChars(cfgPath, cfgPath_);
    if (observer == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cse_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(observer);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createTimeSeriesObserver(JNIEnv* env, jobject obj, jint bufferSize)
{
    if (bufferSize <= 0) {
        return reinterpret_cast<jlong>(cse_time_series_observer_create());
    } else {
        return reinterpret_cast<jlong>(cse_buffered_time_series_observer_create((static_cast<size_t>(bufferSize))));
    }
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_startObserving(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_observer_start_observing(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, static_cast<cse_variable_type>(variableType), static_cast<cse_value_reference>(vr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stopObserving(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_observer_stop_observing(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, static_cast<cse_variable_type>(variableType), static_cast<cse_value_reference>(vr)) == 0;
}


JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyObserver(JNIEnv* env, jobject obj, jlong observerPtr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_observer_destroy(reinterpret_cast<cse_observer*>(observerPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addObserver(JNIEnv* env, jobject obj, jlong executionPtr, jlong observerPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cse_execution_add_observer(reinterpret_cast<cse_execution*>(executionPtr), reinterpret_cast<cse_observer*>(observerPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getReal(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jdoubleArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    auto* ref_ = (double*)malloc(sizeof(double) * size);

    auto* vr__ = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cse_value_reference>(vr_[i]);
    }

    bool status = cse_observer_slave_get_real(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, vr__, size, ref_) == 0;

    env->SetDoubleArrayRegion(ref, 0, size, ref_);

    free(ref_);
    free(vr__);
    env->ReleaseLongArrayElements(vr, vr_, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getInteger(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jintArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    auto ref_ = (int*)malloc(sizeof(int) * size);

    auto vr__ = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        vr__[i] = (cse_value_reference)vr_[i];
    }

    bool status = cse_observer_slave_get_integer(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, vr__, size, ref_) == 0;

    env->SetIntArrayRegion(ref, 0, size, (jint*)ref_);

    free(ref_);
    free(vr__);
    env->ReleaseLongArrayElements(vr, vr_, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getBoolean(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jbooleanArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    auto ref_ = (bool*)malloc(sizeof(bool) * size);

    auto* vr__ = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        vr__[i] = (cse_value_reference)vr_[i];
    }

    bool status = cse_observer_slave_get_boolean(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, vr__, size, ref_) == 0;

    env->SetBooleanArrayRegion(ref, 0, size, (jboolean*)ref_);

    free(ref_);
    free(vr__);
    env->ReleaseLongArrayElements(vr, vr_, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getString(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jobjectArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);

    std::vector<const char*> ref_(size);
    for (int i = 0; i < size; i++) {
        auto str = (jstring)env->GetObjectArrayElement(ref, i);
        ref_[i] = env->GetStringUTFChars(str, nullptr);
    }

    auto vr__ = (cse_value_reference*)malloc(sizeof(cse_value_reference) * size);
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cse_value_reference>(vr_[i]);
    }

    bool status = cse_observer_slave_get_string(reinterpret_cast<cse_observer*>(observerPtr), slaveIndex, vr__, size, ref_.data()) == 0;

    for (int i = 0; i < size; i++) {
        jstring value = env->NewStringUTF(ref_[i]);
        env->SetObjectArrayElement(ref, i, value);
    }

    free(vr__);
    env->ReleaseLongArrayElements(vr, vr_, 0);

    return status;
}

#ifdef __cplusplus
}
#endif

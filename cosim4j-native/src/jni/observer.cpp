
#include <cosim.h>

#include <iostream>
#include <jni.h>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL Java_org_osp_cosim_jni_CosimLibrary_createLastValueObserver(JNIEnv* env, jobject)
{
    return reinterpret_cast<jlong>(cosim_last_value_observer_create());
}

JNIEXPORT jlong JNICALL Java_org_osp_cosim_jni_CosimLibrary_createFileObserver(JNIEnv* env, jobject, jstring logDir)
{
    const char* _logDir = env->GetStringUTFChars(logDir, nullptr);
    cosim_observer* observer = cosim_file_observer_create(_logDir);
    env->ReleaseStringUTFChars(logDir, _logDir);
    if (observer == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cosim_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(observer);
}

JNIEXPORT jlong JNICALL Java_org_osp_cosim_jni_CosimLibrary_createFileObserverFromCfg(JNIEnv* env, jobject, jstring logDir, jstring cfgPath)
{
    const char* logDir_ = env->GetStringUTFChars(logDir, nullptr);
    const char* cfgPath_ = env->GetStringUTFChars(cfgPath, nullptr);
    cosim_observer* observer = cosim_file_observer_create_from_cfg(logDir_, cfgPath_);
    env->ReleaseStringUTFChars(logDir, logDir_);
    env->ReleaseStringUTFChars(cfgPath, cfgPath_);
    if (observer == nullptr) {
        std::cerr << "[JNI-wrapper] Error: Failed to create observer: " << cosim_last_error_message() << std::endl;
        return 0;
    }
    return reinterpret_cast<jlong>(observer);
}

JNIEXPORT jlong JNICALL Java_org_osp_cosim_jni_CosimLibrary_createTimeSeriesObserver(JNIEnv* env, jobject, jint bufferSize)
{
    if (bufferSize <= 0) {
        return reinterpret_cast<jlong>(cosim_time_series_observer_create());
    } else {
        return reinterpret_cast<jlong>(cosim_buffered_time_series_observer_create((static_cast<size_t>(bufferSize))));
    }
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_startObserving(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cosim_observer_start_observing(reinterpret_cast<cosim_observer*>(observerPtr), slaveIndex, static_cast<cosim_variable_type>(variableType), static_cast<cosim_value_reference>(vr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_stopObserving(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cosim_observer_stop_observing(reinterpret_cast<cosim_observer*>(observerPtr), slaveIndex, static_cast<cosim_variable_type>(variableType), static_cast<cosim_value_reference>(vr)) == 0;
}


JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_destroyObserver(JNIEnv* env, jobject, jlong observerPtr)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cosim_observer_destroy(reinterpret_cast<cosim_observer*>(observerPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_addObserver(JNIEnv* env, jobject, jlong executionPtr, jlong observerPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }
    return cosim_execution_add_observer(reinterpret_cast<cosim_execution*>(executionPtr), reinterpret_cast<cosim_observer*>(observerPtr)) == 0;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_getReal(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlongArray vr, jdoubleArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    auto ref_ = reinterpret_cast<jdouble*>(malloc(sizeof(jdouble) * size));

    auto vr__ = reinterpret_cast<cosim_value_reference*>(malloc(sizeof(cosim_value_reference) * size));
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cosim_value_reference>(vr_[i]);
    }

    bool status = cosim_observer_slave_get_real(
                      reinterpret_cast<cosim_observer*>(observerPtr),
                      slaveIndex, vr__, size, ref_) == 0;

    env->SetDoubleArrayRegion(ref, 0, size, ref_);

    free(ref_);
    free(vr__);
    env->ReleaseLongArrayElements(vr, vr_, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_getInteger(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlongArray vr, jintArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    auto ref_ = reinterpret_cast<int*>(malloc(sizeof(jint) * size));

    auto vr__ = reinterpret_cast<cosim_value_reference*>(malloc(sizeof(cosim_value_reference) * size));
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cosim_value_reference>(vr_[i]);
    }

    bool status = cosim_observer_slave_get_integer(
                      reinterpret_cast<cosim_observer*>(observerPtr),
                      slaveIndex, vr__, size, ref_) == 0;

    env->SetIntArrayRegion(ref, 0, size, (jint*)ref_);

    free(ref_);
    free(vr__);
    env->ReleaseLongArrayElements(vr, vr_, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_getBoolean(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlongArray vr, jbooleanArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);
    auto vr__ = reinterpret_cast<cosim_value_reference*>(malloc(sizeof(cosim_value_reference) * size));

    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cosim_value_reference>(vr_[i]);
    }

    auto ref_ = reinterpret_cast<bool*>(malloc(sizeof(bool) * size));
    bool status = cosim_observer_slave_get_boolean(
                      reinterpret_cast<cosim_observer*>(observerPtr),
                      slaveIndex, vr__, size, ref_) == 0;

    auto ref__ = reinterpret_cast<jboolean *>(malloc(sizeof(jboolean) * size));

    for (int i = 0; i < size; ++i) {
        ref__[i] = static_cast<jboolean >(ref_[i]);
    }

    env->SetBooleanArrayRegion(ref, 0, size, ref__);

    free(ref_);
    free(ref__);
    free(vr__);
    env->ReleaseLongArrayElements(vr, vr_, 0);

    return status;
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_getString(JNIEnv* env, jobject, jlong observerPtr, jint slaveIndex, jlongArray vr, jobjectArray ref)
{
    if (observerPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: observerPtr is NULL" << std::endl;
        return false;
    }

    const jsize size = env->GetArrayLength(vr);
    jlong* vr_ = env->GetLongArrayElements(vr, nullptr);

    std::vector<const char*> ref_(size);
    for (int i = 0; i < size; i++) {
        auto str = reinterpret_cast<jstring>(env->GetObjectArrayElement(ref, i));
        ref_[i] = env->GetStringUTFChars(str, nullptr);
    }

    auto vr__ = reinterpret_cast<cosim_value_reference*>(malloc(sizeof(cosim_value_reference) * size));
    for (int i = 0; i < size; ++i) {
        vr__[i] = static_cast<cosim_value_reference>(vr_[i]);
    }

    bool status = cosim_observer_slave_get_string(
                      reinterpret_cast<cosim_observer*>(observerPtr),
                      slaveIndex, vr__, size, ref_.data()) == 0;

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

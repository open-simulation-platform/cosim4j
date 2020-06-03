
#include <cosim.h>
#include <cosim/cosim_slave_s.hpp>
#include <cosim/jvm_slave.hpp>
#include <cosim/model_description_helper.hpp>
#include <cosim/slave_infos_helper.hpp>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_org_osp_cosim_jni_CosimLibrary_getModelDescription(JNIEnv* env, jobject, jlong executionPtr, jint slaveIndex)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }
    return create_model_description(env, executionPtr, slaveIndex);
}

JNIEXPORT jlong JNICALL Java_org_osp_cosim_jni_CosimLibrary_createSlave(JNIEnv* env, jobject, jstring fmuPath, jstring jInstanceName)
{
    auto fmuPath_ = env->GetStringUTFChars(fmuPath, nullptr);
    auto cInstanceName = env->GetStringUTFChars(jInstanceName, nullptr);
    cosim_slave* slave = cosim_local_slave_create(fmuPath_, cInstanceName);
    env->ReleaseStringUTFChars(fmuPath, fmuPath_);
    env->ReleaseStringUTFChars(jInstanceName, cInstanceName);
    return reinterpret_cast<jlong>(slave);
}

JNIEXPORT jlong JNICALL Java_org_osp_cosim_jni_CosimLibrary_createJvmSlave(JNIEnv* env, jobject, jobject jslave, jstring jInstanceName)
{
    auto instance = std::make_shared<cosim::jvm_slave>(env, jslave);
    const auto md = instance->model_description();

    auto slave = std::make_unique<cosim_slave>();
    slave->modelName = md.name;
    slave->instance = instance;
    // slave address not in use yet. Should be something else than a string.
    slave->address = "local";

    auto cInstanceName = env->GetStringUTFChars(jInstanceName, nullptr);
    slave->instanceName = cInstanceName;
    env->ReleaseStringUTFChars(jInstanceName, cInstanceName);

    return reinterpret_cast<jlong>(slave.release());
}

JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_destroySlave(JNIEnv*, jobject, jlong slave)
{
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
        return false;
    }
    return cosim_local_slave_destroy(reinterpret_cast<cosim_slave*>(slave)) == 0;
}

JNIEXPORT jint JNICALL Java_org_osp_cosim_jni_CosimLibrary_addSlave(JNIEnv*, jobject, jlong executionPtr, jlong slave)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
        return false;
    }
    return static_cast<jint>(cosim_execution_add_slave(reinterpret_cast<cosim_execution*>(executionPtr), reinterpret_cast<cosim_slave*>(slave)));
}

JNIEXPORT jint JNICALL Java_org_osp_cosim_jni_CosimLibrary_getNumSlaves(JNIEnv*, jobject, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return static_cast<jint>(cosim_execution_get_num_slaves(reinterpret_cast<cosim_execution*>(executionPtr)));
}

JNIEXPORT jobject JNICALL Java_org_osp_cosim_jni_CosimLibrary_getSlaveInfos(JNIEnv* env, jobject, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }
    return create_slave_infos(env, executionPtr);
}

#ifdef __cplusplus
}
#endif

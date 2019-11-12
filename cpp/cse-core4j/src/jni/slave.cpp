
#include <cse.h>
#include <cse/cse_slave_s.hpp>
#include <cse/jvm_slave.hpp>
#include <cse/model_description_helper.hpp>
#include <cse/py_state.hpp>
#include <cse/python_model.hpp>
#include <cse/slave_infos_helper.hpp>

#include <iostream>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getModelDescription(JNIEnv* env, jobject, jlong executionPtr, jint slaveIndex)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return nullptr;
    }
    return create_model_description(env, executionPtr, slaveIndex);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSlave(JNIEnv* env, jobject, jstring fmuPath, jstring jInstanceName)
{
    auto fmuPath_ = env->GetStringUTFChars(fmuPath, nullptr);
    auto cInstanceName = env->GetStringUTFChars(jInstanceName, nullptr);
    cse_slave* slave = cse_local_slave_create(fmuPath_, cInstanceName);
    env->ReleaseStringUTFChars(fmuPath, fmuPath_);
    env->ReleaseStringUTFChars(jInstanceName, cInstanceName);
    return reinterpret_cast<jlong>(slave);
}

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createJvmSlave(JNIEnv* env, jobject, jobject jslave, jstring jInstanceName)
{
    auto instance = std::make_shared<cse::jvm_slave>(env, jslave);
    const auto md = instance->model_description();

    auto slave = std::make_unique<cse_slave>();
    slave->modelName = md.name;
    slave->instance = instance;
    // slave address not in use yet. Should be something else than a string.
    slave->address = "local";
    slave->source = "memory";

    auto cInstanceName = env->GetStringUTFChars(jInstanceName, nullptr);
    slave->instanceName = cInstanceName;
    env->ReleaseStringUTFChars(jInstanceName, cInstanceName);

    return reinterpret_cast<jlong>(slave.release());
}

#ifdef HAS_PYTHON

std::unique_ptr<cse::py_state> pyState = nullptr;

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createPySlave(JNIEnv* env, jobject, jstring pyPath, jstring jInstanceName)
{

    if (pyState == nullptr) {
        pyState = std::make_unique<cse::py_state>();
    }

    auto pyPath_ = env->GetStringUTFChars(pyPath, nullptr);
    auto model = std::make_shared<cse::python_model>(pyPath_);
    env->ReleaseStringUTFChars(pyPath, pyPath_);

    auto instance = model->instantiate_slave();
    const auto md = instance->model_description();

    auto slave = std::make_unique<cse_slave>();
    slave->modelName = md.name;
    slave->instance = instance;
    // slave address not in use yet. Should be something else than a string.
    slave->address = "local";
    slave->source = "memory";

    auto cInstanceName = env->GetStringUTFChars(jInstanceName, nullptr);
    slave->instanceName = cInstanceName;
    env->ReleaseStringUTFChars(jInstanceName, cInstanceName);

    return reinterpret_cast<jlong>(slave.release());
}

#endif

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroySlave(JNIEnv* env, jobject, jlong slave)
{
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
        return false;
    }
    return cse_local_slave_destroy(reinterpret_cast<cse_slave*>(slave)) == 0;
}

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_addSlave(JNIEnv* env, jobject, jlong executionPtr, jlong slave)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    if (slave == 0) {
        std::cerr << "[JNI-wrapper] Error: slave is NULL" << std::endl;
        return false;
    }

    return static_cast<jint>(cse_execution_add_slave(reinterpret_cast<cse_execution*>(executionPtr), reinterpret_cast<cse_slave*>(slave)));
}

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumSlaves(JNIEnv* env, jobject, jlong executionPtr)
{
    if (executionPtr == 0) {
        std::cerr << "[JNI-wrapper] Error: executionPtr is NULL" << std::endl;
        return false;
    }
    return static_cast<jint>(cse_execution_get_num_slaves(reinterpret_cast<cse_execution*>(executionPtr)));
}

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getSlaveInfos(JNIEnv* env, jobject, jlong executionPtr)
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


#ifndef CSECOREJNI_MODEL_DESCRIPTION_HELPER_HPP
#define CSECOREJNI_MODEL_DESCRIPTION_HELPER_HPP

#include <cse/structs.hpp>

#include <iostream>
#include <jni.h>

namespace
{

jobject create_variable(JNIEnv* env, cse::variable_description& description)
{
    jclass cls = env->FindClass("org/osp/cse/CseVariableDescription");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;JIII)V");

    return env->NewObject(
        cls,
        ctor,
        env->NewStringUTF(description.name.c_str()),
        static_cast<jlong>(description.reference),
        static_cast<jint>(description.type),
        static_cast<jint>(description.causality),
        static_cast<jint>(description.variability));
}

jobject create_variables(JNIEnv* env, std::vector<cse::variable_description>& vars)
{
    jclass cls = env->FindClass("java/util/ArrayList");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(I)V");
    jmethodID addId = env->GetMethodID(cls, "add", "(Ljava/lang/Object;)Z");

    auto list = env->NewObject(cls, ctor, vars.size());
    for (auto& var : vars) {
        auto v = create_variable(env, var);
        env->CallBooleanMethod(list, addId, v);
    }
    return list;
}

jobject create_model_description(JNIEnv* env, jlong executionPtr, jint slaveIndex)
{

    auto execution = reinterpret_cast<cse_execution*>(executionPtr);
    auto description = execution->get_model_description(slaveIndex);

    jclass cls = env->FindClass("org/osp/cse/CseModelDescription");
    jmethodID ctor = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/util/List;)V");

    return env->NewObject(cls,
        ctor,
        env->NewStringUTF(description.name.c_str()),
        env->NewStringUTF(description.uuid.c_str()),
        env->NewStringUTF(description.description.c_str()),
        env->NewStringUTF(description.author.c_str()),
        env->NewStringUTF(description.version.c_str()),
        create_variables(env, description.variables));
}

} // namespace

#endif

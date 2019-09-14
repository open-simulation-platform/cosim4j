//
// Created by LarsIvar on 13.09.2019.
//

#ifndef CSECOREJNI_SLAVE_INFOS_HELPER_HPP
#define CSECOREJNI_SLAVE_INFOS_HELPER_HPP

#include <cse.h>
#include <cse/structs.hpp>

#include <jni.h>

namespace
{

jobject create_slave_infos(JNIEnv* env, jlong executionPtr)
{
    auto execution = reinterpret_cast<cse_execution*>(executionPtr);
    
    jclass listCls = env->FindClass("java/util/ArrayList");
    jmethodID listCtor = env->GetMethodID(listCls, "<init>", "(I)V");
    jmethodID addId = env->GetMethodID(listCls, "add", "(Ljava/lang/Object;)Z");

    jclass infoCls = env->FindClass("org/osp/cse/CseSlaveInfo");
    jmethodID infoCtor = env->GetMethodID(infoCls, "<init>", "(ILjava/lang/String;Ljava/lang/String;)V");

    const auto size = execution->get_num_slaves();

    auto ids = execution->simulators;
    auto list = env->NewObject(listCls, listCtor, size);
    for (const auto& [name, entry] : ids) {

        auto info = env->NewObject(
            infoCls,
            infoCtor,
            entry.index,
            env->NewStringUTF(name.c_str()),
            env->NewStringUTF(entry.source.c_str()));

        env->CallBooleanMethod(list, addId, info);
    }

    return list;
}


} // namespace

#endif //CSECOREJNI_SLAVE_INFOS_HELPER_HPP

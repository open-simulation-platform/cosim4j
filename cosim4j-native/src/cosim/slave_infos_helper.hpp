
#ifndef COSIMJNI_COSIM_SLAVE_INFOS_HELPER_HPP
#define COSIMJNI_COSIM_SLAVE_INFOS_HELPER_HPP

#include <cosim.h>
#include <cosim/cosim_execution_s.hpp>

#include <jni.h>

namespace
{

jobject create_slave_infos(JNIEnv* env, jlong executionPtr)
{
    auto execution = reinterpret_cast<cosim_execution*>(executionPtr);

    jclass listCls = env->FindClass("java/util/ArrayList");
    jmethodID listCtor = env->GetMethodID(listCls, "<init>", "(I)V");
    jmethodID addId = env->GetMethodID(listCls, "add", "(Ljava/lang/Object;)Z");

    jclass infoCls = env->FindClass("org/osp/cosim/CosimSlaveInfo");
    jmethodID infoCtor = env->GetMethodID(infoCls, "<init>", "(ILjava/lang/String;)V");

    const auto size = execution->get_num_slaves();
    auto infos = reinterpret_cast<cosim_slave_info*>(malloc(sizeof(cosim_slave_info) * size));
    bool status = cosim_execution_get_slave_infos((cosim_execution*)execution, infos, size) == 0;

    jobject list = nullptr;
    if (status) {
        list = env->NewObject(listCls, listCtor, size);
        for (int i = 0; i < size; i++) {
            auto info = env->NewObject(
                infoCls,
                infoCtor,
                infos[i].index,
                env->NewStringUTF(infos[i].name));

            env->CallBooleanMethod(list, addId, info);
        }
    }

    free(infos);
    return list;
}


} // namespace

#endif //COSIMJNI_COSIM_SLAVE_INFOS_HELPER_HPP

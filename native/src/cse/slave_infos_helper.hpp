
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
    auto* infos = (cse_slave_info*)malloc(sizeof(cse_slave_info) * size);
    bool status = cse_execution_get_slave_infos((cse_execution*)execution, infos, size) == 0;

    jobject list = nullptr;
    if (status) {

        list = env->NewObject(listCls, listCtor, size);
        for (int i = 0; i < size; i++) {
            auto info = env->NewObject(
                infoCls,
                infoCtor,
                infos[i].index,
                env->NewStringUTF(infos[i].name),
                env->NewStringUTF(infos[i].source));

            env->CallBooleanMethod(list, addId, info);
        }

    }

    free(infos);
    return list;
}


} // namespace

#endif //CSECOREJNI_SLAVE_INFOS_HELPER_HPP


#include <cse/jvm_helper.hpp>
#include <cse/jvm_slave.hpp>

#include <iostream>

namespace cse
{

jvm_slave::jvm_slave(JNIEnv* env, jobject slave)
{
    env->GetJavaVM(&jvm_);
    slave_ = env->NewGlobalRef(slave);

    jclass cls = env->FindClass("org/osp/cse/CseJvmSlave");

    setupId_ = env->GetMethodID(cls, "setup", "(D)V");
    onBeforeStartId_ = env->GetMethodID(cls, "onBeforeStart", "()V");
    doStepId_ = env->GetMethodID(cls, "doStep", "(DD)V");
    terminateId_ = env->GetMethodID(cls, "terminate", "()V");

    getRealId_ = env->GetMethodID(cls, "getReal", "([J)[D");
    setRealId_ = env->GetMethodID(cls, "setReal", "([J[D)V");

    getIntegerId_ = env->GetMethodID(cls, "getInteger", "([J)[I");
    setIntegerId_ = env->GetMethodID(cls, "setInteger", "([J[I)V");

    getBooleanId_ = env->GetMethodID(cls, "getBoolean", "([J)[Z");
    setBooleanId_ = env->GetMethodID(cls, "setBoolean", "([J[Z)V");

    getStringId_ = env->GetMethodID(cls, "getString", "([J)[Ljava/lang/String;");
    setStringId_ = env->GetMethodID(cls, "setString", "([J[Ljava/lang/String;)V");

    jclass mdCls = env->FindClass("org/osp/cse/CseModelDescription");
    jmethodID modelDescriptionId = env->GetMethodID(cls, "getModelDescription", "()Lorg/osp/cse/CseModelDescription;");
    jobject jmd = env->CallObjectMethod(slave_, modelDescriptionId);

    {
        jmethodID nameId_ = env->GetMethodID(mdCls, "getName", "()Ljava/lang/String;");
        jmethodID uuidId_ = env->GetMethodID(mdCls, "getUuid", "()Ljava/lang/String;");
        jmethodID authorId_ = env->GetMethodID(mdCls, "getAuthor", "()Ljava/lang/String;");
        jmethodID versionId_ = env->GetMethodID(mdCls, "getVersion", "()Ljava/lang/String;");
        jmethodID descriptionId_ = env->GetMethodID(mdCls, "getDescription", "()Ljava/lang/String;");

        model_description_.name = invoke_string_getter(env, jmd, nameId_);
        model_description_.uuid = invoke_string_getter(env, jmd, uuidId_);
        model_description_.author = invoke_string_getter(env, jmd, authorId_);
        model_description_.version = invoke_string_getter(env, jmd, versionId_);
        model_description_.description = invoke_string_getter(env, jmd, descriptionId_);
    }

    {

        jclass varCls = env->FindClass("org/osp/cse/CseVariableDescription");
        jmethodID nameId_ = env->GetMethodID(varCls, "getName", "()Ljava/lang/String;");
        jmethodID referenceId_ = env->GetMethodID(varCls, "getValueReference", "()J");
        jmethodID typeId = env->GetMethodID(varCls, "getTypeId", "()I");
        jmethodID causalityId = env->GetMethodID(varCls, "getCausalityId", "()I");
        jmethodID variabilityId = env->GetMethodID(varCls, "getVariabilityId", "()I");

        jclass listCls = env->FindClass("java/util/List");
        jmethodID sizeId = env->GetMethodID(listCls, "size", "()I");
        jmethodID getId = env->GetMethodID(listCls, "get", "(I)Ljava/lang/Object;");

        jmethodID variablesId_ = env->GetMethodID(mdCls, "getVariables", "()Ljava/util/List;");
        jobject variables = env->CallObjectMethod(jmd, variablesId_);

        const auto size = env->CallIntMethod(variables, sizeId);
        for (int i = 0; i < size; i++) {

            jobject var = env->CallObjectMethod(variables, getId, i);

            variable_description vd{};
            vd.name = invoke_string_getter(env, var, nameId_);
            vd.reference = static_cast<value_reference>(env->CallLongMethod(var, referenceId_));
            vd.type = static_cast<variable_type>(env->CallIntMethod(var, typeId));
            vd.causality = static_cast<variable_causality>(env->CallIntMethod(var, causalityId));
            vd.variability = static_cast<variable_variability>(env->CallIntMethod(var, variabilityId));

            model_description_.variables.push_back(vd);
        }
    }
}

cse::model_description jvm_slave::model_description() const
{
    return model_description_;
}

void jvm_slave::setup(cse::time_point startTime, std::optional<time_point> /*stopTime*/, std::optional<double> /*relativeTolerance*/)
{
    double startTimeSec = to_double_time_point(startTime);
    worker_.work([this, startTimeSec]() {
        jvm_invoke(jvm_, [this, startTimeSec](JNIEnv* env) {
            env->CallVoidMethod(slave_, setupId_, startTimeSec);
        });
    });
}

void jvm_slave::start_simulation()
{
    worker_.work([this]() {
        jvm_invoke(jvm_, [this](JNIEnv* env) {
            env->CallVoidMethod(slave_, onBeforeStartId_);
        });
    });
}

cse::step_result jvm_slave::do_step(cse::time_point currentT, cse::duration deltaT)
{

    double currentTimeSec = to_double_time_point(currentT);
    double stepSizeSec = to_double_duration(deltaT, {});
    worker_.work([this, currentTimeSec, stepSizeSec]() {
        jvm_invoke(jvm_, [this, currentTimeSec, stepSizeSec](JNIEnv* env) {
            env->CallVoidMethod(slave_, doStepId_, currentTimeSec, stepSizeSec);
        });
    });

    return cse::step_result::complete;
}

void jvm_slave::end_simulation()
{
    worker_.work([this]() {
        jvm_invoke(jvm_, [this](JNIEnv* env) {
            env->CallVoidMethod(slave_, terminateId_);
        });
    });
}

void jvm_slave::get_real_variables(gsl::span<const value_reference> variables, gsl::span<double> values) const
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);

            auto valueArray = reinterpret_cast<jdoubleArray>(env->CallObjectMethod(slave_, getRealId_, vrArray));
            auto valueArrayElements = env->GetDoubleArrayElements(valueArray, nullptr);

            for (int i = 0; i < size; i++) {
                values[i] = valueArrayElements[i];
            }

            free(vrArrayElements);
            env->ReleaseDoubleArrayElements(valueArray, valueArrayElements, 0);
        });
    });
}

void jvm_slave::get_integer_variables(gsl::span<const value_reference> variables, gsl::span<int> values) const
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);

            auto valueArray = reinterpret_cast<jintArray>(env->CallObjectMethod(slave_, getIntegerId_, vrArray));
            auto valueArrayElements = env->GetIntArrayElements(valueArray, nullptr);

            for (int i = 0; i < size; i++) {
                values[i] = valueArrayElements[i];
            }

            free(vrArrayElements);
            env->ReleaseIntArrayElements(valueArray, valueArrayElements, 0);
        });
    });
}

void jvm_slave::get_boolean_variables(gsl::span<const value_reference> variables, gsl::span<bool> values) const
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);

            auto valueArray = reinterpret_cast<jbooleanArray>(env->CallObjectMethod(slave_, getBooleanId_, vrArray));
            auto valueArrayElements = env->GetBooleanArrayElements(valueArray, nullptr);

            for (int i = 0; i < size; i++) {
                values[i] = valueArrayElements[i];
            }

            free(vrArrayElements);
            env->ReleaseBooleanArrayElements(valueArray, valueArrayElements, 0);
        });
    });
}

void jvm_slave::get_string_variables(gsl::span<const value_reference> variables, gsl::span<std::string> values) const
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);

            auto valueArray = reinterpret_cast<jobjectArray>(env->CallObjectMethod(slave_, getStringId_, vrArray));

            for (int i = 0; i < size; i++) {
                auto jstr = reinterpret_cast<jstring>(env->GetObjectArrayElement(valueArray, i));
                auto cStr = env->GetStringUTFChars(jstr, nullptr);
                values[i] = cStr;
                env->ReleaseStringUTFChars(jstr, cStr);
            }

            free(vrArrayElements);
        });
    });
}

void jvm_slave::set_real_variables(gsl::span<const value_reference> variables, gsl::span<const double> values)
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            auto valueArray = env->NewDoubleArray(size);
            auto valueArrayElements = reinterpret_cast<jdouble*>(malloc(sizeof(jdouble) * size));

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
                valueArrayElements[i] = values[i];
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);
            env->SetDoubleArrayRegion(valueArray, 0, size, valueArrayElements);

            env->CallVoidMethod(slave_, setRealId_, vrArray, valueArray);

            free(vrArrayElements);
            free(valueArrayElements);
        });
    });
}

void jvm_slave::set_integer_variables(gsl::span<const value_reference> variables, gsl::span<const int> values)
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            auto valueArray = env->NewIntArray(size);
            auto valueArrayElements = reinterpret_cast<jint*>(malloc(sizeof(jint) * size));

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
                valueArrayElements[i] = static_cast<jint>(values[i]);
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);
            env->SetIntArrayRegion(valueArray, 0, size, valueArrayElements);

            env->CallVoidMethod(slave_, setIntegerId_, vrArray, valueArray);

            free(vrArrayElements);
            free(valueArrayElements);
        });
    });
}

void jvm_slave::set_boolean_variables(gsl::span<const value_reference> variables, gsl::span<const bool> values)
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            auto valueArray = env->NewBooleanArray(size);
            auto valueArrayElements = reinterpret_cast<jboolean*>(malloc(sizeof(jboolean) * size));

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
                valueArrayElements[i] = static_cast<jboolean>(values[i]);
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);
            env->SetBooleanArrayRegion(valueArray, 0, size, valueArrayElements);

            env->CallVoidMethod(slave_, setBooleanId_, vrArray, valueArray);

            free(vrArrayElements);
            free(valueArrayElements);
        });
    });
}

void jvm_slave::set_string_variables(gsl::span<const value_reference> variables, gsl::span<const std::string> values)
{
    if (variables.empty()) return;

    worker_.work([this, variables, values]() {
        jvm_invoke(jvm_, [this, variables, values](JNIEnv* env) {
            const auto size = static_cast<int>(variables.size());

            auto vrArray = env->NewLongArray(size);
            auto vrArrayElements = reinterpret_cast<jlong*>(malloc(sizeof(jlong) * size));

            auto valueArray = env->NewObjectArray(size, env->FindClass("java/lang/String"), NULL);

            for (int i = 0; i < size; i++) {
                vrArrayElements[i] = static_cast<jlong>(variables[i]);
                env->SetObjectArrayElement(valueArray, i, env->NewStringUTF(values[i].c_str()));
            }

            env->SetLongArrayRegion(vrArray, 0, size, vrArrayElements);

            env->CallVoidMethod(slave_, setStringId_, vrArray, valueArray);

            free(vrArrayElements);
        });
    });
}

jvm_slave::~jvm_slave()
{
    jvm_invoke(jvm_, [this](JNIEnv* env) {
        env->DeleteGlobalRef(slave_);
    });
}

} // namespace cse

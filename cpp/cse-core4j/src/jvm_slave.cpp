
#include <cse/jni_helper.hpp>
#include <cse/jvm_slave.hpp>

#include <iostream>

namespace
{

cse::variable_causality get_causality(JNIEnv* env, jclass Fmi2ScalarVariable, jobject scalarVariable)
{
    jmethodID getCausality = GetMethodID(env, Fmi2ScalarVariable, "getCausality", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2Causality;");
    jobject causality = env->CallObjectMethod(scalarVariable, getCausality);

    jclass Fmi2Causality = FindClass(env, "no/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2Causality");
    jmethodID ordinal = GetMethodID(env, Fmi2Causality, "ordinal", "()I");
    int result = env->CallIntMethod(causality, ordinal);
    switch (result) {
        case 0: return cse::variable_causality::parameter;
        case 1: return cse::variable_causality::calculated_parameter;
        case 2: return cse::variable_causality::input;
        case 3: return cse::variable_causality::output;
        default: return cse::variable_causality::local;
     }
}

cse::variable_variability get_variability(JNIEnv* env, jclass Fmi2ScalarVariable, jobject scalarVariable)
{
    jmethodID getVariability = GetMethodID(env, Fmi2ScalarVariable, "getVariability", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2Variability;");
    jobject variability = env->CallObjectMethod(scalarVariable, getVariability);

    jclass Fmi2Variability = FindClass(env, "no/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2Variability");
    jmethodID ordinal = GetMethodID(env, Fmi2Variability, "ordinal", "()I");
    int result = env->CallIntMethod(variability, ordinal);
    switch (result) {
        case 1: return cse::variable_variability::constant;
        case 2: return cse::variable_variability::fixed;
        case 3: return cse::variable_variability::tunable;
        case 4: return cse::variable_variability::discrete;
        default: return cse::variable_variability::continuous;
    }
}

cse::variable_type get_type(JNIEnv* env, jclass Fmi2ScalarVariable, jobject scalarVariable)
{
    jmethodID getReal = GetMethodID(env, Fmi2ScalarVariable, "getReal", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ScalarVariable$Real;");
    jobject real = env->CallObjectMethod(scalarVariable, getReal);
    if (real != nullptr) return cse::variable_type ::real;

    jmethodID getInteger = GetMethodID(env, Fmi2ScalarVariable, "getInteger", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ScalarVariable$Integer;");
    jobject integer = env->CallObjectMethod(scalarVariable, getInteger);
    if (integer != nullptr) return cse::variable_type ::integer;

    jmethodID getBoolean = GetMethodID(env, Fmi2ScalarVariable, "getBoolean", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ScalarVariable$Boolean;");
    jobject boolean = env->CallObjectMethod(scalarVariable, getBoolean);
    if (boolean != nullptr) return cse::variable_type ::boolean;

    jmethodID getString = GetMethodID(env, Fmi2ScalarVariable, "getString", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ScalarVariable$String;");
    jobject string = env->CallObjectMethod(scalarVariable, getString);
    if (string != nullptr) return cse::variable_type ::string;

}

cse::variable_description to_variable_description(JNIEnv* env, jobject var)
{
    jclass Fmi2ScalarVariable = env->GetObjectClass(var);
    jmethodID modelNameId_ = GetMethodID(env, Fmi2ScalarVariable, "getName", "()Ljava/lang/String;");
    jmethodID referenceId_ = GetMethodID(env, Fmi2ScalarVariable, "getValueReference", "()J");

    cse::variable_description vd{};
    vd.name = invoke_string_getter(env, var, modelNameId_);
    vd.reference = static_cast<cse::value_reference>(env->CallLongMethod(var, referenceId_));
    vd.type = get_type(env, Fmi2ScalarVariable, var);
    vd.causality = get_causality(env, Fmi2ScalarVariable, var);
    vd.variability = get_variability(env, Fmi2ScalarVariable, var);
    return vd;
}

} // namespace

namespace cse
{

jvm_slave::jvm_slave(JNIEnv* env, jobject slave)
{
    env->GetJavaVM(&jvm_);
    slave_ = env->NewGlobalRef(slave);

    jclass cls = env->GetObjectClass(slave_);
    jmethodID defineId = GetMethodID(env, cls, "define", "()Lno/ntnu/ihb/fmi4j/Fmi2Slave;");
    env->CallObjectMethod(slave_, defineId);

    setupExperimentId_ = GetMethodID(env, cls, "setupExperiment", "(D)Z");
    enterInitialisationModeId_ = GetMethodID(env, cls, "enterInitialisationMode", "()Z");
    exitInitialisationModeId_ = GetMethodID(env, cls, "exitInitialisationMode", "()Z");

    doStepId_ = GetMethodID(env, cls, "doStep", "(DD)Z");
    resetId_ = GetMethodID(env, cls, "reset", "()Z");
    terminateId_ = GetMethodID(env, cls, "terminate", "()Z");

    getRealId_ = GetMethodID(env, cls, "getReal", "([J)[D");
    setRealId_ = GetMethodID(env, cls, "setReal", "([J[D)V");

    getIntegerId_ = GetMethodID(env, cls, "getInteger", "([J)[I");
    setIntegerId_ = GetMethodID(env, cls, "setInteger", "([J[I)V");

    getBooleanId_ = GetMethodID(env, cls, "getBoolean", "([J)[Z");
    setBooleanId_ = GetMethodID(env, cls, "setBoolean", "([J[Z)V");

    getStringId_ = GetMethodID(env, cls, "getString", "([J)[Ljava/lang/String;");
    setStringId_ = GetMethodID(env, cls, "setString", "([J[Ljava/lang/String;)V");

    jclass Fmi2ModelDescription = env->FindClass("no/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ModelDescription");
    jmethodID getModelDescription = GetMethodID(env, cls, "getModelDescription", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ModelDescription;");
    jobject modelDescription = env->CallObjectMethod(slave_, getModelDescription);

    jclass ModelVariables = FindClass(env, "no/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ModelDescription$ModelVariables");
    jmethodID getModelVariables = GetMethodID(env, Fmi2ModelDescription, "getModelVariables", "()Lno/ntnu/ihb/fmi4j/modeldescription/fmi2/Fmi2ModelDescription$ModelVariables;");
    jobject modelVariables = env->CallObjectMethod(modelDescription, getModelVariables);

    {
        jmethodID nameId_ = GetMethodID(env, Fmi2ModelDescription, "getModelName", "()Ljava/lang/String;");
        jmethodID uuidId_ = GetMethodID(env, Fmi2ModelDescription, "getGuid", "()Ljava/lang/String;");
        jmethodID authorId_ = GetMethodID(env, Fmi2ModelDescription, "getAuthor", "()Ljava/lang/String;");
        jmethodID versionId_ = GetMethodID(env, Fmi2ModelDescription, "getVersion", "()Ljava/lang/String;");
        jmethodID descriptionId_ = GetMethodID(env, Fmi2ModelDescription, "getDescription", "()Ljava/lang/String;");

        model_description_.name = invoke_string_getter(env, modelDescription, nameId_);
        model_description_.uuid = invoke_string_getter(env, modelDescription, uuidId_);
        model_description_.author = invoke_string_getter(env, modelDescription, authorId_);
        model_description_.version = invoke_string_getter(env, modelDescription, versionId_);
        model_description_.description = invoke_string_getter(env, modelDescription, descriptionId_);
    }

    {
        jclass listCls = env->FindClass("java/util/List");
        jmethodID sizeId = GetMethodID(env, listCls, "size", "()I");
        jmethodID getId = GetMethodID(env, listCls, "get", "(I)Ljava/lang/Object;");

        jmethodID getScalarVariable = GetMethodID(env, ModelVariables, "getScalarVariable", "()Ljava/util/List;");
        jobject variables = env->CallObjectMethod(modelVariables, getScalarVariable);

        const auto size = env->CallIntMethod(variables, sizeId);
        for (int i = 0; i < size; i++) {
            jobject var = env->CallObjectMethod(variables, getId, i);
            model_description_.variables.push_back(to_variable_description(env, var));
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
            env->CallBooleanMethod(slave_, setupExperimentId_, startTimeSec);
            env->CallBooleanMethod(slave_, enterInitialisationModeId_);
        });
    });
}

void jvm_slave::start_simulation()
{
    worker_.work([this]() {
        jvm_invoke(jvm_, [this](JNIEnv* env) {
            env->CallBooleanMethod(slave_, exitInitialisationModeId_);
        });
    });
}

cse::step_result jvm_slave::do_step(cse::time_point currentT, cse::duration deltaT)
{
    double currentTimeSec = to_double_time_point(currentT);
    double stepSizeSec = to_double_duration(deltaT, {});
    worker_.work([this, currentTimeSec, stepSizeSec]() {
        jvm_invoke(jvm_, [this, currentTimeSec, stepSizeSec](JNIEnv* env) {
            env->CallBooleanMethod(slave_, doStepId_, currentTimeSec, stepSizeSec);
        });
    });

    return cse::step_result::complete;
}

void jvm_slave::end_simulation()
{
    worker_.work([this]() {
        jvm_invoke(jvm_, [this](JNIEnv* env) {
            env->CallBooleanMethod(slave_, terminateId_);
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

            auto valueArray = env->NewObjectArray(size, env->FindClass("java/lang/String"), nullptr);

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

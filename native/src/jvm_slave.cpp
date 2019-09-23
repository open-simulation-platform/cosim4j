
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

    modelDescriptionId_ = env->GetMethodID(cls, "getModelDescription", "()Lorg/osp/cse/CseModelDescription;");

    setupId_ = env->GetMethodID(cls, "setup", "(D)V");
    onBeforeStartId_ = env->GetMethodID(cls, "onBeforeStart", "()V");
    doStepId_ = env->GetMethodID(cls, "doStep", "(DD)V");
    terminateId_ = env->GetMethodID(cls, "terminate", "()V");
}

cse::model_description jvm_slave::model_description() const
{
    cse::model_description md;
    jvm_invoke(jvm_, [this, &md](JNIEnv* env) {
        jobject jmd = env->CallObjectMethod(slave_, modelDescriptionId_);

        jclass cls = env->FindClass("org/osp/cse/CseModelDescription");

        {
            jmethodID nameId_ = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
            jmethodID uuidId_ = env->GetMethodID(cls, "getUuid", "()Ljava/lang/String;");
            jmethodID authorId_ = env->GetMethodID(cls, "getAuthor", "()Ljava/lang/String;");
            jmethodID versionId_ = env->GetMethodID(cls, "getVersion", "()Ljava/lang/String;");
            jmethodID descriptionId_ = env->GetMethodID(cls, "getDescription", "()Ljava/lang/String;");

            md.name = invoke_string_getter(env, jmd, nameId_);
            md.uuid = invoke_string_getter(env, jmd, uuidId_);
            md.author = invoke_string_getter(env, jmd, authorId_);
            md.version = invoke_string_getter(env, jmd, versionId_);
            md.description = invoke_string_getter(env, jmd, descriptionId_);
        }

        {

            jclass listCls = env->FindClass("java/util/List");
            jmethodID variablesId_ = env->GetMethodID(cls, "getVariables", "()Ljava/util/List;");

            jobject jvars = env->CallObjectMethod(jmd, variablesId_);

            jmethodID sizeId = env->GetMethodID(listCls, "size", "()I");
            jmethodID getId = env->GetMethodID(listCls, "get", "(I)Ljava/lang/Object;");

            jclass varCls = env->FindClass("org/osp/cse/CseVariableDescription");
            jmethodID nameId_ = env->GetMethodID(varCls, "getName", "()Ljava/lang/String;");
            jmethodID referenceId_ = env->GetMethodID(varCls, "getValueReference", "()J");
            jmethodID typeId = env->GetMethodID(varCls, "getTypeId", "()I");
            jmethodID causalityId = env->GetMethodID(varCls, "getCausalityId", "()I");
            jmethodID variabilityId = env->GetMethodID(varCls, "getVariabilityId", "()I");

            const auto size = env->CallIntMethod(jvars, sizeId);
            for (int i = 0; i < size; i++) {
                jobject jvar = env->CallObjectMethod(jvars, getId, i);

                variable_description vd;
                vd.name = invoke_string_getter(env, jvar, nameId_);
                vd.reference = static_cast<value_reference>(env->CallLongMethod(jvar, referenceId_));
                vd.type = static_cast<variable_type>(env->CallIntMethod(jvar, typeId));
                vd.causality = static_cast<variable_causality>(env->CallIntMethod(jvar, causalityId));
                vd.variability = static_cast<variable_variability>(env->CallIntMethod(jvar, variabilityId));

                md.variables.push_back(vd);
            }
        }
    });

    return md;
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

void jvm_slave::end_simulation()
{
    worker_.work([this]() {
        jvm_invoke(jvm_, [this](JNIEnv* env) {
            env->CallVoidMethod(slave_, terminateId_);
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
void jvm_slave::get_real_variables(gsl::span<const value_reference> variables, gsl::span<double> values) const
{
}

void jvm_slave::get_integer_variables(gsl::span<const value_reference> variables, gsl::span<int> values) const
{
}

void jvm_slave::get_boolean_variables(gsl::span<const value_reference> variables, gsl::span<bool> values) const
{
}

void jvm_slave::get_string_variables(gsl::span<const value_reference> variables, gsl::span<std::string> values) const
{
}

void jvm_slave::set_real_variables(gsl::span<const value_reference> variables, gsl::span<const double> values)
{
}

void jvm_slave::set_integer_variables(gsl::span<const value_reference> variables, gsl::span<const int> values)
{
}

void jvm_slave::set_boolean_variables(gsl::span<const value_reference> variables, gsl::span<const bool> values)
{
}

void jvm_slave::set_string_variables(gsl::span<const value_reference> variables, gsl::span<const std::string> values)
{
}

jvm_slave::~jvm_slave()
{
    jvm_invoke(jvm_, [this](JNIEnv* env) {
        env->DeleteGlobalRef(slave_);
    });
}

} // namespace cse

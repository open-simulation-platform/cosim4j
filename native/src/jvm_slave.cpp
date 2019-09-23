
#include <cse/jvm_helper.hpp>
#include <cse/jvm_slave.hpp>

namespace cse
{

jvm_slave::jvm_slave(JNIEnv* env, jobject slave)
    : model_description_(nullptr)
{
    env->GetJavaVM(&jvm_);
    slave_ = env->NewGlobalRef(slave);

    jclass cls = env->FindClass("org/osp/cse/CseJvmSlave");

    doStepId_ = env->GetMethodID(cls, "doStep", "(DD)Z");
}

cse::model_description jvm_slave::model_description() const
{
    cse::model_description md;

    return md;
}

void jvm_slave::setup(cse::time_point startTime, std::optional<time_point> /*stopTime*/, std::optional<double> /*relativeTolerance*/)
{
    double startTimeSec = to_double_time_point(startTime);
}

void jvm_slave::start_simulation()
{
}

void jvm_slave::end_simulation()
{
}

cse::step_result jvm_slave::do_step(cse::time_point currentT, cse::duration deltaT)
{
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

} // namespace cse

cse::jvm_slave::~jvm_slave()
{
    jvm_invoke(jvm_, [this](JNIEnv* env) {
        env->DeleteGlobalRef(slave_);
    });
}

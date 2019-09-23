
#ifndef CSECOREJNI_JVM_SLAVE_HPP
#define CSECOREJNI_JVM_SLAVE_HPP

#include <cse/model.hpp>
#include <cse/slave.hpp>
#include <cse/thread_worker.hpp>

#include <jni.h>

namespace cse
{

class jvm_slave : public cse::slave
{

public:
    jvm_slave(JNIEnv* env, jobject slave);

    [[nodiscard]] cse::model_description model_description() const override;
    void setup(cse::time_point startTime, std::optional<time_point> stopTime, std::optional<double> relativeTolerance) override;
    void start_simulation() override;
    void end_simulation() override;
    cse::step_result do_step(cse::time_point currentT, cse::duration deltaT) override;
    void get_real_variables(gsl::span<const value_reference> variables, gsl::span<double> values) const override;
    void get_integer_variables(gsl::span<const value_reference> variables, gsl::span<int> values) const override;
    void get_boolean_variables(gsl::span<const value_reference> variables, gsl::span<bool> values) const override;
    void get_string_variables(gsl::span<const value_reference> variables, gsl::span<std::string> values) const override;
    void set_real_variables(gsl::span<const value_reference> variables, gsl::span<const double> values) override;
    void set_integer_variables(gsl::span<const value_reference> variables, gsl::span<const int> values) override;
    void set_boolean_variables(gsl::span<const value_reference> variables, gsl::span<const bool> values) override;
    void set_string_variables(gsl::span<const value_reference> variables, gsl::span<const std::string> values) override;

    ~jvm_slave() override;

private:
    JavaVM* jvm_;
    jobject slave_;

    jmethodID setupId_;
    jmethodID onBeforeStartId_;
    jmethodID doStepId_;
    jmethodID terminateId_;

    jmethodID getRealId_;
    jmethodID setRealId_;
    
    jmethodID getIntegerId_;
    jmethodID setIntegerId_;

    jmethodID getBooleanId_;
    jmethodID setBooleanId_;

    jmethodID getStringId_;
    jmethodID setStringId_;

    thread_worker worker_;
    cse::model_description model_description_;

};

} // namespace cse

#endif

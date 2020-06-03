
#include <cosim.h>
#include <cosim/cosim_observer_s.hpp>
#include <cosim/step_event_listener.hpp>

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_org_osp_cosim_jni_CosimLibrary_setupSimpleConsoleLogging(JNIEnv* env, jobject)
{
    return cosim_log_setup_simple_console_logging() == 0;
}

JNIEXPORT void JNICALL Java_org_osp_cosim_jni_CosimLibrary_setLogLevel(JNIEnv* env, jobject, jint level)
{
    cosim_log_set_output_level(static_cast<cosim_log_severity_level>(level));
}


JNIEXPORT jlong JNICALL Java_org_osp_cosim_jni_CosimLibrary_createStepEventListener(JNIEnv* env, jobject, jobject listener)
{
    auto observer = std::make_unique<cosim_observer>();
    observer->cpp_observer = std::make_unique<cosim::step_event_listener>(env, listener);
    return reinterpret_cast<jlong>(observer.release());
}


#ifdef __cplusplus
}
#endif


#include <cse.h>
#include <cse/cse_observer_s.hpp>
#include <cse/step_event_listener.hpp>

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setupSimpleConsoleLogging(JNIEnv* env, jobject)
{
    return cse_log_setup_simple_console_logging() == 0;
}

JNIEXPORT void JNICALL Java_org_osp_cse_jni_CseLibrary_setLogLevel(JNIEnv* env, jobject, jint level)
{
    cse_log_set_output_level(static_cast<cse_log_severity_level>(level));
}


JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createStepEventListener(JNIEnv* env, jobject, jobject listener)
{
    auto observer = std::make_unique<cse_observer>();
    observer->cpp_observer = std::make_unique<cse::step_event_listener>(env, listener);
    return reinterpret_cast<jlong>(observer.release());
}


#ifdef __cplusplus
}
#endif

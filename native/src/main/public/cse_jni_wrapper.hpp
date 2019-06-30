#ifndef CSE_JNI_JNIWRAPPER_HPP
#define CSE_JNI_JNIWRAPPER_HPP

#include <jni.h>

#include "cse.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorCode(JNIEnv *env, jobject obj);

JNIEXPORT jstring JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorMessage(JNIEnv *env, jobject obj);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv *env, jobject obj, jdouble startTime, jdouble stepSize);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSspExecution(JNIEnv *env, jobject obj, jstring sspDir, jdouble startTime);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv *env, jobject obj, jlong execution);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSlave(JNIEnv *env, jobject obj, jstring fmuPath);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroySlave(JNIEnv *env, jobject obj, jlong slave);

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_addSlave(JNIEnv *env, jobject obj, jlong execution, jlong slave);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_start(JNIEnv *env, jobject obj, jlong execution);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_step(JNIEnv *env, jobject obj, jlong execution, jlong numSteps);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stop(JNIEnv *env, jobject obj, jlong execution);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_enableRealTimeSimulation(JNIEnv *env, jobject obj, jlong execution);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_disableRealTimeSimulation(JNIEnv *env, jobject obj, jlong execution);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setRealTimeFactorTarget(JNIEnv *env, jobject obj, jlong execution, jdouble realTimeFactor);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStatus(JNIEnv *env, jobject obj, jlong execution, jobject status);

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumVariables(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getVariables(JNIEnv *env, jobject obj, jlong execution, jint slaveIndex, jobjectArray vars);

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumSlaves(JNIEnv *env, jobject obj, jlong execution);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getSlaveInfos(JNIEnv *env, jobject obj, jlong execution, jobjectArray infos);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setReal(JNIEnv *env, jobject obj, jlong manipulator, jint slaveIndex, jlongArray vr, jdoubleArray values);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInteger(JNIEnv *env, jobject obj, jlong manipulator, jint slaveIndex, jlongArray vr, jintArray values);

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setBoolean(JNIEnv *env, jobject obj, jlong manipulator, jint slaveIndex, jlongArray vr, jbooleanArray values);
//
//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setString(JNIEnv *env, jobject obj, jlong manipulator, jint slaveIndex, jlongArray vr, jstringArray values);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_resetSlave(JNIEnv *env, jobject obj, jlong manipulator, jint slaveIndex, jlongArray vr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getReal(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlongArray vr, jdoubleArray ref);

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getRealDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jobject vr, jint nvr, jobject ref);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getInteger(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlongArray vr, jintArray ref);

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jobject vr, jint nvr, jobject ref);

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamples(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples);

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamplesDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples);

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamples(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples);

//JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamplesDirect(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbersForDuration(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jdouble duration, jlongArray steps);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbers(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jdouble begin, jdouble end, jlongArray steps);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectReals(JNIEnv *env, jobject obj, jlong execution, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectIntegers(JNIEnv *env, jobject obj, jlong execution, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createLastValueObserver(JNIEnv *env, jobject obj);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserver(JNIEnv *env, jobject obj, jstring logDir);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserverFromCfg(JNIEnv *env, jobject obj, jstring logDir, jstring cfgPath);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createTimeSeriesObserver(JNIEnv *env, jobject obj);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createBufferedTimeSeriesObserver(JNIEnv *env, jobject obj, jint bufferSize);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_startObserving(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jint variableType, jlong vr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stopObserving(JNIEnv *env, jobject obj, jlong observer, jint slaveIndex, jint variableType, jlong vr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyObserver(JNIEnv *env, jobject obj, jlong observer);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addObserver(JNIEnv *env, jobject obj, jlong execution, jlong observer);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createOverrideManipulator(JNIEnv *env, jobject obj);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addManipulator(JNIEnv *env, jobject obj, jlong execution, jlong manipulator);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyManipulator(JNIEnv *env, jobject obj, jlong manipulator);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createScenarioManager(JNIEnv *env, jobject obj);

#ifdef __cplusplus
}
#endif


#endif

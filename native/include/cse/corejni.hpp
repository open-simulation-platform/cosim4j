#ifndef CORE4j_HPP
#define CORE4j_HPP

#include "cse.h"

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorCode(JNIEnv* env, jobject obj);

JNIEXPORT jstring JNICALL Java_org_osp_cse_jni_CseLibrary_getLastErrorMessage(JNIEnv* env, jobject obj);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createExecution(JNIEnv* env, jobject obj, jdouble startTime, jdouble stepSize);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSspExecution(JNIEnv* env, jobject obj, jstring sspDir, jdouble startTime);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyExecution(JNIEnv* env, jobject obj, jlong executionPtr);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createSlave(JNIEnv* env, jobject obj, jstring fmuPath);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroySlave(JNIEnv* env, jobject obj, jlong slave);

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_addSlave(JNIEnv* env, jobject obj, jlong executionPtr, jlong slave);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_start(JNIEnv* env, jobject obj, jlong executionPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_step(JNIEnv* env, jobject obj, jlong executionPtr, jlong numSteps);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_simulateUntil(JNIEnv* env, jobject obj, jlong executionPtr, jdouble targetTime);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stop(JNIEnv* env, jobject obj, jlong executionPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_enableRealTimeSimulation(JNIEnv* env, jobject obj, jlong executionPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_disableRealTimeSimulation(JNIEnv* env, jobject obj, jlong executionPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setRealTimeFactorTarget(JNIEnv* env, jobject obj, jlong executionPtr, jdouble realTimeFactor);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStatus(JNIEnv* env, jobject obj, jlong executionPtr, jobject status);

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getModelDescription(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex);

JNIEXPORT jint JNICALL Java_org_osp_cse_jni_CseLibrary_getNumSlaves(JNIEnv* env, jobject obj, jlong executionPtr);

JNIEXPORT jobject JNICALL Java_org_osp_cse_jni_CseLibrary_getSlaveInfos(JNIEnv* env, jobject obj, jlong executionPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setReal(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jdoubleArray values);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialRealValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jdouble value);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInteger(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jintArray values);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialIntegerValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jint value);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setBoolean(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jbooleanArray values);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialBooleanValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jboolean value);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setString(JNIEnv* env, jobject obj, jlong manipulatorPtr, jint slaveIndex, jlongArray vr, jobjectArray values);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setInitialStringValue(JNIEnv* env, jobject obj, jlong executionPtr, jint slaveIndex, jlong vr, jstring value);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getReal(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jdoubleArray ref);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getInteger(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jintArray ref);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getBoolean(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jbooleanArray ref);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getString(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlongArray vr, jobjectArray ref);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getRealSamples(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getIntegerSamples(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jlong vr, jlong fromStep, jint nSamples, jobject samples);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbersForDuration(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jdouble duration, jlongArray steps);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_getStepNumbers(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jdouble begin, jdouble end, jlongArray steps);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectReals(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_connectIntegers(JNIEnv* env, jobject obj, jlong executionPtr, jint outputSlaveIndex, jlong outputValueRef, jint inputSlaveIndex, jlong inputValueRef);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createLastValueObserver(JNIEnv* env, jobject obj);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserver(JNIEnv* env, jobject obj, jstring logDir);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createFileObserverFromCfg(JNIEnv* env, jobject obj, jstring logDir, jstring cfgPath);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createTimeSeriesObserver(JNIEnv* env, jobject obj);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createBufferedTimeSeriesObserver(JNIEnv* env, jobject obj, jint bufferSize);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_startObserving(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_stopObserving(JNIEnv* env, jobject obj, jlong observerPtr, jint slaveIndex, jint variableType, jlong vr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyObserver(JNIEnv* env, jobject obj, jlong observerPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addObserver(JNIEnv* env, jobject obj, jlong executionPtr, jlong observerPtr);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createOverrideManipulator(JNIEnv* env, jobject obj);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_addManipulator(JNIEnv* env, jobject obj, jlong executionPtr, jlong manipulatorPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_destroyManipulator(JNIEnv* env, jobject obj, jlong manipulatorPtr);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createScenarioManager(JNIEnv* env, jobject obj);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_loadScenario(JNIEnv* env, jobject obj, jlong executionPtr, jlong manipulatorPtr, jstring scenarioFile);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_isScenarioRunning(JNIEnv* env, jobject obj, jlong manipulatorPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_abortScenario(JNIEnv* env, jobject obj, jlong manipulatorPtr);

JNIEXPORT jboolean JNICALL Java_org_osp_cse_jni_CseLibrary_setupSimpleConsoleLogging(JNIEnv* env, jobject obj);

JNIEXPORT void JNICALL Java_org_osp_cse_jni_CseLibrary_setLogLevel(JNIEnv* env, jobject obj, jint level);

JNIEXPORT jlong JNICALL Java_org_osp_cse_jni_CseLibrary_createStepEventListener(JNIEnv* env, jobject obj, jobject listener);

#ifdef __cplusplus
}
#endif

#endif

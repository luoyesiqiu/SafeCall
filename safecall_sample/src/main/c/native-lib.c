#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <android/log.h>
#include <assert.h>

#include "safe_call.h"

#define TAG "safecall_native"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

const char* getThreadName(){
    static char threadName[256];
    memset(threadName,0,256);
    prctl(PR_GET_NAME, (unsigned long)threadName);
    return threadName;
}

void normal_call1() {
    LOGD("normal_call1 called!");
}

void normal_call2(int *n) {
    (*n)++;
}

int normal_call3(int *n) {
    (*n)++;
    return *n;
}

void problem_call1() {
    abort();
}

int problem_call2(int arg) {
    int *p = NULL;
    *p = arg;
    return arg;
}

void normalCall1(JNIEnv *env,jclass __unused klass) {
    SAFE_CALL_VOID(normal_call1);
}

void *sub_thread1(void* args) {
    const int N = 1 * 10000;
    int n = 0;
    for(int i = 0;i < N;i++) {
        SAFE_CALL_VOID(normal_call2,&n);
    }
    assert(n == N);
    LOGD("Thread [%s] has been safe run!",getThreadName());
}

void *sub_thread2(void* args) {
    const int N = 1 * 10000;
    int n = 0;
    int a = 0;
    for(int i = 0;i < N;i++) {
        SAFE_CALL(normal_call3,a,&n);
    }
    assert(a == N);
    LOGD("Thread [%s] has been safe run!",getThreadName());
}

void normalCall2(JNIEnv *env,jclass __unused klass) {
    LOGD("Thread [normalCall2] start to run");
    pthread_t t;
    pthread_create(&t, NULL, sub_thread1, NULL);
    pthread_setname_np(t, "normalCall2");
}

void normalCall3(JNIEnv *env,jclass __unused klass) {
    LOGD("Thread [normalCall3] start to run");
    pthread_t t;
    pthread_create(&t, NULL, sub_thread2, NULL);
    pthread_setname_np(t, "normalCall3");
}

void problemCall1(JNIEnv *env,jclass __unused klass) {
    SAFE_CALL_VOID(problem_call1);
    LOGD("problem_call1 do not call!");
}

void problemCall2(JNIEnv *env,jclass __unused klass) {
    int ret = 0;
    SAFE_CALL(problem_call2,ret,123);
    LOGD("problem_call2 do not call!");
}

static JNINativeMethod gMethods[] = {
        {"normalCall1", "()V",   (void *) normalCall1},
        {"normalCall2", "()V",   (void *) normalCall2},
        {"normalCall3", "()V",   (void *) normalCall3},
        {"problemCall1", "()V",  (void *) problemCall1},
        {"problemCall2", "()V", (void *) problemCall2},
};

static jboolean registerNativeMethods(JNIEnv *env) {
    jclass JniBridgeClass = (*env)->FindClass(env,"com/luoye/safecaller/JNIHelper");
    if ((*env)->RegisterNatives(env,JniBridgeClass, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) ==
        0) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env = NULL;
    if ((*vm)->GetEnv(vm,(void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_ERR;
    }

    if (registerNativeMethods(env) == JNI_FALSE) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_4;
}
#include <jni.h>
#include <stdlib.h>
#include <android/log.h>

#include "safe_call.h"

#define TAG "safecall_native"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

void normal_call() {
    LOGD("normal_call called!");
}

void problem_call1() {
    abort();
}

int problem_call2(int arg) {
    int *p = NULL;
    *p = arg;
    return arg;
}

void normalCall(JNIEnv *env,jclass __unused klass) {
    SAFE_CALL_VOID(normal_call);
}

void problemCall1(JNIEnv *env,jclass __unused klass) {
    SAFE_CALL_VOID(problem_call1);
    LOGD("problemCall1 do not call!");
}

void problemCall2(JNIEnv *env,jclass __unused klass) {
    int ret = 0;
    SAFE_CALL(problem_call2,ret,123);
    LOGD("problemCall2 do not call!");
}

static JNINativeMethod gMethods[] = {
        {"normalCall", "()V",   (void *) normalCall},
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
//
// Created by luoyesiqiu
//

#ifndef SAFECALLER_SAFE_CALL_H
#define SAFECALLER_SAFE_CALL_H

#include <pthread.h>

#ifdef __cplusplus
extern "C"  {
#endif

JNIEXPORT extern int before_call(const void *func_addr);
JNIEXPORT extern void after_call(const void *func_addr);

pthread_mutex_t safe_call_mutex = PTHREAD_MUTEX_INITIALIZER;

#define SAFE_CALL(func_addr,ret,...) do { \
    pthread_mutex_lock(&safe_call_mutex);             \
    int call = before_call(func_addr); \
    if(call) {                           \
        ret = func_addr(__VA_ARGS__);                 \
    }                                         \
    if(call) { \
        after_call(func_addr);                    \
    }                                     \
	pthread_mutex_unlock(&safe_call_mutex); \
} while(0)

#define SAFE_CALL_VOID(func_addr,...) do { \
    pthread_mutex_lock(&safe_call_mutex);             \
    int call = before_call(func_addr); \
    if(call) {                           \
        func_addr(__VA_ARGS__);                 \
    }                                         \
    if(call) { \
        after_call(func_addr);                    \
    }                                      \
	pthread_mutex_unlock(&safe_call_mutex); \
} while(0)

#ifdef __cplusplus
};
#endif

#endif //SAFECALLER_SAFE_CALL_H

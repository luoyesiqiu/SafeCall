//
// Created by luoyesiqiu
//

#ifndef SAFECALLER_SAFE_CALL_H
#define SAFECALLER_SAFE_CALL_H

#ifdef __cplusplus
extern "C"  {
#endif

JNIEXPORT extern int before_call(const void *func_addr);
JNIEXPORT extern void after_call(const void *func_addr);

#define SAFE_CALL(func_addr,ret,...) do { \
int call = before_call(func_addr); \
    if(call) {                           \
        ret = func_addr(__VA_ARGS__);                 \
    }                                         \
    if(call) { \
        after_call(func_addr);                    \
    }                                          \
} while(0)

#define SAFE_CALL_VOID(func_addr,...) do { \
    int call = before_call(func_addr); \
    if(call) {                           \
        func_addr(__VA_ARGS__);                 \
    }                                         \
    if(call) { \
        after_call(func_addr);                    \
    }                                          \
} while(0)

#ifdef __cplusplus
};
#endif

#endif //SAFECALLER_SAFE_CALL_H

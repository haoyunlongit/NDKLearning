//
// Created by stevenhao on 3/28/24.
//

#include <shadowhook.h>
#include <jni.h>
#include <android/log.h>
#include <bits/pthread_types.h>
#include <pthread.h>
#include "shadowhook_wrapper.h"

static void LOGE(const char *string, int i);
static void pthread_exit_proxy(void* _return_value);

///实现声明的函数
void hook_thread() {
    int ret = shadowhook_init(SHADOWHOOK_MODE_SHARED, true);
    LOGE("shadowhook_init ret: %d", ret);
    shadowhook_hook_sym_name("libc.so", "pthread_exit", (void *) pthread_exit_proxy, nullptr);
}

void start_thread_dosomething(JNIEnv *env) {
    pthread_t thread;
    pthread_create(&thread, nullptr, [](void *) -> void * {
        LOGE("shadowhook_init start_thread_dosomething", 0);
        return nullptr;
    }, nullptr);
    pthread_detach(thread);
}

static void pthread_exit_proxy(void* _return_value) {
    SHADOWHOOK_STACK_SCOPE();
    ///输出一行日志
    LOGE("shadowhook_init : hook success", 0);
    SHADOWHOOK_CALL_PREV(pthread_exit_proxy, _return_value);
}

static void LOGE(const char *string, int i) {
    __android_log_print(ANDROID_LOG_ERROR, "AqtsQaTag", "%s", string);
}

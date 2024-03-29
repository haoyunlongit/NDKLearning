//
// Created by stevenhao on 3/28/24.
//

#include "inlinehook_wrapper.h"
#include <jni.h>
#include <bytehook.h>
#include <bits/pthread_types.h>
#include <android/log.h>

int
my_pthread_create(pthread_t *thread_out, pthread_attr_t const *attr, void *(*start_routine)(void *),
                  void *arg);
static void LOGE(const char *string, int i);

void hook_thread2() {
    //hook pthread_exit lib.so
    bytehook_hook_all(nullptr, "pthread_exit", (void *) my_pthread_create, nullptr, nullptr);
}

int
my_pthread_create(pthread_t *thread_out, pthread_attr_t const *attr, void *(*start_routine)(void *),
                  void *arg) {
    BYTEHOOK_STACK_SCOPE();
    LOGE("inline hook : hook success", 0);
    int result = BYTEHOOK_CALL_PREV(my_pthread_create, thread_out, attr, start_routine, arg);
    return result;
}

static void LOGE(const char *string, int i) {
    __android_log_print(ANDROID_LOG_ERROR, "AqtsQaTag", "%s", string);
}
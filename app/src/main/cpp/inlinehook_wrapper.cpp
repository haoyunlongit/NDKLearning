//
// Created by stevenhao on 3/28/24.
//

#include "inlinehook_wrapper.h"
#include <jni.h>
#include <bytehook.h>
#include <bits/pthread_types.h>
#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <byopen/byopen.h>
#include <link.h>

int
my_pthread_create(pthread_t *thread_out, pthread_attr_t const *attr, void *(*start_routine)(void *),
                  void *arg);
static void LOGE(const char *tag, const char *format, ...);

typedef void (*SetJdwpAllowedFunc)(bool);
typedef bool (*IsJdwpAllowed)();

void set_gJdwpAllowed(int allowed);

int get_gJdwpAllowed();

static const char* TAG = "stevenhao";

void hook_thread2() {
    //hook pthread_exit lib.so
    bytehook_hook_all(nullptr, "pthread_exit", (void *) my_pthread_create, nullptr, nullptr);
}

void show_message() {
//    int gDwp = get_gJdwpAllowed();
    int t = get_gJdwpAllowed();
    LOGE("stevenhao", "gJdwpAllowed: %i\n", t);
    set_gJdwpAllowed(0);
    t = get_gJdwpAllowed();
    LOGE("stevenhao", "change gJdwpAllowed: %i\n", t);
    set_gJdwpAllowed(1);
}

// Function to get the offset of gJdwpAllowed symbol
int get_gJdwpAllowed() {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            LOGE(TAG, "Failed to open libart.so: %s", error);
        } else {
            LOGE(TAG, "Failed to open libart.so");
        }
    }
    LOGE(TAG, "handle point %p", handle);

    IsJdwpAllowed funcPtr = reinterpret_cast<IsJdwpAllowed>(by_dlsym(handle, "_ZN3art3Dbg13IsJdwpAllowedEv"));
    if (funcPtr == NULL) {
        LOGE(TAG,"not found IsJdwpAllowed symbol");
        by_dlclose(handle);
        return 0;
    }
    LOGE(TAG,"found IsJdwpAllowed symbol");
    return funcPtr();
}

void set_gJdwpAllowed(int allowed) {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            LOGE(TAG, "Failed to open libart.so: %s", error);
        } else {
            LOGE(TAG, "Failed to open libart.so");
        }
    }
    LOGE(TAG, "handle point %p", handle);

    SetJdwpAllowedFunc funcPtr = reinterpret_cast<SetJdwpAllowedFunc>(by_dlsym(handle, "_ZN3art3Dbg14SetJdwpAllowedEb"));
    if (funcPtr == NULL) {
        LOGE(TAG,"not found SetJdwpAllowed symbol");
        by_dlclose(handle);
        return;
    }
    LOGE(TAG,"found SetJdwpAllowed symbol");
    funcPtr(allowed);
    by_dlclose(handle);
}


int
my_pthread_create(pthread_t *thread_out, pthread_attr_t const *attr, void *(*start_routine)(void *),
                  void *arg) {
    BYTEHOOK_STACK_SCOPE();
    LOGE(TAG,"inline hook : hook success");
    int result = BYTEHOOK_CALL_PREV(my_pthread_create, thread_out, attr, start_routine, arg);
    return result;
}

static void LOGE(const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    __android_log_vprint(ANDROID_LOG_ERROR, tag, format, args);
    va_end(args);
}

//
// Created by stevenhao on 4/17/24.
//
// Function to get the offset of gJdwpAllowed symbol
#include <jni.h>
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
#include "utils/log.h"
#include "utils/aqts_library_loader.h"

typedef void (*SetJdwpAllowedFunc)(bool);
typedef bool (*IsJdwpAllowed)();

static const char* TAG = "stevenhao";

int get_gJdwpAllowed() {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            Logger::info(TAG, "Failed to open libart.so: %s", error);
        } else {
            Logger::info(TAG, "Failed to open libart.so");
        }
    }
    Logger::info(TAG, "handle point %p", handle);

    IsJdwpAllowed funcPtr = reinterpret_cast<IsJdwpAllowed>(by_dlsym(handle, "_ZN3art3Dbg13IsJdwpAllowedEv"));
    if (funcPtr == NULL) {
        Logger::info(TAG,"not found IsJdwpAllowed symbol");
        by_dlclose(handle);
        return 0;
    }
    Logger::info(TAG,"found IsJdwpAllowed symbol");
    return funcPtr();
}

void set_gJdwpAllowed(int allowed) {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            Logger::info(TAG, "Failed to open libart.so: %s", error);
        } else {
            Logger::info(TAG, "Failed to open libart.so");
        }
    }
    Logger::info(TAG, "handle point %p", handle);

    SetJdwpAllowedFunc funcPtr = reinterpret_cast<SetJdwpAllowedFunc>(by_dlsym(handle, "_ZN3art3Dbg14SetJdwpAllowedEb"));
    if (funcPtr == NULL) {
        Logger::info(TAG,"not found SetJdwpAllowed symbol");
        by_dlclose(handle);
        return;
    }
    Logger::info(TAG,"found SetJdwpAllowed symbol");
    funcPtr(allowed);
    by_dlclose(handle);
}

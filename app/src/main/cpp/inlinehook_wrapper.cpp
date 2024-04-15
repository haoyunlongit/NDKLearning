//
// Created by stevenhao on 3/28/24.
//

#include "inlinehook_wrapper.h"
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

void set_gJdwpAllowed(int allowed);

int get_gJdwpAllowed();

static const char* TAG = "stevenhao";

void hook_thread2() {
}

void show_message() {
    int gDwp = get_gJdwpAllowed();
    int t = get_gJdwpAllowed();
    Logger::debug("stevenhao", "gJdwpAllowed: %i\n", t);
    set_gJdwpAllowed(0);
    t = get_gJdwpAllowed();
    Logger::debug("stevenhao", "change gJdwpAllowed: %i\n", t);
    set_gJdwpAllowed(1);
}

// Function to get the offset of gJdwpAllowed symbol
int get_gJdwpAllowed() {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            Logger::debug(TAG, "Failed to open libart.so: %s", error);
        } else {
            Logger::debug(TAG, "Failed to open libart.so");
        }
    }
    Logger::debug(TAG, "handle point %p", handle);

    IsJdwpAllowed funcPtr = reinterpret_cast<IsJdwpAllowed>(by_dlsym(handle, "_ZN3art3Dbg13IsJdwpAllowedEv"));
    if (funcPtr == NULL) {
        Logger::debug(TAG,"not found IsJdwpAllowed symbol");
        by_dlclose(handle);
        return 0;
    }
    Logger::debug(TAG,"found IsJdwpAllowed symbol");
    return funcPtr();
}

void set_gJdwpAllowed(int allowed) {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            Logger::debug(TAG, "Failed to open libart.so: %s", error);
        } else {
            Logger::debug(TAG, "Failed to open libart.so");
        }
    }
    Logger::debug(TAG, "handle point %p", handle);

    SetJdwpAllowedFunc funcPtr = reinterpret_cast<SetJdwpAllowedFunc>(by_dlsym(handle, "_ZN3art3Dbg14SetJdwpAllowedEb"));
    if (funcPtr == NULL) {
        Logger::debug(TAG,"not found SetJdwpAllowed symbol");
        by_dlclose(handle);
        return;
    }
    Logger::debug(TAG,"found SetJdwpAllowed symbol");
    funcPtr(allowed);
    by_dlclose(handle);
}

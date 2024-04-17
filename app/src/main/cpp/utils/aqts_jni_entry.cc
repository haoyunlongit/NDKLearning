//
// Created by liwei41 on 2017/9/12.
//
#include "aqts_library_loader.h"
#include "log.h"

// This is called by the VM when the shared library is first loaded.
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    Logger::info("stevenhao", "JNI_OnLoad success");
    return aqts::AqtsOnLoad(vm, reserved);
}

extern "C" void JNI_OnUnLoad(JavaVM* vm, void* reserved) {
    aqts::AqtsOnUnLoad(vm, reserved);
}

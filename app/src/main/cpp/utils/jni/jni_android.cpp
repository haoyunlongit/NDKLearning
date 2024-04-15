//
// Created by stevenhao on 4/12/24.
//

#include "jni_android.h"
#include "log.h"

namespace {
    JavaVM *g_jvm = NULL;
}

namespace basejni {
    void InitVM(JavaVM *vm) {
        g_jvm = vm;
    }

    JavaVM *GetVM(){
        return g_jvm;
    }

    JNIEnv *AttachCurrentThread() {
        if(!g_jvm) {
            Logger::debug("steven", "AttachCurrentThread failed! becasue:g_jvm == NULL");
            return NULL;
        }
        JNIEnv *env = NULL;
        jint ret = g_jvm->AttachCurrentThread(&env, NULL);
        if (ret < 0) {
            Logger::debug("steven", "AttachCurrentThread failed! becasue: g_jvm->AttachCurrentThread failed");
            return NULL;
        }
        return env;
    }

    jint GetEnv(void** env) {
        if(!g_jvm) {
            Logger::debug("stevenhao", "GetEnv failed! becasue:g_jvm == NULL");
            return JNI_ERR;
        }
        return g_jvm->GetEnv(env,JNI_VERSION_1_2);
    }

    void DetachCurrentThread() {
        // Ignore the return value, if the thread is not attached, DetachCurrentThread
        // will fail. But it is ok as the native thread may never be attached.
        if (g_jvm)
            g_jvm->DetachCurrentThread();
    }
}
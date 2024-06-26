//
// Created by stevenhao on 4/12/24.
//

#include "jni_android.h"
#include "log.h"

namespace {
    JavaVM *g_jvm = NULL;
    jvmtiEnv *g_jvmti = NULL;
}

namespace basejni {
    void InitVM(JavaVM *vm) {
        g_jvm = vm;
    }

    void setJVMTIEnv(jvmtiEnv *env) {
        g_jvmti = env;
    }

    jvmtiEnv *getJVMTIEnv() {
        return g_jvmti;
    }

    JavaVM *GetVM(){
        return g_jvm;
    }

    JNIEnv *AttachCurrentThread() {
        if(!g_jvm) {
            Logger::info("steven", "AttachCurrentThread failed! becasue:g_jvm == NULL");
            return NULL;
        }
        JNIEnv *env = NULL;
        jint ret = g_jvm->AttachCurrentThread(&env, NULL);
        if (ret < 0) {
            Logger::info("steven", "AttachCurrentThread failed! becasue: g_jvm->AttachCurrentThread failed");
            return NULL;
        }
        return env;
    }

    jint GetEnv(void** env) {
        if(!g_jvm) {
            Logger::info("stevenhao", "GetEnv failed! becasue:g_jvm == NULL");
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
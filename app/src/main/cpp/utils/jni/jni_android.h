//
// Created by stevenhao on 4/12/24.
//

#ifndef NDKLEARNING_JNI_ANDROID_H
#define NDKLEARNING_JNI_ANDROID_H
#include <jni.h>
#include "jvmti.h"

namespace basejni {
    void InitVM(JavaVM *vm);

    JavaVM *GetVM();

    void setJVMTIEnv(jvmtiEnv *env);

    jvmtiEnv *getJVMTIEnv();

    JNIEnv *AttachCurrentThread();

    void DetachCurrentThread();

    jint GetEnv(void** env);
}


#endif //NDKLEARNING_JNI_ANDROID_H

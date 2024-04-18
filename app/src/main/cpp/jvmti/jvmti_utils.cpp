//
// Created by stevenhao on 4/18/24.
//

#include <jni.h>
#include "jvmti_log.h"


jstring get_class_name(JNIEnv* env, jobject thiz) {
    // 获取 temp 对应的 jclass
    jclass tempClass = env->GetObjectClass(thiz);
    if (tempClass == NULL) {
        JVMTI_Logger::info("stevenhao", "class null");
        return nullptr;
    }

// 获取 getClass 方法
    const char *methodName = "getClass";
    jmethodID getClassMethodId = env->GetMethodID(tempClass, methodName, "()Ljava/lang/Class;");
    if (getClassMethodId == NULL) {
        JVMTI_Logger::info("stevenhao", "getClass method null");
        return nullptr;
    }

// 调用 getClass 方法获取类对象
    jobject classObject = env->CallObjectMethod(thiz, getClassMethodId);
    if (classObject == NULL) {
        JVMTI_Logger::info("stevenhao", "class object null");
        return nullptr;
    }

// 获取类名
    const char *methodName2 = "getName";
    jmethodID getNameMethodId = env->GetMethodID(env->GetObjectClass(classObject), methodName2, "()Ljava/lang/String;");
    if (getNameMethodId == NULL) {
        JVMTI_Logger::info("stevenhao", "getName method null");
        return nullptr;
    }

// 调用 getName 方法获取类名
    jstring className = (jstring)env->CallObjectMethod(classObject, getNameMethodId);
    return className;
}

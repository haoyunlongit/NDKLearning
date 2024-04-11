#include <jni.h>
#include <string>
#include "string_utils.h"
#include "inlinehook_wrapper.h"

extern "C" jstring
Java_com_stevenhao_ndklearning_MainActivity_hookPThread(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("hook pthread_exit success");
}

extern "C" jstring
Java_com_stevenhao_ndklearning_MainActivity_startJNIThread(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("start thread success");
}

extern "C" jstring
Java_com_stevenhao_ndklearning_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = string_utils::get_hello_string(0);
    return env->NewStringUTF(hello.c_str());
}

extern "C" jstring
Java_com_stevenhao_ndklearning_MainActivity_showGJdwpAllowed(JNIEnv *env, jobject thiz) {
    show_message();
    return env->NewStringUTF("hook pthread_exit success");
}
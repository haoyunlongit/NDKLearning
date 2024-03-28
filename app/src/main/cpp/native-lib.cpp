#include <jni.h>
#include <string>
#include <shadowhook.h>
#include "string_utils.h"
#include "shadowhook_wrapper.h"


extern "C"
JNIEXPORT jstring JNICALL
Java_com_stevenhao_ndklearning_MainActivity_hookPThread(JNIEnv *env, jobject thiz) {
    hook_thread();
    return env->NewStringUTF("hook pthread_exit success");
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_stevenhao_ndklearning_MainActivity_startJNIThread(JNIEnv *env, jobject thiz) {
    start_thread_dosomething(env);
    return env->NewStringUTF("start thread success");
}
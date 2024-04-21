#include <jni.h>
#include <string>
#include "string_utils.h"
#include "log.h"
#include "jni_android.h"
//#include "jvmti/jvmti_helper.h"
#include <unordered_map>
#include <thread>

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
    return env->NewStringUTF("hook pthread_exit success");
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_stevenhao_ndklearning_JVMHelper_getGJdwpAllowed(JNIEnv *env, jclass clazz) {
    return 1;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_stevenhao_ndklearning_JVMHelper_setGJdwpAllowed(JNIEnv *env, jclass clazz,
                                                         jboolean allowed) {

}

extern "C"
JNIEXPORT void JNICALL
Java_com_stevenhao_ndklearning_MainActivity_scanMemory(JNIEnv *env, jobject thiz) {


}
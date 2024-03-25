#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_stevenhao_ndklearning_MainActivity_getStringFromJni(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
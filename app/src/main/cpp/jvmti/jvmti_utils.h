//
// Created by stevenhao on 4/18/24.
//

#ifndef NDKLEARNING_JVMTI_UTILS_H
#define NDKLEARNING_JVMTI_UTILS_H

#include <jni.h>

jstring get_class_name(JNIEnv* env, jobject thiz);

#endif //NDKLEARNING_JVMTI_UTILS_H

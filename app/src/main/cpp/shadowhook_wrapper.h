//
// Created by stevenhao on 3/28/24.
//

#ifndef NDKLEARNING_SHADOWHOOK_WRAPPER_H
#define NDKLEARNING_SHADOWHOOK_WRAPPER_H
///声明一个hook函数
void hook_thread();

void start_thread_dosomething(JNIEnv *env);

#endif //NDKLEARNING_SHADOWHOOK_WRAPPER_H

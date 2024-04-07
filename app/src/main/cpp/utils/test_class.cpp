//
// Created by stevenhao on 4/7/24.
//

#include "test_class.h"
#include <android/log.h>
#include "log.h"
#include "test2_class.h"

void hook_thread2_test() {
    LOGD("hook_thread2_test");
    int result = test2(1, 2);
    LOGE("hook_thread2_test result: %d", result);
}
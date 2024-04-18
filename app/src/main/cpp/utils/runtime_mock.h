//
// Created by stevenhao on 4/15/24.
//

#include "MockCompatFramework.h"
#include <stdint.h>
#include <string>
#include <vector>
#include <list>

#ifndef NDKLEARNING_RUNTIME_MOCK_H
#define NDKLEARNING_RUNTIME_MOCK_H


class Heap;

class mock_runtime {

public:
    uint32_t target_sdk_version_;
    MockCompatFramework compat_framework_;
    bool implicit_null_checks_;       // 是否启用隐式指针检查，此处取值为true
    bool implicit_so_checks_;         // 是否启用隐式堆栈溢出检查，此处取值为true.
    bool implicit_suspend_checks_;    // 是否启用隐式线程暂停检查，此处取值为false
    bool no_sig_chain_;
    bool force_native_bridge_;
    bool is_native_bridge_loaded_;
    bool is_native_debuggable_;
    bool async_exceptions_thrown_;
    bool non_standard_exits_enabled_;
    bool is_java_debuggable_;
    bool monitor_timeout_enable_;
    uint64_t monitor_timeout_ns_;
    bool is_profileable_from_shell_ = false;
    bool is_profileable_ = false;
    uint32_t zygote_max_failed_boots_;
};
#endif //NDKLEARNING_RUNTIME_MOCK_H

//
// Created by liwei41 on 2017/9/12.
//


#include "log.h"
#include "aqts_library_loader.h"
#include "jni_android.h"
#include "linker/art_helper.h"
#include "test2_class.h"
#include "runtime_mock.h"
#include "prefix.h"
#include "byopen.h"
#include <string>
#include <iostream>
#include <dlfcn.h>

namespace aqts {

    static const char* sym_runtime_instance = "_ZN3art7Runtime9instance_E";

    template<typename T>
    static int find_offset(void* _base, size_t _size, T _target);
    static void print_data(void* _base, size_t _size);
    void* getRuntime();

    jint AqtsOnLoad(JavaVM *vm, void *reserved) {
        basejni::InitVM(vm);

        auto vm_ext = reinterpret_cast<JavaVMExt*>(vm);
        void * runtime_instance_ = vm_ext->runtime;
        void * runtime_instance_2 = getRuntime();
        Logger::info("stevenhao", "runtime_instance_ %p", runtime_instance_);

        /// 获取target_sdk_version
        int api_level = android_get_device_api_level();

        uint32_t apiLevel = (uint32_t)api_level;
        int offset = find_offset(runtime_instance_, 2048, apiLevel);
        Logger::info("stevenhao", "vm_ext %d", offset);

        if (offset > 0) {
//            mock_runtime * tartget = reinterpret_cast<mock_runtime*>(reinterpret_cast<char*>(runtime_instance_) + offset);
//
////
////            print_data(tartget, 2000);
//
//            MockCompatFramework& compat_framework_ = tartget->compat_framework_;
//            MockMutex& mutex1 = compat_framework_.reported_compat_changes_lock_;
//            Logger::info("stevenhao", "monitor_timeout_ns_ %d", tartget->monitor_timeout_ns_);
//            Logger::info("stevenhao", "implicit_null_checks_ %d", tartget->implicit_null_checks_);
//            Logger::info("stevenhao", "implicit_so_checks_ %d", tartget->implicit_so_checks_);
//            Logger::info("stevenhao", "implicit_suspend_checks_ %d", tartget->implicit_suspend_checks_);
//            Logger::info("stevenhao", "no_sig_chain_ %d", tartget->no_sig_chain_);
//            Logger::info("stevenhao", "force_native_bridge_ %d", tartget->force_native_bridge_);
//            Logger::info("stevenhao", "is_native_bridge_loaded_ %d", tartget->is_native_bridge_loaded_);
//            Logger::info("stevenhao", "is_native_debuggable_ %d", tartget->is_native_debuggable_);
//            Logger::info("stevenhao", "async_exceptions_thrown_ %d", tartget->async_exceptions_thrown_);
//            Logger::info("stevenhao", "non_standard_exits_enabled_ %d", tartget->non_standard_exits_enabled_);
//            Logger::info("stevenhao", "is_java_debuggable_ %d", tartget->is_java_debuggable_);

        }
        return JNI_VERSION_1_6;
    }

    void* getRuntime() {
        by_char_t const* libraryName_cstr = "libart.so";
        by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
        if (handle == NULL) {
            const char* error = dlerror();
            if (error != NULL) {
                Logger::info("stevenhao", "Failed to open libart.so: %s", error);
            } else {
                Logger::info("stevenhao", "Failed to open libart.so");
            }
        }
        Logger::info("stevenhao", "handle point %p", handle);
        void** runtime_obj = reinterpret_cast<void**>(by_dlsym(handle,
                                                               sym_runtime_instance));
        return *runtime_obj;
    }

    void AqtsOnUnLoad(JavaVM *jvm, void *reserved) {
        //base::android::LibraryLoaderExitHook();
    }

    template<typename T>
    static int find_offset(void* _base, size_t _size, T _target) {
        for(auto i = 0; i < _size; i += sizeof(_target)) {
            auto current = reinterpret_cast<T*>(reinterpret_cast<char*>(_base) + i);
            if(*current == _target){
                return i;
            }
        }
        return -1;
    }

    /// 打印连续地址的二进制数据 拼接位char* 打印以16进制打印
    static void print_data(void* _base, size_t _size) {
        for(auto i = 0; i < _size; i += 16) {
            auto current = reinterpret_cast<char*>(_base) + i;
            char buffer[64] = {0};
            for(auto j = 0; j < 16; j++) {
                if(i + j < _size) {
                    sprintf(buffer + j * 3, "%02x ", current[j]);
                } else {
                    sprintf(buffer + j * 3, "   ");
                }
            }
            Logger::info("stevenhao", "%p: %s", current, buffer);
        }
    }



}//namespace aqts
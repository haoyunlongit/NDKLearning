//
// Created by stevenhao on 4/17/24.
//
// Function to get the offset of gJdwpAllowed symbol
#include <jni.h>
#include <bits/pthread_types.h>
#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include "byopen/byopen.h"
#include <link.h>
#include "jvmti_log.h"
#include "jvmti.h"
#include <unordered_map>
#include "mem_object_info.h"
#include "jvmti_helper.h"
#include "jvmti_utils.h"

typedef void (*SetJdwpAllowedFunc)(bool);
typedef bool (*IsJdwpAllowed)();

static const char* TAG = "stevenhao";

namespace {
    jvmtiEnv *g_jvmti = NULL;
    static std::unordered_map<jlong, ObjectInfo> objectMap;
}

jvmtiEnv* CreateJvmtiEnv(JavaVM *vm);
void SetAllCapabilities(jvmtiEnv *jvmti);
void SetEventNotification(jvmtiEnv *jvmti, jvmtiEventMode mode,
                          jvmtiEvent event_type);
void ObjectFree(jvmtiEnv *jvmti_env, jlong tag);
void addObjectInfo(jthread thread, jobject object, jclass klass, jlong size, jlong tag);

int index = 0;
void ObjectAllocCallback(jvmtiEnv *jvmti, JNIEnv *jni,
                         jthread thread, jobject object,
                         jclass klass, jlong size) {

    if (size > 100) {
        // 获取 temp 对应的 jclass
        jstring className = get_class_name(jni, object);
        if (className == nullptr) {
            JVMTI_Logger::info("stevenhao", "class null");
            return;
        }
        char* class_name = (char*)jni->GetStringUTFChars(className, nullptr);
        JVMTI_Logger::info("stevenhao", "class name %s", class_name);
    }

    char *classSignature;
    jvmti->GetClassSignature(klass, &classSignature, nullptr);
    if (strcmp(classSignature, "Lcom/stevenhao/ndklearning/MyString;") == 0) {
        // 这是一个大的MyString对象，打印信息
        JVMTI_Logger::info("stevenhao", "Large MyString object allocated with size: %lld", size);

        jvmti->SetTag(object, index);
        index++;
        addObjectInfo(thread, object, klass, size, index);
    }

    jvmti->Deallocate((unsigned char *) classSignature);
}

void GCStartCallback(jvmtiEnv *jvmti) {
    JVMTI_Logger::info("stevenhao","==========触发 GCStart=======");

}

void GCFinishCallback(jvmtiEnv *jvmti) {
    JVMTI_Logger::info("stevenhao","==========触发 GCFinish=======");
}

jint referenceCallback
        (jvmtiHeapReferenceKind reference_kind,
         const jvmtiHeapReferenceInfo* reference_info,
         jlong class_tag, jlong referrer_class_tag,
         jlong size, jlong* tag_ptr,
         jlong* referrer_tag_ptr,
         jint length, void* user_data) {
    return JVMTI_VISIT_OBJECTS;
}

extern "C" JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM *vm, char *options,
                                                 void *reserved) {
    g_jvmti = CreateJvmtiEnv(vm);

    if (g_jvmti == nullptr) {
        JVMTI_Logger::info("stevenhao", "==========CreateJvmtiEnv fail=======");
        return JNI_ERR;
    } else {
        JVMTI_Logger::info("stevenhao", "==========CreateJvmtiEnv success=======");
    }
    SetAllCapabilities(g_jvmti);
    jvmtiEventCallbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));

    callbacks.VMObjectAlloc = &ObjectAllocCallback;
    callbacks.ObjectFree = &ObjectFree;

//    callbacks.NativeMethodBind = &JvmTINativeMethodBind;
//
    callbacks.GarbageCollectionStart = &GCStartCallback;
    callbacks.GarbageCollectionFinish = &GCFinishCallback;
    int error = g_jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
    JVMTI_Logger::info("stevenhao", "register error %d", error);
    SetEventNotification(g_jvmti, JVMTI_ENABLE,
                         JVMTI_EVENT_GARBAGE_COLLECTION_START);
    SetEventNotification(g_jvmti, JVMTI_ENABLE,
                         JVMTI_EVENT_GARBAGE_COLLECTION_FINISH);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_NATIVE_METHOD_BIND);
    SetEventNotification(g_jvmti, JVMTI_ENABLE,
                         JVMTI_EVENT_VM_OBJECT_ALLOC);
    SetEventNotification(g_jvmti, JVMTI_ENABLE,
                         JVMTI_EVENT_OBJECT_FREE);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_CLASS_FILE_LOAD_HOOK);
    JVMTI_Logger::info("stevenhao", "==========Agent_OnAttach=======");
    return JNI_OK;

}

void SetAllCapabilities(jvmtiEnv *jvmti) {
    jvmtiCapabilities caps;
    jvmtiError error;
    error = jvmti->GetPotentialCapabilities(&caps);
    caps.can_tag_objects = 1;
    error = jvmti->AddCapabilities(&caps);
}

jvmtiEnv* CreateJvmtiEnv(JavaVM *vm) {
    jvmtiEnv *jvmti_env;
    jint result = vm->GetEnv((void **) &jvmti_env, (JVMTI_VERSION_1_2 | JVMTI_THREAD_STATE_VENDOR_3));
    if (result != JNI_OK) {
        return nullptr;
    }
    return jvmti_env;
}

void SetEventNotification(jvmtiEnv *jvmti, jvmtiEventMode mode,
                          jvmtiEvent event_type) {
    jvmtiError err = jvmti->SetEventNotificationMode(mode, event_type, nullptr);
}


int baseJVMTI::get_gJdwpAllowed() {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            JVMTI_Logger::info(TAG, "Failed to open libart.so: %s", error);
        } else {
            JVMTI_Logger::info(TAG, "Failed to open libart.so");
        }
    }
    JVMTI_Logger::info(TAG, "handle point %p", handle);

    IsJdwpAllowed funcPtr = reinterpret_cast<IsJdwpAllowed>(by_dlsym(handle, "_ZN3art3Dbg13IsJdwpAllowedEv"));
    if (funcPtr == NULL) {
        JVMTI_Logger::info(TAG,"not found IsJdwpAllowed symbol");
        by_dlclose(handle);
        return 0;
    }
    JVMTI_Logger::info(TAG,"found IsJdwpAllowed symbol");
    return funcPtr();
}

void baseJVMTI::set_gJdwpAllowed(int allowed) {
    by_char_t const* libraryName_cstr = "libart.so";
    by_pointer_t handle = by_dlopen(libraryName_cstr, BY_RTLD_NOW | BY_RTLD_LAZY);
    if (handle == NULL) {
        const char* error = dlerror();
        if (error != NULL) {
            JVMTI_Logger::info(TAG, "Failed to open libart.so: %s", error);
        } else {
            JVMTI_Logger::info(TAG, "Failed to open libart.so");
        }
    }
    JVMTI_Logger::info(TAG, "handle point %p", handle);

    SetJdwpAllowedFunc funcPtr = reinterpret_cast<SetJdwpAllowedFunc>(by_dlsym(handle, "_ZN3art3Dbg14SetJdwpAllowedEb"));
    if (funcPtr == NULL) {
        JVMTI_Logger::info(TAG,"not found SetJdwpAllowed symbol");
        by_dlclose(handle);
        return;
    }
    JVMTI_Logger::info(TAG,"found SetJdwpAllowed symbol");
    funcPtr(allowed);
    by_dlclose(handle);
}

void addObjectInfo(jthread thread, jobject object, jclass klass, jlong size, jlong tag) {
    ObjectInfo objInfo(thread, object, klass, size);

    // 使用 insert_or_assign 来插入或更新键值对
    objectMap.insert(std::make_pair(tag, objInfo));
}

// 从 objectMap 中移除对象信息
void removeObject(jint tag) {
    objectMap.erase(tag);
    JVMTI_Logger::info("stevenhao", "~~~~~~~removeObject %d", tag);
}

void ObjectFree(jvmtiEnv *jvmti_env, jlong tag) {
    JVMTI_Logger::info("stevenhao", "ObjectFree tag %i", tag);
    removeObject(tag);
}

void baseJVMTI::print_jvm_object() {
    for (auto &obj : objectMap) {
        JVMTI_Logger::info("stevenhao", "Object tag %d", obj.first);
    }
}

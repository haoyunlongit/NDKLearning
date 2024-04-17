#include <jni.h>
#include <string>
#include "string_utils.h"
#include "jvmti.h"
#include "log.h"
#include "jvmti_helper.h"
#include "jni_android.h"
#include "mem_object_info.h"
#include <unordered_map>

namespace {
    static std::unordered_map<jlong, ObjectInfo> objectMap;
}


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
    std::unordered_map<jlong, ObjectInfo>::iterator it = objectMap.begin();
    ///遍历所有的对象
    for (; it != objectMap.end(); it++) {
        Logger::info("stevenhao", "~~~~~~~~~~~~~r %d", it->first);
    }
    std::string hello = string_utils::get_hello_string(0);
    return env->NewStringUTF(hello.c_str());
}

extern "C" jstring
Java_com_stevenhao_ndklearning_MainActivity_showGJdwpAllowed(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("hook pthread_exit success");
}

void addObjectInfo(jthread thread, jobject object, jclass klass, jlong size, jlong tag) {
    ObjectInfo objInfo(thread, object, klass, size);

    // 使用 insert_or_assign 来插入或更新键值对
    objectMap.insert(std::make_pair(tag, objInfo));
}


// 从 objectMap 中移除对象信息
void removeObject(jint tag) {
    objectMap.erase(tag);
    Logger::info("stevenhao", "~~~~~~~removeObject %d", tag);
}

jint tempMethord
        (jvmtiHeapReferenceKind reference_kind,
                const jvmtiHeapReferenceInfo* reference_info,
                jlong class_tag, jlong referrer_class_tag,
                jlong size, jlong* tag_ptr, jlong* referrer_tag_ptr, jint length, void* user_data) {
  ///打印信息
   Logger::info("stevenhao", "~~~~~~~size %d", size);
}

void ObjectFree(jvmtiEnv *jvmti_env, jlong tag) {
    Logger::info("stevenhao", "ObjectFree tag %i", tag);
    removeObject(tag);
}

int index = 0;
void ObjectAllocCallback(jvmtiEnv *jvmti, JNIEnv *jni,
                         jthread thread, jobject object,
                         jclass klass, jlong size) {
    char *classSignature;
    jvmti->GetClassSignature(klass, &classSignature, nullptr);
//    jclass cls = jni->FindClass("java/lang/Class");
//    jmethodID mid_getName = jni->GetMethodID(cls, "getName", "()Ljava/lang/String;");
//    jstring name = static_cast<jstring>(jni->CallObjectMethod(klass, mid_getName));
//     const char *name1 = jni->GetStringUTFChars(name, nullptr);
//    Logger::info("stevenhao", "name1 %s", name1);
//    jni->ReleaseStringUTFChars(name, name1);

    if (strcmp(classSignature, "Lcom/stevenhao/ndklearning/MyString;") == 0) {
        // 这是一个大的MyString对象，打印信息
        Logger::info("stevenhao", "Large MyString object allocated with size: %lld", size);

        jvmti->SetTag(object, index);
        index++;
        addObjectInfo(thread, object, klass, size, index);
    }

    jvmti->Deallocate((unsigned char *) classSignature);
//    if (size > 1000) {
//        Logger::info("stevenhao", "=========================================");
//        ////打印对象的基本信息
//        char *signature;
//        jvmti->GetClassSignature(klass, &signature, nullptr);
//        Logger::info("stevenhao", "signature:%s size: %d", signature, size);
//        jvmti->Deallocate(reinterpret_cast<unsigned char *>(signature));

//        //获取线程信息
//        jvmtiThreadInfo threadInfo;
//        jvmti->GetThreadInfo(thread, &threadInfo);
//
//        //获得 创建的对象的类签名
//        char *classSignature;
//        jvmti->GetClassSignature(klass, &classSignature, nullptr);
//
//        char str[500];
//        Logger::info("stevenhao", "object alloc {Thread:%s Class:%s Size:%lld Tag:%lld} \r\n",threadInfo.name, classSignature, size);
//
//        jvmti->Deallocate((unsigned char *) classSignature);

//        jvmtiFrameInfo frames[5];
//        jint count;
//        jvmtiError error = jvmti->GetStackTrace(thread, 0, 5,  , &count);
//        if (error == JVMTI_ERROR_NONE) {
//            for (int i = 0; i < count; i++) {
//                char *methodName;
//                char *methodSignature;
//                char *methodGeneric;
//
//                error = jvmti->GetMethodName(jvmtiFrameInfo1.method, &methodName, &methodSignature, &methodGeneric);
//                if (error == JVMTI_ERROR_NONE) {
//                    Logger::info("stevenhao", "methodName:%s methodSignature %s methodGeneric: %s", methodName, methodSignature, methodGeneric);
//                    jvmti->Deallocate(reinterpret_cast<unsigned char *>(methodName));
//                    jvmti->Deallocate(reinterpret_cast<unsigned char *>(methodSignature));
//                    jvmti->Deallocate(reinterpret_cast<unsigned char *>(methodGeneric));
//                }
//            }
//        }
//        Logger::info("stevenhao", "=========================================");
//    }
}

jvmtiEnv* CreateJvmtiEnv(JavaVM *vm);
void SetAllCapabilities(jvmtiEnv *jvmti);

void SetEventNotification(jvmtiEnv *jvmti, jvmtiEventMode mode,
                          jvmtiEvent event_type);


void GCStartCallback(jvmtiEnv *jvmti) {
    Logger::info("stevenhao","==========触发 GCStart=======");

}

void GCFinishCallback(jvmtiEnv *jvmti) {
    Logger::info("stevenhao","==========触发 GCFinish=======");
}

void JNICALL ReferenceCallback(jvmtiHeapReferenceKind reference_kind,
                               const jvmtiHeapReferenceInfo* reference_info,
                               jlong class_tag,
                               jlong referrer_class_tag,
                               jlong size,
                               jlong* tag_ptr,
                               jlong* referrer_tag_ptr,
                               jint length,
                               void* user_data) {
    // 这个函数会在遇到一个引用时被调用
    // 你可以在这里添加你的代码来处理引用
}

extern "C" JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM *vm, char *options,
                                                 void *reserved) {
    jvmtiEnv *jvmti_env = CreateJvmtiEnv(vm);

    if (jvmti_env == nullptr) {
        Logger::info("stevenhao", "==========CreateJvmtiEnv fail=======");
        return JNI_ERR;
    } else {
        Logger::info("stevenhao", "==========CreateJvmtiEnv success=======");
    }
    basejni::setJVMTIEnv(jvmti_env);
    SetAllCapabilities(jvmti_env);
    jvmtiEventCallbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));

    callbacks.VMObjectAlloc = &ObjectAllocCallback;
    callbacks.ObjectFree = &ObjectFree;

//    callbacks.NativeMethodBind = &JvmTINativeMethodBind;
//
    callbacks.GarbageCollectionStart = &GCStartCallback;
    callbacks.GarbageCollectionFinish = &GCFinishCallback;
    int error = jvmti_env->SetEventCallbacks(&callbacks, sizeof(callbacks));
    Logger::info("stevenhao", "register error %d", error);
    SetEventNotification(jvmti_env, JVMTI_ENABLE,
                         JVMTI_EVENT_GARBAGE_COLLECTION_START);
    SetEventNotification(jvmti_env, JVMTI_ENABLE,
                         JVMTI_EVENT_GARBAGE_COLLECTION_FINISH);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_NATIVE_METHOD_BIND);
    SetEventNotification(jvmti_env, JVMTI_ENABLE,
                         JVMTI_EVENT_VM_OBJECT_ALLOC);
    SetEventNotification(jvmti_env, JVMTI_ENABLE,
                         JVMTI_EVENT_OBJECT_FREE);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_CLASS_FILE_LOAD_HOOK);
    Logger::info("stevenhao", "==========Agent_OnAttach=======");
    return JNI_OK;

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

void SetAllCapabilities(jvmtiEnv *jvmti) {
    jvmtiCapabilities caps;
    jvmtiError error;
    error = jvmti->GetPotentialCapabilities(&caps);
    caps.can_tag_objects = 1;
    error = jvmti->AddCapabilities(&caps);
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_stevenhao_ndklearning_JVMHelper_getGJdwpAllowed(JNIEnv *env, jclass clazz) {
    return get_gJdwpAllowed();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_stevenhao_ndklearning_JVMHelper_setGJdwpAllowed(JNIEnv *env, jclass clazz,
                                                         jboolean allowed) {
    set_gJdwpAllowed(allowed);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_stevenhao_ndklearning_MainActivity_scanMemory(JNIEnv *env, jobject thiz) {
//    std::unordered_map<jlong, ObjectInfo>::iterator it = objectMap.begin();
//    ///遍历所有的对象
//    for (; it != objectMap.end(); it++) {
//        Logger::info("stevenhao", "~~~~~~~~~~~~~r %d", it->first);
//    }
//    Logger::info("stevenhao", "~~~~~~~~~~~~~r");
//    ///objectMap 获取任意一个值
//    std::unordered_map<jlong, ObjectInfo>::iterator it = objectMap.begin();
//    Logger::info("stevenhao", "~~~~~~~~~~~~~r %d", it->first);
//    if (it != objectMap.end()) {
//        jlong tag = it->first;
//        ObjectInfo objInfo = it->second;
//        Logger::info("stevenhao", "~~~~~~~scanMemory %d", tag);
//        jvmtiHeapCallbacks callbacks;
//        memset(&callbacks, 0, sizeof(callbacks));
//        callbacks.heap_reference_callback = &tempMethord;
//        jvmtiError error = basejni::getJVMTIEnv()->FollowReferences(0, nullptr, objInfo.object, &callbacks, nullptr);
//        Logger::info("stevenhao", "~~~~~~~scanMemory error %d", error);
//        if (error != JVMTI_ERROR_NONE) {
//            Logger::info("stevenhao", "~~~~~~~scanMemory error %d", error);
//        } else {
//            Logger::info("stevenhao", "~~~~~~~scanMemory success");
//        }
//
//    }
}
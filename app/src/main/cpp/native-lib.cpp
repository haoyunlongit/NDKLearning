#include <jni.h>
#include <string>
#include "string_utils.h"
#include "jvmti.h"
#include "log.h"
#include "jvmti_helper.h"

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


void ObjectAllocCallback(jvmtiEnv *jvmti, JNIEnv *jni,
                         jthread thread, jobject object,
                         jclass klass, jlong size) {
    if (size > 200) {
        Logger::info("stevenhao", "=========================================");
        ////打印对象的基本信息
        char *signature;
        jvmti->GetClassSignature(klass, &signature, nullptr);
        Logger::info("stevenhao", "signature:%s size: %d", signature, size);
        jvmti->Deallocate(reinterpret_cast<unsigned char *>(signature));

        ////打印类名称
        jclass cls = jni->FindClass("java/lang/Class");
        jmethodID mid_getName = jni->GetMethodID(cls, "getName", "()Ljava/lang/String;");
        jstring name = static_cast<jstring>(jni->CallObjectMethod(klass, mid_getName));
        const char *nameChars = jni->GetStringUTFChars(name, nullptr);
        Logger::info("stevenhao", "class name:%s", nameChars);
        jni->ReleaseStringUTFChars(name, nameChars);

        ////打印对象的堆栈信息
        jvmtiFrameInfo frames[5];
        jint count;
        jvmtiError error = jvmti->GetStackTrace(thread, 0, 5, frames, &count);
        if (error == JVMTI_ERROR_NONE) {
            for (int i = 0; i < count; i++) {
                char *methodName;
                jvmti->GetMethodName(frames[i].method, &methodName, nullptr, nullptr);
                Logger::info("stevenhao", "method name:%s", methodName);
                jvmti->Deallocate(reinterpret_cast<unsigned char *>(methodName));
            }
        }
        Logger::info("stevenhao", "=========================================");
    }
}

jvmtiEnv* CreateJvmtiEnv(JavaVM *vm);
void SetAllCapabilities(jvmtiEnv *jvmti);
static jvmtiEnv *localJvmtiEnv;
void SetEventNotification(jvmtiEnv *jvmti, jvmtiEventMode mode,
                          jvmtiEvent event_type);


void GCStartCallback(jvmtiEnv *jvmti) {
    Logger::info("stevenhao","==========触发 GCStart=======");

}

void GCFinishCallback(jvmtiEnv *jvmti) {
    Logger::info("stevenhao","==========触发 GCFinish=======");

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
    localJvmtiEnv = jvmti_env;
    SetAllCapabilities(jvmti_env);
    jvmtiEventCallbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));

    callbacks.VMObjectAlloc = &ObjectAllocCallback;
//    callbacks.NativeMethodBind = &JvmTINativeMethodBind;
//
//    callbacks.GarbageCollectionStart = &GCStartCallback;
//    callbacks.GarbageCollectionFinish = &GCFinishCallback;
    int error = jvmti_env->SetEventCallbacks(&callbacks, sizeof(callbacks));
    Logger::info("stevenhao", "register error %d", error);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_GARBAGE_COLLECTION_START);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_GARBAGE_COLLECTION_FINISH);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_NATIVE_METHOD_BIND);
    SetEventNotification(jvmti_env, JVMTI_ENABLE,
                         JVMTI_EVENT_VM_OBJECT_ALLOC);
//    SetEventNotification(jvmti_env, JVMTI_ENABLE,
//                         JVMTI_EVENT_OBJECT_FREE);
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
//
// Created by liwei41 on 2017/9/12.
//


#include "log.h"
#include "aqts_library_loader.h"
#include "jni_android.h"
#include "linker/art_helper.h"

namespace aqts {

    jint AqtsOnLoad(JavaVM *vm, void *reserved) {
        basejni::InitVM(vm);

        auto vm_ext = reinterpret_cast<JavaVMExt*>(vm);
        void * runtime_instance_ = vm_ext->runtime;

        Logger::debug("stevenhao", "AqtsOnLoad success3333 %p", runtime_instance_);
        return JNI_VERSION_1_6;
    }

    void AqtsOnUnLoad(JavaVM *jvm, void *reserved) {
        //base::android::LibraryLoaderExitHook();
    }

    void test123() {
        Logger::debug("stevenhao", "test success");
    }

}//namespace aqts
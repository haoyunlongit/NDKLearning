//
// Created by liwei41 on 2017/9/12.
//


#include "log.h"
#include "aqts_library_loader.h"
namespace aqts {

    jint AqtsOnLoad(JavaVM *vm, void *reserved) {
        Logger::debug("stevenhao", "AqtsOnLoad success");
        return JNI_VERSION_1_6;
    }

    void AqtsOnUnLoad(JavaVM *jvm, void *reserved) {
        //base::android::LibraryLoaderExitHook();
    }

    void test123() {
        Logger::debug("stevenhao", "test success");
    }

}//namespace aqts
//
// Created by liwei41 on 2017/9/12.
//

#ifndef HTTPDNS_WB_HTTPDNS_LIBARAY_LOADER_H
#define HTTPDNS_WB_HTTPDNS_LIBARAY_LOADER_H
#include <jni.h>

namespace aqts {
    jint AqtsOnLoad(JavaVM *vm, void *reserved);

    void AqtsOnUnLoad(JavaVM *jvm, void *reserved);

    void test123();
}

#endif //HTTPDNS_WB_HTTPDNS_LIBARAY_LOADER_H

//
// Created by stevenhao on 4/17/24.
//

#ifndef NDKLEARNING_MEM_OBJECT_INFO_H
#define NDKLEARNING_MEM_OBJECT_INFO_H

#include "jvmti.h"

struct ObjectInfo {
    jthread thread;
    jobject object;
    jclass klass;
    jlong size;

    // 构造函数
    ObjectInfo(jthread thr, jobject obj, jclass cls, jlong sz)
            : thread(thr), object(obj), klass(cls), size(sz) {}
};
#endif //NDKLEARNING_MEM_OBJECT_INFO_H

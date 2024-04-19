//
// Created by stevenhao on 4/17/24.
//

#ifndef NDKLEARNING_MEM_OBJECT_INFO_H
#define NDKLEARNING_MEM_OBJECT_INFO_H

#include "jvmti.h"

class ObjectInfo {
    jobject object;
    jclass klass;
    jlong size;
    jvmtiFrameInfo *frames;

public:
    // 析构函数
    ~ObjectInfo() {
        JVMTI_Logger::info("stevenhao", "~~~~~~~~析构函数 ObjectInfo");
        delete frames;
    }

    // 构造函数
    ObjectInfo(jvmtiFrameInfo* frames, jobject obj, jclass cls, jlong sz)
            : object(obj), klass(cls), size(sz), frames(frames){};
};
#endif //NDKLEARNING_MEM_OBJECT_INFO_H

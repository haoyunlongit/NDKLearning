//
// Created by stevenhao on 4/17/24.
//

#ifndef NDKLEARNING_MEM_OBJECT_INFO_H
#define NDKLEARNING_MEM_OBJECT_INFO_H

#include "jvmti.h"

class ObjectInfo {
public:
    jclass klass;
    jlong size;
    jvmtiFrameInfo *frames;
    int frameSize;

    // 析构函数
    ~ObjectInfo() {
        JVMTI_Logger::info("stevenhao", "~~~~~~~~析构函数 ObjectInfo");
        delete frames;
    }

    // 构造函数
    ObjectInfo(jvmtiFrameInfo* frames, jint frameSize, jclass cls, jlong sz)
            : klass(cls), size(sz), frameSize(frameSize), frames(frames){};
};
#endif //NDKLEARNING_MEM_OBJECT_INFO_H

# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
project("ndk-starter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-shift-count-overflow -Wno-error=non-virtual-dtor -Wno-sign-compare -Wno-switch -Wno-missing-braces")

# 添加文件路径
file(GLOB starter_srcs
        "*.cpp"
        "*.h"
        "byopen/*.cpp"
        "byopen/*.h"
        "byopen/byopen_android.c"
        "utils/jni/jni_android.cpp"
        "utils/jni/jni_android.h"
        "linker/*.cpp"
        "test/*.cpp"
        "utils/*.h"
        "utils/*.cc"
        "utils/*.cpp")

# 添加动态库
add_library(${CMAKE_PROJECT_NAME} SHARED
        ${starter_srcs}
)

# 添加链接库
target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log)

# 添加头文件路径
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/byopen
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/jni
        ${CMAKE_CURRENT_SOURCE_DIR}/test)





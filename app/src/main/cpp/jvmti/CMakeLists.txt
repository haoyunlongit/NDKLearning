# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
project("jvmti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-shift-count-overflow -Wno-error=non-virtual-dtor -Wno-sign-compare -Wno-switch -Wno-missing-braces")

# 添加文件路径
file(GLOB jvmti_srcs
        "*.cpp"
        "*.h"
        "byopen/*.h"
        "byopen/*.c")

# 添加动态库
add_library("jvmti" SHARED
        ${jvmti_srcs}
)

# 添加链接库
target_link_libraries("jvmti"
        android
        log)

# 添加头文件路径
target_include_directories("jvmti" PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR/byopen})





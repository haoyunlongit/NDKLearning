# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
project("ndk-starter")

find_package(shadowhook REQUIRED CONFIG)
find_package(bytehook REQUIRED CONFIG)
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        shadowhook_wrapper.cpp
        string_utils.cpp
        inlinehook_wrapper.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        shadowhook::shadowhook
        bytehook::bytehook
        log)


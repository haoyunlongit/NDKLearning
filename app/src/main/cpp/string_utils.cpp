//
// Created by stevenhao on 3/25/24.
//

#include "string_utils.h"

const char *string_utils::get_hello_string(int index) {
    switch (index) {
        case 0:
            return "Hello from C++";
        case 1:
            return "Hello from C++ 1";
        case 2:
            return "Hello from C++ 2";
        default:
            return "Hello from C++x";
    }
    return nullptr;
}

//
// Created by stevenhao on 4/17/24.
//

#ifndef NDKLEARNING_JVMTI_HELPER_H
#define NDKLEARNING_JVMTI_HELPER_H
namespace baseJVMTI {
    int get_gJdwpAllowed();
    void set_gJdwpAllowed(int allowed);

    void print_jvm_object();
}
#endif //NDKLEARNING_JVMTI_HELPER_H

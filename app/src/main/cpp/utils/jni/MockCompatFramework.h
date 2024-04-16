//
// Created by stevenhao on 4/15/24.
//

#ifndef NDKLEARNING_MOCKCOMPATFRAMEWORK_H
#define NDKLEARNING_MOCKCOMPATFRAMEWORK_H

#include <set>
#include "MockMutex.h"
//#include <stdint.h>

class MockCompatFramework {
public:
    // A set of disabled compat changes for the running app, all other changes are enabled.
    std::set<uint64_t> disabled_compat_changes_;

    // A set of reported compat changes for the running app.
    std::set<uint64_t> reported_compat_changes_;
    MockMutex reported_compat_changes_lock_;
};
#endif //NDKLEARNING_MOCKCOMPATFRAMEWORK_H

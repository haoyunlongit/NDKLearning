//
// Created by stevenhao on 4/15/24.
//

#ifndef NDKLEARNING_MOCKMUTEX_H
#define NDKLEARNING_MOCKMUTEX_H
#include "jvmti/byopen/prefix.h"
#include "locks.h"
#include <stdint.h>
#include <atomic>
#include <pthread.h>
#include <unistd.h>

#include <limits.h>  // for INT_MAX
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>  // for pid_t

#include <iosfwd>
#include <string>
#include "MockMutex.h"
#include "mock_atomic.h"

//#include <android-base/logging.h>
//
//#include "base/aborting.h"
//#include "atomic.h"
//#include "runtime_globals.h"
//#include "base/macros.h"
//#include "locks.h"

// Base class for all Mutex implementations
#if defined(__linux__)
#define ART_USE_FUTEXES 1
#else
#define ART_USE_FUTEXES 0
#endif

#define DISALLOW_COPY_AND_ASSIGN_1(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// Currently Darwin doesn't support locks with timeouts.
#if !defined(__APPLE__)
#define HAVE_TIMED_RWLOCK 1
#else
#define HAVE_TIMED_RWLOCK 0
#endif

#if ART_USE_FUTEXES
// To enable lock contention logging, set this to true.
constexpr bool kLogLockContentions = false;
// FUTEX_WAKE first argument:
constexpr int kWakeOne = 1;
constexpr int kWakeAll = INT_MAX;
#else
// Keep this false as lock contention logging is supported only with
// futex.
constexpr bool kLogLockContentions = false;
#endif
constexpr size_t kContentionLogSize = 4;
constexpr size_t kContentionLogDataSize = kLogLockContentions ? 1 : 0;
constexpr size_t kAllMutexDataSize = kLogLockContentions ? 1 : 0;

class BaseMutex {
public:
    virtual ~BaseMutex();

    const char* name_;

    struct ContentionLogEntry {
        ContentionLogEntry() : blocked_tid(0), owner_tid(0) {}
        uint64_t blocked_tid;
        uint64_t owner_tid;
        AtomicInteger count;
    };
    struct ContentionLogData {
        ContentionLogEntry contention_log[kContentionLogSize];
        // The next entry in the contention log to be updated. Value ranges from 0 to
        // kContentionLogSize - 1.
        AtomicInteger cur_content_log_entry;
        // Number of times the Mutex has been contended.
        AtomicInteger contention_count;
        // Sum of time waited by all contenders in ns.
        MockAtomic<uint64_t> wait_time;
    };
    ContentionLogData contention_log_data_[kContentionLogDataSize];

    LockLevel level_;  // Support for lock hierarchy.
    bool should_respond_to_empty_checkpoint_request_;
};


class MockMutex : public BaseMutex {
public:
    ~MockMutex();
#if ART_USE_FUTEXES

#endif  // ART_USE_FUTEXES

#if ART_USE_FUTEXES
// Low order bit: 0 is unheld, 1 is held.
  // High order bits: Number of waiting contenders.
    ///todo 这里和源码有出入
    AtomicInteger  state_and_contenders_;

  // Exclusive owner.
  MockAtomic<pid_t> exclusive_owner_;
#else
pthread_mutex_t mutex_;
std::atomic<pid_t> exclusive_owner_;  // Guarded by mutex_. Asynchronous reads are OK.
#endif

unsigned int recursion_count_;
bool recursive;  // Can the lock be recursively held?

bool enable_monitor_timeout_ = false;

uint32_t monitor_id_;
friend class ConditionVariable;
DISALLOW_COPY_AND_ASSIGN_1(MockMutex);
};
#endif //NDKLEARNING_MOCKMUTEX_H

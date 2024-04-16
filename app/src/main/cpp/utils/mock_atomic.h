/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_LIBARTBASE_BASE_ATOMIC_H_
#define ART_LIBARTBASE_BASE_ATOMIC_H_

#include <stdint.h>
#include <atomic>
#include <limits>
#include <vector>

enum class CASMode {
    kStrong,
    kWeak,
};

#define PACKED_2(x) __attribute__ ((__aligned__(x), __packed__))

template<typename T>
class PACKED_2(sizeof(T)) MockAtomic : public std::atomic<T> {
public:
    MockAtomic<T>() : std::atomic<T>(T()) { }

    explicit MockAtomic<T>(T value) : std::atomic<T>(value) { }

    // Load data from an atomic variable with Java data memory order semantics.
    //
    // Promises memory access semantics of ordinary Java data.
    // Does not order other memory accesses.
    // Long and double accesses may be performed 32 bits at a time.
    // There are no "cache coherence" guarantees; e.g. loads from the same location may be reordered.
    // In contrast to normal C++ accesses, racing accesses are allowed.
    T LoadJavaData() const {
        return this->load(std::memory_order_relaxed);
    }

    // Store data in an atomic variable with Java data memory ordering semantics.
    //
    // Promises memory access semantics of ordinary Java data.
    // Does not order other memory accesses.
    // Long and double accesses may be performed 32 bits at a time.
    // There are no "cache coherence" guarantees; e.g. loads from the same location may be reordered.
    // In contrast to normal C++ accesses, racing accesses are allowed.
    void StoreJavaData(T desired_value) {
        this->store(desired_value, std::memory_order_relaxed);
    }

    // Atomically replace the value with desired_value if it matches the expected_value.
    // Participates in total ordering of atomic operations.
    bool CompareAndSetStrongSequentiallyConsistent(T expected_value, T desired_value) {
        return this->compare_exchange_strong(expected_value, desired_value, std::memory_order_seq_cst);
    }

    // The same, except it may fail spuriously.
    bool CompareAndSetWeakSequentiallyConsistent(T expected_value, T desired_value) {
        return this->compare_exchange_weak(expected_value, desired_value, std::memory_order_seq_cst);
    }

    // Atomically replace the value with desired_value if it matches the expected_value. Doesn't
    // imply ordering or synchronization constraints.
    bool CompareAndSetStrongRelaxed(T expected_value, T desired_value) {
        return this->compare_exchange_strong(expected_value, desired_value, std::memory_order_relaxed);
    }

    // Atomically replace the value with desired_value if it matches the expected_value. Prior writes
    // to other memory locations become visible to the threads that do a consume or an acquire on the
    // same location.
    bool CompareAndSetStrongRelease(T expected_value, T desired_value) {
        return this->compare_exchange_strong(expected_value, desired_value, std::memory_order_release);
    }

    // The same, except it may fail spuriously.
    bool CompareAndSetWeakRelaxed(T expected_value, T desired_value) {
        return this->compare_exchange_weak(expected_value, desired_value, std::memory_order_relaxed);
    }

    // Atomically replace the value with desired_value if it matches the expected_value. Prior writes
    // made to other memory locations by the thread that did the release become visible in this
    // thread.
    bool CompareAndSetWeakAcquire(T expected_value, T desired_value) {
        return this->compare_exchange_weak(expected_value, desired_value, std::memory_order_acquire);
    }

    // Atomically replace the value with desired_value if it matches the expected_value. Prior writes
    // to other memory locations become visible to the threads that do a consume or an acquire on the
    // same location.
    bool CompareAndSetWeakRelease(T expected_value, T desired_value) {
        return this->compare_exchange_weak(expected_value, desired_value, std::memory_order_release);
    }

    bool CompareAndSet(T expected_value,
                       T desired_value,
                       CASMode mode,
                       std::memory_order memory_order) {
        return mode == CASMode::kStrong
               ? this->compare_exchange_strong(expected_value, desired_value, memory_order)
               : this->compare_exchange_weak(expected_value, desired_value, memory_order);
    }

    // Returns the address of the current atomic variable. This is only used by futex() which is
    // declared to take a volatile address (see base/mutex-inl.h).
    volatile T* Address() {
        return reinterpret_cast<T*>(this);
    }

    static T MaxValue() {
        return std::numeric_limits<T>::max();
    }
};

using AtomicInteger = MockAtomic<int32_t>;

#endif  // ART_LIBARTBASE_BASE_ATOMIC_H_

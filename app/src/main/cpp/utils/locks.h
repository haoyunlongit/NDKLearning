//
// Created by stevenhao on 4/16/24.
//

#ifndef NDKLEARNING_LOCKS_H
#define NDKLEARNING_LOCKS_H
#include <stdint.h>

enum LockLevel : uint8_t {
    kLoggingLock = 0,
    kSwapMutexesLock,
    kUnexpectedSignalLock,
    kThreadSuspendCountLock,
    kAbortLock,
    kJniIdLock,
    kNativeDebugInterfaceLock,
    kSignalHandlingLock,
    // A generic lock level for mutexes that should not allow any additional mutexes to be gained
    // after acquiring it.
    kGenericBottomLock,
    // Tracks the second acquisition at the same lock level for kThreadWaitLock. This is an exception
    // to the normal lock ordering, used to implement Monitor::Wait - while holding one kThreadWait
    // level lock, it is permitted to acquire a second one - with internal safeguards to ensure that
    // the second lock acquisition does not result in deadlock. This is implemented in the lock
    // order by treating the second acquisition of a kThreadWaitLock as a kThreadWaitWakeLock
    // acquisition. Thus, acquiring kThreadWaitWakeLock requires holding kThreadWaitLock. This entry
    // is here near the bottom of the hierarchy because other locks should not be
    // acquired while it is held. kThreadWaitLock cannot be moved here because GC
    // activity acquires locks while holding the wait lock.
    kThreadWaitWakeLock,
    kJdwpAdbStateLock,
    kJdwpSocketLock,
    kRegionSpaceRegionLock,
    kMarkSweepMarkStackLock,
    // Can be held while GC related work is done, and thus must be above kMarkSweepMarkStackLock
    kThreadWaitLock,
    kCHALock,
    kJitCodeCacheLock,
    kRosAllocGlobalLock,
    kRosAllocBracketLock,
    kRosAllocBulkFreeLock,
    kAllocSpaceLock,
    kTaggingLockLevel,
    kTransactionLogLock,
    kCustomTlsLock,
    kJniFunctionTableLock,
    kJniWeakGlobalsLock,
    kJniGlobalsLock,
    kReferenceQueueSoftReferencesLock,
    kReferenceQueuePhantomReferencesLock,
    kReferenceQueueFinalizerReferencesLock,
    kReferenceQueueWeakReferencesLock,
    kReferenceQueueClearedReferencesLock,
    kReferenceProcessorLock,
    kJitDebugInterfaceLock,
    kBumpPointerSpaceBlockLock,
    kArenaPoolLock,
    kInternTableLock,
    kOatFileSecondaryLookupLock,
    kHostDlOpenHandlesLock,
    kVerifierDepsLock,
    kOatFileManagerLock,
    kTracingUniqueMethodsLock,
    kTracingStreamingLock,
    kJniLoadLibraryLock,
    kClassLoaderClassesLock,
    kDefaultMutexLevel,
    kDexCacheLock,
    kDexLock,
    kMarkSweepLargeObjectLock,
    kJdwpObjectRegistryLock,
    kModifyLdtLock,
    kAllocatedThreadIdsLock,
    kMonitorPoolLock,
    kClassLinkerClassesLock,  // TODO rename.
    kSubtypeCheckLock,
    kBreakpointLock,
    // This is a generic lock level for a lock meant to be gained after having a
    // monitor lock.
    kPostMonitorLock,
    kMonitorLock,
    kMonitorListLock,
    kThreadListLock,
    kAllocTrackerLock,
    kDeoptimizationLock,
    kProfilerLock,
    kJdwpShutdownLock,
    kJdwpEventListLock,
    kJdwpAttachLock,
    kJdwpStartLock,
    kRuntimeThreadPoolLock,
    kRuntimeShutdownLock,
    kTraceLock,
    kHeapBitmapLock,

    // This is a generic lock level for a top-level lock meant to be gained after having the
    // mutator_lock_.
    kPostMutatorTopLockLevel,

    kMutatorLock,
    kInstrumentEntrypointsLock,
    // This is a generic lock level for a top-level lock meant to be gained after having the
    // UserCodeSuspensionLock.
    kPostUserCodeSuspensionTopLevelLock,
    kUserCodeSuspensionLock,
    kZygoteCreationLock,

    // The highest valid lock level. Use this if there is code that should only be called with no
    // other locks held. Since this is the highest lock level we also allow it to be held even if the
    // runtime or current thread is not fully set-up yet (for example during thread attach). Note that
    // this lock also has special behavior around the mutator_lock_. Since the mutator_lock_ is not
    // really a 'real' lock we allow this to be locked when the mutator_lock_ is held exclusive.
    // Furthermore, the mutator_lock_ may not be acquired in any form when a lock of this level is
    // held. Since the mutator_lock_ being held strong means that all other threads are suspended this
    // will prevent deadlocks while still allowing this lock level to function as a "highest" level.
    kTopLockLevel,

    kLockLevelCount  // Must come last.
};
#endif //NDKLEARNING_LOCKS_H

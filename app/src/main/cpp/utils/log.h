#ifndef LOG_H
#define LOG_H

#define PRINT_LOG
#define ANDROID

#if defined (PRINT_LOG)
#if defined(ANDROID)
#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG ""
#endif
 
#define NATIVE_LOG_TAG "starter_" LOG_TAG
#define LOGE(...)   __android_log_print(ANDROID_LOG_ERROR,NATIVE_LOG_TAG,__VA_ARGS__)
#define LOGW(...)   __android_log_print(ANDROID_LOG_WARN,NATIVE_LOG_TAG, __VA_ARGS__)
#define LOGD(...)   __android_log_print(ANDROID_LOG_DEBUG,NATIVE_LOG_TAG, __VA_ARGS__)
#define LOGI(...)   __android_log_print(ANDROID_LOG_INFO,NATIVE_LOG_TAG, __VA_ARGS__)
#define LOGV(...)   __android_log_print(ANDROID_LOG_VERBOSE,NATIVE_LOG_TAG, __VA_ARGS__)

#ifdef DUMP_DEBUG
#define DEBUG_LOGE(...) \
	LOGE(__VA_ARGS__)
#else
#define DEBUG_LOGE(...)
#endif

#elif defined(__APPLE__)
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#define LOGE(format, ...) printf("httpdns_[error] <%s : %d> " format "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOGD(format, ...) printf("httpdns_[debug] <%s : %d> " format "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOGI(format, ...) printf("httpdns_[info] <%s : %d> " format "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#endif

#else
#define LOGE(...)
#define LOGW(...)
#define LOGD(...)
#define LOGI(...)
#define LOGV(...)
#endif

#ifndef CHECK
#define CHECK(assertion)                                                                           \
  if (!(assertion)) {                                                                              \
    LOGE("CHECK failed at %s (line: %d) - <%s>: %s: %s", __FILE__, __LINE__, __FUNCTION__,        \
          #assertion, strerror(errno));                                                            \
  }
#endif

#ifndef FINISHP_FUC
#define FINISHP_FUC(assertion, func)                                                               \
  if (!(assertion)) {                                                                              \
    LOGE("CHECK failed at %s (line: %d) - <%s>: %s: %s", __FILE__, __LINE__, __FUNCTION__,        \
          #assertion, strerror(errno));                                                            \
    func();                                                                                        \
    return nullptr;                                                                                \
  }
#endif

#endif

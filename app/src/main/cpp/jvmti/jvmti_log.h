#include <android/log.h>
#include <stdarg.h>

class JVMTI_Logger {
public:
    static void error(const char *tag, const char *format, ...) {
        va_list args;
        va_start(args, format);
        __android_log_vprint(ANDROID_LOG_ERROR, tag, format, args);
        va_end(args);
    }

    static void info(const char *tag, const char *format, ...) {
        va_list args;
        va_start(args, format);
        __android_log_vprint(ANDROID_LOG_INFO, tag, format, args);
        va_end(args);
    }

    static void debug(const char *tag, const char *format, ...) {
        va_list args;
        va_start(args, format);
        __android_log_vprint(ANDROID_LOG_DEBUG, tag, format, args);
        va_end(args);
    }
};
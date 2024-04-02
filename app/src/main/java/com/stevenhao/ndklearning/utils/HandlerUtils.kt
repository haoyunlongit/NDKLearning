package com.stevenhao.ndklearning.utils

import android.os.Handler
import android.os.Looper
/**
 * @Autor : yunlong20
 * @Date  : on 2024-04-02
 * @Description :
 */
class HandlerUtils {
    companion object {
        private val handler: Handler = Handler(Looper.getMainLooper())

        fun postDelay(runnable: Runnable, delayed: Long): Boolean {
            return handler.postDelayed(runnable, delayed)
        }
    }
}
package com.stevenhao.ndklearning.utils

import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.DeadSystemException
import android.util.Log
import androidx.annotation.RequiresApi

/**
 * @Autor : yunlong20
 * @Date  : on 2024-04-02
 * @Description :
 */
class Utils {
    companion object {
        @RequiresApi(Build.VERSION_CODES.N)
        @JvmStatic
        fun sendAppInsideBroadcast(ctx: Context, intent: Intent) {
            intent.setPackage(Constants.PACKAGE_NAME)
            try {
                ctx.sendBroadcast(intent)
            } catch (exception: RuntimeException) {
                if (exception.cause is DeadSystemException) {
                    Log.e("stevenhao", exception.message ?: "sendBroadcast failed")
                    return
                }
                throw exception
            }
        }
    }
}
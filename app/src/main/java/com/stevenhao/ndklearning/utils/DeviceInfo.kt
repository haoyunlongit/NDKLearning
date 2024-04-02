package com.stevenhao.ndklearning.utils

import android.util.DisplayMetrics

/**
 * @Autor : yunlong20
 * @Date  : on 2024-04-02
 * @Description :
 */
class DeviceInfo {
    companion object {
        private var mDisplayMetrics: DisplayMetrics? = null
        fun getDefaultDisplayMetrics(): DisplayMetrics? {
            if (DeviceInfo.mDisplayMetrics == null) {
                DeviceInfo.mDisplayMetrics =
                    App.getInstance().resources.displayMetrics
            }
            return DeviceInfo.mDisplayMetrics
        }

        fun getCacheScreenWidth(): Int {
            val dm = getDefaultDisplayMetrics()
            return dm?.widthPixels ?: 0
        }

        fun getCacheScreenHeight(): Int {
            val dm = getDefaultDisplayMetrics()
            return dm?.heightPixels ?: 0
        }
    }
}
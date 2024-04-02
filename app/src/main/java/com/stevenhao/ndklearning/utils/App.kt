package com.stevenhao.ndklearning.utils

import android.app.Activity
import android.app.Application
import com.bytedance.android.bytehook.ByteHook

/**
 * @Autor : yunlong20
 * @Date  : on 2024-04-02
 * @Description :
 */
class App : Application() {

    companion object {
        private lateinit var instance: App

        fun getInstance(): App {
            return instance
        }

        private var rootActivity: Activity? = null

        @JvmStatic
        fun setRootActivity(activity: Activity) {
            rootActivity = activity
        }

        @JvmStatic
        fun getRootActivity(): Activity? {
            return rootActivity
        }

        fun getResources() = instance.resources
    }

    init {
        ByteHook.init()
    }

    override fun onCreate() {
        super.onCreate()
    }
}
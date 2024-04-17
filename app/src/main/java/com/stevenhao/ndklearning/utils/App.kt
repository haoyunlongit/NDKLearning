package com.stevenhao.ndklearning.utils

import android.app.Activity
import android.app.Application
import android.content.pm.ApplicationInfo

import java.security.AccessController.getContext




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


    override fun onCreate() {
        super.onCreate()
    }

    open fun isDebug(): Boolean {
        return 0 != applicationInfo.flags and ApplicationInfo.FLAG_DEBUGGABLE
    }
}
package com.stevenhao.ndklearning.base

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Build
import android.os.Bundle
import android.os.PersistableBundle
import android.util.Log
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import com.stevenhao.ndklearning.utils.Constants

/**
 * @Autor : yunlong20
 * @Date  : on 2024-04-02
 * @Description :
 */
open class BaseActivity : AppCompatActivity() {

    private val broadcastReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            finish()
            Log.d("stevenhao", "do broadcastReceiver finish")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            registerBroadcast()
        };
    }

    @RequiresApi(Build.VERSION_CODES.O)
    private fun registerBroadcast() {
        Log.d("stevenhao", "register broadcast")
        val intentFilter: IntentFilter = IntentFilter()
        intentFilter.addAction(Constants.ACTION_RESTART)
        try {
            registerReceiver(broadcastReceiver, intentFilter, RECEIVER_NOT_EXPORTED)
        } catch (e: Exception) {
            Log.e("stevenhao", e.toString())
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        try {
            unregisterReceiver(broadcastReceiver)
        } catch (e: Exception) {
            Log.e("stevenhao", e.toString())
        }
    }
}
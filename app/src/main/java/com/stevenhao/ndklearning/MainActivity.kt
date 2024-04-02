package com.stevenhao.ndklearning

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.view.View
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import androidx.navigation.ui.setupActionBarWithNavController
import com.bytedance.android.bytehook.ByteHook
import com.stevenhao.ndklearning.activity.SettingActivity
import com.stevenhao.ndklearning.databinding.ActivityMainBinding
import com.stevenhao.ndklearning.utils.App
import com.stevenhao.ndklearning.utils.Constants
import com.stevenhao.ndklearning.utils.HandlerUtils
import kotlinx.coroutines.ExecutorCoroutineDispatcher
import kotlinx.coroutines.newSingleThreadContext

class MainActivity : AppCompatActivity() {

    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: ActivityMainBinding
    private val threadPoolExecutor: ExecutorCoroutineDispatcher = newSingleThreadContext("test");

    private val broadcastReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            val i = context!!.packageManager.getLaunchIntentForPackage(
                context.packageName
            )
            if (i != null) {
                i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
                startActivity(i)
                finish()
            }
        }
    }

    init {
        System.loadLibrary("ndk-starter")
        App.setRootActivity(this)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        setSupportActionBar(binding.toolbar)
        val navController = findNavController(R.id.nav_host_fragment_content_main)
        appBarConfiguration = AppBarConfiguration(navController.graph)
        setupActionBarWithNavController(navController, appBarConfiguration)

        binding.fab.setOnClickListener { view ->
            Log.e("stevenhao", "do action")
            SettingActivity.startActivity(this)
        }
        binding.root.findViewById<View>(R.id.button).setOnClickListener {
            startJNIThread()
        }

        binding.root.findViewById<View>(R.id.goto_setting_button).setOnClickListener {
            SettingActivity.startActivity(this)
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            registerBroadcast()
        }
    }

    @RequiresApi(Build.VERSION_CODES.O)
    private fun registerBroadcast() {
        registerReceiver(broadcastReceiver, IntentFilter(Constants.ACTION_RESTART),
            RECEIVER_NOT_EXPORTED
        )
    }

    private external fun startJNIThread(): String

    private external fun hookPThread(): String

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.nav_host_fragment_content_main)
        return navController.navigateUp(appBarConfiguration)
                || super.onSupportNavigateUp()
    }
}
package com.stevenhao.ndklearning.activity

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import com.stevenhao.ndklearning.R
import com.stevenhao.ndklearning.base.BaseActivity
import com.stevenhao.ndklearning.databinding.ActivitySettingBinding
import com.stevenhao.ndklearning.utils.Constants

/**
 * @Autor : yunlong20
 * @Date  : on 2024-04-02
 * @Description :
 */
class SettingActivity : BaseActivity() {

    companion object {
        const val TAG = "SettingActivity"
        fun startActivity(context: Context) {
            val intent = Intent(context, SettingActivity::class.java)
            intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK
            context.startActivity(intent)
        }
    }

    private var binding: ActivitySettingBinding? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initView()
    }

    private fun initView() {
        binding = ActivitySettingBinding.inflate(layoutInflater)
        setContentView(binding?.root)
        binding?.root?.findViewById<View>(R.id.go_back)?.setOnClickListener {
            finish()
        }

        binding?.root?.findViewById<View>(R.id.restart)?.setOnClickListener {
            LogoutDialogActivity.startLogoutDialogActivity(this)
        }
    }

    private fun sendBroadcast2() {
        val intent: Intent = Intent()
        intent.setPackage(Constants.PACKAGE_NAME)
        intent.action = Constants.ACTION_RESTART
        sendBroadcast(intent)
    }
}
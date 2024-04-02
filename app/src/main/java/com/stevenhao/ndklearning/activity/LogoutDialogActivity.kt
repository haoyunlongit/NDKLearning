package com.stevenhao.ndklearning.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.os.AsyncTask
import android.os.Build
import android.os.Bundle
import android.os.Process
import android.view.View
import androidx.annotation.RequiresApi
import com.google.android.material.bottomsheet.BottomSheetDialog
import com.stevenhao.ndklearning.R
import com.stevenhao.ndklearning.base.BaseActivity
import com.stevenhao.ndklearning.databinding.AccountsDialogLayoutBinding
import com.stevenhao.ndklearning.utils.App
import com.stevenhao.ndklearning.utils.Constants
import com.stevenhao.ndklearning.utils.Utils
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

/**
 * @Autor : yunlong20
 * @Date  : on 2024-04-02
 * @Description :
 */
class LogoutDialogActivity : BaseActivity() {

    val threadPoolExecutor: ExecutorService = Executors.newCachedThreadPool()

    companion object {
        @JvmStatic
        fun startLogoutDialogActivity(context: Context) {
            val intent = Intent(context, LogoutDialogActivity::class.java)
            context.startActivity(intent)
        }
    }
    val binding: AccountsDialogLayoutBinding by lazy {
        AccountsDialogLayoutBinding.inflate(layoutInflater)
    }

    @RequiresApi(Build.VERSION_CODES.N)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(binding.root)
        showDialog()
    }

    @RequiresApi(Build.VERSION_CODES.N)
    private fun showDialog() {
        val dialog = BottomSheetDialog(this)
        val view = layoutInflater.inflate(R.layout.setting_lougout_dialog_layout, null)
        dialog.setContentView(view)
        dialog.show()

        view.findViewById<View>(R.id.confirm_button).setOnClickListener {
            Utils.sendAppInsideBroadcast(this,  Intent(Constants.ACTION_RESTART))
            doExit()
            finish()
        }
    }

    private fun doExit() {
        App.getRootActivity()?.finish()
        val task: SendTask = SendTask(this)
        task.execute()
    }


    private class SendTask constructor(
        private val activity: Activity,
    ) : AsyncTask<Void?, Void, Void>() {
        override fun doInBackground(vararg params: Void?): Void? {
            return null
        }

        override fun onPostExecute(result: Void?) {
            super.onPostExecute(result)
            activity.finish()
            Process.killProcess(Process.myPid())
        }
    }


}
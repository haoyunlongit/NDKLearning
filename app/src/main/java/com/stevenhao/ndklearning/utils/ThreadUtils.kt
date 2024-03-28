package com.stevenhao.ndklearning.utils

import android.util.Log
import java.io.BufferedReader
import java.io.File
import java.io.FileReader

/**
 * @Autor : yunlong20
 * @Date  : on 2024-03-27
 * @Description :
 */
class ThreadUtils {
    companion object {
        fun printProcessInfo() {
            val statusFile = File("/proc/self/status")
            val lines = statusFile.readLines()

            for (line in lines) {
                Log.e("stevenhao123", line + "\n")
            }
        }

        fun printThreadInfo() {
            try {
                // 获取当前进程的PID
                var pid = android.os.Process.myPid();
                // 打开smaps文件
                var reader = BufferedReader( FileReader("/proc/" + pid + "/smaps"))
                var line = reader.readLine()
                while (line != null) {
//                    Log.d("stevenhao123", line);
                    line = reader.readLine()
                }
                reader.close();
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}
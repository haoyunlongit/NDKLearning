package com.stevenhao.ndklearning;

import android.app.Activity;
import android.content.Context;
import android.os.Build;
import android.os.Debug;
import android.util.Log;

import com.stevenhao.ndklearning.utils.App;

import java.io.File;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.nio.file.Paths;

/**
 * @Autor : yunlong20
 * @Date : on 2024-04-17
 * @Description :
 */
public class JVMHelper {
    static String packageCodePath = "";

    ///为什么复制
    ////安全性：将库文件复制到应用的私有文件夹中，可以防止其他应用或用户访问或修改这个库文件，提高了应用的安全性。
    //管理方便：将库文件放在应用的私有文件夹中，可以方便应用管理和使用这个库文件。例如，应用可以在需要时删除这个库文件，或者在更新应用时更新这个库文件。
    //兼容性：在某些情况下，直接使用库文件的原始路径可能会导致问题。例如，如果库文件位于外部存储设备上，那么当设备被卸载时，应用将无法访问库文件。将库文件复制到应用的私有文件夹中，可以避免这种问题。
    public static void init(Context context) {

        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                packageCodePath = context.getPackageCodePath();
                ClassLoader classLoader = context.getClassLoader();
                Method findLibrary = ClassLoader.class.getDeclaredMethod("findLibrary", String.class);
                String jvmtiAgentLibPath = (String) findLibrary.invoke(classLoader, "jvmti");
                //copy lib to /data/data/com.dodola.jvmti/files/jvmti
                if (jvmtiAgentLibPath == null) {
                    Log.e("stevenhao", "not find jvmtiAgentLibPath");
                    return;
                }

                Log.d("stevenhao", "jvmtiagentlibpath " + jvmtiAgentLibPath);
                File filesDir = context.getFilesDir();
                File jvmtiLibDir = new File(filesDir, "jvmti");
                if (!jvmtiLibDir.exists()) {
                    jvmtiLibDir.mkdirs();

                }
                File agentLibSo = new File(jvmtiLibDir, "agent.so");
                if (agentLibSo.exists()) {
                    agentLibSo.delete();
                }
                Files.copy(Paths.get(new File(jvmtiAgentLibPath).getAbsolutePath()), Paths.get((agentLibSo).getAbsolutePath()));

                System.load(agentLibSo.getAbsolutePath());
                Log.d("stevenhao", agentLibSo.getAbsolutePath() + "," + context.getPackageCodePath());
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                    ///判断当前是否是debug环境
                    boolean gjdwpAllowed = getGJdwpAllowed();
                    Log.d("stevenhao", "gjdwp allowed is " + gjdwpAllowed);
                    if (!gjdwpAllowed) {
                        Log.d("stevenhao", "set gjdwp allowed");
                        setGJdwpAllowed(true);
                    }
                    Debug.attachJvmtiAgent(agentLibSo.getAbsolutePath(), null, classLoader);
                    gjdwpAllowed = getGJdwpAllowed();
                    Log.d("stevenhao", "gjdwp allowed is " + gjdwpAllowed);
                    if (!gjdwpAllowed) {
                        Log.d("stevenhao", "set gjdwp disallowed");
                        setGJdwpAllowed(false);
                    }
                } else {
                    try {
                        Class vmDebugClazz = Class.forName("dalvik.system.VMDebug");
                        Method attachAgentMethod = vmDebugClazz.getMethod("attachAgent", String.class);
                        attachAgentMethod.setAccessible(true);
                        attachAgentMethod.invoke(null, agentLibSo.getAbsolutePath());
                    } catch (Exception ex) {
                        ex.printStackTrace();
                    }
                }
            }
        } catch (Exception ex) {
            Log.e("stevenhao", "init jvmti error" + ex.toString());
        }
    }


    static native boolean getGJdwpAllowed();

    static native void setGJdwpAllowed(boolean allowed);

}

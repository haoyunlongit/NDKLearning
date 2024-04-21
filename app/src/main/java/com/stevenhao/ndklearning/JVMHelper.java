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

    public static boolean loadJvmtiAgent(Context context) {
        boolean result = false;
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                packageCodePath = context.getPackageCodePath();
                ClassLoader classLoader = context.getClassLoader();
                Method findLibrary = ClassLoader.class.getDeclaredMethod("findLibrary", String.class);
                String jvmtiAgentLibPath = (String) findLibrary.invoke(classLoader, "jvmti");
                //copy lib to /data/data/com.dodola.jvmti/files/jvmti
                if (jvmtiAgentLibPath == null) {
                    Log.e("stevenhao", "not find jvmtiAgentLibPath");
                    return result;
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
                    result = true;
                } else {
                    try {
                        Class vmDebugClazz = Class.forName("dalvik.system.VMDebug");
                        Method attachAgentMethod = vmDebugClazz.getMethod("attachAgent", String.class);
                        attachAgentMethod.setAccessible(true);
                        attachAgentMethod.invoke(null, agentLibSo.getAbsolutePath());
                        result = true;
                    } catch (Exception ex) {
                        ex.printStackTrace();
                        result = false;
                    }
                }
            }
        } catch (Exception ex) {
            Log.e("stevenhao", "init jvmti error" + ex.toString());
            result = false;
        }
        return result;
    }



    static native boolean initJvmtiAgent();


    static native boolean getGJdwpAllowed();

    static native void setGJdwpAllowed(boolean allowed);

}

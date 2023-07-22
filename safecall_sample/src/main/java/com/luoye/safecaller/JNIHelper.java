package com.luoye.safecaller;

public class JNIHelper {

    static {
        System.loadLibrary("safecaller");
    }

    public static native void normalCall();
    public static native void problemCall1();
    public static native void problemCall2();

}

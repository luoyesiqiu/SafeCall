package com.luoye.safecaller;

public class JNIHelper {

    static {
        System.loadLibrary("safecaller");
    }

    public static native void normalCall1();
    public static native void normalCall2();
    public static native void normalCall3();
    public static native void problemCall1();
    public static native void problemCall2();

}

package io.jewsaten.ndksample;

/**
 * Created by Administrator on 2017/9/22.
 */

public class AESUtils {
    static {
        System.loadLibrary("AES");
    }
    public static native String AESEncode(String content);

    public static native String AESDecode(byte[] content);
}

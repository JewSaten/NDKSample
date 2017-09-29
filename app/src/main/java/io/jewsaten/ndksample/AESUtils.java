package io.jewsaten.ndksample;

/**
 * Created by Administrator on 2017/9/22.
 */

public class AESUtils {
    static {
        System.loadLibrary("AES");
    }
    public static native byte[] AESEncode(byte[] content);

    public static native byte[] AESDecode(byte[] content);
}

package io.jewsaten.ndksample;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        String source = "hello world";
        byte[] encodedStr = AESUtils.AESEncode(source.getBytes());
        Log.e("aaa", "encodedStr: " + new String(encodedStr));
        Log.e("aaa", "decodedStr: " + new String(AESUtils.AESDecode(encodedStr)));
    }
}

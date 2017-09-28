package io.jewsaten.ndksample;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import static io.jewsaten.ndksample.AESUtils.AESDecode;
import static io.jewsaten.ndksample.AESUtils.AESEncode;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        String str = "hello jni";
        String encodedStr = AESEncode(str);
        System.out.println("encodedStr: "+encodedStr);
        System.out.println("decodedStr: "+AESDecode(encodedStr.getBytes()));
    }
}

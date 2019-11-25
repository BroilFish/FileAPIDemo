package com.example.fileapidemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public static native int fileEncryptJNI();
    public static native void fileReadJNI();
    public static native void fileWriteJNI();



    // 文件加密功能按钮
    public void DoEncryptFile(View view) {

        fileEncryptJNI();

    }

    public void FileRead(View view) {

        fileReadJNI();

    }

    public void FileWrite(View view) {

        fileWriteJNI();
    }
}



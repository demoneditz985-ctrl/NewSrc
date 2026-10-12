package com.akn;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

/**
 * SHADOW MODZ - app shell.
 *
 * The release archive shipped the native module (app/src/main/jni) but no Java
 * sources at all, while AndroidManifest.xml declares .MainActivity and
 * .MenuService. This is a minimal placeholder so the project builds, installs
 * and launches: it loads the rebranded native library and shows the branded
 * screen. Drop the real activity in here (same package, same class name) and
 * delete this file - nothing else in the build has to change.
 */
public class MainActivity extends Activity {

    private static final String TAG = "ShadowModz";

    static {
        try {
            System.loadLibrary("NikuMods");
        } catch (Throwable t) {
            // The module only hooks a target process; failing to load it here
            // must not take the whole app down.
            Log.w(TAG, "native module not loaded: " + t.getMessage());
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button action = (Button) findViewById(R.id.OpenMenu);
        action.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(),
                        "SHADOW MODZ - placeholder shell",
                        Toast.LENGTH_SHORT).show();
            }
        });
    }
}

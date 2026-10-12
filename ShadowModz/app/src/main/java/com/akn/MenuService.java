package com.akn;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

/**
 * SHADOW MODZ - placeholder for the menu service declared in
 * AndroidManifest.xml. The release archive shipped no Java sources, so this
 * keeps the manifest resolvable. Replace it with the real service when you
 * drop your original Java into the project.
 */
public class MenuService extends Service {

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_NOT_STICKY;
    }
}

/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFFramework_test;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import com.ZFFramework.Android.ZFCore.ZFMainEntry;

public class Loader extends Activity {
    static {
        System.loadLibrary("stlport_shared");

        System.loadLibrary("ZFFramework");
        System.loadLibrary("ZFFramework_impl");
        System.loadLibrary("ZFFramework_impl_ZFUIWebKit");
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        

        System.loadLibrary("ZFFramework_test");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = new Intent(Loader.this, ZFMainEntry.class);
        startActivity(intent);
        this.finish();
    }
}


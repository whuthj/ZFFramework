/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFDebugLevel {
    public static final int ZFDebugLevelVerbose = ZFDebugLevel.native_ZFDebugLevelVerbose();
    private native static int native_ZFDebugLevelVerbose();
    public static final int ZFDebugLevelDebug = ZFDebugLevel.native_ZFDebugLevelDebug();
    private native static int native_ZFDebugLevelDebug();
    public static final int ZFDebugLevelInfo = ZFDebugLevel.native_ZFDebugLevelInfo();
    private native static int native_ZFDebugLevelInfo();
    public static final int ZFDebugLevelWarning = ZFDebugLevel.native_ZFDebugLevelWarning();
    private native static int native_ZFDebugLevelWarning();
    public static final int ZFDebugLevelError = ZFDebugLevel.native_ZFDebugLevelError();
    private native static int native_ZFDebugLevelError();
    public static final int ZFDebugLevelAssert = ZFDebugLevel.native_ZFDebugLevelAssert();
    private native static int native_ZFDebugLevelAssert();
}

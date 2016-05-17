/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFLogLevel {
    public static final int ZFLogLevelVerbose = ZFLogLevel.native_ZFLogLevelVerbose();
    private native static int native_ZFLogLevelVerbose();
    public static final int ZFLogLevelDebug = ZFLogLevel.native_ZFLogLevelDebug();
    private native static int native_ZFLogLevelDebug();
    public static final int ZFLogLevelInfo = ZFLogLevel.native_ZFLogLevelInfo();
    private native static int native_ZFLogLevelInfo();
    public static final int ZFLogLevelWarning = ZFLogLevel.native_ZFLogLevelWarning();
    private native static int native_ZFLogLevelWarning();
    public static final int ZFLogLevelError = ZFLogLevel.native_ZFLogLevelError();
    private native static int native_ZFLogLevelError();
    public static final int ZFLogLevelAssert = ZFLogLevel.native_ZFLogLevelAssert();
    private native static int native_ZFLogLevelAssert();
}

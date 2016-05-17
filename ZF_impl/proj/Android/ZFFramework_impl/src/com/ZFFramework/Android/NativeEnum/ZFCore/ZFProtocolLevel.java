/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFProtocolLevel {
    public static final int ZFProtocolLevelDefault = ZFProtocolLevel.native_ZFProtocolLevelDefault();
    private native static int native_ZFProtocolLevelDefault();
    public static final int ZFProtocolLevelSystemLow = ZFProtocolLevel.native_ZFProtocolLevelSystemLow();
    private native static int native_ZFProtocolLevelSystemLow();
    public static final int ZFProtocolLevelSystemNormal = ZFProtocolLevel.native_ZFProtocolLevelSystemNormal();
    private native static int native_ZFProtocolLevelSystemNormal();
    public static final int ZFProtocolLevelSystemHigh = ZFProtocolLevel.native_ZFProtocolLevelSystemHigh();
    private native static int native_ZFProtocolLevelSystemHigh();
    public static final int ZFProtocolLevelApp = ZFProtocolLevel.native_ZFProtocolLevelApp();
    private native static int native_ZFProtocolLevelApp();
}

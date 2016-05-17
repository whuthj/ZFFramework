/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIWidget;

public class ZFUIPagePauseReason {
    public static final int e_ByManagerPause = ZFUIPagePauseReason.native_e_ByManagerPause();
    private native static int native_e_ByManagerPause();
    public static final int e_ToBackground = ZFUIPagePauseReason.native_e_ToBackground();
    private native static int native_e_ToBackground();
    public static final int e_BeforeDestroy = ZFUIPagePauseReason.native_e_BeforeDestroy();
    private native static int native_e_BeforeDestroy();
}

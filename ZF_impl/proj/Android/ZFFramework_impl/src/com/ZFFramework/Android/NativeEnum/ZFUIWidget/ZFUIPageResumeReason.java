/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIWidget;

public class ZFUIPageResumeReason {
    public static final int e_ByManagerResume = ZFUIPageResumeReason.native_e_ByManagerResume();
    private native static int native_e_ByManagerResume();
    public static final int e_ByRequest = ZFUIPageResumeReason.native_e_ByRequest();
    private native static int native_e_ByRequest();
    public static final int e_FromBackground = ZFUIPageResumeReason.native_e_FromBackground();
    private native static int native_e_FromBackground();
}

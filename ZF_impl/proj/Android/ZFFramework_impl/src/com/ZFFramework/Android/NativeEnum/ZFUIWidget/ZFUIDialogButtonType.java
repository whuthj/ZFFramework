/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIWidget;

public class ZFUIDialogButtonType {
    public static final int e_Normal = ZFUIDialogButtonType.native_e_Normal();
    private native static int native_e_Normal();
    public static final int e_Yes = ZFUIDialogButtonType.native_e_Yes();
    private native static int native_e_Yes();
    public static final int e_No = ZFUIDialogButtonType.native_e_No();
    private native static int native_e_No();
    public static final int e_Cancel = ZFUIDialogButtonType.native_e_Cancel();
    private native static int native_e_Cancel();
    public static final int e_Destructive = ZFUIDialogButtonType.native_e_Destructive();
    private native static int native_e_Destructive();
}

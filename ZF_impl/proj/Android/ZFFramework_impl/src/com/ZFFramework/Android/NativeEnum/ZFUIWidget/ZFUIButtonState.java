/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIWidget;

public class ZFUIButtonState {
    public static final int e_Normal = ZFUIButtonState.native_e_Normal();
    private native static int native_e_Normal();
    public static final int e_Highlighted = ZFUIButtonState.native_e_Highlighted();
    private native static int native_e_Highlighted();
    public static final int e_Checked = ZFUIButtonState.native_e_Checked();
    private native static int native_e_Checked();
    public static final int e_CheckedHighlighted = ZFUIButtonState.native_e_CheckedHighlighted();
    private native static int native_e_CheckedHighlighted();
    public static final int e_Disabled = ZFUIButtonState.native_e_Disabled();
    private native static int native_e_Disabled();
}

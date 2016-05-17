/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUIMouseButton {
    public static final int e_LeftButton = ZFUIMouseButton.native_e_LeftButton();
    private static native int native_e_LeftButton();
    public static final int e_RightButton = ZFUIMouseButton.native_e_RightButton();
    private static native int native_e_RightButton();
    public static final int e_CenterButton = ZFUIMouseButton.native_e_CenterButton();
    private static native int native_e_CenterButton();
}

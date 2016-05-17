/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFMethodType {
    public static final int ZFMethodTypeClassStatic = ZFMethodType.native_ZFMethodTypeClassStatic();
    private native static int native_ZFMethodTypeClassStatic();
    public static final int ZFMethodTypeClassMember = ZFMethodType.native_ZFMethodTypeClassMember();
    private native static int native_ZFMethodTypeClassMember();
    public static final int ZFMethodTypeRawFunction = ZFMethodType.native_ZFMethodTypeRawFunction();
    private native static int native_ZFMethodTypeRawFunction();
}

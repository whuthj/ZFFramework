/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUtility;

public class ZFOperationCacheMatchAction {
    public static final int e_Unspecified = ZFOperationCacheMatchAction.native_e_Unspecified();
    private native static int native_e_Unspecified();
    public static final int e_NotifyFinish = ZFOperationCacheMatchAction.native_e_NotifyFinish();
    private native static int native_e_NotifyFinish();
    public static final int e_NotifyFinishThenRemove = ZFOperationCacheMatchAction.native_e_NotifyFinishThenRemove();
    private native static int native_e_NotifyFinishThenRemove();
    public static final int e_StartNew = ZFOperationCacheMatchAction.native_e_StartNew();
    private native static int native_e_StartNew();
    public static final int e_StartNewThenRemove = ZFOperationCacheMatchAction.native_e_StartNewThenRemove();
    private native static int native_e_StartNewThenRemove();
}

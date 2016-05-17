/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUtility;

public class ZFOperationTaskDuplicateAction {
    public static final int e_Unspecified = ZFOperationTaskDuplicateAction.native_e_Unspecified();
    private native static int native_e_Unspecified();
    public static final int e_Merge = ZFOperationTaskDuplicateAction.native_e_Merge();
    private native static int native_e_Merge();
    public static final int e_NewTask = ZFOperationTaskDuplicateAction.native_e_NewTask();
    private native static int native_e_NewTask();
    public static final int e_Ignore = ZFOperationTaskDuplicateAction.native_e_Ignore();
    private native static int native_e_Ignore();
}

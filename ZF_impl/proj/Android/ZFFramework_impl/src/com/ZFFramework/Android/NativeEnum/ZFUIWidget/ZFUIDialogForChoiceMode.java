/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIWidget;

public class ZFUIDialogForChoiceMode {
    public static final int e_ChoiceSingle = ZFUIDialogForChoiceMode.native_e_ChoiceSingle();
    private native static int native_e_ChoiceSingle();
    public static final int e_ChoiceSingleQuickly = ZFUIDialogForChoiceMode.native_e_ChoiceSingleQuickly();
    private native static int native_e_ChoiceSingleQuickly();
    public static final int e_ChoiceMultiple = ZFUIDialogForChoiceMode.native_e_ChoiceMultiple();
    private native static int native_e_ChoiceMultiple();

    public static final int e_Default = ZFUIDialogForChoiceMode.native_e_Default();
    private native static int native_e_Default();
}


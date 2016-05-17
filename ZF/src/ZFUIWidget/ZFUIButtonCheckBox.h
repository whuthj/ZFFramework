/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIButtonCheckBox.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonCheckBox_h_
#define _ZFI_ZFUIButtonCheckBox_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonCheckBoxStyle
/**
 * @brief style for #ZFUIButtonCheckBox
 */
zfinterface ZF_ENV_EXPORT ZFUIButtonCheckBoxStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIButtonCheckBoxStyle, ZFUIButtonBasicStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonCheckBoxStyle, ZFUIButtonBasicStyle)

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN(zfbool, buttonCheckable)
    {
        propertyValue = zftrue;
    }

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_ASSIGN_NO_AUTO_INIT(ZFUIAlignFlags, buttonContentAlign)
    {
        propertyValue = ZFUIAlign::e_LeftInner;
    }

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUIImageViewStyle *, buttonIconStyleNormal)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonCheckBox_IconNormal.png")).to<ZFUIImage *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, buttonIconStyleHighlighted)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonCheckBox_IconHighlighted.png")).to<ZFUIImage *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, buttonIconStyleChecked)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonCheckBox_IconChecked.png")).to<ZFUIImage *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, buttonIconStyleCheckedHighlighted)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonCheckBox_IconCheckedHighlighted.png")).to<ZFUIImage *>());
    }
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIButtonCheckBoxStyle, ZFUIButtonBasicStyle)

// ============================================================
// ZFUIButtonCheckBox
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZF_ENV_EXPORT ZFUIButtonCheckBox : zfextends ZFUIButtonBasic, zfimplements ZFUIButtonCheckBoxStyle
{
    ZFOBJECT_DECLARE(ZFUIButtonCheckBox, ZFUIButtonBasic)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonCheckBoxStyle)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonCheckBox_h_


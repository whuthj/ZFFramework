/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIButtonRatio.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonRatio_h_
#define _ZFI_ZFUIButtonRatio_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonRatioStyle
/**
 * @brief style for #ZFUIButtonRatio
 */
zfinterface ZF_ENV_EXPORT ZFUIButtonRatioStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIButtonRatioStyle, ZFUIButtonBasicStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonRatioStyle, ZFUIButtonBasicStyle)

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
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonRatio_IconNormal.png")).to<ZFUIImage *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, buttonIconStyleHighlighted)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonRatio_IconHighlighted.png")).to<ZFUIImage *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, buttonIconStyleChecked)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonRatio_IconChecked.png")).to<ZFUIImage *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, buttonIconStyleCheckedHighlighted)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageRes(zfText("ZFFramework/ZFUIWidget/ZFUIButtonRatio_IconCheckedHighlighted.png")).to<ZFUIImage *>());
    }
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIButtonRatioStyle, ZFUIButtonBasicStyle)

// ============================================================
// ZFUIButtonRatio
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZF_ENV_EXPORT ZFUIButtonRatio : zfextends ZFUIButtonBasic, zfimplements ZFUIButtonRatioStyle
{
    ZFOBJECT_DECLARE(ZFUIButtonRatio, ZFUIButtonBasic)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonRatioStyle)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonRatio_h_


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048AppButton_h_
#define _ZFI_ZF2048AppButton_h_

#include "ZFUIWidget.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZF2048AppButton : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZF2048AppButton, ZFUIButtonBasic)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(ZFUISize, viewSizeMin, ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUITextViewStyle *, buttonLabelStyleNormal)
    {
        propertyValue.to<ZFUITextViewStyle *>()->textColorSet(ZFUIColorBlue);
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUITextViewStyle *, buttonLabelStyleHighlighted)
    {
        propertyValue.to<ZFUITextViewStyle *>()->textColorSet(ZFUIColorBlue);
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUITextViewStyle *, buttonLabelStyleDisabled)
    {
        propertyValue.to<ZFUITextViewStyle *>()->textColorSet(ZFUIColorBlue);
    }

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUIImageViewStyle *, buttonBackgroundStyleNormal)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageResXml(zfText("ZF2048/app/button_normal.xml")).toAny());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUIImageViewStyle *, buttonBackgroundStyleHighlighted)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageResXml(zfText("ZF2048/app/button_highlighted.xml")).toAny());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN(ZFUIImageViewStyle *, buttonBackgroundStyleDisabled)
    {
        propertyValue.to<ZFUIImageViewStyle *>()->imageContentSet(ZFUIImageResXml(zfText("ZF2048/app/button_disabled.xml")).toAny());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppButton_h_


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIDialogForInput.h
 * @brief basic dialog for user to input a short text
 */

#ifndef _ZFI_ZFUIDialogForInput_h_
#define _ZFI_ZFUIDialogForInput_h_

#include "ZFUIDialogBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogForInputStyle
/**
 * @brief style for #ZFUIDialogForInput
 */
zfinterface ZF_ENV_EXPORT ZFUIDialogForInputStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogForInputStyle, ZFUIDialogBasicStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogForInputStyle, ZFUIDialogBasicStyle)

    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, inputHintView, ZFPropertyNoInitValue)
    static void _ZFP_inputHintViewInit(ZF_IN ZFUITextViewStyle *inputHintView)
    {
        inputHintView->textColorSet(ZFUIGlobalStyle::DefaultStyle()->textColorSecondary());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, inputHintView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder));
        zfself::_ZFP_inputHintViewInit(propertyValue.to<ZFUITextViewStyle *>());
    }

    ZFPROPERTY_RETAIN_READONLY(ZFUITextEditStyle *, inputView, ZFPropertyNoInitValue)
    static void _ZFP_inputViewInit(ZF_IN ZFUITextEditStyle *inputView)
    {
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextEditStyle *, inputView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextEditStyleHolder));
        zfself::_ZFP_inputViewInit(propertyValue.to<ZFUITextEditStyle *>());
    }
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIDialogForInputStyle, ZFUIDialogBasicStyle)

// ============================================================
// ZFUIDialogForInput
/**
 * @brief basic dialog for input
 */
zfclass ZF_ENV_EXPORT ZFUIDialogForInput : zfextends ZFUIDialogBasic
                                         , zfimplements ZFUIDialogForInputStyle
{
    ZFOBJECT_DECLARE(ZFUIDialogForInput, ZFUIDialogBasic)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogForInputStyle)

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, inputHintView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextView));
        zfself::_ZFP_inputHintViewInit(propertyValue.to<ZFUITextViewStyle *>());
    }

    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextEditStyle *, inputView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextEdit));
        zfself::_ZFP_inputViewInit(propertyValue.to<ZFUITextEditStyle *>());
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogForInput_h_


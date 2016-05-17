/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIDialogContentBasic_Buttons.h
 * @brief buttons for #ZFUIDialogContentBasic
 */

#ifndef _ZFI_ZFUIDialogContentBasic_Buttons_h_
#define _ZFI_ZFUIDialogContentBasic_Buttons_h_

#include "ZFUIDialogBasic.h"
#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogButtonType::e_Yes
/** @brief style for #ZFUIDialogContentBasicButtonYes */
zfinterface ZF_ENV_EXPORT ZFUIDialogContentBasicButtonYesStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogContentBasicButtonYesStyle, ZFUIButtonBasicStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonYesStyle, ZFUIButtonBasicStyle)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(ZFUISize, viewSizeMin, ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIDialogContentBasicButtonYesStyle, ZFUIButtonBasicStyle)
/** @brief #ZFUIDialogButtonType::e_Yes for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonYes : zfextends ZFUIButtonBasic, zfimplements ZFUIDialogContentBasicButtonYesStyle
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonYes, ZFUIButtonBasic)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContentBasicButtonYesStyle)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(ZFUISize, viewSizeMin, ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))
};

// ============================================================
// ZFUIDialogButtonType::e_No
/** @brief style for #ZFUIDialogContentBasicButtonNo */
zfinterface ZF_ENV_EXPORT ZFUIDialogContentBasicButtonNoStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogContentBasicButtonNoStyle, ZFUIButtonBasicStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonNoStyle, ZFUIButtonBasicStyle)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(ZFUISize, viewSizeMin, ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIDialogContentBasicButtonNoStyle, ZFUIButtonBasicStyle)
/** @brief #ZFUIDialogButtonType::e_No for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonNo : zfextends ZFUIButtonBasic, zfimplements ZFUIDialogContentBasicButtonNoStyle
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonNo, ZFUIButtonBasic)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContentBasicButtonNoStyle)
};

// ============================================================
// ZFUIDialogButtonType::e_Cancel
/** @brief style for #ZFUIDialogContentBasicButtonCancel */
zfinterface ZF_ENV_EXPORT ZFUIDialogContentBasicButtonCancelStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogContentBasicButtonCancelStyle, ZFUIButtonBasicStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonCancelStyle, ZFUIButtonBasicStyle)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(ZFUISize, viewSizeMin, ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIDialogContentBasicButtonCancelStyle, ZFUIButtonBasicStyle)
/** @brief #ZFUIDialogButtonType::e_Cancel for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonCancel : zfextends ZFUIButtonBasic, zfimplements ZFUIDialogContentBasicButtonCancelStyle
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonCancel, ZFUIButtonBasic)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContentBasicButtonCancelStyle)
};

// ============================================================
// ZFUIDialogButtonType::e_Destructive
/** @brief style for #ZFUIDialogContentBasicButtonDestructive */
zfinterface ZF_ENV_EXPORT ZFUIDialogContentBasicButtonDestructiveStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogContentBasicButtonDestructiveStyle, ZFUIButtonBasicStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonDestructiveStyle, ZFUIButtonBasicStyle)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(ZFUISize, viewSizeMin, ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIDialogContentBasicButtonDestructiveStyle, ZFUIButtonBasicStyle)
/** @brief #ZFUIDialogButtonType::e_Destructive for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonDestructive : zfextends ZFUIButtonBasic, zfimplements ZFUIDialogContentBasicButtonDestructiveStyle
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonDestructive, ZFUIButtonBasic)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContentBasicButtonDestructiveStyle)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogContentBasic_Buttons_h_


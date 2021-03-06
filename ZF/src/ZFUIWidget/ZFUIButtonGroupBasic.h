/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIButtonGroupBasic.h
 * @brief abstract button group management
 */

#ifndef _ZFI_ZFUIButtonGroupBasic_h_
#define _ZFI_ZFUIButtonGroupBasic_h_

#include "ZFUIButtonGroup.h"
#include "ZFUILinearLayout.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonGroupBasicStyle
/**
 * @brief style for #ZFUIButtonGroupBasic
 */
zfinterface ZF_ENV_EXPORT ZFUIButtonGroupBasicStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIButtonGroupBasicStyle, ZFUILinearLayoutStyle, ZFUIButtonGroup)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonGroupBasicStyle, ZFUILinearLayoutStyle)
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIButtonGroupBasicStyle, ZFUILinearLayoutStyle)

// ============================================================
// ZFUIButtonGroupBasic
/**
 * @brief basic button group which layout buttons as #ZFUILinearLayout
 */
zfclass ZF_ENV_EXPORT ZFUIButtonGroupBasic : zfextends ZFUILinearLayout, zfimplements ZFUIButtonGroupBasicStyle, zfimplements ZFUIButtonGroup
{
    ZFOBJECT_DECLARE(ZFUIButtonGroupBasic, ZFUILinearLayout)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonGroupBasicStyle, ZFUIButtonGroup)

protected:
    zfoverride
    virtual inline void buttonOnAdd(ZF_IN ZFUIButton *button,
                                    ZF_IN zfindex buttonIndex)
    {
        this->childAdd(button, zfnull, buttonIndex);
        zfsuperI(ZFUIButtonGroup)::buttonOnAdd(button, buttonIndex);
    }
    zfoverride
    virtual inline void buttonOnRemove(ZF_IN ZFUIButton *button,
                                       ZF_IN zfindex buttonIndex)
    {
        this->childRemoveAtIndex(buttonIndex);
        zfsuperI(ZFUIButtonGroup)::buttonOnRemove(button, buttonIndex);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonGroupBasic_h_


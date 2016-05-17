/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIListCellViewBasic.h
 * @brief basic list cell view with icon and label
 */

#ifndef _ZFI_ZFUIListCellViewBasic_h_
#define _ZFI_ZFUIListCellViewBasic_h_

#include "ZFUIListCellView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT void _ZFP_ZFUIListCellViewBasic_cellIconInit(ZF_IN ZFUIViewStyle *cellIcon);
extern ZF_ENV_EXPORT void _ZFP_ZFUIListCellViewBasic_cellLabelMainInit(ZF_IN ZFUITextViewStyle *cellLabelMain);
extern ZF_ENV_EXPORT void _ZFP_ZFUIListCellViewBasic_cellLabelSubInit(ZF_IN ZFUITextViewStyle *cellLabelSub);
// ============================================================
// ZFUIListCellViewBasicStyle
/**
 * @brief style for #ZFUIListCellViewBasic
 */
zfinterface ZF_ENV_EXPORT ZFUIListCellViewBasicStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIListCellViewBasicStyle, ZFUIListCellViewStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIListCellViewBasicStyle, ZFUIListCellViewStyle)

    /**
     * @brief cell's icon
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, cellIcon, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, cellIcon)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder));
        _ZFP_ZFUIListCellViewBasic_cellIconInit(propertyValue.to<ZFUIViewStyle *>());
    }

    /**
     * @brief cell's main label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, cellLabelMain, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, cellLabelMain)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder));
        _ZFP_ZFUIListCellViewBasic_cellLabelMainInit(propertyValue.to<ZFUITextViewStyle *>());
    }

    /**
     * @brief cell's sub label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, cellLabelSub, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, cellLabelSub)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder));
        _ZFP_ZFUIListCellViewBasic_cellLabelSubInit(propertyValue.to<ZFUITextViewStyle *>());
    }
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIListCellViewBasicStyle, ZFUIListCellViewStyle)

// ============================================================
// ZFUIListCellViewBasic
zfclassFwd _ZFP_ZFUIListCellViewBasicPrivate;
/**
 * @brief basic list cell view with icon and label
 */
zfclass ZF_ENV_EXPORT ZFUIListCellViewBasic : zfextends ZFUIListCellView, zfimplements ZFUIListCellViewBasicStyle
{
    ZFOBJECT_DECLARE(ZFUIListCellViewBasic, ZFUIListCellView)
    ZFIMPLEMENTS_DECLARE(ZFUIListCellViewBasicStyle)

public:
    // ============================================================
    // styles
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUIImageViewStyle *, cellIcon)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUIImageView));
        _ZFP_ZFUIListCellViewBasic_cellIconInit(propertyValue.to<ZFUIViewStyle *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, cellLabelMain)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextView));
        _ZFP_ZFUIListCellViewBasic_cellLabelMainInit(propertyValue.to<ZFUITextViewStyle *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, cellLabelSub)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextView));
        _ZFP_ZFUIListCellViewBasic_cellLabelSubInit(propertyValue.to<ZFUITextViewStyle *>());
    }

public:
    /**
     * @brief container for icon, you may add your own custom view
     */
    zffinal ZFUIView *cellIconContainer(void);
    /**
     * @brief container for center item, you may add your own custom view
     */
    zffinal ZFUIView *cellCenterContainer(void);
    /**
     * @brief container for main label, you may add your own custom view
     */
    zffinal ZFUIView *cellLabelMainContainer(void);
    /**
     * @brief container for sub label, you may add your own custom view
     */
    zffinal ZFUIView *cellLabelSubContainer(void);
    /**
     * @brief container for accessory, you may add your own custom view
     */
    zffinal ZFUIView *cellAccessoryContainer(void);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectPropertyValueOnInit(ZF_IN const ZFProperty *property);

private:
    _ZFP_ZFUIListCellViewBasicPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCellViewBasic_h_


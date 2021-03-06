/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIDialogContentBasic.h
 * @brief basic content for #ZFUIDialogBasic
 */

#ifndef _ZFI_ZFUIDialogContentBasic_h_
#define _ZFI_ZFUIDialogContentBasic_h_

#include "ZFUIDialogBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogContentBasicStyle
/**
 * @brief style for #ZFUIDialogContentBasic
 */
zfinterface ZF_ENV_EXPORT ZFUIDialogContentBasicStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIDialogContentBasicStyle, ZFUIViewStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicStyle, ZFUIViewStyle)

    /**
     * @brief dialog title view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, titleTextView, ZFPropertyNoInitValue)
    static void _ZFP_titleTextViewInit(ZF_IN ZFUITextViewStyle *titleTextView)
    {
        titleTextView->textSizeSet(ZFUIGlobalStyle::DefaultStyle()->textSizeBig());
        titleTextView->textAppearanceSet(ZFUITextAppearance::e_Bold);
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, titleTextView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder));
        zfself::_ZFP_titleTextViewInit(propertyValue.to<ZFUITextViewStyle *>());
    }

    /**
     * @brief dialog content view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, contentTextView, ZFPropertyNoInitValue)
    static void _ZFP_contentTextViewInit(ZF_IN ZFUITextViewStyle *contentTextView)
    {
        contentTextView->textSingleLineSet(zffalse);
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, contentTextView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder));
        zfself::_ZFP_contentTextViewInit(propertyValue.to<ZFUITextViewStyle *>());
    }
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIDialogContentBasicStyle, ZFUIViewStyle)

// ============================================================
// ZFUIDialogContentBasic
zfclassFwd _ZFP_ZFUIDialogContentBasicPrivate;
/**
 * @brief basic content for #ZFUIDialogBasic
 */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasic : zfextends ZFUIView
                                             , zfimplements ZFUIDialogContentBasicStyle
                                             , zfimplements ZFUIDialogContent
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasic, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContentBasicStyle, ZFUIDialogContent)

public:
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN_NO_AUTO_INIT(ZFUISize, viewSizeMin, ZFUISizeMake(
        ZFUIGlobalStyle::DefaultStyle()->itemSizeDialogWidth(),
        ZFUIGlobalStyle::DefaultStyle()->itemSizeDialog()))
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, titleTextView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextView));
        zfself::_ZFP_titleTextViewInit(propertyValue.to<ZFUITextViewStyle *>());
    }
    ZFPROPERTY_OVERRIDE_INIT_STEP_DECLARE_RETAIN_NO_AUTO_INIT(ZFUITextViewStyle *, contentTextView)
    {
        propertyValue = zfautoObjectCreateWithoutLeakTest(zflineAllocWithoutLeakTest(ZFUITextView));
        zfself::_ZFP_contentTextViewInit(propertyValue.to<ZFUITextViewStyle *>());
    }

public:
    // ============================================================
    // title
    zfoverride
    virtual ZFUIView *dialogTitleContainer(void);
    zfoverride
    virtual ZFUITextViewStyle *dialogTitleView(void);

    // ============================================================
    // content
    zfoverride
    virtual ZFUIView *dialogContentContainer(void);
    zfoverride
    virtual ZFUITextViewStyle *dialogContentView(void);

    // ============================================================
    // button
    zfoverride
    virtual ZFUIView *dialogButtonContainer(void);
    zfoverride
    virtual ZFUIButton *dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue);
    zfoverride
    virtual const zfchar *dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType);
    zfoverride
    virtual void dialogButtonTextSet(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN const zfchar *text);
    zfoverride
    virtual void dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType);

    // ============================================================
    // button
    zfoverride
    virtual zfindex dialogButtonCount(void);
    zfoverride
    virtual ZFUIButton *dialogButtonAtIndex(ZF_IN zfindex index);
    zfoverride
    virtual zfindex dialogButtonFind(ZF_IN ZFUIButton *dialogButton);
    zfoverride
    virtual void dialogButtonAdd(ZF_IN ZFUIButton *button,
                                 ZF_IN_OPT zfindex atIndex = zfindexMax);
    zfoverride
    virtual void dialogButtonRemove(ZF_IN ZFUIButton *button);
    zfoverride
    virtual void dialogButtonRemoveAtIndex(ZF_IN zfindex index);
    zfoverride
    virtual void dialogButtonRemoveAll(void);

    // ============================================================
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void internalForegroundViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);

private:
    _ZFP_ZFUIDialogContentBasicPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogContentBasic_h_


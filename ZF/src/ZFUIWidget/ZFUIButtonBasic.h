/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIButtonBasic.h
 * @brief basic button
 */

#ifndef _ZFI_ZFUIButtonBasic_h_
#define _ZFI_ZFUIButtonBasic_h_

#include "ZFUIButton.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonBasicStyle
/**
 * @brief style for #ZFUIButtonBasic
 */
zfinterface ZF_ENV_EXPORT ZFUIButtonBasicStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIButtonBasicStyle, ZFUIButtonStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonBasicStyle, ZFUIButtonStyle)

public:
    // ============================================================
    // content layout
    /**
     * @brief button icon's orientation (relative to label), #ZFUIOrientation::e_Left by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIOrientationEnum, buttonContentIconPosition, ZFPropertyInitValue(ZFUIOrientation::e_Left))
    /**
     * @brief button content(icon, label)'s layout align, #ZFUIAlign::e_Center by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, buttonContentAlign, ZFPropertyInitValue(ZFUIAlign::e_Center))
    /**
     * @brief button content(icon, label)'s layout margin, #ZFUIGlobalStyle::itemMargin by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, buttonContentMargin, ZFPropertyInitValue(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin())))
    /**
     * @brief button icon and label's space, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, buttonContentSpace, ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->itemSpace()))

    /**
     * @brief button background's layout margin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, buttonBackgroundMargin, ZFPropertyInitValue(ZFUIMarginZero))

    // ============================================================
    // label
    /** @brief label style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, buttonLabelStyleNormal,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder)))
    ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE(ZFUITextViewStyle *, buttonLabelStyleNormal)
    {
        ZFUITextViewStyle *v = propertyValue.to<ZFUITextViewStyle *>();
        v->textColorSet(ZFUIGlobalStyle::DefaultStyle()->controlColorNormal());
    }
    /** @brief label style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, buttonLabelStyleHighlighted,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder)))
    ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE(ZFUITextViewStyle *, buttonLabelStyleHighlighted)
    {
        ZFUITextViewStyle *v = propertyValue.to<ZFUITextViewStyle *>();
        v->textColorSet(ZFUIGlobalStyle::DefaultStyle()->controlColorHighlighted());
    }
    /** @brief label style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, buttonLabelStyleChecked,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder)))
    ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE(ZFUITextViewStyle *, buttonLabelStyleChecked)
    {
        ZFUITextViewStyle *v = propertyValue.to<ZFUITextViewStyle *>();
        v->textColorSet(ZFUIGlobalStyle::DefaultStyle()->controlColorChecked());
    }
    /** @brief label style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, buttonLabelStyleCheckedHighlighted,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder)))
    ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE(ZFUITextViewStyle *, buttonLabelStyleCheckedHighlighted)
    {
        ZFUITextViewStyle *v = propertyValue.to<ZFUITextViewStyle *>();
        v->textColorSet(ZFUIGlobalStyle::DefaultStyle()->controlColorCheckedHighlighted());
    }
    /** @brief label style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextViewStyle *, buttonLabelStyleDisabled,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUITextViewStyleHolder)))
    ZFPROPERTY_RAW_INIT_STEP_RETAIN_DECLARE(ZFUITextViewStyle *, buttonLabelStyleDisabled)
    {
        ZFUITextViewStyle *v = propertyValue.to<ZFUITextViewStyle *>();
        v->textColorSet(ZFUIGlobalStyle::DefaultStyle()->controlColorDisabled());
    }

    // ============================================================
    // icon
    /** @brief icon style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonIconStyleNormal,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief icon style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonIconStyleHighlighted,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief icon style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonIconStyleChecked,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief icon style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonIconStyleCheckedHighlighted,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief icon style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonIconStyleDisabled,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))

    // ============================================================
    // background
    /** @brief background style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonBackgroundStyleNormal,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief background style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonBackgroundStyleHighlighted,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief background style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonBackgroundStyleChecked,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief background style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonBackgroundStyleCheckedHighlighted,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))
    /** @brief background style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageViewStyle *, buttonBackgroundStyleDisabled,
                               ZFPropertyInitValue(zflineAllocWithoutLeakTest(ZFUIImageViewStyleHolder)))

    // ============================================================
    // util method
public:
    /**
     * @brief util method to get button label's style
     */
    virtual ZFUITextViewStyle *buttonLabelStyle(ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        switch(forState)
        {
            case ZFUIButtonState::e_Normal:
                return this->buttonLabelStyleNormal();
            case ZFUIButtonState::e_Highlighted:
                return this->buttonLabelStyleHighlighted();
            case ZFUIButtonState::e_Checked:
                return this->buttonLabelStyleChecked();
            case ZFUIButtonState::e_CheckedHighlighted:
                return this->buttonLabelStyleCheckedHighlighted();
            case ZFUIButtonState::e_Disabled:
                return this->buttonLabelStyleDisabled();
            default:
                zfCoreCriticalShouldNotGoHere();
                return zfnull;
        }
    }
    /**
     * @brief util method to get button icon's style
     */
    virtual ZFUIImageViewStyle *buttonIconStyle(ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        switch(forState)
        {
            case ZFUIButtonState::e_Normal:
                return this->buttonIconStyleNormal();
            case ZFUIButtonState::e_Highlighted:
                return this->buttonIconStyleHighlighted();
            case ZFUIButtonState::e_Checked:
                return this->buttonIconStyleChecked();
            case ZFUIButtonState::e_CheckedHighlighted:
                return this->buttonIconStyleCheckedHighlighted();
            case ZFUIButtonState::e_Disabled:
                return this->buttonIconStyleDisabled();
            default:
                zfCoreCriticalShouldNotGoHere();
                return zfnull;
        }
    }
    /**
     * @brief util method to get button icon's style
     */
    virtual ZFUIImageViewStyle *buttonBackgroundStyle(ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        switch(forState)
        {
            case ZFUIButtonState::e_Normal:
                return this->buttonBackgroundStyleNormal();
            case ZFUIButtonState::e_Highlighted:
                return this->buttonBackgroundStyleHighlighted();
            case ZFUIButtonState::e_Checked:
                return this->buttonBackgroundStyleChecked();
            case ZFUIButtonState::e_CheckedHighlighted:
                return this->buttonBackgroundStyleCheckedHighlighted();
            case ZFUIButtonState::e_Disabled:
                return this->buttonBackgroundStyleDisabled();
            default:
                zfCoreCriticalShouldNotGoHere();
                return zfnull;
        }
    }

    /**
     * @brief util method to get button label's text
     */
    virtual ZFString *buttonLabelText(ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        return this->buttonLabelStyle(forState)->textContent();
    }
    /**
     * @brief util method to set button label's text
     */
    virtual void buttonLabelTextSet(ZF_IN ZFString *text,
                                    ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        this->buttonLabelStyle(forState)->textContentSet(text);
    }
    /**
     * @brief util method to get button label's text
     */
    virtual const zfchar *buttonLabelTextString(ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        return this->buttonLabelStyle(forState)->textContentString();
    }
    /**
     * @brief util method to set button label's text
     */
    virtual void buttonLabelTextStringSet(ZF_IN const zfchar *text,
                                          ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        this->buttonLabelStyle(forState)->textContentStringSet(text);
    }

    /**
     * @brief util method to get button icon's image
     */
    virtual ZFUIImage *buttonIconImage(ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        return this->buttonIconStyle(forState)->imageContent();
    }
    /**
     * @brief util method to set button icon's image
     */
    virtual void buttonIconImageSet(ZF_IN ZFUIImage *image,
                                    ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        this->buttonIconStyle(forState)->imageContentSet(image);
    }

    /**
     * @brief util method to get button background's image
     */
    virtual ZFUIImage *buttonBackgroundImage(ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        return this->buttonBackgroundStyle(forState)->imageContent();
    }
    /**
     * @brief util method to set button background's image
     */
    virtual void buttonBackgroundImageSet(ZF_IN ZFUIImage *image,
                                          ZF_IN_OPT ZFUIButtonStateEnum forState = ZFUIButtonState::e_Normal)
    {
        this->buttonBackgroundStyle(forState)->imageContentSet(image);
    }
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIButtonBasicStyle, ZFUIButtonStyle)

// ============================================================
// ZFUIButtonBasic
zfclassFwd _ZFP_ZFUIButtonBasicPrivate;
/**
 * @brief basic button, see #ZFUIButton
 *
 * a basic button contains:
 * -  label: to show button's text
 * -  icon: to show button's icon
 * -  background: to show button's background
 *
 * to change the button's property,
 * you must use ZFUIButtonBasicStyle's styleable property,
 * such as #ZFUIButtonBasicStyle::buttonLabelStyleNormal\n
 * \n
 * specially, those property's normal state's value would be used
 * if other state doesn't supply value:
 * -  label's #ZFUITextViewStyle::textContent
 * -  icon's #ZFUIImageViewStyle::imageContent
 * -  background's #ZFUIImageViewStyle::imageContent
 *
 * @note button's components (label, icon, background) would be created
 *   only if accessed or text/image has been set,
 *   or whenever you called #prepareButtonLabel/#prepareButtonIcon/#prepareButtonBackground
 */
zfclass ZF_ENV_EXPORT ZFUIButtonBasic : zfextends ZFUIButton, zfimplements ZFUIButtonBasicStyle
{
    ZFOBJECT_DECLARE(ZFUIButtonBasic, ZFUIButton)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonBasicStyle)

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);

protected:
    zfoverride
    virtual void buttonStateOnChange(void);

protected:
    /**
     * @brief must be type of #ZFUITextViewStyle, by default it's #ZFUITextView
     */
    virtual const ZFClass *buttonLabelClass(void);
    /**
     * @brief must be type of #ZFUIImageViewStyle, by default it's #ZFUIImageView
     */
    virtual const ZFClass *buttonIconClass(void);
    /**
     * @brief must be type of #ZFUIImageViewStyle, by default it's #ZFUIImageView
     */
    virtual const ZFClass *buttonBackgroundClass(void);

public:
    /**
     * @brief button's label view, style of this view must be changed by #ZFUIButtonBasicStyle::buttonLabelStyleNormal, etc
     *
     * @note property may not updated immediately,
     *   if really want to update manually,
     *   use #ZFUIButton::buttonStateUpdate
     */
    virtual ZFUITextViewStyle *buttonLabel(void);
    /**
     * @brief button's icon view, style of this view must be changed by #ZFUIButtonBasicStyle::buttonIconStyleNormal, etc
     *
     * @note property may not updated immediately,
     *   if really want to update manually,
     *   use #ZFUIButton::buttonStateUpdate
     */
    virtual ZFUIImageViewStyle *buttonIcon(void);
    /**
     * @brief button's background view, style of this view must be changed by #ZFUIButtonBasicStyle::buttonBackgroundStyleNormal, etc
     *
     * @note property may not updated immediately,
     *   if really want to update manually,
     *   use #ZFUIButton::buttonStateUpdate
     */
    virtual ZFUIImageViewStyle *buttonBackground(void);

public:
    /**
     * @brief prepare button label
     *
     * by default, button component (label/icon/background) won't be created until it would be accessed,
     * either by access them directly by #buttonLabel,
     * or change #buttonLabelStyle's property\n
     * you may manually call this method to create the button component, if necessary
     */
    zffinal void prepareButtonLabel(void);
    /**
     * @brief see #prepareButtonLabel
     */
    zffinal void prepareButtonIcon(void);
    /**
     * @brief see #prepareButtonLabel
     */
    zffinal void prepareButtonBackground(void);

protected:
    zfoverride
    virtual void objectPropertyValueOnInit(ZF_IN const ZFProperty *property);

private:
    _ZFP_ZFUIButtonBasicPrivate *d;
    friend zfclassFwd _ZFP_ZFUIButtonBasicPrivate;
};

// ============================================================
ZFLANGAPPLY_IMPL(ZFUIButtonBasicStyle, ZFUIButtonBasicTextNormal, {obj->buttonLabelTextStringSet(langValue, ZFUIButtonState::e_Normal);})
ZFLANGAPPLY_IMPL(ZFUIButtonBasicStyle, ZFUIButtonBasicTextHighlighted, {obj->buttonLabelTextStringSet(langValue, ZFUIButtonState::e_Highlighted);})
ZFLANGAPPLY_IMPL(ZFUIButtonBasicStyle, ZFUIButtonBasicTextChecked, {obj->buttonLabelTextStringSet(langValue, ZFUIButtonState::e_Checked);})
ZFLANGAPPLY_IMPL(ZFUIButtonBasicStyle, ZFUIButtonBasicTextCheckedHighlighted, {obj->buttonLabelTextStringSet(langValue, ZFUIButtonState::e_CheckedHighlighted);})
ZFLANGAPPLY_IMPL(ZFUIButtonBasicStyle, ZFUIButtonBasicTextDisabled, {obj->buttonLabelTextStringSet(langValue, ZFUIButtonState::e_Disabled);})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonBasic_h_


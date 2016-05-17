/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIButton.h
 * @brief abstract button
 */

#ifndef _ZFI_ZFUIButton_h_
#define _ZFI_ZFUIButton_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonState
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIButtonStateEnum value="value">
 *   </ZFUIButtonStateEnum>
 * @endcode
 */
#define ZFPropertyTypeId_ZFUIButtonStateEnum zfText("ZFUIButtonStateEnum")
/**
 * @brief button state
 */
ZFENUM_BEGIN(ZFUIButtonState)
    ZFENUM_VALUE(Normal) /**< @brief normal state */
    ZFENUM_VALUE(Highlighted) /**< @brief pressed state */
    ZFENUM_VALUE(Checked) /**< @brief checked state */
    ZFENUM_VALUE(CheckedHighlighted) /**< @brief pressed when in checked state */
    ZFENUM_VALUE(Disabled) /**< @brief disabled state */
ZFENUM_SEPARATOR(ZFUIButtonState)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Highlighted)
    ZFENUM_VALUE_REGISTER(Checked)
    ZFENUM_VALUE_REGISTER(CheckedHighlighted)
    ZFENUM_VALUE_REGISTER(Disabled)
ZFENUM_END(ZFUIButtonState)

// ============================================================
// ZFUIButtonStyle
/**
 * @brief style for #ZFUIButton
 */
zfinterface ZF_ENV_EXPORT ZFUIButtonStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIButtonStyle, ZFUIViewStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonStyle, ZFUIViewStyle)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewFocusable, zftrue)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(ZFUISize, viewSizeMin, ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(), ZFUIGlobalStyle::DefaultStyle()->itemSizeButton()))

public:
    /**
     * @brief true if the button is enabled, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, buttonEnable,
                                ZFPropertyInitValue(zftrue))
    /**
     * @brief true if the button is checkable button, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, buttonCheckable,
                                ZFPropertyInitValue(zffalse))
    /**
     * @brief true if the button is checked, valid only if #buttonCheckable, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, buttonChecked,
                                ZFPropertyInitValue(zffalse))
    /**
     * @brief mouse up tolerance to detect as click, (0 - #ZFUIGlobalStyle::itemSizeText) by default
     *
     * a negative value would increase the button area while mouse up,
     * so that button is easier to be clicked,
     * and a positive value would reduce the button area
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, buttonClickTolerance,
                                ZFPropertyInitValue(ZFUIMarginMake(- ZFUIGlobalStyle::DefaultStyle()->itemSizeText())))
    /**
     * @brief if two click event's interval smaller than this value,
     *   the second one would be ignored,
     *   100 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, buttonClickInterval,
                                ZFPropertyInitValue(100))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIButtonStyle, ZFUIViewStyle)

// ============================================================
// ZFUIButton
zfclassFwd _ZFP_ZFUIButtonPrivate;
/**
 * @brief abstract button
 *
 * use #ZFUIButton::EventButtonOnClick to register button click listener,
 * see #ZFObject::observerNotify for more info\n
 * #ZFUIButton is an abstract button, use its subclass to create
 */
zfclass ZF_ENV_EXPORT ZFUIButton : zfextends ZFUIView, zfimplements ZFUIButtonStyle
{
    ZFOBJECT_DECLARE(ZFUIButton, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonStyle)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button clicked
     * (for both checkable and non-checkable button)
     */
    ZFOBSERVER_EVENT(ButtonOnClick)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button state changed
     */
    ZFOBSERVER_EVENT(ButtonStateOnChange)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button mouse down,
     * param0 is the #ZFUIMouseEvent
     */
    ZFOBSERVER_EVENT(ButtonMouseOnDown)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move from outside to inside,
     * param0 is the #ZFUIMouseEvent
     */
    ZFOBSERVER_EVENT(ButtonMouseOnMoveEnter)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move from inside to outside,
     * param0 is the #ZFUIMouseEvent
     */
    ZFOBSERVER_EVENT(ButtonMouseOnMoveExit)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move inside the button,
     * param0 is the #ZFUIMouseEvent
     */
    ZFOBSERVER_EVENT(ButtonMouseOnMoveInside)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when mouse move outside the button,
     * param0 is the #ZFUIMouseEvent
     */
    ZFOBSERVER_EVENT(ButtonMouseOnMoveOutside)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button mouse up inside (clicked),
     * param0 is the #ZFUIMouseEvent
     */
    ZFOBSERVER_EVENT(ButtonMouseOnUpInside)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when button mouse up (outside or cancel),
     * param0 is the #ZFUIMouseEvent
     */
    ZFOBSERVER_EVENT(ButtonMouseOnUp)

public:
    // ============================================================
    // styles
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, buttonEnable);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, buttonCheckable);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, buttonChecked);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief util method to simulate click button
     */
    virtual void buttonSimulateClick(ZF_IN_OPT zfbool preventFrequentlyClick = zffalse);

protected:
    zfoverride
    virtual void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent);
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent);
    zfoverride
    virtual void viewFocusOnChange(void);
    zfoverride
    virtual void viewPropertyOnUpdate(void);

protected:
    /** @brief see #EventButtonOnClick */
    virtual inline void buttonOnClick(void)
    {
        this->observerNotify(ZFUIButton::EventButtonOnClick());
    }

protected:
    /** @brief see #EventButtonMouseOnDown */
    virtual void buttonMouseOnDown(ZF_IN ZFUIMouseEvent *event)
    {
        this->observerNotify(ZFUIButton::EventButtonMouseOnDown(), event);
    }
    /** @brief see #EventButtonMouseOnMoveEnter */
    virtual void buttonMouseOnMoveEnter(ZF_IN ZFUIMouseEvent *event)
    {
        this->observerNotify(ZFUIButton::EventButtonMouseOnMoveEnter(), event);
    }
    /** @brief see #EventButtonMouseOnMoveExit */
    virtual void buttonMouseOnMoveExit(ZF_IN ZFUIMouseEvent *event)
    {
        this->observerNotify(ZFUIButton::EventButtonMouseOnMoveExit(), event);
    }
    /** @brief see #EventButtonMouseOnMoveInside */
    virtual void buttonMouseOnMoveInside(ZF_IN ZFUIMouseEvent *event)
    {
        this->observerNotify(ZFUIButton::EventButtonMouseOnMoveInside(), event);
    }
    /** @brief see #EventButtonMouseOnMoveOutside */
    virtual void buttonMouseOnMoveOutside(ZF_IN ZFUIMouseEvent *event)
    {
        this->observerNotify(ZFUIButton::EventButtonMouseOnMoveOutside(), event);
    }
    /** @brief see #EventButtonMouseOnUpInside */
    virtual void buttonMouseOnUpInside(ZF_IN ZFUIMouseEvent *event)
    {
        this->observerNotify(ZFUIButton::EventButtonMouseOnUpInside(), event);
    }
    /** @brief see #EventButtonMouseOnUp */
    virtual void buttonMouseOnUp(ZF_IN ZFUIMouseEvent *event)
    {
        this->observerNotify(ZFUIButton::EventButtonMouseOnUp(), event);
    }

public:
    /**
     * @brief current button state
     */
    virtual ZFUIButtonStateEnum buttonState(void);
    /**
     * @brief manually update current button state without notify event
     */
    virtual void buttonStateUpdate(void);

protected:
    /**
     * @brief called when button state changed
     */
    virtual inline void buttonStateOnChange(void)
    {
        this->observerNotify(ZFUIButton::EventButtonStateOnChange());
    }

private:
    _ZFP_ZFUIButtonPrivate *d;
    friend zfclassFwd _ZFP_ZFUIButtonPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButton_h_


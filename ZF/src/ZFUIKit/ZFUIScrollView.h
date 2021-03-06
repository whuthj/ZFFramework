/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIScrollView.h
 * @brief scroll view
 */

#ifndef _ZFI_ZFUIScrollView_h_
#define _ZFI_ZFUIScrollView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIScrollViewStateEnum value="value">
 *   </ZFUIScrollViewStateEnum>
 * @endcode
 */
#define ZFPropertyTypeId_ZFUIScrollViewStateEnum zfText("ZFUIScrollViewStateEnum")
/**
 * @brief state for #ZFUIScrollView
 */
ZFENUM_BEGIN(ZFUIScrollViewState)
    ZFENUM_VALUE(Idle)
    ZFENUM_VALUE(Dragging)
    ZFENUM_VALUE(Scrolling)
ZFENUM_SEPARATOR(ZFUIScrollViewState)
    ZFENUM_VALUE_REGISTER(Idle)
    ZFENUM_VALUE_REGISTER(Dragging)
    ZFENUM_VALUE_REGISTER(Scrolling)
ZFENUM_END(ZFUIScrollViewState)

// ============================================================
// ZFUIScrollViewStyle
/**
 * @brief style for #ZFUIScrollView
 */
zfinterface ZF_ENV_EXPORT ZFUIScrollViewStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIScrollViewStyle, ZFUIViewStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIScrollViewStyle, ZFUIViewStyle)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewMouseHoverEventEnable, zftrue)

public:
    /**
     * @brief whether the scroll view is scrollable, zftrue by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollEnable,
                                ZFPropertyInitValue(zftrue))

    /**
     * @brief if content is larger than scroll view, make it scrollable even if reaches edge, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceHorizontal,
                                ZFPropertyInitValue(zftrue))
    /**
     * @brief if content is larger than scroll view, make it scrollable even if reaches edge, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceVertical,
                                ZFPropertyInitValue(zftrue))
    /**
     * @brief scrollable even if content is not larger than scrollview, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceHorizontalAlways,
                                ZFPropertyInitValue(zffalse))
    /**
     * @brief scrollable even if content is not larger than scrollview, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceVerticalAlways,
                                ZFPropertyInitValue(zffalse))

    /**
     * @brief whether limit scroll horizontal or vertical only, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollAlignToAxis,
                                ZFPropertyInitValue(zffalse))
    /**
     * @brief whether align to page when scroll, false by default
     *
     * if true, during scrolling, scroll view would try to stop at integer multiples of scroll view's size,
     * excluding scroll thumb's size\n
     * if align to page, #scrollAlignToAxis would be activated no matter what value of it
     */
    ZFPROPERTY_ASSIGN(zfbool, scrollAlignToPageHorizontal)
    /**
     * @brief whether align to page when scroll, false by default
     *
     * if true, during scrolling, scroll view would try to stop at integer multiples of scroll view's size,
     * excluding scroll thumb's size\n
     * if align to page, #scrollAlignToAxis would be activated no matter what value of it
     */
    ZFPROPERTY_ASSIGN(zfbool, scrollAlignToPageVertical)

    /**
     * @brief scroll view's content frame
     *
     * change this value would have no scroll animation,
     * and any of previous scroll animation would be stopped\n
     * use #ZFUIScrollView::scrollContentFrameSetAnimated if you want to start scroll animation,
     * or #ZFUIScrollView::scrollContentFrameSetWhileAnimating if you want to change frame
     * and keep previous scroll animation
     */
    ZFPROPERTY_ASSIGN(ZFUIRect, scrollContentFrame)
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIScrollViewStyle, ZFUIViewStyle)

zfclassFwd _ZFP_ZFUIScrollViewPrivate;
// ============================================================
// ZFUIScrollView
/**
 * @brief native scroll view
 *
 * note that background view of scroll view and all other views behind scroll view
 * are ensured won't receive mouse event
 */
zfclass ZF_ENV_EXPORT ZFUIScrollView : zfextends ZFUIView, zfimplements ZFUIScrollViewStyle
{
    ZFOBJECT_DECLARE(ZFUIScrollView, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZFUIScrollViewStyle)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when drag begin
     */
    ZFOBSERVER_EVENT(ScrollOnDragBegin)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when drag
     */
    ZFOBSERVER_EVENT(ScrollOnDrag)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when drag end
     */
    ZFOBSERVER_EVENT(ScrollOnDragEnd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scroll begin
     */
    ZFOBSERVER_EVENT(ScrollOnScrollBegin)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scroll
     */
    ZFOBSERVER_EVENT(ScrollOnScroll)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scroll end
     */
    ZFOBSERVER_EVENT(ScrollOnScrollEnd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #scrollThumbMargin changed
     */
    ZFOBSERVER_EVENT(ScrollThumbMarginOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #scrollContentFrame changed
     */
    ZFOBSERVER_EVENT(ScrollContentFrameOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when auto scroll started
     */
    ZFOBSERVER_EVENT(ScrollAutoScrollOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when auto scroll stopped
     */
    ZFOBSERVER_EVENT(ScrollAutoScrollOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scrolled by user, activated if any of these methods called manually or event occurred:
     * -  #scrollByPoint
     * -  #scrollBySpeed
     * -  #scrollContentFrameSet or #scrollContentFrameSetAnimated
     * -  #autoScrollStartX series
     * -  drag begin
     *
     * "called manually" means not overrided by #scrollOverrideSet
     */
    ZFOBSERVER_EVENT(ScrollOnScrolledByUser)

public:
    // ============================================================
    // styles
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollEnable);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollBounceHorizontal);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollBounceHorizontalAlways);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollBounceVertical);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollBounceVerticalAlways);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollAlignToAxis);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollAlignToPageHorizontal);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, scrollAlignToPageVertical);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIRect, scrollContentFrame);
    zffinal void _ZFP_ZFUIScrollView_scrollContentFrameSetByImpl(ZF_IN const ZFUIRect &rect);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

    // ============================================================
    // override ZFUIView
protected:
    zfoverride
    virtual void implChildOnAdd(ZF_IN ZFUIView *child,
                                ZF_IN zfindex virtualIndex,
                                ZF_IN ZFUIViewChildLayerEnum childLayer,
                                ZF_IN zfindex childLayerIndex);
    zfoverride
    virtual void implChildOnRemove(ZF_IN ZFUIView *child,
                                   ZF_IN zfindex virtualIndex,
                                   ZF_IN ZFUIViewChildLayerEnum childLayer,
                                   ZF_IN zfindex childLayerIndex);
protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    /**
     * @brief override ZFUIView to layout scroll view's internal view and content view
     *
     * usually you should not override this method,
     * if necessary, you must call super
     */
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutedFrameFixedOnUpdateForChild(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &childFrame);

    /**
     * @brief due to implementations limitation, ZFUIScrollView may or may not have mouse event callbacks
     */
    zfoverride
    virtual void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent);
    zfoverride
    virtual void viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent);

    // ============================================================
    // util methods
public:
    /**
     * @brief scroll child to proper position so that it's visible to user
     *
     * it's your responsibility to ensure th child is indeed this view's child
     */
    virtual void scrollChildToVisible(ZF_IN ZFUIView *child);

    // ============================================================
    // scroll thumb
public:
    /**
     * @brief margin for scroll thumb impl, #ZFUIMarginZero by default
     */
    virtual void scrollThumbMarginSet(ZF_IN const ZFUIMargin &margin);
    /**
     * @brief see #scrollThumbMarginSet
     */
    virtual const ZFUIMargin &scrollThumbMargin(void);
    /**
     * @brief change current scroll thumb, null to disable
     *
     * by default, #ZFUIScrollThumbHorizontalClass and #ZFUIScrollThumbHorizontalClass
     * would be used
     */
    virtual void scrollThumbHorizontalClassSet(ZF_IN const ZFClass *cls);
    /**
     * @brief see #scrollThumbHorizontalClassSet
     */
    virtual const ZFClass *scrollThumbHorizontalClass(void);
    /**
     * @brief see #scrollThumbHorizontalClassSet
     */
    virtual void scrollThumbVerticalClassSet(ZF_IN const ZFClass *cls);
    /**
     * @brief see #scrollThumbHorizontalClassSet
     */
    virtual const ZFClass *scrollThumbVerticalClass(void);
protected:
    /**
     * @brief called to init scroll thumb
     */
    virtual void scrollThumbHorizontalOnInit(void);
    /**
     * @brief called to init scroll thumb
     */
    virtual void scrollThumbVerticalOnInit(void);

    // ============================================================
    // scroll control
public:
    /**
     * @brief class for internal scroller, must be type of #ZFUIScroller,
     *   #ZFUIScrollerClass by default
     *
     * scroller can only be changed by #ZFUIScrollerClassSet,
     * and only affect newly created scroll view
     */
    virtual const ZFClass *scrollerClass(void);

public:
    /** @brief util method to get offset of the scroll content */
    virtual inline zfint scrollContentOffsetLeft(void)
    {
        return -this->scrollContentFrame().point.x;
    }
    /** @brief util method to get offset of the scroll content */
    virtual inline zfint scrollContentOffsetTop(void)
    {
        return -this->scrollContentFrame().point.y;
    }
    /** @brief util method to get offset of the scroll content */
    virtual inline zfint scrollContentOffsetRight(void)
    {
        return this->layoutedFrame().size.width - ZFUIMarginGetX(this->scrollThumbMargin()) - ZFUIRectGetRight(this->scrollContentFrame());
    }
    /** @brief util method to get offset of the scroll content */
    virtual inline zfint scrollContentOffsetBottom(void)
    {
        return this->layoutedFrame().size.height - ZFUIMarginGetY(this->scrollThumbMargin()) - ZFUIRectGetBottom(this->scrollContentFrame());
    }

public:
    /**
     * @brief animated change scroll content frame
     *
     * this is a util method to combine #scrollContentFrameSetWhileAnimating
     * and #scrollByPoint
     */
    virtual void scrollContentFrameSetAnimated(ZF_IN const ZFUIRect &scrollContentFrame);
    /**
     * @brief change scroll content frame without interrupt current scroll animation
     */
    virtual void scrollContentFrameSetWhileAnimating(ZF_IN const ZFUIRect &scrollContentFrame);
    /**
     * @brief animated scroll to desired position
     */
    virtual void scrollByPoint(ZF_IN zfint xPos,
                               ZF_IN zfint yPos);
    /**
     * @brief return end point of #scrollByPoint, or current content offset if not scrolling
     */
    virtual ZFUIPoint scrollByPointEndPoint(void);
    /**
     * @brief scroll by desired initial speed, in pixels per second
     */
    virtual void scrollBySpeed(ZF_IN zfint xSpeedInPixelsPerSecond,
                               ZF_IN zfint ySpeedInPixelsPerSecond);
    /**
     * @brief return current speed of #scrollBySpeed
     */
    virtual zfint scrollBySpeedCurrentSpeedX(void);
    /**
     * @brief return current speed of #scrollBySpeed
     */
    virtual zfint scrollBySpeedCurrentSpeedY(void);
    /**
     * @brief return predicted end point of #scrollBySpeed, or current content offset if not scrolling,
     *   valid only if isn't bouncing and won't bouncing when stop
     */
    virtual ZFUIPoint scrollBySpeedEndPointPredicted(void);

    /**
     * @brief #scrollByPointEndPoint or #scrollBySpeedEndPointPredicted depending on scroll type
     */
    virtual ZFUIPoint scrollEndPointPredicted(void);

public:
    /**
     * @brief start an auto scroll with specified speed
     *
     * auto scroll would be stopped if:
     * -  #autoScrollStopX/#autoScrollStopY is called
     * -  drag event occurred
     * -  #scrollByPoint/#scrollContentFrameSetAnimated occurred
     * -  scroll reached content's edge
     */
    virtual void autoScrollStartX(ZF_IN zfint speedInPixelsPerSecond);
    /**
     * @brief see #autoScrollStartX
     */
    virtual void autoScrollStartY(ZF_IN zfint speedInPixelsPerSecond);
    /**
     * @brief see #autoScrollStartX
     */
    virtual void autoScrollStopX(void);
    /**
     * @brief see #autoScrollStartX
     */
    virtual void autoScrollStopY(void);
    /**
     * @brief current auto scroll speed, 0 means not auto scrolling, see #autoScrollStartX
     */
    virtual zfint autoScrollSpeedX(void);
    /**
     * @brief current auto scroll speed, 0 means not auto scrolling, see #autoScrollStartX
     */
    virtual zfint autoScrollSpeedY(void);

public:
    /**
     * @brief simulate drag
     */
    virtual void scrollSimulateDragBegin(ZF_IN const ZFUIPoint &mousePos,
                                         ZF_IN const zftimet &mouseTime);
    /**
     * @brief simulate drag
     */
    virtual void scrollSimulateDrag(ZF_IN const ZFUIPoint &mousePos,
                                    ZF_IN const zftimet &mouseTime);
    /**
     * @brief simulate drag
     */
    virtual void scrollSimulateDragEnd(ZF_IN const zftimet &mouseTime,
                                       ZF_IN_OPT zfbool needScrollAni = zftrue);

protected:
    /**
     * @brief for subclass to supply advanced scroll controll
     *
     * this method is used to ignore some internal scroll actions
     * from being recognized as user's manual scroll action,
     * see #EventScrollOnScrolledByUser\n
     * this method can be called more than one time,
     * but must be paired
     */
    zffinal void scrollOverrideSet(ZF_IN zfbool scrollOverride);
    /** @brief see #scrollOverrideSet */
    zffinal zfbool scrollOverride(void);

    // ============================================================
    // scroll status
public:
    /**
     * @brief current state for the scroll view
     */
    virtual ZFUIScrollViewStateEnum scrollViewState(void);

    // ============================================================
    // scroll callbacks
public:
    zffinal void _ZFP_ZFUIScrollView_notifyDragBegin(ZF_IN const ZFUIPoint &mousePos,
                                                     ZF_IN const zftimet &mouseTime);
    zffinal void _ZFP_ZFUIScrollView_notifyDrag(ZF_IN const ZFUIPoint &mousePos,
                                                ZF_IN const zftimet &mouseTime);
    zffinal void _ZFP_ZFUIScrollView_notifyDragEnd(ZF_IN const zftimet &mouseTime,
                                                   ZF_IN zfbool needScrollAni);
    zffinal void _ZFP_ZFUIScrollView_notifyScrollAnimation(ZF_IN const zftimet &relativeTimeInMiliseconds);
protected:
    /** @brief see #EventScrollOnDragBegin */
    virtual inline void scrollOnDragBegin(void)
    {
        if(ZFOBSERVER_HAS_ADD(HasAdd_ScrollOnDragBegin))
        {
            this->observerNotify(ZFUIScrollView::EventScrollOnDragBegin());
        }
    }
    /** @brief see #EventScrollOnDrag */
    virtual inline void scrollOnDrag(void)
    {
        if(ZFOBSERVER_HAS_ADD(HasAdd_ScrollOnDrag))
        {
            this->observerNotify(ZFUIScrollView::EventScrollOnDrag());
        }
    }
    /** @brief see #EventScrollOnDragEnd */
    virtual inline void scrollOnDragEnd(void)
    {
        if(ZFOBSERVER_HAS_ADD(HasAdd_ScrollOnDragEnd))
        {
            this->observerNotify(ZFUIScrollView::EventScrollOnDragEnd());
        }
    }
    /** @brief see #EventScrollOnScrollBegin */
    virtual inline void scrollOnScrollBegin(void)
    {
        if(ZFOBSERVER_HAS_ADD(HasAdd_ScrollOnScrollBegin))
        {
            this->observerNotify(ZFUIScrollView::EventScrollOnScrollBegin());
        }
    }
    /** @brief see #EventScrollOnScroll */
    virtual inline void scrollOnScroll(void)
    {
        if(ZFOBSERVER_HAS_ADD(HasAdd_ScrollOnScroll))
        {
            this->observerNotify(ZFUIScrollView::EventScrollOnScroll());
        }
    }
    /** @brief see #EventScrollOnScrollEnd */
    virtual inline void scrollOnScrollEnd(void)
    {
        if(ZFOBSERVER_HAS_ADD(HasAdd_ScrollOnScrollEnd))
        {
            this->observerNotify(ZFUIScrollView::EventScrollOnScrollEnd());
        }
    }
    /** @brief see #EventScrollThumbMarginOnChange */
    virtual inline void scrollThumbMarginOnChange(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollThumbMarginOnChange());
    }
    /** @brief see #EventScrollContentFrameOnChange */
    virtual inline void scrollContentFrameOnChange(void)
    {
        if(ZFOBSERVER_HAS_ADD(HasAdd_ScrollContentFrameOnChange))
        {
            this->observerNotify(ZFUIScrollView::EventScrollContentFrameOnChange());
        }
    }
    /** @brief see #EventScrollAutoScrollOnStart */
    virtual inline void scrollAutoScrollOnStart(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollAutoScrollOnStart());
    }
    /** @brief see #EventScrollAutoScrollOnStop */
    virtual inline void scrollAutoScrollOnStop(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollAutoScrollOnStop());
    }
    /** @brief see #EventScrollOnScrolledByUser */
    virtual inline void scrollOnScrolledByUser(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnScrolledByUser());
    }

private:
    _ZFP_ZFUIScrollViewPrivate *d;
    friend zfclassFwd _ZFP_ZFUIScrollViewPrivate;

    ZFOBSERVER_HAS_ADD_BEGIN()
        ZFOBSERVER_HAS_ADD_VALUE(HasAdd_ScrollOnDragBegin)
        ZFOBSERVER_HAS_ADD_VALUE(HasAdd_ScrollOnDrag)
        ZFOBSERVER_HAS_ADD_VALUE(HasAdd_ScrollOnDragEnd)
        ZFOBSERVER_HAS_ADD_VALUE(HasAdd_ScrollOnScrollBegin)
        ZFOBSERVER_HAS_ADD_VALUE(HasAdd_ScrollOnScroll)
        ZFOBSERVER_HAS_ADD_VALUE(HasAdd_ScrollOnScrollEnd)
        ZFOBSERVER_HAS_ADD_VALUE(HasAdd_ScrollContentFrameOnChange)
    ZFOBSERVER_HAS_ADD_SEPARATOR()
        ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_ScrollOnDragBegin, ZFUIScrollView::EventScrollOnDragBegin())
        ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_ScrollOnDrag, ZFUIScrollView::EventScrollOnDrag())
        ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_ScrollOnDragEnd, ZFUIScrollView::EventScrollOnDragEnd())
        ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_ScrollOnScrollBegin, ZFUIScrollView::EventScrollOnScrollBegin())
        ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_ScrollOnScroll, ZFUIScrollView::EventScrollOnScroll())
        ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_ScrollOnScrollEnd, ZFUIScrollView::EventScrollOnScrollEnd())
        ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_ScrollContentFrameOnChange, ZFUIScrollView::EventScrollContentFrameOnChange())
    ZFOBSERVER_HAS_ADD_END()
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollView_h_


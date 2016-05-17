/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUIView.h
 * @brief protocol for ZFUIView
 */

#ifndef _ZFI_ZFProtocolZFUIView_h_
#define _ZFI_ZFProtocolZFUIView_h_

#include "../ZFUIView.h"
#include "../ZFUIScrollView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFPROTOCOL_INTERFACE_CLASS(ZFUIView);
extern ZF_ENV_EXPORT zftimet _ZFP_ZFProtocolZFUIView_scrollViewScrollAnimationStart(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl,
                                                                                    ZF_IN ZFUIScrollView *scrollView);
extern ZF_ENV_EXPORT void _ZFP_ZFProtocolZFUIView_scrollViewScrollAnimationStop(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl,
                                                                                ZF_IN ZFUIScrollView *scrollView);

/**
 * @brief protocol for ZFUIView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIView)
public:
    /**
     * @brief called when a ZFUIView created,
     *   to register native view to ZFUIView
     *
     * return a native view token,
     * which is automatically saved to ZFUIView.nativeView\n
     * \n
     * due to some limitations (mainly ZFProtocol's design
     * and some platform doesn't support intercept and pass modified mouse event),
     * ZFUIView's protocol should also support ScrollView\n
     * to see whether it's ScrollView,
     * you may test whether view is type of ZFUIScrollView\n
     * \n
     * for convenient, here is a list shows that those protocols usually
     * need special implementation for ScrollView:
     * -  nativeViewCreate/nativeViewDestroy
     *   create you own native ScrollView type if necessary
     * -  nativeImplViewSet
     *   scrollview should set internal native view as background most view
     * -  childAdd/childRemove
     *   usually have different behavior for a scroll view
     * -  viewFrameSet
     *   may or may not have different behavior for a scroll view's
     *   normal view, foreground and background view
     * -  scrollViewXXX
     *   special protocols for ZFUIScrollView,
     *   ensured only called for ZFUIScrollView
     * -  also
     *   you should ensure the scroll view is safe to be embeded with another scroll view,
     *   while the child one has higher priority to scroll,
     *   the parent one would scroll only if child scrolled to end
     */
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view) = 0;
    /**
     * @brief called when a ZFUIView destroyed,
     *   the internal nativeView could be accessed by ZFUIView.nativeView
     *
     * it's ensured that view has been remove from parent before destroy
     */
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView) = 0;

    /**
     * @brief attach a native view to this view, see #ZFUINativeViewWrapper
     *
     * set null to remove the attached native view,
     * and it's ensured set null before view destroy\n
     * implementations must note that the nativeImplView should not affect
     * the index when #childAdd, and it must at the bottom of all children\n
     * \n
     * nativeImplView has two use:
     * -  used to store native view for #ZFUINativeViewWrapper
     * -  used to store native view for different view's implementation
     *   such as EditText
     */
    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView) = 0;

    /**
     * @brief get proper scale for the view
     */
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView) = 0;
    /**
     * @brief get proper scale for physical pixel
     */
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView) = 0;

    // ============================================================
    // properties
public:
    /** @brief see #ZFUIViewStyle */
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible) = 0;
    /** @brief see #ZFUIViewStyle */
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha) = 0;
    /** @brief see #ZFUIViewStyle */
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable) = 0;
    /** @brief see #ZFUIViewStyle */
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree) = 0;
    /** @brief see #ZFUIViewStyle */
    virtual void viewMouseHoverEventEnableSet(ZF_IN ZFUIView *view,
                                              ZF_IN zfbool viewMouseHoverEventEnable)
    {
        // no hover event support by default
    }
    /** @brief see #ZFUIViewStyle */
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor) = 0;

    // ============================================================
    // scroll view's special protocol
public:
    /**
     * @brief see #ZFUIScrollView::scrollEnable
     */
    virtual void scrollViewScrollEnableSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollEnable) = 0;
    /**
     * @brief see #ZFUIScrollView::scrollBounceHorizontal
     *
     * actual bounce logic would be done by #ZFUIScrollView for you,
     * this is only a hint for implementation,
     * for example,
     * a none bounceable scroll view won't start drag action
     */
    virtual void scrollViewScrollBounceSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollBounceHorizontal,
                                           ZF_IN zfbool scrollBounceVertical,
                                           ZF_IN zfbool scrollBounceHorizontalAlways,
                                           ZF_IN zfbool scrollBounceVerticalAlways) = 0;
    /**
     * @brief change content's frame
     *
     * scroll view's content frame logic, including scrolling animation logic,
     * would be done by #ZFUIScrollView for you,
     * what you should do, is to supply proper mouse intercept logic,
     * make sure mouse down/up is paired both to the scroll view and its children,
     * and notify necessary events:
     * -  #notifyScrollViewDragBegin
     * -  #notifyScrollViewDrag
     * -  #notifyScrollViewDragEnd
     */
    virtual void scrollViewScrollContentFrameSet(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN const ZFUIRect &frame) = 0;
    /**
     * @brief for impl to update scroll content frame without activating any other event
     */
    zffinal void scrollViewScrollContentFrameSetForImpl(ZF_IN ZFUIScrollView *scrollView,
                                                        ZF_IN const ZFUIRect &frame)
    {
        scrollView->_ZFP_ZFUIScrollView_scrollContentFrameSetByImpl(ZFUIRectApplyScaleReversely(frame, scrollView->scaleGetFixed()));
    }
    /**
     * @brief used to start scroll animation for performance
     *
     * you should schedule a timer task that repeatly call #notifyScrollViewScrollAnimation,
     * return a relative time in miliseconds to represents scroll animation's timing\n
     * timer interval should be decided by implementation internally,
     * accorrding to runtime performance,
     * and typically 50 miliseconds would suit for most cases\n
     * \n
     * by default, we would use #ZFTimer to supply a default implementation,
     * which may have worse performance,
     * and depends on #ZFTimer and #ZFTime
     */
    virtual zftimet scrollViewScrollAnimationStart(ZF_IN ZFUIScrollView *scrollView)
    {
        return _ZFP_ZFProtocolZFUIView_scrollViewScrollAnimationStart(this, scrollView);
    }
    /**
     * @brief see #scrollViewScrollAnimationStart
     */
    virtual void scrollViewScrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFProtocolZFUIView_scrollViewScrollAnimationStop(this, scrollView);
    }

    // ============================================================
    // children
public:
    /**
     * @brief add view, no need to worry about layout param or internal views
     *
     * you should not retain or release those views\n
     * atIndex is ensured valid, range [0, curChildCount],
     * meaning if 0 add to first and if curChildCount add to last\n
     * childLayer and childLayerIndex shows the virtual layer of ZFUIView,
     * useful when you need to put them in different layer for implementation
     */
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex atIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex) = 0;
    /**
     * @brief remove view, no need to worry about layout param or ZFUIView's internal views
     *
     * you should not retain or release those views\n
     * atIndex is ensured valid, range [0, curChildCount)\n
     * you should take good care if your implementation has your own internal child views
     * which is not attached to ZFUIView tree
     */
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN zfindex atIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex) = 0;

    // ============================================================
    // frame and layout
public:
    /**
     * @brief set view's frame, no need to worry about layout param or auto resizing
     */
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect) = 0;
    /**
     * @brief called by ZFUIView to notify the implementation that the view needs layout
     *
     * implementations should ensure next layout step would layout all views from root to children,
     * otherwise, you should request layout recursively from child to all of its parent\n
     * \n
     * this method is called by ZFUIView to notify implementations,
     * and #notifyNeedLayout is called by implementations to notify ZFUIView,
     * take good care of them
     */
    virtual void layoutRequest(ZF_IN ZFUIView *view) = 0;

    /**
     * @brief measure a native view, only called by #ZFUINativeViewWrapper
     */
    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam) = 0;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when native view needs layout
     *   if view's parent isn't ZFUIView
     *
     * typically, you should call this method only if the view's parent is not type of ZFUIView
     */
    zffinal void notifyNeedLayout(ZF_IN ZFUIView *view)
    {
        view->layoutRequest();
    }
    /**
     * @brief implementations must notify when need layout if view's parent isn't ZFUIView
     *
     * typically, you should call this method only if the view's parent is not type of ZFUIView
     */
    zffinal void notifyLayoutRootView(ZF_IN ZFUIView *view,
                                      ZF_IN const ZFUIRect &rect)
    {
        view->_ZFP_ZFUIView_notifyLayoutRootView(ZFUIRectApplyScaleReversely(rect, view->scaleGetFixed()));
    }
    /**
     * @brief implementations must notify just before layout native impl view
     */
    zffinal void notifyLayoutNativeImplView(ZF_IN ZFUIView *view,
                                            ZF_IN_OUT ZFUIRect &result)
    {
        view->_ZFP_ZFUIView_notifyLayoutNativeImplView(result);
        result = ZFUIRectApplyScale(result, view->scaleGetFixed());
    }
    /**
     * @brief implementation must notify when UI event occurred
     *
     * here is a list of UI events you may want to take care:
     * -  #ZFUIMouseEvent
     * -  #ZFUIKeyEvent
     */
    zffinal void notifyUIEvent(ZF_IN ZFUIView *view,
                               ZF_IN ZFUIEvent *uiEvent)
    {
        uiEvent->_ZFP_ZFUIEvent_eventOnApplyScaleReversely(view->scaleGetFixed());
        view->viewEventSend(uiEvent);
    }

    /**
     * @brief see #scrollViewScrollContentFrameSet for how to implements scroll logic
     */
    zffinal void notifyScrollViewDragBegin(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN const ZFUIPoint &mousePos,
                                           ZF_IN const zftimet &mouseTime)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyDragBegin(ZFUIPointApplyScaleReversely(mousePos, scrollView->scaleGetFixed()), mouseTime);
    }
    /**
     * @brief see #scrollViewScrollContentFrameSet for how to implements scroll logic
     */
    zffinal void notifyScrollViewDrag(ZF_IN ZFUIScrollView *scrollView,
                                      ZF_IN const ZFUIPoint &mousePos,
                                      ZF_IN const zftimet &mouseTime)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyDrag(ZFUIPointApplyScaleReversely(mousePos, scrollView->scaleGetFixed()), mouseTime);
    }
    /**
     * @brief see #scrollViewScrollContentFrameSet for how to implements scroll logic
     */
    zffinal void notifyScrollViewDragEnd(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN const zftimet &mouseTime,
                                         ZF_IN zfbool needScrollAni)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyDragEnd(mouseTime, needScrollAni);
    }
    /**
     * @brief see #scrollViewScrollAnimationStart for how to implements scroll animation logic
     */
    zffinal void notifyScrollViewScrollAnimation(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN const zftimet &relativeTimeInMiliseconds)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyScrollAnimation(relativeTimeInMiliseconds);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIView)

zfclassFwd ZFUIScrollViewImplHelper;
/** @brief see #ZFUIScrollViewImplHelper */
zfclassNotPOD ZF_ENV_EXPORT ZFUIScrollViewImplHelperProtocol
{
public:
    virtual ~ZFUIScrollViewImplHelperProtocol(void) {}

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual zftimet nativeTime(void) = 0;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual zfint mouseEventGetX(ZF_IN void *nativeMouseEvent) = 0;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual zfint mouseEventGetY(ZF_IN void *nativeMouseEvent) = 0;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void *mouseEventClone(ZF_IN void *nativeMouseEvent,
                                  ZF_IN_OPT zfbool changeMouseAction = zffalse,
                                  ZF_IN_OPT ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel) = 0;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void mouseEventCleanup(ZF_IN void *nativeMouseEvent) = 0;
    /**
     * @brief see #ZFUIScrollViewImplHelper
     *
     * the forwarded mouse event must be ignored from this scroll view's intercept step,
     * and if forwardToChildScrollView is false,
     * the mouse event must also be ignored from all child scroll view
     */
    virtual void mouseEventForward(ZF_IN void *nativeChild,
                                   ZF_IN void *nativeMouseEvent,
                                   ZF_IN zfbool forwardToChildScrollView) = 0;

    /** @brief see #ZFUIScrollViewImplHelper */
    virtual ZFUIScrollViewImplHelper *findTouchedChildScrollView(ZF_IN zfint x, ZF_IN zfint y) = 0;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void findTouchedChildScrollViewCleanup(ZF_IN void *nativeChild) = 0;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void *findTouchedChild(ZF_IN zfint x, ZF_IN zfint y) = 0;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void findTouchedChildCleanup(ZF_IN void *nativeChild) = 0;

    /**
     * @brief see #ZFUIScrollViewImplHelper, after time out, you must invoke #ZFUIScrollViewImplHelper::trackDelayNotifyTimeout
     *
     * this method has default implementation using #ZFTimer,
     * you may supply your own logic by override this method and #trackDelayStop without calling super
     */
    virtual void trackDelayStart(ZF_IN zftimet timeoutMiliSeconds,
                                 ZF_IN ZFUIScrollViewImplHelper *owner);
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void trackDelayStop(ZF_IN ZFUIScrollViewImplHelper *owner);

    /**
     * @brief see #ZFUIScrollViewImplHelper
     *
     * return original native mouse event or create new copy if not able to modify,
     * if you created new native mouse event and return, you should destroy old one manually by #mouseEventCleanup
     */
    virtual void *translateFromParentToChild(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zfint const &xInParent, ZF_IN zfint const &yInParent) = 0;
    /** @brief see #translateFromParentToChild */
    virtual void *translateFromChildToParent(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zfint const &xInChild, ZF_IN zfint const &yInChild) = 0;
};
zfclassFwd _ZFP_ZFUIScrollViewImplHelperPrivate;
/**
 * @brief util class to achieve #ZFUIScrollView's drag tracking logic
 *
 * to use this util class, you native view must support:
 * -  intercept mouse event
 * -  simulate mouse event and event forwarding
 *
 * to use it:
 * -  implements all methods in ZFUIScrollViewImplHelperProtocol
 * -  have all ZFUIScrollViewImplHelper's member been setup properly
 * -  intercept all mouse events by #ZFUIScrollViewImplHelper::interceptMouse
 */
zfclassNotPOD ZF_ENV_EXPORT ZFUIScrollViewImplHelper
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFUIScrollViewImplHelper)

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    ZFUIScrollView *scrollView;
    /** @brief see #ZFUIScrollViewImplHelper */
    ZFUIScrollViewImplHelperProtocol *implProtocol;
    /** @brief see #ZFUIScrollViewImplHelper */
    zfint nativeScrollTolerance;

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    ZFUIScrollViewImplHelper(void);
    ~ZFUIScrollViewImplHelper(void);

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    void interceptMouse(ZF_IN void *nativeMouseEvent,
                        ZF_IN ZFUIMouseActionEnum mouseAction);
    /** @brief see #ZFUIScrollViewImplHelper */
    void trackDelayNotifyTimeout(void);

private:
    _ZFP_ZFUIScrollViewImplHelperPrivate *d;
    friend zfclassFwd _ZFP_ZFUIScrollViewImplHelperPrivate;
    friend zfclassFwd ZFUIScrollViewImplHelperProtocol;
    ZFTimer *_trackDelayDefaultImplTimer;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIView_h_


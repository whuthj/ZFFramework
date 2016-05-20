/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "../ZFPrivate_ZFUIKit.hh"
#include "ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFUIView)

// ============================================================
zftimet _ZFP_ZFProtocolZFUIView_scrollViewScrollAnimationStart(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl,
                                                               ZF_IN ZFUIScrollView *scrollView)
{
    zfblockedAllocWithoutLeakTest(ZFTimer, scrollTimer);
    scrollTimer->timerIntervalSet(30);
    ZFLISTENER_LOCAL(scrollTimerEvent, {
        ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl = (ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *)(userData->tagGet<ZFTypeHolder *>(zfText("_ZFP_ZFProtocolZFUIView_impl"))->holdedData);
        ZFUIScrollView *scrollView = userData->tagGet<ZFObjectHolder *>(zfText("_ZFP_ZFProtocolZFUIView_scrollView"))->holdedObj;
        impl->notifyScrollViewScrollAnimation(scrollView, ZFTime::timestamp());
    })
    zfblockedAllocWithoutLeakTest(ZFObject, userData);
    userData->tagSet(zfText("_ZFP_ZFProtocolZFUIView_impl"), zflineAllocWithoutLeakTest(ZFTypeHolder, impl, ZFTypeHolder::DeletePointerRef));
    userData->tagSet(zfText("_ZFP_ZFProtocolZFUIView_scrollView"), scrollView->objectHolder());
    scrollTimer->observerAdd(ZFTimer::EventTimerOnActivate(), scrollTimerEvent, userData);
    scrollView->tagSet(zfText("_ZFP_ZFProtocolZFUIView_scrollTimer"), scrollTimer);
    scrollTimer->objectCachedSet(zftrue);
    scrollTimer->timerStart();
    return ZFTime::timestamp();
}
void _ZFP_ZFProtocolZFUIView_scrollViewScrollAnimationStop(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl,
                                                           ZF_IN ZFUIScrollView *scrollView)
{
    ZFTimer *scrollTimer = scrollView->tagGet<ZFTimer *>(zfText("_ZFP_ZFProtocolZFUIView_scrollTimer"));
    zfRetainWithoutLeakTest(scrollTimer);
    scrollView->tagRemove(zfText("_ZFP_ZFProtocolZFUIView_scrollTimer"));
    scrollTimer->timerStop();
    scrollTimer->objectCachedSet(zffalse);
    zfReleaseWithoutLeakTest(scrollTimer);
}

// ============================================================
// default impl for track delay
void ZFUIScrollViewImplHelperProtocol::trackDelayStart(ZF_IN zftimet timeoutMiliSeconds,
                                                       ZF_IN ZFUIScrollViewImplHelper *owner)
{
    if(owner->_trackDelayDefaultImplTimer == zfnull)
    {
        owner->_trackDelayDefaultImplTimer = zfAllocWithoutLeakTest(ZFTimer);
        owner->_trackDelayDefaultImplTimer->timerActivateInMainThreadSet(zftrue);
        owner->_trackDelayDefaultImplTimer->timerIntervalSet(timeoutMiliSeconds);
        ZFLISTENER_LOCAL(timerActivated, {
            listenerData.sender->to<ZFTimer *>()->timerStop();
            userData->to<ZFTypeHolder *>()->holdedDataPointer<ZFUIScrollViewImplHelper *>()->trackDelayNotifyTimeout();
        })
        owner->_trackDelayDefaultImplTimer->observerAdd(
            ZFTimer::EventTimerOnActivate(),
            timerActivated,
            zflineAllocWithoutLeakTest(ZFTypeHolder, owner, ZFTypeHolder::DeletePointerRef));
    }
    owner->_trackDelayDefaultImplTimer->timerIntervalSet(timeoutMiliSeconds);
    owner->_trackDelayDefaultImplTimer->timerStart();
}
void ZFUIScrollViewImplHelperProtocol::trackDelayStop(ZF_IN ZFUIScrollViewImplHelper *owner)
{
    if(owner->_trackDelayDefaultImplTimer != zfnull)
    {
        owner->_trackDelayDefaultImplTimer->timerStop();
    }
}

// ============================================================
typedef enum {
    _ZFP_ZFUIScrollViewImplHelperDragStateTracking,
    _ZFP_ZFUIScrollViewImplHelperDragStateDragging,
    _ZFP_ZFUIScrollViewImplHelperDragStateIgnored,
} _ZFP_ZFUIScrollViewImplHelperDragState;
zfclassNotPOD _ZFP_ZFUIScrollViewImplHelperPrivate
{
public:
    ZFUIScrollViewImplHelper *owner;
    _ZFP_ZFUIScrollViewImplHelperDragState dragState;
    zfbool mouseDownFlag;
    void *mouseDownSaved;
    ZFUIScrollViewImplHelper *trackingParent;
    ZFUIScrollViewImplHelper *trackingChild;
    void *processingChild;

public:
    _ZFP_ZFUIScrollViewImplHelperPrivate(void)
    : owner(zfnull)
    , dragState(_ZFP_ZFUIScrollViewImplHelperDragStateTracking)
    , mouseDownFlag(zffalse)
    , mouseDownSaved(zfnull)
    , trackingParent(zfnull)
    , trackingChild(zfnull)
    , processingChild(zfnull)
    {
    }

public:
    zfbool scrollable(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().size.width > v->layoutedFrame().size.width || v->scrollContentFrame().size.height > v->layoutedFrame().size.height));
    }
    zfbool scrollableX(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().size.width > v->layoutedFrame().size.width));
    }
    zfbool scrollableY(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().size.height > v->layoutedFrame().size.height));
    }
    zfbool scrollableLeft(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().point.x < 0));
    }
    zfbool scrollableTop(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().point.y < 0));
    }
    zfbool scrollableRight(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (ZFUIRectGetRight(v->scrollContentFrame()) > v->layoutedFrame().size.width));
    }
    zfbool scrollableBottom(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (ZFUIRectGetBottom(v->scrollContentFrame()) > v->layoutedFrame().size.height));
    }

    zfbool scrollableOrBounceable(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || v->scrollBounceVerticalAlways() || v->scrollContentFrame().size.width > v->layoutedFrame().size.width || v->scrollContentFrame().size.height > v->layoutedFrame().size.height));
    }
    zfbool scrollableOrBounceableX(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || v->scrollContentFrame().size.width > v->layoutedFrame().size.width));
    }
    zfbool scrollableOrBounceableY(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceVerticalAlways() || v->scrollContentFrame().size.height > v->layoutedFrame().size.height));
    }
    zfbool scrollableOrBounceableLeft(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || (v->scrollBounceHorizontal() && v->scrollContentFrame().size.width > v->layoutedFrame().size.width) || v->scrollContentFrame().point.x < 0));
    }
    zfbool scrollableOrBounceableTop(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceVerticalAlways() || (v->scrollBounceVertical() && v->scrollContentFrame().size.height > v->layoutedFrame().size.height) || v->scrollContentFrame().point.y < 0));
    }
    zfbool scrollableOrBounceableRight(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || (v->scrollBounceHorizontal() && v->scrollContentFrame().size.width > v->layoutedFrame().size.width) || ZFUIRectGetRight(v->scrollContentFrame()) > v->layoutedFrame().size.width));
    }
    zfbool scrollableOrBounceableBottom(void)
    {
        ZFUIScrollView *v = owner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceVerticalAlways() || (v->scrollBounceVertical() && v->scrollContentFrame().size.height > v->layoutedFrame().size.height) || ZFUIRectGetBottom(v->scrollContentFrame()) > v->layoutedFrame().size.height));
    }

public:
    void mouseDownResend(void)
    {
        if(this->mouseDownSaved && this->processingChild)
        {
            this->mouseDownSaved = this->owner->implProtocol->translateFromParentToChild(
                this->processingChild,
                this->mouseDownSaved,
                this->mouseX(this->mouseDownSaved),
                this->mouseY(this->mouseDownSaved));
            this->owner->implProtocol->mouseEventForward(this->processingChild, this->mouseDownSaved, zffalse);
        }
        this->mouseDownCleanup();
    }
    void mouseDownCleanup(void)
    {
        if(this->mouseDownSaved)
        {
            this->owner->implProtocol->mouseEventCleanup(this->mouseDownSaved);
            this->mouseDownSaved = zfnull;
        }
    }

public:
    zfint mouseX(ZF_IN void *nativeMouseEvent)
    {
        return this->owner->implProtocol->mouseEventGetX(nativeMouseEvent);
    }
    zfint mouseY(ZF_IN void *nativeMouseEvent)
    {
        return this->owner->implProtocol->mouseEventGetY(nativeMouseEvent);
    }

public:
    void trackMouseDown(ZF_IN void *nativeMouseEvent);
    void trackMouseMove(ZF_IN void *nativeMouseEvent);
    void trackMouseUp(ZF_IN void *nativeMouseEvent);
    void trackMouseCancel(ZF_IN void *nativeMouseEvent);

    void resolveMouseDown(ZF_IN void *nativeMouseEvent);
    void resolveMouseMove(ZF_IN void *nativeMouseEvent);
    void resolveMouseUp(ZF_IN void *nativeMouseEvent);
    void resolveMouseCancel(ZF_IN void *nativeMouseEvent);
};

ZFUIScrollViewImplHelper::ZFUIScrollViewImplHelper(void)
: scrollView(zfnull)
, implProtocol(zfnull)
, nativeScrollTolerance(4)
, d(zfnull)
, _trackDelayDefaultImplTimer(zfnull)
{
    d = zfpoolNew(_ZFP_ZFUIScrollViewImplHelperPrivate);
    d->owner = this;
}
ZFUIScrollViewImplHelper::~ZFUIScrollViewImplHelper(void)
{
    if(_trackDelayDefaultImplTimer != zfnull)
    {
        _trackDelayDefaultImplTimer->timerStop();
        zfReleaseWithoutLeakTest(_trackDelayDefaultImplTimer);
    }
    zfpoolDelete(d);
}

void ZFUIScrollViewImplHelper::interceptMouse(ZF_IN void *nativeMouseEvent,
                                              ZF_IN ZFUIMouseActionEnum mouseAction)
{
    switch(mouseAction)
    {
        case ZFUIMouseAction::e_MouseDown:
            d->mouseDownFlag = zftrue;
            d->trackMouseDown(nativeMouseEvent);
            break;
        case ZFUIMouseAction::e_MouseMove:
            if(!d->mouseDownFlag)
            {
                d->owner->implProtocol->mouseEventForward(d->owner->scrollView->nativeView(), nativeMouseEvent, zffalse);
            }
            else
            {
                d->trackMouseMove(nativeMouseEvent);
            }
            break;
        case ZFUIMouseAction::e_MouseUp:
            if(!d->mouseDownFlag)
            {
                d->owner->implProtocol->mouseEventForward(d->owner->scrollView->nativeView(), nativeMouseEvent, zffalse);
            }
            else
            {
                d->mouseDownFlag = zffalse;
                d->trackMouseUp(nativeMouseEvent);
            }
            break;
        case ZFUIMouseAction::e_MouseCancel:
            if(!d->mouseDownFlag)
            {
                d->owner->implProtocol->mouseEventForward(d->owner->scrollView->nativeView(), nativeMouseEvent, zffalse);
            }
            else
            {
                d->mouseDownFlag = zffalse;
                d->trackMouseCancel(nativeMouseEvent);
            }
            break;
        case ZFUIMouseAction::e_MouseHoverEnter:
        case ZFUIMouseAction::e_MouseHover:
        case ZFUIMouseAction::e_MouseHoverExit:
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
void ZFUIScrollViewImplHelper::trackDelayNotifyTimeout(void)
{
    // mouse without drag and timeout,
    // restore mouse down event and ignore all following events
    d->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateIgnored;

    // restore mouse down
    d->mouseDownResend();
}

// ============================================================
// track logic
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseDown(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);

    this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
    this->trackingChild = this->owner->implProtocol->findTouchedChildScrollView(x, y);
    if(this->trackingChild != zfnull && this->trackingChild->d->scrollableOrBounceable())
    {
        this->trackingChild->d->trackingParent = this->owner;

        void *tmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
        tmp = this->owner->implProtocol->translateFromParentToChild(
            this->trackingChild->scrollView->nativeView(),
            tmp,
            x, y);
        this->owner->implProtocol->mouseEventForward(this->trackingChild->scrollView->nativeView(), tmp, zftrue);
        this->owner->implProtocol->mouseEventCleanup(tmp);
        return ;
    }

    if(this->trackingChild != zfnull)
    {
        this->owner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingChild);
        this->trackingChild = zfnull;
    }
    this->resolveMouseDown(nativeMouseEvent);
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseMove(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);
    if(this->trackingChild != zfnull)
    {
        void *tmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
        tmp = this->owner->implProtocol->translateFromParentToChild(
            this->trackingChild->scrollView->nativeView(),
            tmp,
            x, y);
        this->owner->implProtocol->mouseEventForward(this->trackingChild->scrollView->nativeView(), tmp, zftrue);
        this->owner->implProtocol->mouseEventCleanup(tmp);
        return ;
    }

    if(this->trackingParent == zfnull || this->dragState != _ZFP_ZFUIScrollViewImplHelperDragStateTracking)
    {
        this->resolveMouseMove(nativeMouseEvent);
        return ;
    }

    zfint xOffset = x - this->mouseX(this->mouseDownSaved);
    zfint yOffset = y - this->mouseY(this->mouseDownSaved);
    if(zfmAbs(xOffset) <= this->owner->nativeScrollTolerance && zfmAbs(yOffset) <= this->owner->nativeScrollTolerance)
    {
        return ;
    }
    zfbool stopTracking = zffalse;
    if(zfmAbs(yOffset) >= zfmAbs(xOffset))
    { // drag vertical
        if(yOffset > 0)
        {
            if(this->scrollableTop())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableTop())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableTop())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableTop())
            {
                stopTracking = zftrue;
            }
        }
        else if(yOffset < 0)
        {
            if(this->scrollableBottom())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableBottom())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableBottom())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableBottom())
            {
                stopTracking = zftrue;
            }
        }
    }
    else { // drag horizontal
        if(xOffset > 0)
        {
            if(this->scrollableLeft())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableLeft())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableLeft())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableLeft())
            {
                stopTracking = zftrue;
            }
        }
        else if(xOffset < 0)
        {
            if(this->scrollableRight())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableRight())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableRight())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableRight())
            {
                stopTracking = zftrue;
            }
        }
    }

    if(stopTracking)
    { // stop child's drag and restore parent's touch down
        void *mouseDownTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent, zftrue, ZFUIMouseAction::e_MouseDown);

        void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent, zftrue, ZFUIMouseAction::e_MouseCancel);
        this->resolveMouseCancel(evTmp);
        this->owner->implProtocol->mouseEventCleanup(evTmp);

        this->mouseDownCleanup();
        mouseDownTmp = this->trackingParent->implProtocol->translateFromChildToParent(
            this->owner->scrollView->nativeView(),
            mouseDownTmp,
            x, y);
        if(this->trackingParent->d->trackingChild != zfnull)
        {
            this->owner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingParent->d->trackingChild);
            this->trackingParent->d->trackingChild = zfnull;
        }
        this->trackingParent->d->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
        this->trackingParent->d->resolveMouseDown(mouseDownTmp);
        this->owner->implProtocol->mouseEventCleanup(mouseDownTmp);
    }
    else
    {
        this->resolveMouseMove(nativeMouseEvent);
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseUp(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);
    if(this->trackingChild != zfnull)
    {
        ZFUIScrollViewImplHelper *tmp = this->trackingChild;
        this->trackingChild->d->trackingParent = zfnull;
        this->owner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingChild);
        this->trackingChild = zfnull;

        void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->owner->implProtocol->translateFromParentToChild(tmp->scrollView->nativeView(), evTmp, x, y);
        tmp->d->trackMouseUp(evTmp);
        this->owner->implProtocol->mouseEventCleanup(evTmp);
        return ;
    }
    this->resolveMouseUp(nativeMouseEvent);
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseCancel(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);
    if(this->trackingChild != zfnull)
    {
        ZFUIScrollViewImplHelper *tmp = this->trackingChild;
        this->trackingChild->d->trackingParent = zfnull;
        this->owner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingChild);
        this->trackingChild = zfnull;

        void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->owner->implProtocol->translateFromParentToChild(tmp->scrollView->nativeView(), evTmp, x, y);
        tmp->d->trackMouseCancel(evTmp);
        this->owner->implProtocol->mouseEventCleanup(evTmp);
        return ;
    }
    this->resolveMouseCancel(nativeMouseEvent);
}

// ============================================================
// mouse resolve
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseDown(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);
    this->mouseDownCleanup();

    if(!this->scrollableOrBounceable())
    {
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateIgnored;
        this->processingChild = this->owner->implProtocol->findTouchedChild(x, y);

        if(this->processingChild != zfnull)
        {
            void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->owner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            this->owner->implProtocol->mouseEventForward(this->processingChild, evTmp, zftrue);
            this->owner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    this->mouseDownSaved = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);

    if(this->owner->scrollView->scrollViewState() != ZFUIScrollViewState::e_Idle)
    {
        if(this->processingChild != zfnull)
        {
            this->owner->implProtocol->findTouchedChildCleanup(this->processingChild);
            this->processingChild = zfnull;
        }
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateDragging;
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDragBegin(this->owner->scrollView, ZFUIPointMake(x, y), this->owner->implProtocol->nativeTime());
        return ;
    }

    this->processingChild = this->owner->implProtocol->findTouchedChild(x, y);
    if(this->processingChild != zfnull)
    {
        this->owner->implProtocol->trackDelayStart(200, this->owner);
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseMove(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);
    if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateDragging)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDrag(this->owner->scrollView, ZFUIPointMake(x, y), this->owner->implProtocol->nativeTime());
        return ;
    }
    else if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateIgnored)
    {
        if(this->processingChild != zfnull)
        {
            void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->owner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            this->owner->implProtocol->mouseEventForward(this->processingChild, evTmp, zftrue);
            this->owner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    int xOffset = (int)(x - this->mouseX(this->mouseDownSaved));
    int yOffset = (int)(y - this->mouseY(this->mouseDownSaved));
    int xOffsetAbs = zfmAbs(xOffset);
    int yOffsetAbs = zfmAbs(yOffset);
    if(xOffsetAbs <= this->owner->nativeScrollTolerance && yOffsetAbs <= this->owner->nativeScrollTolerance) {
        return ;
    }

    zfbool childIntercepted = zffalse;
    if(this->processingChild != zfnull)
    {
        if(yOffsetAbs >= xOffsetAbs * 1.5f)
        { // drag vertical
            if(yOffset > 0)
            {
                if(!this->scrollableOrBounceableTop())
                {
                    childIntercepted = true;
                }
            }
            else
            {
                if(!this->scrollableOrBounceableBottom())
                {
                    childIntercepted = true;
                }
            }
        }
        else if(xOffsetAbs >= yOffsetAbs * 1.5f)
        { // drag horizontal
            if(xOffset > 0)
            {
                if(!this->scrollableOrBounceableLeft())
                {
                    childIntercepted = true;
                }
            }
            else
            {
                if(!this->scrollableOrBounceableRight())
                {
                    childIntercepted = true;
                }
            }
        }
    }

    if(childIntercepted)
    {
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateIgnored;
        this->mouseDownResend();
        void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->owner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
        this->owner->implProtocol->mouseEventForward(this->processingChild, evTmp, zffalse);
        this->owner->implProtocol->mouseEventCleanup(evTmp);
    }
    else
    {
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateDragging;
        this->owner->implProtocol->trackDelayStop(this->owner);
        zfint xSaved = this->mouseX(this->mouseDownSaved);
        zfint ySaved = this->mouseY(this->mouseDownSaved);
        this->mouseDownCleanup();
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDragBegin(this->owner->scrollView, ZFUIPointMake(xSaved, ySaved), this->owner->implProtocol->nativeTime());
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDrag(this->owner->scrollView, ZFUIPointMake(x, y), this->owner->implProtocol->nativeTime());
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseUp(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);

    if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateDragging)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDragEnd(this->owner->scrollView, this->owner->implProtocol->nativeTime(), zftrue);
        return ;
    }
    else if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateIgnored)
    {
        if(this->processingChild != zfnull)
        {
            void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->owner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            this->owner->implProtocol->mouseEventForward(this->processingChild, evTmp, zftrue);
            this->owner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    // mouse down then up quickly
    this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
    if(this->processingChild != zfnull)
    {
        this->mouseDownResend();
        void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->owner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
        this->owner->implProtocol->mouseEventForward(this->processingChild, evTmp, zftrue);
        this->owner->implProtocol->mouseEventCleanup(evTmp);
    }
    else {
        this->mouseDownCleanup();
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseCancel(ZF_IN void *nativeMouseEvent)
{
    zfint x = this->mouseX(nativeMouseEvent);
    zfint y = this->mouseY(nativeMouseEvent);

    if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateDragging)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDragEnd(this->owner->scrollView, this->owner->implProtocol->nativeTime(), zffalse);
        return ;
    }
    else if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateIgnored)
    {
        if(this->processingChild != zfnull)
        {
            void *evTmp = this->owner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->owner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            this->owner->implProtocol->mouseEventForward(this->processingChild, evTmp, zftrue);
            this->owner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    // mouse down then cancel quickly
    this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
    this->mouseDownCleanup();
}

ZF_NAMESPACE_GLOBAL_END


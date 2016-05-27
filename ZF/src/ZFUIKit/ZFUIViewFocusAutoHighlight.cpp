/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIViewFocusAutoHighlight.h"
#include "ZFUIViewBlink.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// auto highlight filter
ZFFilterForZFObject ZFUIViewFocusAutoHighlightFilter;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusAutoHighlightFilterDataHolder, ZFLevelZFFrameworkHigh)
{
    ZFUIViewFocusAutoHighlightFilter = ZFFilterForZFObject();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusAutoHighlightFilterDataHolder)

// ============================================================
zfbool ZFUIViewFocusAutoHighlightAutoStart;
zfautoObject ZFUIViewFocusAutoHighlightMaskImage;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusAutoHighlight_autoStart, ZFLevelAppLow)
{
    if(ZFUIViewFocusAutoHighlightAutoStart)
    {
        ZFUIViewFocusAutoHighlightStart();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusAutoHighlight_autoStart)

// ============================================================
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewFocusAutoHighlight_focusChange);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewFocusAutoHighlight_mouseDown);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewFocusAutoHighlight_viewOnDealloc);
static zfbool _ZFP_ZFUIViewFocusAutoHighlight_started = zffalse;
static zfint _ZFP_ZFUIViewFocusAutoHighlight_paused = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusAutoHighlightDataHolder, ZFLevelZFFrameworkLow)
{
    this->focusChangeListener = ZFCallbackForRawFunction(_ZFP_ZFUIViewFocusAutoHighlight_focusChange);
    this->mouseDownListener = ZFCallbackForRawFunction(_ZFP_ZFUIViewFocusAutoHighlight_mouseDown);
    this->viewOnDeallocListener = ZFCallbackForRawFunction(_ZFP_ZFUIViewFocusAutoHighlight_viewOnDealloc);

    ZFUIViewFocusAutoHighlightAutoStart = zftrue;
    this->viewBlinkOnListener = ZFCallbackForRawFunction(zfself::viewBlinkOn);
    this->viewBlinkOffListener = ZFCallbackForRawFunction(zfself::viewBlinkOff);
    ZFGlobalEventCenter::instance()->observerAdd(ZFGlobalEvent::EventViewBlinkOn(), this->viewBlinkOnListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFGlobalEvent::EventViewBlinkOff(), this->viewBlinkOffListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusAutoHighlightDataHolder)
{
    ZFUIViewFocusAutoHighlightStop();
    ZFGlobalEventCenter::instance()->observerRemove(ZFGlobalEvent::EventViewBlinkOn(), this->viewBlinkOnListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFGlobalEvent::EventViewBlinkOff(), this->viewBlinkOffListener);
    ZFUIViewFocusAutoHighlightMaskImage = zfautoObjectNull;
}
public:
    zfautoObject maskImageCur;
    ZFListener viewBlinkOnListener;
    ZFListener viewBlinkOffListener;
    ZFListener focusChangeListener;
    ZFListener mouseDownListener;
    ZFListener viewOnDeallocListener;
    ZFCoreArrayPOD<ZFUIView *> focusedViews;
    static ZFLISTENER_PROTOTYPE_EXPAND(viewBlinkOn)
    {
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(
            listenerData.sender, ZFGlobalEvent::EventViewFocusAutoHighlightViewOn());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewBlinkOff)
    {
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(
            listenerData.sender, ZFGlobalEvent::EventViewFocusAutoHighlightViewOff());
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusAutoHighlightDataHolder)

// ============================================================
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewFocusAutoHighlight_focusChange)
{
    ZFUIView *view = listenerData.sender->to<ZFUIView *>();

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusAutoHighlightDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightDataHolder);

    if(view->viewFocused())
    {
        if(!ZFUIViewFocusAutoHighlightFilter.filterCheckActive(view))
        {
            return ;
        }
        d->focusedViews.add(view);
        view->observerAdd(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerAdd(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
        if(!ZFUIViewFocusAutoHighlightPaused())
        {
            ZFUIViewBlink(view, d->maskImageCur.to<ZFUIImage *>());
        }
    }
    else
    {
        d->focusedViews.removeElement(view);
        view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewFocusAutoHighlight_mouseDown)
{
    ZFUIMouseEvent *event = ZFCastZFObject(ZFUIMouseEvent *, listenerData.param0);
    if(event == zfnull || event->mouseAction != ZFUIMouseAction::e_MouseDown)
    {
        return ;
    }
    ZFUIView *view = listenerData.sender->to<ZFUIView *>();
    if(!ZFUIViewFocusAutoHighlightFilter.filterCheckActive(view))
    {
        return ;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusAutoHighlightDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightDataHolder);
    if(!ZFUIViewFocusAutoHighlightPaused())
    {
        ZFUIViewBlink(view, d->maskImageCur.to<ZFUIImage *>());
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIViewFocusAutoHighlight_viewOnDealloc)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusAutoHighlightDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightDataHolder);
    ZFUIView *view = listenerData.sender->to<ZFUIView *>();
    d->focusedViews.removeElement(view);
    view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
    view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
}

// ============================================================
void ZFUIViewFocusAutoHighlightStart(ZF_IN_OPT ZFUIImage *img /* = zfnull */)
{
    ZFUIViewFocusAutoHighlightStop();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusAutoHighlightDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightDataHolder);

    _ZFP_ZFUIViewFocusAutoHighlight_started = zftrue;
    d->maskImageCur = zfautoObjectCreateMarkCached((img != zfnull) ? img : ZFUIViewFocusAutoHighlightMaskImage.to<ZFUIImage *>());

    ZFGlobalEventCenter::instance()->observerAdd(ZFUIView::EventViewFocusOnChange(), d->focusChangeListener);
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewFocusAutoHighlightStart());
}
void ZFUIViewFocusAutoHighlightStop(void)
{
    if(!_ZFP_ZFUIViewFocusAutoHighlight_started)
    {
        return ;
    }
    _ZFP_ZFUIViewFocusAutoHighlight_started = zffalse;

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusAutoHighlightDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightDataHolder);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIView::EventViewFocusOnChange(), d->focusChangeListener);
    for(zfindex i = 0; i < d->focusedViews.count(); ++i)
    {
        ZFUIView *view = d->focusedViews[i];
        view->observerRemove(ZFUIView::EventObjectBeforeDealloc(), d->viewOnDeallocListener);
        view->observerRemove(ZFUIView::EventViewOnEvent(), d->mouseDownListener);
    }
    d->focusedViews.removeAll();
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewFocusAutoHighlightStop());
}
zfbool ZFUIViewFocusAutoHighlightStarted(void)
{
    return _ZFP_ZFUIViewFocusAutoHighlight_started;
}

void ZFUIViewFocusAutoHighlightPause(void)
{
    ++_ZFP_ZFUIViewFocusAutoHighlight_paused;
    if(_ZFP_ZFUIViewFocusAutoHighlight_paused == 1)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewFocusAutoHighlightPause());
    }
}
void ZFUIViewFocusAutoHighlightResume(void)
{
    zfCoreAssert(_ZFP_ZFUIViewFocusAutoHighlight_paused > 0);
    --_ZFP_ZFUIViewFocusAutoHighlight_paused;
    if(_ZFP_ZFUIViewFocusAutoHighlight_paused == 0)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewFocusAutoHighlightResume());
    }
}
zfindex ZFUIViewFocusAutoHighlightPaused(void)
{
    return (!_ZFP_ZFUIViewFocusAutoHighlight_started || _ZFP_ZFUIViewFocusAutoHighlight_paused);
}

// ============================================================
// ZFUIViewFocusAutoHighlightPauseForTime
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder, ZFLevelZFFrameworkLow)
{
    this->started = zffalse;
    this->delayTimer = zfnull;
    this->doStopListener = ZFCallbackForRawFunction(zfself::doStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder)
{
    zfReleaseWithoutLeakTest(this->delayTimer);
}
zfbool started;
ZFTimer *delayTimer;
zftimet endTime;
ZFListener doStopListener;
static ZFLISTENER_PROTOTYPE_EXPAND(doStop)
{
    ZFUIViewFocusAutoHighlightPauseForTimeCancel();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder)

void ZFUIViewFocusAutoHighlightPauseForTime(ZF_IN zftimet time /* = 0 */)
{
    if(ZFUIViewFocusAutoHighlightPaused())
    {
        return ;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder);

    time = zfmMax<zftimet>(time, 100);
    zftimet endTime = ZFTime::timestamp() + time;
    if(d->started && endTime <= d->endTime + 100)
    {
        return ;
    }

    if(d->delayTimer == zfnull)
    {
        d->delayTimer = zfAllocWithoutLeakTest(ZFTimer);
        d->delayTimer->observerAdd(ZFTimer::EventTimerOnActivate(), d->doStopListener);
        d->delayTimer->timerActivateInMainThreadSet(zftrue);
    }
    d->delayTimer->timerStop();

    d->endTime = endTime;
    d->delayTimer->timerIntervalSet(time);
    ZFUIViewFocusAutoHighlightPause();
    d->started = zftrue;
    d->delayTimer->objectCachedSet(zftrue);
    d->delayTimer->timerStart();
}
void ZFUIViewFocusAutoHighlightPauseForTimeCancel(void)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder);
    if(!d->started)
    {
        return ;
    }
    d->started = zffalse;
    d->delayTimer->timerStop();
    d->delayTimer->objectCachedSet(zffalse);
    ZFUIViewFocusAutoHighlightResume();
}

// ZFUIViewFocusAutoHighlightPauseForTime auto cleanup
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusAutoHighlightPauseForTimeAutoCleanup, ZFLevelZFFrameworkLow)
{
    // ensure init order
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewFocusAutoHighlightPauseForTimeDataHolder);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusAutoHighlightPauseForTimeAutoCleanup)
{
    ZFUIViewFocusAutoHighlightPauseForTimeCancel();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusAutoHighlightPauseForTimeAutoCleanup)

ZF_NAMESPACE_GLOBAL_END


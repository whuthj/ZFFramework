/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIAutoFitLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIAutoFitLayoutStyle, ZFUIViewStyle)

// ============================================================
// scroll view
zfclass _ZFP_ZFUIAutoFitLayoutScrollView : zfextends ZFUIScrollView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIAutoFitLayoutScrollView, ZFUIScrollView)

protected:
    // disable scroll thumb
    zfoverride
    virtual void scrollThumbHorizontalOnInit(void)
    {
    }
    zfoverride
    virtual void scrollThumbVerticalOnInit(void)
    {
    }
};

// ============================================================
// _ZFP_ZFUIAutoFitLayoutPrivate
zfclassNotPOD _ZFP_ZFUIAutoFitLayoutPrivate
{
public:
    ZFUIAutoFitLayout *owner;
    ZFUIScrollView *scrollView;
    ZFUIView *autoFitFocusedView;
    ZFUIMargin autoFitMargin;
    ZFListener onScreenKeyboardStateOnChangeListener;
    ZFListener viewFocusOnChangeListener;
    ZFListener scrollFocusedViewToVisibleDelayListener;
    zfbool autoFitFocusedViewNeedUpdate;

public:
    void autoFitEnableSet(ZF_IN zfbool value)
    {
        if(value)
        {
            ZFGlobalEventCenter::instance()->observerAdd(
                ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(),
                this->onScreenKeyboardStateOnChangeListener,
                this->owner->objectHolder());

            this->owner->layoutRequest();
        }
        else
        {
            this->scrollView->scrollEnableSet(zffalse);
            ZFGlobalEventCenter::instance()->observerRemove(
                ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(),
                this->onScreenKeyboardStateOnChangeListener);

            this->scrollView->scrollContentFrameSet(ZFUIRectGetBounds(this->owner->layoutedFrame()));
            if(this->autoFitMargin == ZFUIMarginZero || this->owner->layoutParam() == zfnull)
            {
                return ;
            }
            this->autoFitMargin = ZFUIMarginZero;
            this->scrollView->layoutParam()->layoutMarginSet(this->autoFitMargin);
        }
    }
    void autoFitFocusedViewToVisibleSet(ZF_IN zfbool value)
    {
        if(value)
        {
            ZFGlobalEventCenter::instance()->observerAdd(
                ZFUIView::EventViewFocusOnChange(),
                this->viewFocusOnChangeListener,
                this->owner->objectHolder());
        }
        else
        {
            ZFGlobalEventCenter::instance()->observerRemove(
                ZFUIView::EventViewFocusOnChange(),
                this->viewFocusOnChangeListener);
            this->autoFitFocusedView = zfnull;
        }
    }

public:
    void autoFitUpdateFrame(void)
    {
        if(this->owner->layoutParam() == zfnull)
        {
            return ;
        }

        ZFUIRect orgRect = ZFUIViewPositionOnScreen(this->owner);
        if(orgRect.size != this->owner->layoutedFrame().size)
        {
            return ;
        }

        this->autoFitMargin = this->autoFitMarginCalc(orgRect, ZFUIOnScreenKeyboardState::instanceForView(this->owner));
        this->scrollView->layoutParam()->layoutMarginSet(this->autoFitMargin);
    }
    ZFUIMargin autoFitMarginCalc(ZF_IN const ZFUIRect &orgRect,
                                 ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        ZFUIMargin ret = ZFUIMarginZero;
        if(keyboardState->keyboardShowing() && ZFUIRectGetBottom(orgRect) > ZFUIRectGetTop(keyboardState->keyboardFrame()))
        {
            ret.bottom = ZFUIRectGetBottom(orgRect) - ZFUIRectGetTop(keyboardState->keyboardFrame());
        }
        return ret;
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(onScreenKeyboardStateOnChange)
    {
        ZFUIOnScreenKeyboardState *keyboardState = listenerData.sender->to<ZFUIOnScreenKeyboardState *>();
        ZFUIAutoFitLayout *layout = userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIAutoFitLayout *>();
        if(keyboardState->keyboardShowing())
        {
            if(layout->autoFitFocusedView() != zfnull)
            {
                layout->d->autoFitFocusedViewNeedUpdate = zftrue;
            }
            layout->d->scrollView->scrollEnableSet(zftrue);
            layout->layoutRequest();
        }
        else
        {
            layout->d->scrollView->scrollEnableSet(zffalse);
            layout->d->scrollView->layoutParam()->layoutMarginSet(ZFUIMarginZero);
            layout->d->scrollView->scrollContentFrameSet(ZFUIRectGetBounds(layout->layoutedFrame()));
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewFocusOnChange)
    {
        ZFUIAutoFitLayout *layout = userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIAutoFitLayout *>();
        ZFUIView *view = listenerData.sender->to<ZFUIView *>();
        if(!view->viewFocused())
        {
            if(view == layout->d->autoFitFocusedView)
            {
                layout->d->autoFitFocusedView = zfnull;
            }
            return ;
        }

        {
            ZFUIView *parent = view;
            while(parent != zfnull && parent != layout)
            {
                parent = parent->viewParent();
            }
            if(parent == zfnull)
            {
                return ;
            }
        }

        layout->d->autoFitFocusedView = view;
        layout->d->autoFitFocusedViewNeedUpdate = zftrue;
        layout->layoutRequest();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(scrollFocusedViewToVisibleDelay)
    {
        ZFUIAutoFitLayout *layout = userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIAutoFitLayout *>();
        if(layout->autoFitFocusedView() != zfnull)
        {
            layout->d->scrollView->scrollChildToVisible(layout->autoFitFocusedView());
        }
    }

public:
    _ZFP_ZFUIAutoFitLayoutPrivate(void)
    : owner(zfnull)
    , scrollView(zfnull)
    , autoFitFocusedView(zfnull)
    , autoFitMargin(ZFUIMarginZero)
    , onScreenKeyboardStateOnChangeListener(ZFCallbackForRawFunction(_ZFP_ZFUIAutoFitLayoutPrivate::onScreenKeyboardStateOnChange))
    , viewFocusOnChangeListener(ZFCallbackForRawFunction(_ZFP_ZFUIAutoFitLayoutPrivate::viewFocusOnChange))
    , scrollFocusedViewToVisibleDelayListener(ZFCallbackForRawFunction(_ZFP_ZFUIAutoFitLayoutPrivate::scrollFocusedViewToVisibleDelay))
    , autoFitFocusedViewNeedUpdate(zffalse)
    {
    }
};

// ============================================================
// ZFUIAutoFitLayout
ZFOBJECT_REGISTER(ZFUIAutoFitLayout)

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIAutoFitLayout, zfbool, autoFitEnable)
{
    zfbool changed = (newValue != this->autoFitEnable());
    zfsuperI(ZFUIAutoFitLayoutStyle)::autoFitEnableSet(newValue);
    if(changed)
    {
        d->autoFitEnableSet(this->autoFitEnable());
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIAutoFitLayout, zfbool, autoFitFocusedViewToVisible)
{
    zfbool changed = (newValue != this->autoFitFocusedViewToVisible());
    zfsuperI(ZFUIAutoFitLayoutStyle)::autoFitFocusedViewToVisibleSet(newValue);
    if(changed)
    {
        d->autoFitFocusedViewToVisibleSet(this->autoFitFocusedViewToVisible());
    }
}

ZFUIView *ZFUIAutoFitLayout::autoFitFocusedView(void)
{
    return d->autoFitFocusedView;
}

ZFObject *ZFUIAutoFitLayout::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIAutoFitLayoutPrivate);
    d->owner = this;
    zfblockedAllocWithoutLeakTest(_ZFP_ZFUIAutoFitLayoutScrollView, scrollView);
    d->scrollView = scrollView;
    d->scrollView->scrollEnableSet(zffalse);
    this->viewDelegateSet(d->scrollView);
    return this;
}
void ZFUIAutoFitLayout::objectOnDealloc(void)
{
    ZFThreadTaskCancel(d->scrollFocusedViewToVisibleDelayListener);

    d->autoFitEnableSet(zffalse);
    d->autoFitFocusedViewToVisibleSet(zffalse);

    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIAutoFitLayout::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->autoFitEnableSet(this->autoFitEnable());
    d->autoFitFocusedViewToVisibleSet(this->autoFitFocusedViewToVisible());
}

void ZFUIAutoFitLayout::viewDelegateLayoutOnMeasure(ZF_OUT ZFUISize &ret,
                                                    ZF_IN const ZFUISize &sizeHint,
                                                    ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = ZFUISizeZero;
    ZFUISize sizeHintTmp = ZFUIViewLayoutParam::sizeHintOffset(sizeHint,
        ZFUISizeMake(ZFUIMarginGetX(d->autoFitMargin), ZFUIMarginGetY(d->autoFitMargin)));
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        child->layoutMeasure(sizeHintTmp, child->layoutParam()->sizeParam());
        ret.width = zfmMax(ret.width, child->layoutMeasuredSize().width + ZFUIMarginGetX(child->layoutParam()->layoutMargin()));
        ret.height = zfmMax(ret.height, child->layoutMeasuredSize().height + ZFUIMarginGetX(child->layoutParam()->layoutMargin()));
    }
}
void ZFUIAutoFitLayout::viewDelegateLayoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::viewDelegateLayoutOnLayoutPrepare(bounds);
    if(!this->autoFitEnable()
        || this->layoutedFrame().size == ZFUISizeZero || this->layoutedFrame().size != bounds.size
        || !d->scrollView->scrollEnable())
    {
        d->scrollView->scrollContentFrameSet(bounds);
    }
    else
    {
        ZFUIRect scrollContentFrame = d->scrollView->scrollContentFrame();
        scrollContentFrame.size = bounds.size;
        d->scrollView->scrollContentFrameSet(scrollContentFrame);
        d->autoFitUpdateFrame();
        if(d->autoFitFocusedViewNeedUpdate)
        {
            d->autoFitFocusedViewNeedUpdate = zffalse;
            ZFThreadTaskRequest(d->scrollFocusedViewToVisibleDelayListener, this->objectHolder());
        }
    }
}

ZF_NAMESPACE_GLOBAL_END


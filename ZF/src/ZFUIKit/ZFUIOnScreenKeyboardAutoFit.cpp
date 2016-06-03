/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIOnScreenKeyboardAutoFit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// start how many times for the same window
#define _ZFP_ZFUIOnScreenKeyboardAutoFit_tag_startCount zfText("_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_startCount")
// holds a ZFTypeHolder to a ZFUIMargin
#define _ZFP_ZFUIOnScreenKeyboardAutoFit_tag_layoutMarginSaved zfText("_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_layoutMarginSaved")

static void _ZFP_ZFUIOnScreenKeyboardAutoFit_doStart(ZF_IN ZFUIWindow *window);
static void _ZFP_ZFUIOnScreenKeyboardAutoFit_doStop(ZF_IN ZFUIWindow *window);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoFit_onScreenKeyboardStateChange);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoFit_windowLayoutMarginChange);

void ZFUIOnScreenKeyboardAutoFit(ZF_IN ZFUIWindow *window)
{
    if(window == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    ZFValueEditable *count = window->tagGet<ZFValueEditable *>(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_startCount);
    if(count != zfnull)
    {
        count->indexValueSet(count->indexValue() + 1);
        return ;
    }
    zfautoObject countHolder = ZFValueEditable::indexValueCreate(1);
    window->tagSetMarkCached(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_startCount, countHolder.toObject());

    _ZFP_ZFUIOnScreenKeyboardAutoFit_doStart(window);
}
void ZFUIOnScreenKeyboardAutoFitCancel(ZF_IN ZFUIWindow *window)
{
    if(window == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    ZFValueEditable *count = window->tagGet<ZFValueEditable *>(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_startCount);
    if(count == zfnull)
    {
        return ;
    }
    if(count->indexValue() > 1)
    {
        count->indexValueSet(count->indexValue() - 1);
        return ;
    }
    window->tagRemove(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_startCount);

    _ZFP_ZFUIOnScreenKeyboardAutoFit_doStop(window);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoFitDataHolder, ZFLevelZFFrameworkNormal)
{
    this->onScreenKeyboardStateChangeListener = ZFCallbackForRawFunction(_ZFP_ZFUIOnScreenKeyboardAutoFit_onScreenKeyboardStateChange);
    this->windowLayoutMarginChangeListener = ZFCallbackForRawFunction(_ZFP_ZFUIOnScreenKeyboardAutoFit_windowLayoutMarginChange);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoFitDataHolder)
{
    zfCoreAssertWithMessageTrim(this->windowList.isEmpty(),
        zfText("ZFUIOnScreenKeyboardAutoFit/ZFUIOnScreenKeyboardAutoFitCancel mismatch, have you forgot to cancel?"));
}
ZFCoreArrayPOD<ZFUIWindow *> windowList;
ZFListener onScreenKeyboardStateChangeListener;
ZFListener windowLayoutMarginChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoFitDataHolder)

static void _ZFP_ZFUIOnScreenKeyboardAutoFit_apply(ZF_IN ZFUIWindow *window, ZF_IN ZFUIOnScreenKeyboardState *state)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoFitDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoFitDataHolder);
    ZFTypeHolder *layoutMarginSaved = window->windowLayoutParam()->tagGet<ZFTypeHolder *>(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_layoutMarginSaved);
    if(layoutMarginSaved == zfnull && state->keyboardShowing())
    {
        ZFUIMargin margin = window->windowLayoutParam()->layoutMargin();
        ZFUIMargin keyboardMargin = ZFUIMarginZero;
        state->keyboardFixClientFrame(keyboardMargin);
        window->windowLayoutParam()->layoutMarginSet(ZFUIMarginInc(margin, keyboardMargin));

        window->windowLayoutParam()->tagSetMarkCached(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_layoutMarginSaved,
            zflineAlloc(ZFTypeHolder, zfnew(ZFUIMargin, margin), ZFTypeHolder::DeleteObject<ZFUIMargin *>));
        window->windowLayoutParam()->observerAdd(
            ZFObject::EventObjectPropertyValueAfterChange(),
            d->windowLayoutMarginChangeListener);
    }
    else if(layoutMarginSaved != zfnull && !state->keyboardShowing())
    {
        window->windowLayoutParam()->observerRemove(
            ZFObject::EventObjectPropertyValueAfterChange(),
            d->windowLayoutMarginChangeListener);
        ZFUIMargin margin = layoutMarginSaved->holdedDataRef<ZFUIMargin &>();
        window->windowLayoutParam()->tagRemove(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_layoutMarginSaved);
        window->windowLayoutParam()->layoutMarginSet(margin);
    }
}

static void _ZFP_ZFUIOnScreenKeyboardAutoFit_doStart(ZF_IN ZFUIWindow *window)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoFitDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoFitDataHolder);
    if(d->windowList.isEmpty())
    {
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), d->onScreenKeyboardStateChangeListener);
    }
    d->windowList.add(window);

    _ZFP_ZFUIOnScreenKeyboardAutoFit_apply(window, ZFUIOnScreenKeyboardState::instanceForView(window));
}
static void _ZFP_ZFUIOnScreenKeyboardAutoFit_doStop(ZF_IN ZFUIWindow *window)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoFitDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoFitDataHolder);

    window->windowLayoutParam()->observerRemove(ZFObject::EventObjectPropertyValueAfterChange(), d->windowLayoutMarginChangeListener);
    window->layoutRequest();

    _ZFP_ZFUIOnScreenKeyboardAutoFit_apply(window, ZFUIOnScreenKeyboardState::instanceForView(window));

    d->windowList.removeElement(window);
    if(d->windowList.isEmpty())
    {
        ZFGlobalEventCenter::instance()->observerRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), d->onScreenKeyboardStateChangeListener);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoFit_onScreenKeyboardStateChange)
{
    ZFUIOnScreenKeyboardState *state = listenerData.sender->to<ZFUIOnScreenKeyboardState *>();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoFitDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoFitDataHolder);
    for(zfindex i = 0; i < d->windowList.count(); ++i)
    {
        ZFUIWindow *window = d->windowList[i];
        if(window->ownerSysWindow() != state->ownerSysWindow())
        {
            continue;
        }

        _ZFP_ZFUIOnScreenKeyboardAutoFit_apply(window, state);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoFit_windowLayoutMarginChange)
{
    const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
    if(property != ZFPropertyAccess(ZFUIViewLayoutParam, layoutMargin))
    {
        return ;
    }

    ZFUIViewLayoutParam *layoutParam = listenerData.sender->to<ZFUIViewLayoutParam *>();
    ZFTypeHolder *layoutMarginSaved = layoutParam->tagGet<ZFTypeHolder *>(_ZFP_ZFUIOnScreenKeyboardAutoFit_tag_layoutMarginSaved);
    if(layoutMarginSaved != zfnull)
    {
        layoutMarginSaved->holdedDataRef<ZFUIMargin &>() = layoutParam->layoutMargin();
    }
}

ZF_NAMESPACE_GLOBAL_END


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIWidget.hh"
#include "ZFUIHint.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zftimet ZFUIHintDurationDefault = 1500;

// ============================================================
static void _ZFP_ZFUIHint_onScreenKeyboardApply(ZF_IN ZFUIHint *hint,
                                                ZF_IN const ZFUIRect &keyboardFrame,
                                                ZF_IN const ZFUIRect &keyboardFramePrev)
{
    ZFUIViewLayoutParam *lp = hint->hintWindow()->windowLayoutParam();
    ZFUIMargin newMargin = lp->layoutMargin();
    if(newMargin.bottom >= keyboardFramePrev.size.height)
    {
        newMargin.bottom -= keyboardFramePrev.size.height;
    }
    newMargin.bottom += keyboardFrame.size.height;
    if(newMargin != lp->layoutMargin())
    {
        lp->layoutMarginSet(newMargin);
    }
}

// ============================================================
static ZFArray *_ZFP_ZFUIHint_hintListForRead(ZF_IN ZFUISysWindow *inSysWindow)
{
    if(inSysWindow == zfnull)
    {
        inSysWindow = ZFUISysWindow::mainWindow();
    }
    if(inSysWindow == zfnull)
    {
        return zfnull;
    }
    return inSysWindow->tagGet<ZFArray *>(zfText("_ZFP_ZFUIHint_hintList"));
}
static ZFArrayEditable *_ZFP_ZFUIHint_hintListForWrite(ZF_IN ZFUISysWindow *inSysWindow)
{
    if(inSysWindow == zfnull)
    {
        inSysWindow = ZFUISysWindow::mainWindow();
    }
    if(inSysWindow == zfnull)
    {
        return zfnull;
    }
    ZFArrayEditable *hintList = inSysWindow->tagGet<ZFArrayEditable *>(zfText("_ZFP_ZFUIHint_hintList"));
    if(hintList == zfnull)
    {
        zfblockedAllocWithoutLeakTest(ZFArrayEditable, hintListTmp);
        hintList = hintListTmp;
        inSysWindow->tagSet(zfText("_ZFP_ZFUIHint_hintList"), hintList);
    }
    return hintList;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIHintDataHolder, ZFLevelZFFrameworkNormal)
{
    this->syncObject = zfAllocWithoutLeakTest(ZFObject);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIHintDataHolder)
{
    zfReleaseWithoutLeakTest(this->syncObject);
    this->syncObject = zfnull;
}
ZFObject *syncObject;
ZF_GLOBAL_INITIALIZER_END(ZFUIHintDataHolder)
#define _ZFP_ZFUIHintSyncObj (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIHintDataHolder)->syncObject)

// ============================================================
zfclass _ZFP_ZFUIHintWindow : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIHintWindow, ZFUIWindow)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewFocusable, zffalse)

protected:
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam)
    {
        if(this->childCount() == 0)
        {
            ret = ZFUISizeZero;
        }
        else
        {
            ret = this->childAtIndex(0)->layoutMeasure(sizeHint, sizeParam);
        }
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        if(this->childCount() > 0)
        {
            this->childAtIndex(0)->layout(bounds);
        }
    }
};

// ============================================================
zfclassNotPOD _ZFP_ZFUIHintPrivate
{
public:
    ZFUIHint *ownerZFUIHint;
    _ZFP_ZFUIHintWindow *hintWindow;
    zfbool hintShowing;
    zfbool hintDelaying;
    ZFAnimation *hintAnimating; // auto retain
    zfidentity hintShowDelayTaskId;
    ZFListener hintAniShowOnStopListener;
    ZFListener hintShowDelayTimeoutListener;
    ZFListener hintAniHideOnStopListener;

public:
    _ZFP_ZFUIHintPrivate(void)
    : ownerZFUIHint(zfnull)
    , hintWindow(zfnull)
    , hintShowing(zffalse)
    , hintDelaying(zffalse)
    , hintAnimating(zfnull)
    , hintShowDelayTaskId(zfidentityInvalid)
    , hintAniShowOnStopListener(ZFCallbackForRawFunction(_ZFP_ZFUIHintPrivate::hintAniShowOnStop))
    , hintShowDelayTimeoutListener(ZFCallbackForRawFunction(_ZFP_ZFUIHintPrivate::hintShowDelayTimeout))
    , hintAniHideOnStopListener(ZFCallbackForRawFunction(_ZFP_ZFUIHintPrivate::hintAniHideOnStop))
    {
    }

public:
    void hintDoDelay(void)
    {
        this->hintShowing = zftrue;
        this->hintDelaying = zftrue;
    }
    void hintDoShow(void)
    {
        this->ownerZFUIHint->hintOnUpdate();
        this->ownerZFUIHint->hintOnShow();
        this->hintShowing = zftrue;
        this->hintDelaying = zffalse;
        this->ownerZFUIHint->hintWindow()->windowShow();
        _ZFP_ZFUIHint_onScreenKeyboardApply(
            this->ownerZFUIHint,
            ZFUIOnScreenKeyboardState::instanceForSysWindow(this->ownerZFUIHint->hintWindow()->ownerSysWindow())->keyboardFrame(),
            ZFUIRectZero);
        ZFPropertyChangeWithoutLeakTest(this->hintAnimating, this->ownerZFUIHint->hintAniShow());
        if(this->hintAnimating != zfnull)
        {
            this->hintAnimating->observerAdd(ZFObject::ObserverAddParam()
                .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
                .observerSet(this->hintAniShowOnStopListener)
                .userDataSet(this->ownerZFUIHint->objectHolder())
                .autoRemoveAfterActivateSet(zftrue));
            this->hintAnimating->aniTargetSet(this->ownerZFUIHint->hintWindow());
            this->hintAnimating->aniStart();
        }
        else
        {
            this->hintDoShowDelay();
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(hintAniShowOnStop)
    {
        zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);
        ZFUIHint *hint = userData->to<ZFObjectHolder *>()->holdedObj;
        hint->d->hintDoShowDelay();
    }
    void hintDoShowDelay(void)
    {
        ZFPropertyChangeWithoutLeakTest(this->hintAnimating, zfnull);
        this->hintShowDelayTaskId = ZFThreadExecuteInMainThreadAfterDelay(
            this->ownerZFUIHint->hintDuration(),
            this->hintShowDelayTimeoutListener,
            this->ownerZFUIHint->objectHolder());
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(hintShowDelayTimeout)
    {
        zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);
        ZFUIHint *hint = userData->to<ZFObjectHolder *>()->holdedObj;
        hint->d->hintShowDelayTaskId = zfidentityInvalid;
        hint->d->hintDoHide();
    }
    void hintDoHide(void)
    {
        ZFPropertyChangeWithoutLeakTest(this->hintAnimating, this->ownerZFUIHint->hintAniHide());
        if(this->hintAnimating != zfnull)
        {
            this->hintAnimating->observerAdd(ZFObject::ObserverAddParam()
                .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
                .observerSet(ZFCallbackForRawFunction(_ZFP_ZFUIHintPrivate::hintAniHideOnStop))
                .userDataSet(this->ownerZFUIHint->objectHolder())
                .autoRemoveAfterActivateSet(zftrue));
            this->hintAnimating->aniTargetSet(this->ownerZFUIHint->hintWindow());
            this->hintAnimating->aniStart();
        }
        else
        {
            this->hintDoFinish();
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(hintAniHideOnStop)
    {
        zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);
        ZFUIHint *hint = userData->to<ZFObjectHolder *>()->holdedObj;
        hint->d->hintDoFinish();
    }
    void hintDoFinish(void)
    {
        this->hintShowing = zffalse;
        ZFPropertyChangeWithoutLeakTest(this->hintAnimating, zfnull);
        ZFArrayEditable *hintList = _ZFP_ZFUIHint_hintListForWrite(this->ownerZFUIHint->hintWindow()->ownerSysWindow());
        zfRetainWithoutLeakTest(this->ownerZFUIHint);
        zfblockedReleaseWithoutLeakTest(this->ownerZFUIHint);
        hintList->removeElement(this->ownerZFUIHint, ZFComparerCheckEqualOnly);
        _ZFP_ZFUIHint_onScreenKeyboardApply(
            this->ownerZFUIHint,
            ZFUIRectZero,
            ZFUIOnScreenKeyboardState::instanceForSysWindow(this->ownerZFUIHint->hintWindow()->ownerSysWindow())->keyboardFrame());
        this->ownerZFUIHint->hintOnHide();
        this->ownerZFUIHint->hintWindow()->windowHide();
        if(!hintList->isEmpty())
        {
            ZFUIHint *hint = hintList->getFirst<ZFUIHint *>();
            if(hint->hintDelaying())
            {
                hint->d->hintDoShow();
            }
        }
        ZFLeakTestLogReleaseVerbose(this->ownerZFUIHint, ZF_CALLER_FILE, zfTextA("hintHide"), ZF_CALLER_LINE);
        zfReleaseWithoutLeakTest(this->ownerZFUIHint);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIHint)

ZFCoreArrayPOD<ZFUIHint *> ZFUIHint::hintList(ZF_IN_OPT ZFUISysWindow *inSysWindow /* = zfnull */)
{
    zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);

    ZFCoreArrayPOD<ZFUIHint *> ret;

    ZFArray *hintList = _ZFP_ZFUIHint_hintListForRead(inSysWindow);
    if(hintList == zfnull)
    {
        return ret;
    }

    for(zfindex i = 0; i < hintList->count(); ++i)
    {
        ret.add(hintList->get<ZFUIHint *>(i));
    }

    return ret;
}

ZFPROPERTY_CUSTOM_SETTER_DEFINE(ZFUIHint, ZFUIView *, hintContent)
{
    ZFUIView *old = this->hintContent();
    zfRetainWithoutLeakTest(old);
    zfblockedReleaseWithoutLeakTest(old);
    if(old != zfnull)
    {
        old->viewRemoveFromParent();
    }
    this->hintContentSetInternal(newValue);
    if(this->hintContent() != zfnull)
    {
        this->hintWindow()->childAdd(this->hintContent());
    }
}

ZFUIWindow *ZFUIHint::hintWindow(void)
{
    return d->hintWindow;
}

void ZFUIHint::hintShow(void)
{
    zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);
    if(this->hintShowing())
    {
        return ;
    }
    d->hintShowing = zftrue;
    zfRetain(this);

    ZFArrayEditable *hintList = _ZFP_ZFUIHint_hintListForWrite(this->hintWindow()->ownerSysWindow());
    hintList->add(this);
    if(hintList->count() == 1)
    {
        d->hintDoShow();
    }
    else
    {
        d->hintDoDelay();
    }
}
void ZFUIHint::hintHide(void)
{
    zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);
    if(d->hintShowing)
    {
        if(d->hintDelaying)
        {
            d->hintShowing = zffalse;
            d->hintDelaying = zffalse;
            zfRetainWithoutLeakTest(this);
            zfblockedReleaseWithoutLeakTest(this);
            ZFArrayEditable *hintList = _ZFP_ZFUIHint_hintListForWrite(this->hintWindow()->ownerSysWindow());
            hintList->removeElement(this, ZFComparerCheckEqualOnly);
            _ZFP_ZFUIHint_onScreenKeyboardApply(
                this,
                ZFUIRectZero,
                ZFUIOnScreenKeyboardState::instanceForSysWindow(this->hintWindow()->ownerSysWindow())->keyboardFrame());
            zfRelease(this);
        }
        else
        {
            ZFThreadExecuteCancel(d->hintShowDelayTaskId);
            if(d->hintAnimating != zfnull)
            {
                d->hintAnimating->observerRemove(ZFAnimation::EventAniOnStopOrOnInvalid(), d->hintAniShowOnStopListener);
                d->hintAnimating->observerRemove(ZFAnimation::EventAniOnStopOrOnInvalid(), d->hintAniHideOnStopListener);
                ZFAnimation *hintAniTmp = d->hintAnimating;
                d->hintAnimating = zfnull;
                hintAniTmp->aniStop();
                zfReleaseWithoutLeakTest(hintAniTmp);
            }
            d->hintShowing = zffalse;
            d->hintDelaying = zffalse;
            this->hintOnHide();
            zfRelease(this);
        }
    }
}
zfbool ZFUIHint::hintShowing(void)
{
    return d->hintShowing;
}
zfbool ZFUIHint::hintDelaying(void)
{
    return d->hintDelaying;
}
ZFAnimation *ZFUIHint::hintAnimating(void)
{
    return d->hintAnimating;
}

ZFObject *ZFUIHint::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIHintPrivate);

    d->ownerZFUIHint = this;

    d->hintWindow = zfAllocWithoutLeakTest(_ZFP_ZFUIHintWindow);
    d->hintWindow->windowLevelSet(ZFUIWindowLevel::e_ZFFrameworkFgHighest);
    d->hintWindow->viewSizeMinSet(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeText()));
    d->hintWindow->windowLayoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    d->hintWindow->windowLayoutParam()->sizeParamSet(ZFUISizeParamWrapWidthWrapHeight);
    d->hintWindow->windowLayoutParam()->layoutMarginSet(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
    d->hintWindow->viewUIEnableTreeSet(zffalse);

    ZFLISTENER_LOCAL(hintWindowChanged, {
        zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);
        ZFUIHint *hint = userData->to<ZFObjectHolder *>()->holdedObj;
        zfCoreAssertWithMessage(!hint->hintShowing(), zfTextA("you must not change ZFUIHint's window while it's showing or delaying"));
        ZFUISysWindow *sysWindowOld = listenerData.param0->toAny();
        ZFArrayEditable *hintListOld = _ZFP_ZFUIHint_hintListForWrite(sysWindowOld);
        ZFArrayEditable *hintListNew = _ZFP_ZFUIHint_hintListForWrite(hint->hintWindow()->ownerSysWindow());
        hintListNew->add(hint);
        hintListOld->removeElement(hint, ZFComparerCheckEqualOnly);
        _ZFP_ZFUIHint_onScreenKeyboardApply(
            hint,
            ZFUIOnScreenKeyboardState::instanceForSysWindow(hint->hintWindow()->ownerSysWindow())->keyboardFrame(),
            ZFUIOnScreenKeyboardState::instanceForSysWindow(sysWindowOld)->keyboardFrame());
        if(!hintListOld->isEmpty())
        {
            ZFUIHint *hint = hintListOld->getFirst<ZFUIHint *>();
            if(hint->hintDelaying())
            {
                hint->d->hintDoShow();
            }
        }
        if(!hintListNew->isEmpty())
        {
            ZFUIHint *tmp = hintListNew->getFirst<ZFUIHint *>();
            if(!tmp->d->hintShowing)
            {
                tmp->d->hintDoShow();
            }
        }
    })
    d->hintWindow->observerAdd(ZFUIWindow::EventWindowOwnerSysWindowOnChange(), hintWindowChanged, this->objectHolder());

    ZFLISTENER_LOCAL(hintWindowOnLayoutPrepare, {
        zfsynchronizedObject(_ZFP_ZFUIHintSyncObj);
        ZFUIHint *hint = userData->to<ZFObjectHolder *>()->holdedObj;
        hint->hintOnUpdate();
    })
    d->hintWindow->observerAdd(ZFUIView::EventViewLayoutOnLayoutPrepare(), hintWindowOnLayoutPrepare, this->objectHolder());

    return this;
}
void ZFUIHint::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    this->hintOnInit();
}
void ZFUIHint::objectOnDealloc(void)
{
    if(d->hintAnimating != zfnull)
    {
        d->hintAnimating->aniStop();
        ZFPropertyChangeWithoutLeakTest(d->hintAnimating, zfnull);
    }
    if(d->hintShowDelayTaskId != zfidentityInvalid)
    {
        ZFThreadExecuteCancel(d->hintShowDelayTaskId);
        d->hintShowDelayTaskId = zfidentityInvalid;
    }
    d->hintWindow->windowHide();
    ZFPropertyChangeWithoutLeakTest(d->hintWindow, zfnull);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
// auto adjust hint position when on screen keyboard showed
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIHintPositionAdjustHolder, ZFLevelZFFrameworkNormal)
{
    this->onScreenKeyboardOnChangeListener = ZFCallbackForRawFunction(zfself::onScreenKeyboardOnChange);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), this->onScreenKeyboardOnChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIHintPositionAdjustHolder)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), this->onScreenKeyboardOnChangeListener);
}
ZFListener onScreenKeyboardOnChangeListener;
static ZFLISTENER_PROTOTYPE_EXPAND(onScreenKeyboardOnChange)
{
    ZFUIOnScreenKeyboardState *onScreenKeyboardState = listenerData.sender->toAny();
    ZFCoreArrayPOD<ZFUIHint *> hintList = ZFUIHint::hintList(onScreenKeyboardState->ownerSysWindow());
    for(zfindex i = 0; i < hintList.count(); ++i)
    {
        ZFUIHint *hint = hintList[i];
        _ZFP_ZFUIHint_onScreenKeyboardApply(
            hint,
            ZFUIOnScreenKeyboardState::instanceForSysWindow(hint->hintWindow()->ownerSysWindow())->keyboardFrame(),
            ZFUIOnScreenKeyboardState::instanceForSysWindow(hint->hintWindow()->ownerSysWindow())->keyboardFramePrev());
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIHintPositionAdjustHolder)

ZF_NAMESPACE_GLOBAL_END


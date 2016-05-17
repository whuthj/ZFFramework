/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIKit.hh"
#include "ZFUIViewBlink.h"
#include "ZFUIImageView.h"
#include "ZFAnimationNativeView.h"
#include "protocol/ZFProtocolZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFUIViewBlink_DEBUG_noAni 0
#define _ZFP_ZFUIViewBlink_DEBUG_duration 0
#define _ZFP_ZFUIViewBlink_DEBUG_color 0

#define _ZFP_ZFUIViewBlink_tag_ani zfText("_ZFP_ZFUIViewBlink_tag_ani")
#define _ZFP_ZFUIViewBlink_tag_blinkView zfText("_ZFP_ZFUIViewBlink_tag_blinkView")
#define _ZFP_ZFUIViewBlink_tag_delayTaskId zfText("_ZFP_ZFUIViewBlink_tag_delayTaskId")
#define _ZFP_ZFUIViewBlink_tag_delayId zfText("_ZFP_ZFUIViewBlink_tag_delayId")

zfclass _ZFP_ZFUIViewBlinkView : zfextends ZFUIImageView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIViewBlinkView, ZFUIImageView)
};

static void _ZFP_ZFUIViewBlinkDoOn(ZF_IN ZFUIView *view, ZF_IN const ZFUIViewBlinkParam &blinkParam);
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view);
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlinkDataHolder, ZFLevelZFFrameworkLow)
{
    this->viewOnDeallocListener = ZFCallbackForRawFunction(zfself::viewOnDealloc);
}
ZFListener viewOnDeallocListener;
ZFCoreArrayPOD<ZFUIView *> blinkingViews;
ZFIdentityGenerator delayTaskIdGenerator;
static ZFLISTENER_PROTOTYPE_EXPAND(viewOnDealloc)
{
    _ZFP_ZFUIViewBlinkDoOff(listenerData.sender->to<ZFUIView *>());
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlinkDataHolder)

// ============================================================
void ZFUIViewBlink(ZF_IN ZFUIView *view,
                   ZF_IN const ZFUIViewBlinkParam &blinkParam /* = ZFUIViewBlinkParam() */)
{
    if(view)
    {
        _ZFP_ZFUIViewBlinkDoOff(view);
        _ZFP_ZFUIViewBlinkDoOn(view, blinkParam);
    }
}

// ============================================================
// blink action
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view);
static void _ZFP_ZFUIViewBlinkDoOn(ZF_IN ZFUIView *view, ZF_IN const ZFUIViewBlinkParam &blinkParam)
{
    _ZFP_ZFUIViewBlinkView *blinkView = view->tagGet<_ZFP_ZFUIViewBlinkView *>(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView != zfnull)
    {
        _ZFP_ZFUIViewBlinkDoOff(view);
        return ;
    }
    {
        zfblockedAllocInternal(_ZFP_ZFUIViewBlinkView, t);
        view->internalForegroundViewAdd(t);
        view->tagSetMarkCached(_ZFP_ZFUIViewBlink_tag_blinkView, t);
        t->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        blinkView = t;
    }
    blinkView->imageContentSet((blinkParam.blinkImage() != zfnull)
        ? blinkParam.blinkImage()
        : ZFUIViewBlinkImageDefault.to<ZFUIImage *>());

    if(ZFPROTOCOL_IS_AVAILABLE(ZFAnimationNativeView) && !_ZFP_ZFUIViewBlink_DEBUG_noAni)
    {
        zfblockedAllocInternal(ZFAnimationNativeView, ani);
        view->tagSetMarkCached(_ZFP_ZFUIViewBlink_tag_ani, ani);
        ani->aniAlphaToSet(0);
        #if _ZFP_ZFUIViewBlink_DEBUG_duration
            ani->aniDurationSet(5000);
        #else
            ani->aniDurationSet(blinkParam.blinkDuration());
        #endif

        ZFLISTENER_LOCAL(aniOnStopListener, {
            ZFAnimation *ani = listenerData.sender->to<ZFAnimation *>();
            ZFUIView *blinkView = ani->aniTarget()->to<ZFUIView *>();
            ani->aniTargetSet(zfnull);

            ZFUIView *view = userData->to<ZFObjectHolder *>()->holdedObj;
            view->observerRemove(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

            view->tagRemove(_ZFP_ZFUIViewBlink_tag_ani);
            view->tagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
            blinkView->viewRemoveFromParent();

            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
            ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOff());
        })
        view->observerAdd(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);
        ani->observerAdd(ZFAnimation::EventAniOnStopOrOnInvalid(), aniOnStopListener, view->objectHolder());
        ani->aniTargetSet(blinkView);

        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOn());
        ani->aniStart();
    }
    else
    {
        ZFLISTENER_LOCAL(blinkDelayOnFinish, {
            ZFUIView *view = userData->to<ZFObjectHolder *>()->holdedObj;
            ZFValue *delayTaskId = listenerData.param0->to<ZFValue *>();
            ZFValue *delayTaskIdCur = view->tagGet<ZFValue *>(_ZFP_ZFUIViewBlink_tag_delayTaskId);
            if(delayTaskId != delayTaskIdCur)
            {
                return ;
            }

            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.markUnused(delayTaskId->identityValue());
            _ZFP_ZFUIViewBlink_noAni_doOff(view);
        })
        view->observerAdd(ZFObject::EventObjectBeforeDealloc(), ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->viewOnDeallocListener);

        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOn());
        zfidentity delayTaskId = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->delayTaskIdGenerator.nextMarkUsed();
        zfautoObject delayTaskIdTag = ZFValue::identityValueCreate(delayTaskId);
        view->tagSetMarkCached(_ZFP_ZFUIViewBlink_tag_delayTaskId, delayTaskIdTag.toObject());
        zfidentity delayId = ZFThread::executeInMainThreadAfterDelay(
            #if _ZFP_ZFUIViewBlink_DEBUG_duration
                5000
            #else
                blinkParam.blinkDuration()
            #endif
            ,
            blinkDelayOnFinish,
            view->objectHolder(),
            delayTaskIdTag.toObject());
        view->tagSetMarkCached(_ZFP_ZFUIViewBlink_tag_delayId, ZFValue::identityValueCreate(delayId).toObject());
    }
}
static void _ZFP_ZFUIViewBlink_noAni_doOff(ZF_IN ZFUIView *view)
{
    ZFUIView *blinkView = view->tagGet<ZFUIView *>(_ZFP_ZFUIViewBlink_tag_blinkView);

    view->tagRemove(_ZFP_ZFUIViewBlink_tag_blinkView);
    blinkView->viewRemoveFromParent();

    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder)->blinkingViews.removeElement(view);
    view->tagRemove(_ZFP_ZFUIViewBlink_tag_delayTaskId);
    view->tagRemove(_ZFP_ZFUIViewBlink_tag_delayId);
    ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(view, ZFGlobalEvent::EventViewBlinkOff());
}
static void _ZFP_ZFUIViewBlinkDoOff(ZF_IN ZFUIView *view)
{
    _ZFP_ZFUIViewBlinkView *blinkView = view->tagGet<_ZFP_ZFUIViewBlinkView *>(_ZFP_ZFUIViewBlink_tag_blinkView);
    if(blinkView == zfnull)
    {
        return ;
    }

    ZFAnimation *ani = view->tagGet<ZFAnimation *>(_ZFP_ZFUIViewBlink_tag_ani);
    if(ani != zfnull)
    {
        ani->aniStop();
    }
    else
    {
        _ZFP_ZFUIViewBlink_noAni_doOff(view);
    }
}

// ============================================================
// other
zfautoObject ZFUIViewBlinkImageDefault;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewBlink_dataInit, ZFLevelZFFrameworkLow)
{
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    #if _ZFP_ZFUIViewBlink_DEBUG_color
        ZFUIViewBlinkImageDefault = ZFUIImageLoadFromColor(ZFUIColorMake(255, 255, 0, 0));
    #else
        ZFUIViewBlinkImageDefault = ZFUIImageResXml(zfText("ZFFramework/ZFUIKit/ZFUIViewBlinkImage.xml"));
    #endif
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewBlink_dataInit)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewBlinkDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewBlinkDataHolder);
    while(!d->blinkingViews.isEmpty())
    {
        _ZFP_ZFUIViewBlinkDoOff(d->blinkingViews[d->blinkingViews.count() - 1]);
    }
    ZFUIViewBlinkImageDefault = zfautoObjectNull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewBlink_dataInit)

ZF_NAMESPACE_GLOBAL_END


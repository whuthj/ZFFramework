/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIScrollThumbDefault.h"
#include "ZFUIWindow.h"
#include "ZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIScrollThumbDefaultStyle, ZFStyleable)

// ============================================================
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAni : zfextends ZFAnimationTimeLine
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAni, ZFAnimationTimeLine)

private:
    ZFUIImageView *thumbView;

public:
    virtual ZFObject *objectOnInit(ZF_IN ZFUIImageView *thumbView)
    {
        this->objectOnInit();
        this->thumbView = thumbView;
        return this;
    }
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress)
    {
        zfsuper::aniTimeLineOnUpdate(progress);
        if(this->thumbView->viewVisible())
        {
            this->thumbView->viewAlphaSet(1 - progress);
        }
    }
};
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView : zfextends ZFAnimationNativeView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView, ZFAnimationNativeView)

public:
    virtual ZFObject *objectOnInit(ZF_IN ZFUIImageView *thumbView)
    {
        this->objectOnInit();
        this->aniTargetSet(thumbView);
        this->aniAlphaToSet(0);
        return this;
    }
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        return zfsuper::objectOnInit();
    }
};
zfclassNotPOD _ZFP_ZFUIScrollThumbDefaultPrivate
{
public:
    zfbool horizontal;
    ZFUIImageView *thumbView;
    ZFAnimation *thumbHideAni;
    ZFListener thumbHideAniAutoStopListener;
    zfint lastPos;
    zfint lastSize;
    zfbool firstTime;
public:
    _ZFP_ZFUIScrollThumbDefaultPrivate(void)
    : horizontal(zffalse)
    , thumbView(zfnull)
    , thumbHideAni(zfnull)
    , thumbHideAniAutoStopListener()
    , lastPos(0)
    , lastSize(0)
    , firstTime(zftrue)
    {
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIScrollThumbDefault)

ZFObject *ZFUIScrollThumbDefault::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIScrollThumbDefaultPrivate);

    d->thumbView = zfAllocWithoutLeakTest(ZFUIImageView);

    if(ZFProtocolIsAvailable(zfText("ZFAnimationNativeView")))
    {
        d->thumbHideAni = zfAllocWithoutLeakTest(_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView, d->thumbView);
    }
    else
    {
        d->thumbHideAni = zfAllocWithoutLeakTest(_ZFP_ZFUIScrollThumbDefault_HideThumbAni, d->thumbView);
    }
    {
        zfblockedAllocWithoutLeakTest(ZFPointerHolder, userData, d);

        ZFLISTENER_LOCAL(aniOnStop, {
            const _ZFP_ZFUIScrollThumbDefaultPrivate *d = userData->to<ZFPointerHolder *>()->holdedDataPointer<const _ZFP_ZFUIScrollThumbDefaultPrivate *>();
            d->thumbView->viewAlphaSet(1);
            d->thumbView->viewVisibleSet(zffalse);
        })
        d->thumbHideAni->observerAdd(ZFAnimation::EventAniOnStop(), aniOnStop, userData);

        ZFLISTENER_LOCAL(thumbHideAniAutoStop, {
            const _ZFP_ZFUIScrollThumbDefaultPrivate *d = userData->to<ZFPointerHolder *>()->holdedDataPointer<const _ZFP_ZFUIScrollThumbDefaultPrivate *>();
            d->thumbHideAni->aniStop();
        })
        d->thumbHideAniAutoStopListener = thumbHideAniAutoStop;
        ZFGlobalEventCenter::instance()->observerAdd(ZFUIWindow::EventWindowOnHide(), d->thumbHideAniAutoStopListener, userData);
    }

    return this;
}
void ZFUIScrollThumbDefault::objectOnDealloc(void)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIWindow::EventWindowOnHide(), d->thumbHideAniAutoStopListener);

    d->thumbHideAni->aniStop();
    zfReleaseWithoutLeakTest(d->thumbHideAni);
    zfReleaseWithoutLeakTest(d->thumbView);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIScrollThumbDefault, ZFUIImage *, scrollThumbImageHorizontal)
{
    zfsuperI(ZFUIScrollThumbDefaultStyle)::scrollThumbImageHorizontalSet(newValue);

    if(d->horizontal)
    {
        d->thumbView->imageContentSet(this->scrollThumbImageHorizontal());
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIScrollThumbDefault, ZFUIImage *, scrollThumbImageVertical)
{
    zfsuperI(ZFUIScrollThumbDefaultStyle)::scrollThumbImageHorizontalSet(newValue);

    if(!d->horizontal)
    {
        d->thumbView->imageContentSet(this->scrollThumbImageVertical());
    }
}

void ZFUIScrollThumbDefault::scrollThumbInit(ZF_IN zfbool horizontal)
{
    d->horizontal = horizontal;
    d->lastPos = 0;
    d->lastSize = 0;

    this->scrollView()->internalForegroundViewAdd(d->thumbView);

    if(horizontal)
    {
        d->thumbView->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner | ZFUIAlign::e_BottomInner);
        d->thumbView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        d->thumbView->imageContentSet(this->scrollThumbImageHorizontal());
    }
    else
    {
        d->thumbView->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
        d->thumbView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
        d->thumbView->imageContentSet(this->scrollThumbImageVertical());
    }

    d->thumbView->viewVisibleSet(zffalse);
}
void ZFUIScrollThumbDefault::scrollThumbDealloc(void)
{
    d->thumbHideAni->aniStop();

    this->scrollView()->internalForegroundViewRemove(d->thumbView);
}

void ZFUIScrollThumbDefault::scrollThumbUpdate(void)
{
    if(this->scrollView()->scrollViewState() != ZFUIScrollViewState::e_Idle)
    {
        d->thumbHideAni->aniStop();
        d->thumbView->viewAlphaSet(1);
    }

    if(d->thumbView->imageContent() == zfnull)
    {
        d->thumbView->viewVisibleSet(zffalse);
    }
    else
    {
        ZFUISize imageSize = d->thumbView->imageContent()->imageSize();
        zfint pos = 0;
        zfint size = 0;
        zfint sizeRange = 0;
        zfbool viewVisibleNew = zffalse;
        if(d->horizontal)
        {
            ZFUIScrollThumb::scrollThumbPosCalc(
                pos,
                size,
                this->scrollView()->layoutedFrame().size.width,
                this->scrollView()->scrollContentFrame().point.x,
                this->scrollView()->scrollContentFrame().size.width,
                imageSize.width,
                imageSize.width / 2,
                imageSize.width / 2);
            sizeRange = this->scrollView()->layoutedFrame().size.width - imageSize.width;
            sizeRange = zfmMax(sizeRange, 0);
            size = zfmMin(size, sizeRange);
            if(size != 0)
            {
                viewVisibleNew = zftrue;
                d->thumbView->layoutParam()->layoutMarginSet(ZFUIMarginMake(pos, 0, 0, 0));
                d->thumbView->layoutParam()->sizeHintSet(ZFUISizeMake(size, imageSize.height));
            }
        }
        else
        {
            ZFUIScrollThumb::scrollThumbPosCalc(
                pos,
                size,
                this->scrollView()->layoutedFrame().size.height,
                this->scrollView()->scrollContentFrame().point.y,
                this->scrollView()->scrollContentFrame().size.height,
                imageSize.height,
                imageSize.height / 2,
                imageSize.height / 2);
            sizeRange = this->scrollView()->layoutedFrame().size.height - imageSize.height;
            sizeRange = zfmMax(sizeRange, 0);
            size = zfmMin(size, sizeRange);
            if(size != 0)
            {
                viewVisibleNew = zftrue;
                d->thumbView->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, pos, 0, 0));
                d->thumbView->layoutParam()->sizeHintSet(ZFUISizeMake(imageSize.width, size));
            }
        }

        if(d->lastPos == 0 && d->lastSize == 0)
        {
            d->lastPos = pos;
            d->lastSize = size;
        }

        if(viewVisibleNew
            && (this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle
                || size == sizeRange)
            && pos == d->lastPos && size == d->lastSize
            && !d->thumbView->viewVisible())
        {
            viewVisibleNew = zffalse;
        }
        else
        {
            if(d->firstTime && viewVisibleNew && this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle
                && size == sizeRange)
            {
                d->firstTime = zffalse;
                viewVisibleNew = zffalse;
            }
            d->lastPos = pos;
            d->lastSize = size;
        }

        if(!viewVisibleNew)
        {
            d->thumbHideAni->aniStop();
        }
        d->thumbView->viewVisibleSet(viewVisibleNew);
    }

    // auto hide scroll thumb
    if(d->thumbView->viewVisible()
        && this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle)
    {
        d->thumbHideAni->aniStop();
        d->thumbView->viewVisibleSet(zftrue);
        zftimet autoHideTime = (d->horizontal ? this->scrollThumbAutoHideDurationHorizontal() : this->scrollThumbAutoHideDurationVertical());
        if(autoHideTime > 0)
        {
            d->thumbHideAni->aniDelaySet(d->horizontal ? this->scrollThumbAutoHideDelayHorizontal() : this->scrollThumbAutoHideDelayVertical());
            d->thumbHideAni->aniDurationSet(autoHideTime);
            d->thumbHideAni->aniStart();
        }
    }

    d->thumbView->layout(
        ZFUIViewLayoutParam::layoutParamApply(
            ZFUIRectGetBounds(d->thumbView->viewParent()->layoutedFrame()),
            d->thumbView,
            d->thumbView->layoutParam()));
}

ZF_NAMESPACE_GLOBAL_END


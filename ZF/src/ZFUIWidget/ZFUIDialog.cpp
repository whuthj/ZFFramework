/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIWidget.hh"
#include "ZFUIDialog.h"
#include "ZFUIButton.h"
#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFObjectCreator ZFUIDialogDefaultLayoutParamCreator = zfnull;
zfautoObject ZFUIDialogDefaultLayoutParamCreatorDefault(void)
{
    zfblockedAlloc(ZFUIViewLayoutParam, lp);
    lp->layoutAlignSet(ZFUIAlign::e_Center);
    lp->layoutMarginSet(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
    return zfautoObjectCreate(lp);
}
ZFObjectCreator ZFUIDialogDefaultAniShowCreator = zfnull;
zfautoObject ZFUIDialogDefaultAniShowCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaFromSet(0);
    return zfautoObjectCreate(ani);
}
ZFObjectCreator ZFUIDialogDefaultAniHideCreator = zfnull;
zfautoObject ZFUIDialogDefaultAniHideCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaToSet(0);
    return zfautoObjectCreate(ani);
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogDefaultCreatorInit, ZFLevelZFFrameworkLow)
{
    ZFUIDialogDefaultLayoutParamCreator = ZFUIDialogDefaultLayoutParamCreatorDefault;
    ZFUIDialogDefaultAniShowCreator = ZFUIDialogDefaultAniShowCreatorDefault;
    ZFUIDialogDefaultAniHideCreator = ZFUIDialogDefaultAniHideCreatorDefault;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogDefaultCreatorInit)

// ============================================================
// ZFUIDialogStyle
ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogStyle, ZFUIStyle)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAllDialog, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAllDialog)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(this->allDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
ZFCoreArrayPOD<ZFUIDialog *> allDialog;
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAllDialog)
#define _ZFP_ZFUIDialogAllDialog ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogAllDialog)->allDialog

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAutoHide, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAutoHide)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(_ZFP_ZFUIDialogAllDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAutoHide)

// ============================================================
// _ZFP_ZFUIDialogPrivate
/*
 * memo:
 *
 * _ZFP_ZFUIDialogPrivate (ZFUIWindow, handle all key event)
 * ^ dialogWindowBg (focusable, exclude from ZFUIViewFocusNextFind)
 *   dialogBg
 *   ^ dialogContainer
 *     ^ (fg) dialogView
 */
zfclass _ZFP_ZFUIDialogPrivate : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogPrivate, ZFUIWindow)

public:
    ZFUIDialog *owner;
    ZFUIButton *dialogWindowBg;
    ZFAnimationNativeView *dialogWindowAniShow;
    ZFAnimationNativeView *dialogWindowAniHide;
    ZFUIImageView *dialogBg;
    ZFUIView *dialogContainer;
    ZFListener aniShowOnStopListener;
    ZFListener aniHideOnStopListener;

public:
    void aniStop(void)
    {
        if(this->dialogWindowAniShow != zfnull)
        {
            this->dialogWindowAniShow->aniStop();
        }
        if(this->dialogWindowAniHide != zfnull)
        {
            this->dialogWindowAniHide->aniStop();
        }
        if(this->owner->dialogAniShow() != zfnull)
        {
            this->owner->dialogAniShow()->aniStop();
        }
        if(this->owner->dialogAniHide() != zfnull)
        {
            this->owner->dialogAniHide()->aniStop();
        }
    }

public:
    ZFLISTENER_MEMBER_DECLARE(dialogWindowBgOnClick)
    {
        if(this->owner->dialogHideWhenTouchOutside())
        {
            this->owner->dialogHide();
        }
    }
    ZFLISTENER_MEMBER_DECLARE(aniShowOnStop)
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->owner->dialogAniShow() == zfnull || !this->owner->dialogAniShow()->aniRunning())
            && (this->owner->dialogAniHide() == zfnull || !this->owner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTreeSet(zftrue);
            ZFUIViewFocusAutoHighlightPause();
            if(this->owner->dialogFocusAutomatically())
            {
                ZFUIView *next = ZFUIViewFocusNextMove(this->dialogContainer,
                    ZFUIViewFocusNextParam().focusDirectionSet(ZFUIOrientation::e_Left | ZFUIOrientation::e_Top));
                if(next == zfnull)
                {
                    this->dialogWindowBg->viewFocusRequest(zftrue);
                }
            }
            else
            {
                this->dialogWindowBg->viewFocusRequest(zftrue);
            }
            ZFUIViewFocusAutoHighlightResume();
            this->owner->dialogAfterShow();
        }
    }
    ZFLISTENER_MEMBER_DECLARE(aniHideOnStop)
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->owner->dialogAniShow() == zfnull || !this->owner->dialogAniShow()->aniRunning())
            && (this->owner->dialogAniHide() == zfnull || !this->owner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTreeSet(zftrue);
            this->windowHide();
            this->owner->dialogAfterHide();
            zfRelease(this->owner);
        }
    }

protected:
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        this->dialogWindowBg->layout(bounds);

        ZFUIMargin dialogMargin = this->owner->dialogLayoutParam()->layoutMargin();
        ZFUIMargin contentMargin = ZFUIMarginZero;
        if(this->owner->dialogBackgroundImage() != zfnull)
        {
            contentMargin = this->owner->dialogBackgroundImage()->imageNinePatch();
        }

        ZFUIMargin totalMargin = ZFUIMarginInc(dialogMargin, contentMargin);

        this->dialogContainer->layoutMeasure(ZFUISizeApplyMargin(bounds.size, totalMargin), this->owner->dialogLayoutParam()->sizeParam());
        ZFUISize dialogContainerSize = this->dialogContainer->layoutMeasuredSize();
        if(this->owner->dialogView() != zfnull && this->owner->dialogView()->viewVisible())
        {
            this->owner->dialogView()->layoutMeasure(ZFUISizeApplyMargin(bounds.size, totalMargin), this->owner->dialogLayoutParam()->sizeParam());
            dialogContainerSize.width = zfmMax(dialogContainerSize.width, this->owner->dialogView()->layoutMeasuredSize().width);
            dialogContainerSize.height = zfmMax(dialogContainerSize.height, this->owner->dialogView()->layoutMeasuredSize().height);
        }

        ZFUISize dialogBgSize = dialogContainerSize;
        dialogBgSize.width += ZFUIMarginGetX(contentMargin);
        dialogBgSize.height += ZFUIMarginGetY(contentMargin);
        this->dialogBg->layout(ZFUIAlignApply(
            this->owner->dialogLayoutParam()->layoutAlign(),
            bounds,
            dialogBgSize,
            dialogMargin));

        this->dialogContainer->layout(ZFUIAlignApply(
            this->owner->dialogLayoutParam()->layoutAlign(),
            ZFUIRectMake(ZFUIPointZero, dialogBgSize),
            dialogContainerSize,
            contentMargin));
    }

    // event handle
protected:
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
    {
        // dialog would always resolve key event
        keyEvent->eventResolvedSet(zftrue);

        if(!this->owner->dialogHideWhenClickBack()
            || keyEvent->keyAction != ZFUIKeyAction::e_KeyUp)
        {
            return ;
        }

        switch(keyEvent->keyCode)
        {
            case ZFUIKeyCode::e_kPhoneBack:
            case ZFUIKeyCode::e_kEsc:
                this->owner->dialogHide();
                break;
            default:
                break;
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFUIDialogPrivate)

zfclass _ZFP_ZFUIDialog_DialogWindowBg : zfextends ZFUIButton
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialog_DialogWindowBg, ZFUIButton)
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogIgnoreFromFocus, ZFLevelZFFrameworkNormal)
{
    ZFUIViewFocusNextFilter.classFilter.filterAdd(_ZFP_ZFUIDialog_DialogWindowBg::ClassData());
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogIgnoreFromFocus)

// ============================================================
// ZFUIDialog
ZFOBJECT_REGISTER(ZFUIDialog)

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIDialog, ZFUIColor, dialogWindowColor)
{
    zfsuperI(ZFUIDialogStyle)::dialogWindowColorSet(newValue);
    d->dialogWindowBg->viewBackgroundColorSet(this->dialogWindowColor());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIDialog, ZFUIImage *, dialogBackgroundImage)
{
    zfsuperI(ZFUIDialogStyle)::dialogBackgroundImageSet(newValue);
    d->dialogBg->imageContentSet(this->dialogBackgroundImage());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIDialog, ZFUIView *, dialogView)
{
    if(this->dialogView() == newValue)
    {
        return ;
    }
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->internalForegroundViewRemove(this->dialogView());
    }
    zfsuperI(ZFUIDialogStyle)::dialogViewSet(newValue);
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->internalForegroundViewAdd(this->dialogView());
        this->dialogView()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    }
}

zfbool ZFUIDialog::dialogShowing(void)
{
    return d->windowShowing();
}
void ZFUIDialog::dialogShow(void)
{
    if(d->windowShowing())
    {
        return ;
    }
    zfRetain(this);
    d->aniStop();
    d->windowShow();
    d->viewUIEnableTreeSet(zffalse);
    this->dialogBeforeShow();
    if(this->dialogWindowColor() != ZFUIColorTransparent && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniShow->aniTargetSet(d->dialogWindowBg);
        d->dialogWindowAniShow->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniShowOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        d->dialogWindowAniShow->aniStart();
    }
    if(this->dialogAniShow() != zfnull)
    {
        this->dialogAniShow()->aniTargetSet(d->dialogBg);
        this->dialogAniShow()->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniShowOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        this->dialogAniShow()->aniStart();
    }
}
void ZFUIDialog::dialogHide(void)
{
    if(!d->windowShowing())
    {
        return ;
    }
    d->aniStop();
    if(!d->windowShowing())
    {
        return ;
    }
    d->viewUIEnableTreeSet(zffalse);
    this->dialogBeforeHide();
    if(this->dialogWindowColor() != ZFUIColorTransparent && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniHide->aniTargetSet(d->dialogWindowBg);
        d->dialogWindowAniHide->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniHideOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        d->dialogWindowAniHide->aniStart();
    }
    if(this->dialogAniHide() != zfnull)
    {
        this->dialogAniHide()->aniTargetSet(d->dialogBg);
        this->dialogAniHide()->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniHideOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        this->dialogAniHide()->aniStart();
    }
}
void ZFUIDialog::dialogHideForce(void)
{
    if(d->windowShowing())
    {
        zfRetainWithoutLeakTest(this);
        d->aniStop();
        if(d->windowShowing())
        {
            this->dialogBeforeHide();
            d->windowHide();
            this->dialogAfterHide();
        }
        zfReleaseWithoutLeakTest(this);
    }
}

void ZFUIDialog::dialogApplyAutoHide(ZF_IN ZFUIButton *button)
{
    if(button == zfnull)
    {
        return ;
    }
    ZFLISTENER_LOCAL(buttonOnClick, {
        userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialog *>()->dialogHide();
    })
    button->observerAdd(ZFObject::ObserverAddParam()
            .eventIdSet(ZFUIButton::EventButtonOnClick())
            .observerSet(buttonOnClick)
            .userDataSet(this->objectHolder())
            .observerLevelSet(ZFLevelAppLow)
        );
}

ZFUIWindow *ZFUIDialog::dialogWindow(void)
{
    return d;
}

ZFUIView *ZFUIDialog::dialogInternalContainer(void)
{
    return d->dialogContainer;
}
ZFUIView *ZFUIDialog::dialogInternalBackgroundContainer(void)
{
    return d->dialogBg;
}

ZFObject *ZFUIDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAllocWithoutLeakTest(_ZFP_ZFUIDialogPrivate);
    d->owner = this;
    d->windowLevelSet(ZFUIWindowLevel::e_AppHigh);

    d->dialogWindowBg = zfAllocWithoutLeakTest(_ZFP_ZFUIDialog_DialogWindowBg);
    d->childAdd(d->dialogWindowBg);
    d->dialogWindowBg->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    d->dialogWindowBg->observerAdd(ZFUIButton::EventButtonOnClick(),
        ZFCallbackForMemberMethod(d, ZFMethodAccessClassMember(_ZFP_ZFUIDialogPrivate, dialogWindowBgOnClick)));

    d->dialogBg = zfAllocWithoutLeakTest(ZFUIImageView);
    d->childAdd(d->dialogBg);
    d->dialogBg->viewUIEnableTreeSet(zftrue);
    d->dialogBg->viewUIEnableSet(zftrue);

    d->dialogContainer = zfAllocWithoutLeakTest(ZFUIViewLayout);
    d->dialogBg->childAdd(d->dialogContainer);

    d->dialogWindowAniShow = zfAllocWithoutLeakTest(ZFAnimationNativeView);
    d->dialogWindowAniShow->aniAlphaFromSet(0);
    d->dialogWindowAniHide = zfAllocWithoutLeakTest(ZFAnimationNativeView);
    d->dialogWindowAniHide->aniAlphaToSet(0);

    d->aniShowOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccessClassMember(_ZFP_ZFUIDialogPrivate, aniShowOnStop));
    d->aniHideOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccessClassMember(_ZFP_ZFUIDialogPrivate, aniHideOnStop));

    d->dialogWindowBg->viewBackgroundColorSet(this->dialogWindowColor());
    d->dialogBg->imageContentSet(this->dialogBackgroundImage());

    _ZFP_ZFUIDialogAllDialog.add(this);
    return this;
}
void ZFUIDialog::objectOnDealloc(void)
{
    _ZFP_ZFUIDialogAllDialog.removeElement(this);

    ZFPropertyChangeWithoutLeakTest(d->dialogWindowAniShow, zfnull);
    ZFPropertyChangeWithoutLeakTest(d->dialogWindowAniHide, zfnull);

    zfReleaseWithoutLeakTest(d->dialogContainer);
    zfReleaseWithoutLeakTest(d->dialogBg);
    zfReleaseWithoutLeakTest(d->dialogWindowBg);
    zfReleaseWithoutLeakTest(d);
    d = zfnull;

    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END


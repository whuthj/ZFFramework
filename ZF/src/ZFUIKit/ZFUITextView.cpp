/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUITextView.h"
#include "protocol/ZFProtocolZFUITextView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUITextViewStyle, ZFUIViewStyle)

// ============================================================
// _ZFP_ZFUITextViewPrivate
zfclassNotPOD _ZFP_ZFUITextViewPrivate
{
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUITextView) *impl;
    ZFUITextView *thisView;

public:
    void updateSizeRelatedProperty(void)
    {
        this->impl->textShadowOffsetSet(this->thisView, ZFUISizeApplyScale(this->thisView->textShadowOffset(), this->thisView->scaleGetFixed()));
        this->impl->textSizeSet(this->thisView, ZFUISizeApplyScale(this->thisView->textSize(), this->thisView->scaleGetFixed()));
        this->impl->textSizeAutoChangeMinSizeSet(this->thisView, ZFUISizeApplyScale(this->thisView->textSizeAutoChangeMinSize(), this->thisView->scaleGetFixed()));
        this->impl->textSizeAutoChangeMaxSizeSet(this->thisView, ZFUISizeApplyScale(this->thisView->textSizeAutoChangeMaxSize(), this->thisView->scaleGetFixed()));
    }

public:
    _ZFP_ZFUITextViewPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUITextView))
    , thisView(zfnull)
    {
    }
};

// ============================================================
// ZFUITextView
ZFOBJECT_REGISTER(ZFUITextView)

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, ZFString *, textContent)
{
    zfbool changed = (ZFObjectCompare(this->textContent(), newValue) != ZFCompareTheSame);
    zfsuperI(ZFUITextViewStyle)::textContentSet(newValue);
    d->impl->textContentSet(this, this->textContent());
    if(changed)
    {
        this->layoutRequest();
    }
}

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, ZFUITextAppearanceEnum, textAppearance)
{
    zfbool changed = (this->textAppearance() != newValue);
    zfsuperI(ZFUITextViewStyle)::textAppearanceSet(newValue);
    d->impl->textAppearanceSet(this, this->textAppearance());
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, ZFUIAlignFlags, textAlign)
{
    zfsuperI(ZFUITextViewStyle)::textAlignSet(newValue);
    d->impl->textAlignSet(this, this->textAlign());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, ZFUIColor, textColor)
{
    zfsuperI(ZFUITextViewStyle)::textColorSet(newValue);
    d->impl->textColorSet(this, this->textColor());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, ZFUIColor, textShadowColor)
{
    zfsuperI(ZFUITextViewStyle)::textShadowColorSet(newValue);
    d->impl->textShadowColorSet(this, this->textShadowColor());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, ZFUISize, textShadowOffset)
{
    zfsuperI(ZFUITextViewStyle)::textShadowOffsetSet(newValue);
    d->impl->textShadowOffsetSet(this, ZFUISizeApplyScale(this->textShadowOffset(), this->scaleGetFixed()));
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, zfint, textSize)
{
    zfbool changed = (this->textSize() != newValue);
    zfsuperI(ZFUITextViewStyle)::textSizeSet(newValue);
    d->impl->textSizeSet(this, ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, zfint, textSizeAutoChangeMinSize)
{
    zfbool changed = (this->textSizeAutoChangeMinSize() != newValue);
    zfsuperI(ZFUITextViewStyle)::textSizeAutoChangeMinSizeSet(newValue);
    d->impl->textSizeAutoChangeMinSizeSet(this, ZFUISizeApplyScale(this->textSizeAutoChangeMinSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, zfint, textSizeAutoChangeMaxSize)
{
    zfbool changed = (this->textSizeAutoChangeMaxSize() != newValue);
    zfsuperI(ZFUITextViewStyle)::textSizeAutoChangeMaxSizeSet(newValue);
    d->impl->textSizeAutoChangeMaxSizeSet(this, ZFUISizeApplyScale(this->textSizeAutoChangeMaxSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, zfbool, textSingleLine)
{
    zfbool changed = (this->textSingleLine() != newValue);
    zfsuperI(ZFUITextViewStyle)::textSingleLineSet(newValue);
    d->impl->textSingleLineSet(this, this->textSingleLine());
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextView, ZFUITextTruncateModeEnum, textTruncateMode)
{
    zfbool changed = (this->textTruncateMode() != newValue);
    zfsuperI(ZFUITextViewStyle)::textTruncateModeSet(newValue);
    d->impl->textTruncateModeSet(this, this->textTruncateMode());
    if(changed)
    {
        this->layoutRequest();
    }
}

ZFObject *ZFUITextView::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUITextViewPrivate);
    d->thisView = this;

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUITextView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUITextView)->nativeTextViewDestroy(view->to<ZFUITextView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(d->impl->nativeTextViewCreate(this),
                            _ZFP_ZFUITextView_nativeImplViewDestroy::action);

    return this;
}
void ZFUITextView::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUITextView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->updateSizeRelatedProperty();
}

void ZFUITextView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(this->textContent() != zfnull && this->textContent()->length() > 0)
    {
        ret += zfText(" \"");
        ret += this->textContent()->stringValue();
        ret += zfText("\"");
    }
}

ZFUISize ZFUITextView::measureTextView(ZF_IN const ZFUISize &sizeHint)
{
    return ZFUISizeApplyScaleReversely(d->impl->measureNativeTextView(this,
        ZFUISizeApplyScale(sizeHint, this->scaleGetFixed()),
        ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed())),
        this->scaleGetFixed());
}

zfint ZFUITextView::textSizeCurrent(void)
{
    return ZFUISizeApplyScaleReversely(d->impl->textSizeCurrent(this), this->scaleGetFixed());
}

void ZFUITextView::scaleOnChange(void)
{
    zfsuper::scaleOnChange();
    d->updateSizeRelatedProperty();
}
void ZFUITextView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = ZFUIViewLayoutParam::sizeHintApply(this->measureTextView(sizeHint), sizeHint, sizeParam);
}
void ZFUITextView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayout(bounds);
    d->impl->layoutNativeTextView(this, ZFUISizeApplyScale(bounds.size, this->scaleGetFixed()));
}

ZF_NAMESPACE_GLOBAL_END


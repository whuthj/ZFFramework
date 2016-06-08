/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUITextEdit.h"
#include "protocol/ZFProtocolZFUITextEdit.h"

#include "ZFUIViewFocus.h"
#include "ZFUIOnScreenKeyboardState.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUITextEditKeyboardType)

ZFENUM_DEFINE(ZFUITextEditKeyboardReturnType)
ZFENUM_DEFINE(ZFUITextEditKeyboardReturnAction)

ZFSTYLE_DEFAULT_DEFINE(ZFUITextEditStyle, ZFUIViewStyle)

// ============================================================
// _ZFP_ZFUITextEditPrivate
zfclassNotPOD _ZFP_ZFUITextEditPrivate
{
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUITextEdit) *impl;
    ZFUITextEdit *thisView;
    zfbool textEditing;
    zfbool textChangedByImplFlag;

public:
    void updateSizeRelatedProperty(void)
    {
        this->impl->textShadowOffsetSet(this->thisView, ZFUISizeApplyScale(this->thisView->textShadowOffset(), this->thisView->scaleGetFixed()));
        this->impl->textSizeSet(this->thisView, ZFUISizeApplyScale(this->thisView->textSize(), this->thisView->scaleGetFixed()));
    }

public:
    _ZFP_ZFUITextEditPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUITextEdit))
    , thisView(zfnull)
    , textEditing(zffalse)
    , textChangedByImplFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUITextEdit
ZFOBJECT_REGISTER(ZFUITextEdit)

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, zfbool, textEditEnable)
{
    zfsuperI(ZFUITextEditStyle)::textEditEnableSet(newValue);
    d->impl->textEditEnableSet(this, this->textEditEnable());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, zfbool, textEditSecured)
{
    zfsuperI(ZFUITextEditStyle)::textEditSecuredSet(newValue);
    d->impl->textEditSecuredSet(this, this->textEditSecured());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardTypeEnum, textEditKeyboardType)
{
    zfsuperI(ZFUITextEditStyle)::textEditKeyboardTypeSet(newValue);
    d->impl->textEditKeyboardTypeSet(this, this->textEditKeyboardType());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardReturnTypeEnum, textEditKeyboardReturnType)
{
    zfsuperI(ZFUITextEditStyle)::textEditKeyboardReturnTypeSet(newValue);
    d->impl->textEditKeyboardReturnTypeSet(this, this->textEditKeyboardReturnType());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUITextEditKeyboardReturnActionEnum, textEditKeyboardReturnAction)
{
    zfsuperI(ZFUITextEditStyle)::textEditKeyboardReturnActionSet(newValue);
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFRegExp *, textEditFilter)
{
    zfsuperI(ZFUITextEditStyle)::textEditFilterSet(newValue);
    if(this->textContent() != zfnull && !this->textShouldChange(this->textContent()))
    {
        this->textContentSet(zfnull);
    }
}

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFString *, textContent)
{
    ZFString *newValueTmp = newValue;
    if(newValue != zfnull && !newValue->isEmpty() && !this->textShouldChange(newValue))
    {
        newValueTmp = zfnull;
    }

    zfsuperI(ZFUITextEditStyle)::textContentSet(newValueTmp);
    if(!d->textChangedByImplFlag)
    {
        d->impl->textContentSet(this, this->textContent());
    }
    ZFCastZFObjectUnchecked(ZFUIView *, this->textPlaceHolder())->viewVisibleSet(
        this->textContent() == zfnull || this->textContent()->isEmpty());
    this->layoutRequest();
}

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUITextAppearanceEnum, textAppearance)
{
    zfbool changed = (this->textAppearance() != newValue);
    zfsuperI(ZFUITextEditStyle)::textAppearanceSet(newValue);
    d->impl->textAppearanceSet(this, this->textAppearance());
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUIAlignFlags, textAlign)
{
    zfsuperI(ZFUITextEditStyle)::textAlignSet(newValue);
    d->impl->textAlignSet(this, this->textAlign());
    this->textPlaceHolder()->textAlignSet(this->textAlign());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUIColor, textColor)
{
    zfsuperI(ZFUITextEditStyle)::textColorSet(newValue);
    d->impl->textColorSet(this, this->textColor());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUIColor, textShadowColor)
{
    zfsuperI(ZFUITextEditStyle)::textShadowColorSet(newValue);
    d->impl->textShadowColorSet(this, this->textShadowColor());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, ZFUISize, textShadowOffset)
{
    zfsuperI(ZFUITextEditStyle)::textShadowOffsetSet(newValue);
    d->impl->textShadowOffsetSet(this, ZFUISizeApplyScale(this->textShadowOffset(), this->scaleGetFixed()));
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUITextEdit, zfint, textSize)
{
    zfbool changed = (this->textSize() != newValue);
    zfsuperI(ZFUITextEditStyle)::textSizeSet(newValue);
    d->impl->textSizeSet(this, ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed()));
    if(changed)
    {
        this->layoutRequest();
    }
}

ZFObject *ZFUITextEdit::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUITextEditPrivate);
    d->thisView = this;

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUITextEdit_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUITextEdit)->nativeTextEditDestroy(view->to<ZFUITextEdit *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(d->impl->nativeTextEditCreate(this),
        _ZFP_ZFUITextEdit_nativeImplViewDestroy::action);

    zfautoObject textPlaceHolderLayoutParamTmp = this->layoutParamCreate();
    ZFUIViewLayoutParam *textPlaceHolderLayoutParam = textPlaceHolderLayoutParamTmp.to<ZFUIViewLayoutParam *>();
    textPlaceHolderLayoutParam->sizeParamSet(ZFUISizeParamFillWidthFillHeight);
    textPlaceHolderLayoutParam->layoutMarginSet(ZFUIMarginZero);
    ZFUIView *textPlaceHolderTmp = ZFCastZFObject(ZFUIView *, this->textPlaceHolder());
    if(textPlaceHolderTmp == zfnull)
    {
        if(this->textPlaceHolder() != zfnull)
        {
            zfCoreCriticalMessage(zfTextA("textPlaceHolder must be type of %s, but it's %s"),
                ZFUIView::ClassData()->className(), this->textPlaceHolder()->classData()->className());
        }
        else
        {
            zfCoreCriticalMessage(zfTextA("textPlaceHolder must not be null"));
        }
        return zfnull;
    }
    this->internalBackgroundViewAdd(textPlaceHolderTmp, textPlaceHolderLayoutParam);

    return this;
}
void ZFUITextEdit::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUITextEdit::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->updateSizeRelatedProperty();
}

void ZFUITextEdit::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(this->textPlaceHolder()->textContent() != zfnull)
    {
        zfstringAppend(ret, zfText(" (%s)"), this->textPlaceHolder()->textContent()->stringValue());
    }

    if(this->textEditSecured())
    {
        ret += zfText(" EditSecured");
    }
    else if(this->textContent() != zfnull && this->textContent()->length() > 0)
    {
        zfstringAppend(ret, zfText(" \"%s\""), this->textContent()->stringValue());
    }

    if(!this->textEditEnable())
    {
        ret += zfText(" EditDisabled");
    }
}

ZFUISize ZFUITextEdit::measureTextEdit(ZF_IN_OPT const ZFUISize &sizeHint /* = ZFUISizeZero */)
{
    ZFUISize ret = ZFUISizeZero;
    ZFUISizeApplyScaleReversely(ret, d->impl->measureNativeTextEdit(this,
        ZFUISizeApplyScale(ZFUIViewLayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
                    0 - ZFUIMarginGetX(this->nativeImplViewMargin()),
                    0 - ZFUIMarginGetY(this->nativeImplViewMargin())
                )),
            this->scaleGetFixed()),
        ZFUISizeApplyScale(this->textSize(), this->scaleGetFixed())),
        this->scaleGetFixed());
    if(ZFPropertyIsValueAccessed(ZFPropertyAccess(ZFUITextEditStyle, textPlaceHolder), this)
        && this->textPlaceHolder()->textContentString() != zfnull)
    {
        ZFUISize hintSize = this->textPlaceHolder()->to<ZFUITextView *>()->measureTextView();
        if(ret.width < hintSize.width)
        {
            ret.width = hintSize.width;
        }
        if(ret.height < hintSize.height)
        {
            ret.height = hintSize.height;
        }
    }
    ZFUISizeApplyMarginReversely(ret, ret, this->nativeImplViewMargin());
    return ret;
}

void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyBeginEdit(void)
{
    d->textEditing = zftrue;
    this->textEditOnEditBegin();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyEndEdit(void)
{
    d->textEditing = zffalse;
    this->textEditOnEditEnd();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyChange(ZF_IN ZFString *newText)
{
    d->textChangedByImplFlag = zftrue;
    this->textContentSet(newText);
    d->textChangedByImplFlag = zffalse;

    this->textEditOnChange();
}
void ZFUITextEdit::_ZFP_ZFUITextEdit_textNotifyReturnClicked(void)
{
    this->textEditReturnOnClick();
    this->textEditOnConfirm();

    switch(this->textEditKeyboardReturnAction())
    {
        case ZFUITextEditKeyboardReturnAction::e_None:
            break;
        case ZFUITextEditKeyboardReturnAction::e_FocusNext:
            if(this->viewFocused())
            {
                ZFUIView *next = ZFUIViewFocusNextFind(this);
                if(next == zfnull)
                {
                    this->textEditEnd();
                }
                else
                {
                    ZFUITextEdit *nextTmp = ZFCastZFObject(ZFUITextEdit *, next);
                    if(nextTmp != zfnull)
                    {
                        nextTmp->textEditBegin();
                    }
                    else
                    {
                        next->viewFocusRequest(zftrue);
                    }
                }
            }
            break;
        case ZFUITextEditKeyboardReturnAction::e_HideKeyboard:
        {
            ZFUIOnScreenKeyboardState *keyboardState = ZFUIOnScreenKeyboardState::instanceForView(this);
            if(keyboardState != zfnull && keyboardState->keyboardShowing() && this->textEditing())
            {
                this->textEditEnd();
            }
        }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
zfbool ZFUITextEdit::textShouldChange(ZF_IN ZFString *newText)
{
    if(newText != zfnull && !newText->isEmpty() && !this->textOnCheckShouldChange(newText))
    {
        return zffalse;
    }
    return zftrue;
}
void ZFUITextEdit::textEditBegin(void)
{
    d->impl->textEditBegin(this);
}
void ZFUITextEdit::textEditEnd(void)
{
    d->impl->textEditEnd(this);
}
zfbool ZFUITextEdit::textEditing(void)
{
    return d->textEditing;
}

void ZFUITextEdit::textEditOnEditBegin(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnEditBegin());
    ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(this, ZFUITextEdit::EventTextEditOnEditBegin());
}
void ZFUITextEdit::textEditOnEditEnd(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnEditEnd());
    ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(this, ZFUITextEdit::EventTextEditOnEditEnd());
}
zfbool ZFUITextEdit::textOnCheckShouldChange(ZF_IN ZFString *newText)
{
    if(this->textEditFilter() != zfnull)
    {
        ZFRegExpResult regexpResult;
        this->textEditFilter()->regExpMatchExact(regexpResult, newText->stringValue());
        if(!regexpResult.matched)
        {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFUITextEdit::textEditOnChange(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnChange());
}
void ZFUITextEdit::textEditReturnOnClick(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditReturnOnClick());
}
void ZFUITextEdit::textEditOnConfirm(void)
{
    this->observerNotify(ZFUITextEdit::EventTextEditOnConfirm());
}

void ZFUITextEdit::scaleOnChange(void)
{
    zfsuper::scaleOnChange();
    d->updateSizeRelatedProperty();
}
void ZFUITextEdit::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeControl());
}
void ZFUITextEdit::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayout(bounds);
}
void ZFUITextEdit::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    switch(keyEvent->keyCode)
    {
        case ZFUIKeyCode::e_kShift:
        case ZFUIKeyCode::e_kTab:
            zfsuper::viewEventOnKeyEvent(keyEvent);
            break;
        default:
            keyEvent->eventResolvedSet(zftrue);
            break;
    }
}
void ZFUITextEdit::viewFocusOnChange(void)
{
    zfsuper::viewFocusOnChange();
    this->textEditOnConfirm();
}

ZF_NAMESPACE_GLOBAL_END


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIImageView.h"
#include "protocol/ZFProtocolZFUIImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIImageViewStyle, ZFUIViewStyle)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageViewListenerHolder, ZFLevelZFFrameworkNormal)
{
    this->imageNinePatchChangedListener = ZFCallbackForRawFunction(zfself::imageNinePatchChanged);
}
ZFListener imageNinePatchChangedListener;
static ZFLISTENER_PROTOTYPE_EXPAND(imageNinePatchChanged)
{
    const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
    if(property == ZFPropertyAccess(ZFUIImage, imageNinePatch))
    {
        ZFUIImageView *imageView = userData->to<ZFObjectHolder *>()->holdedObj;
        ZFUIImage *image = imageView->imageContent();
        ZFPROTOCOL_ACCESS(ZFUIImageView)->imageNinePatchChanged(
            imageView,
            image->imageScaleFixed(),
            ZFUIMarginApplyScale(image->imageNinePatch(), image->imageScaleFixed()));
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIImageViewListenerHolder)

// ============================================================
// ZFUIImageView
ZFOBJECT_REGISTER(ZFUIImageView)

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIImageView, ZFUIImage *, imageContent)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageViewListenerHolder);
    if(this->imageContent() != zfnull)
    {
        this->imageContent()->observerRemove(ZFObject::EventObjectPropertyValueAfterChange(), listenerHolder->imageNinePatchChangedListener);
    }

    zfsuperI(ZFUIImageViewStyle)::imageContentSet(newValue);
    ZFPROTOCOL_ACCESS(ZFUIImageView)->imageContentSet(this, this->imageContent());

    if(this->imageContent() != zfnull)
    {
        this->imageContent()->observerAdd(ZFObject::EventObjectPropertyValueAfterChange(), listenerHolder->imageNinePatchChangedListener, this->objectHolder());
        ZFUIImage *image = this->imageContent();
        ZFPROTOCOL_ACCESS(ZFUIImageView)->imageNinePatchChanged(
            this,
            image->imageScaleFixed(),
            ZFUIMarginApplyScale(image->imageNinePatch(), image->imageScaleFixed()));
    }
    this->layoutRequest();
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIImageView, ZFUIContentScaleTypeEnum, imageScaleType)
{
    zfsuperI(ZFUIImageViewStyle)::imageScaleTypeSet(newValue);
    this->layoutRequest();
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIImageView, ZFUIMargin, imageContentMargin)
{
    zfsuperI(ZFUIImageViewStyle)::imageContentMarginSet(newValue);
    this->nativeImplViewMarginUpdate();
}

ZFObject *ZFUIImageView::objectOnInit(ZF_IN ZFUIImage *image)
{
    this->objectOnInit();
    this->imageContentSet(image);
    return this;
}
ZFObject *ZFUIImageView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUIImageView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewDestroy(view->to<ZFUIImageView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(
        ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewCreate(this),
        _ZFP_ZFUIImageView_nativeImplViewDestroy::action);

    return this;
}
void ZFUIImageView::objectOnDealloc(void)
{
    this->imageContentSet(zfnull);
    zfsuper::objectOnDealloc();
}

void ZFUIImageView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->imageContent() != zfnull)
    {
        ret += zfText(" ");
        this->imageContent()->objectInfoT(ret);
    }
}

void ZFUIImageView::nativeImplViewOnLayout(ZF_OUT ZFUIRect &result,
                                           ZF_IN const ZFUIRect &bounds)
{
    if(this->imageContent() == zfnull
        || bounds.size.width == 0
        || bounds.size.height == 0)
    {
        return ;
    }

    ZFUIContentScaleTypeApply(result, this->imageScaleType(), bounds, this->imageContent()->imageSize());
}
void ZFUIImageView::nativeImplViewMarginOnUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin)
{
    zfsuper::nativeImplViewMarginOnUpdate(nativeImplViewMargin);
    ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, this->imageContentMargin());
}
void ZFUIImageView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                    ZF_IN const ZFUISize &sizeHint,
                                    ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = ZFUIViewLayoutParam::sizeHintApply(
        (this->imageContent() != zfnull) ? this->imageContent()->imageSize() : ZFUISizeZero,
        sizeHint,
        sizeParam);
}

ZF_NAMESPACE_GLOBAL_END


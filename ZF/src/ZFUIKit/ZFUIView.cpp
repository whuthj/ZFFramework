/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIView.h"
#include "protocol/ZFProtocolZFUIView.h"
#include "protocol/ZFProtocolZFUIViewFocus.h"

#include "../ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "../ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIViewStyle, ZFStyleable)

// ============================================================
// attached listeners
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewListenerHolder, ZFLevelZFFrameworkNormal)
{
    this->layoutParamChangedListener = ZFCallbackForRawFunction(zfself::layoutParamChanged);
    this->viewPropertyOnUpdateListener = ZFCallbackForRawFunction(zfself::viewPropertyOnUpdate);
}
ZFListener layoutParamChangedListener;
ZFListener viewPropertyOnUpdateListener;
static ZFLISTENER_PROTOTYPE_EXPAND(layoutParamChanged)
{
    userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIView *>()->layoutRequest();
}
static ZFLISTENER_PROTOTYPE_EXPAND(viewPropertyOnUpdate)
{
    userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIView *>()->_ZFP_ZFUIView_viewPropertyNotifyUpdate();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewListenerHolder)

// ============================================================
// _ZFP_ZFUIViewPrivate
static zfindex _ZFP_ZFUIView_layoutRequestOverrideFlag = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewLayoutRequestOverrideInit, ZFLevelZFFrameworkHigh)
{
    _ZFP_ZFUIView_layoutRequestOverrideFlag = 0;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewLayoutRequestOverrideInit)

zfclassLikePOD _ZFP_ZFUIViewLayerData
{
public:
    ZFCoreArrayPOD<ZFUIView *> views;
};
typedef zfstlmap<zfstlstringZ, zfbool> _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType;
zfclassNotPOD _ZFP_ZFUIViewPrivate
{
public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl;
    ZFUIView *thisView;
    void *nativeView;
    void *nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback;

    ZFUIView *viewDelegateParent;
    ZFUIView *viewDelegate;

    ZFUIView *viewParent;
    ZFUIViewChildLayerEnum viewLayer;
    ZFUIViewLayoutParam *layoutParam; // retain
    const ZFClass *layoutParamClass;

    _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType internalViewAutoSerializeTags;

    // scale visible to app, 1 by default
    // used for app to manage scale, set by ZFUIRootView
    zffloat scaleForApp;
    // impl's scale, ZFUIViewImpl::nativeViewScaleForImpl by default
    // this value depends on impl's screen's scale, may have different value on different screen,
    // would be changed by impl while adding to native view
    zffloat scaleForImpl;
    // final scale value, scaleFixed = scaleForApp * scaleForImpl
    // (sizeInZFFramework * scaleFixed) would be the final pixel size that passed to implementation
    zffloat scaleFixed;
    /*
     * here's a memo for the scale logic:
     *
     * while adding to parent,
     * scaleForApp, scaleForImpl and scaleFixed would be automatically changed to parent's setting (during viewOnAddToParent)
     *
     * while adding to native view,
     * only scaleForImpl would be automatically changed to impl's scale (during nativeViewScaleForImpl),
     * but app's scale would be kept
     *
     * ZFUISysWindow would have its ZFUIRootView's scale setting set from ZFUISysWindow's implementation
     *
     * to change scale, app would use ZFUIRootView from ZFUISysWindow
     *
     * scaleOnChange would be called each time scaleFixed changed
     */

    ZFUISize lastMeasuredSize;
    ZFUISize lastMeasuredSizeHint;
    ZFUISizeParam lastMeasuredSizeParam;
    zfbool layoutRequested;
    zfbool layouting;
    ZFUIRect layoutedFramePrev;
    ZFUIRect layoutedFrame;
    _ZFP_ZFUIViewLayerData layerInternalBg;
    _ZFP_ZFUIViewLayerData layerNormal;
    _ZFP_ZFUIViewLayerData layerInternalFg;

public:
    _ZFP_ZFUIViewPrivate(void)
    : impl(ZFPROTOCOL_ACCESS(ZFUIView))
    , thisView(zfnull)
    , nativeView(zfnull)
    , nativeImplView(zfnull)
    , nativeImplViewDeleteCallback(zfnull)
    , viewDelegateParent(zfnull)
    , viewDelegate(zfnull)
    , viewParent(zfnull)
    , viewLayer(ZFUIViewChildLayer::e_Normal)
    , layoutParam(zfnull)
    , layoutParamClass(zfnull)
    , internalViewAutoSerializeTags()
    , scaleForApp(1)
    , scaleForImpl(1)
    , scaleFixed(1)
    , lastMeasuredSize()
    , lastMeasuredSizeHint()
    , lastMeasuredSizeParam()
    , layoutRequested(zftrue)
    , layouting(zffalse)
    , layoutedFramePrev(ZFUIRectZero)
    , layoutedFrame(ZFUIRectZero)
    , layerInternalBg()
    , layerNormal()
    , layerInternalFg()
    {
    }
    ~_ZFP_ZFUIViewPrivate(void)
    {
    }

public:
    void layoutParamSet(ZF_IN ZFUIViewLayoutParam *newLayoutParam)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder);
        if(this->layoutParam != zfnull)
        {
            if(this->thisView->objectCached())
            {
                this->layoutParam->objectCachedSet(zffalse);
            }
            this->layoutParam->observerRemove(ZFUILayoutParam::EventLayoutParamOnChange(), listenerHolder->layoutParamChangedListener);
        }
        ZFPropertyChange(this->layoutParam, newLayoutParam);
        if(newLayoutParam != zfnull)
        {
            if(this->thisView->objectCached())
            {
                this->layoutParam->objectCachedSet(zftrue);
            }
            newLayoutParam->observerAdd(
                ZFUILayoutParam::EventLayoutParamOnChange(),
                listenerHolder->layoutParamChangedListener,
                this->thisView->objectHolder());
        }
    }
    zfindex viewLayerPrevCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        if(&layer == &(this->layerNormal))
        {
            return this->layerInternalBg.views.count();
        }
        else if(&layer == &(this->layerInternalBg))
        {
            return 0;
        }
        else
        {
            return this->layerInternalBg.views.count() + this->layerNormal.views.count();
        }
    }
    void checkUpdateChildScale(ZF_IN ZFUIView *child)
    {
        if(child->scaleGet() != this->thisView->scaleGet()
           || child->scaleGetForImpl() != this->thisView->scaleGetForImpl())
        {
            child->_ZFP_ZFUIView_scaleSetRecursively(this->thisView->scaleGet(), this->thisView->scaleGetForImpl());
        }
    }
    void childAdd(ZF_IN ZFUIViewChildLayerEnum childLayer,
                  ZF_IN _ZFP_ZFUIViewLayerData &layer,
                  ZF_IN ZFUIView *view,
                  ZF_IN ZFUIViewLayoutParam *layoutParam,
                  ZF_IN zfindex atIndex)
    {
        zfCoreAssertWithMessageTrim(!ZFBitTest(this->thisView->objectInstanceState(), ZFObject::ObjectInstanceStateOnDealloc),
            zfTextA("[ZFUIView] add child during parent's dealloc is not allowed"));
        zfCoreAssertWithMessageTrim(view != zfnull, zfTextA("[ZFUIView] add null child"));
        zfCoreAssertWithMessageTrim(view->viewParent() == zfnull, zfTextA("[ZFUIView] add child which already has parent, you should remove it first"));
        zfRetain(view);

        zfbool layoutParamNeedRelease = zffalse;
        if(layoutParam == zfnull)
        {
            layoutParam = view->layoutParam();
        }
        if(layoutParam == zfnull)
        {
            layoutParam = zfRetain(this->thisView->layoutParamCreate().to<ZFUIViewLayoutParam *>());
            layoutParamNeedRelease = zftrue;
        }
        else
        {
            if(!layoutParam->classData()->classIsSubclassOf(this->layoutParamClass))
            {
                layoutParamNeedRelease = zftrue;
                ZFUIViewLayoutParam *tmp = zfRetain(this->thisView->layoutParamCreate().to<ZFUIViewLayoutParam *>());
                tmp->styleableCopyFrom(layoutParam);
            }
        }
        view->_ZFP_ZFUIView_parentChanged(this->thisView, layoutParam, childLayer);
        if(atIndex == zfindexMax)
        {
            atIndex = layer.views.count();
        }
        layer.views.add(atIndex, view);
        this->impl->childAdd(this->thisView, view, this->viewLayerPrevCount(layer) + atIndex, childLayer, atIndex);

        this->thisView->layoutRequest();
        view->layoutRequest();

        if(layoutParamNeedRelease)
        {
            zfRelease(layoutParam);
        }

        this->checkUpdateChildScale(view);
        ZFUIView *virtualParent = ((this->viewDelegateParent != zfnull) ? this->viewDelegateParent : this->thisView);
        virtualParent->viewChildOnAdd(view, childLayer);
        view->viewOnAddToParent(virtualParent);
        virtualParent->viewChildOnChange();
    }
    void childRemove(ZF_IN ZFUIViewChildLayerEnum childLayer,
                     ZF_IN _ZFP_ZFUIViewLayerData &layer,
                     ZF_IN ZFUIView *view)
    {
        zfindex index = this->indexOfView(layer, view);
        if(index != zfindexMax)
        {
            this->childRemoveAtIndex(childLayer, layer, index);
        }
    }
    void childRemoveAtIndex(ZF_IN ZFUIViewChildLayerEnum childLayer,
                            ZF_IN _ZFP_ZFUIViewLayerData &layer,
                            ZF_IN zfindex index)
    {
        ZFUIView *view = ZFCastZFObjectUnchecked(ZFUIView *, layer.views.get(index));

        view->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        layer.views.remove(index);
        this->impl->childRemove(this->thisView, this->viewLayerPrevCount(layer) + index, childLayer, index);

        this->thisView->layoutRequest();

        ZFUIView *virtualParent = ((this->viewDelegateParent != zfnull) ? this->viewDelegateParent : this->thisView);
        virtualParent->viewChildOnRemove(view, childLayer);
        view->viewOnRemoveFromParent(virtualParent);
        virtualParent->viewChildOnChange();
        zfRelease(view);
    }
    void removeAllView(ZF_IN ZFUIViewChildLayerEnum childLayer,
                       ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        if(layer.views.isEmpty())
        {
            return ;
        }

        this->thisView->layoutRequest();

        ZFCoreArrayPOD<ZFUIView *> tmp;
        tmp.copyFrom(layer.views);
        layer.views.removeAll();

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        for(zfindex i = tmp.count() - 1; i != zfindexMax; --i)
        {
            tmp[i]->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            this->impl->childRemove(this->thisView, prevLayerCount + i, childLayer, i);
        }

        ZFUIView *virtualParent = ((this->viewDelegateParent != zfnull) ? this->viewDelegateParent : this->thisView);
        for(zfindex i = tmp.count() - 1; i != zfindexMax; --i)
        {
            ZFUIView *child = tmp[i];
            virtualParent->viewChildOnRemove(child, childLayer);
            child->viewOnRemoveFromParent(virtualParent);
            zfRelease(child);
        }
        virtualParent->viewChildOnChange();
    }
    void moveView(ZF_IN ZFUIViewChildLayerEnum childLayer,
                  ZF_IN _ZFP_ZFUIViewLayerData &layer,
                  ZF_IN zfindex fromIndex,
                  ZF_IN zfindex toIndexOrIndexMax)
    {
        if(toIndexOrIndexMax == zfindexMax)
        {
            toIndexOrIndexMax = this->viewCount(layer) - 1;
        }
        if(fromIndex >= this->viewCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->viewCount(layer));
            return ;
        }
        if(toIndexOrIndexMax >= this->viewCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->viewCount(layer));
            return ;
        }
        if(fromIndex == toIndexOrIndexMax)
        {
            return ;
        }
        layer.views.move(fromIndex, toIndexOrIndexMax);

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        this->impl->childRemove(this->thisView,
            prevLayerCount + fromIndex,
            childLayer, fromIndex);
        this->impl->childAdd(this->thisView, ZFCastZFObjectUnchecked(ZFUIView *, layer.views.get(toIndexOrIndexMax)),
            prevLayerCount + toIndexOrIndexMax,
            childLayer, toIndexOrIndexMax);

        ZFUIView *virtualParent = ((this->viewDelegateParent != zfnull) ? this->viewDelegateParent : this->thisView);
        virtualParent->viewChildOnChange();
    }
    void moveView(ZF_IN ZFUIViewChildLayerEnum childLayer,
                  ZF_IN _ZFP_ZFUIViewLayerData &layer,
                  ZF_IN ZFUIView *child,
                  ZF_IN zfindex toIndexOrIndexMax)
    {
        zfindex fromIndex = this->indexOfView(layer, child);
        if(fromIndex != zfindexMax)
        {
            this->moveView(childLayer, layer, fromIndex, toIndexOrIndexMax);
        }
    }
    void replaceView(ZF_IN ZFUIViewChildLayerEnum childLayer,
                     ZF_IN _ZFP_ZFUIViewLayerData &layer,
                     ZF_IN zfindex atIndex,
                     ZF_IN ZFUIView *toReplace)
    {
        zfCoreAssert(atIndex < layer.views.count());
        zfCoreAssert(toReplace != zfnull);
        zfCoreAssert(toReplace->viewParent() == zfnull);

        ZFUIView *old = layer.views[atIndex];
        layer.views[atIndex] = toReplace;
        zfindex fixedIndex = this->viewLayerPrevCount(layer) + atIndex;
        this->impl->childRemove(this->thisView, fixedIndex, childLayer, atIndex);
        this->impl->childAdd(this->thisView, toReplace, fixedIndex, childLayer, atIndex);

        this->checkUpdateChildScale(toReplace);
        toReplace->_ZFP_ZFUIView_parentChanged(this->thisView, old->layoutParam(), childLayer);
        old->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);

        ZFUIView *virtualParent = ((this->viewDelegateParent != zfnull) ? this->viewDelegateParent : this->thisView);
        virtualParent->viewChildOnRemove(old, childLayer);
        old->viewOnRemoveFromParent(virtualParent);
        virtualParent->viewChildOnAdd(toReplace, childLayer);
        toReplace->viewOnAddToParent(virtualParent);
        virtualParent->viewChildOnChange();

        zfRelease(old);
    }

    zfindex viewCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        return layer.views.count();
    }
    ZFUIView *viewAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                            ZF_IN zfindex index)
    {
        return ZFCastZFObjectUnchecked(ZFUIView *, layer.views.get(index));
    }
    zfindex indexOfView(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                        ZF_IN ZFUIView *view)
    {
        return layer.views.find(view);
    }
    zfbool viewArrayTheSame(ZF_IN ZFUIView *view0,
                            ZF_IN ZFUIView *view1,
                            ZF_IN ZFUIViewChildLayerEnum layer)
    {
        ZFCoreArrayPOD<ZFUIView *> children0;
        ZFCoreArrayPOD<ZFUIView *> children1;
        switch(layer)
        {
            case ZFUIViewChildLayer::e_Normal:
                children0 = view0->childArray();
                children1 = view1->childArray();
                break;
            case ZFUIViewChildLayer::e_Background:
                children0 = view0->internalBackgroundViewArray();
                children1 = view1->internalBackgroundViewArray();
                break;
            case ZFUIViewChildLayer::e_Foreground:
                children0 = view0->internalForegroundViewArray();
                children1 = view1->internalForegroundViewArray();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        if(children0.count() != children1.count())
        {
            return zffalse;
        }
        for(zfindex i = children0.count() - 1; i != zfindexMax; --i)
        {
            if(ZFObjectCompare(children0[i], children1[i]) != ZFCompareTheSame)
            {
                return zffalse;
            }
        }
        return zftrue;
    }
    ZFUIViewLayoutParam *viewLayoutParamAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                                                ZF_IN zfindex index)
    {
        return ZFCastZFObjectUnchecked(ZFUIView *, layer.views.get(index))->layoutParam();
    }

    zfbool serializeInternalViewFromCategoryData(ZF_IN zfbool isInternalBackgroundView,
                                                 ZF_IN const ZFSerializableData &categoryData,
                                                 ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                 ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
    {
        zfautoObject internalView;
        if(!ZFObjectFromSerializableData(internalView, categoryData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
        if(internalView == zfautoObjectNull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                zfText("null view"));
            return zffalse;
        }
        if(!internalView.toObject()->classData()->classIsSubclassOf(ZFUIView::ClassData()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                zfText("%s not type of %s"),
                internalView.toObject()->objectInfoOfInstance().cString(), ZFUIView::ClassData()->className());
            return zffalse;
        }
        ZFUIView *internalViewTmp = ZFCastZFObjectUnchecked(ZFUIView *, internalView.toObject());
        if(internalViewTmp->viewId().isEmpty())
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                zfText("auto serialized internal view %s has no viewId"),
                internalViewTmp->objectInfoOfInstance().cString());
            return zffalse;
        }
        ZFUIView *exist = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> &views = (isInternalBackgroundView ? this->layerInternalBg.views : this->layerInternalFg.views);
        for(zfindex i = views.count() - 1; i != zfindexMax; --i)
        {
            ZFUIView *tmp = views.get(i)->to<ZFUIView *>();
            if(tmp->viewId().compare(internalViewTmp->viewId()) == 0)
            {
                exist = tmp;
                break;
            }
        }
        if(exist == zfnull)
        {
            this->thisView->internalViewAutoSerializeTagAdd(internalViewTmp->viewId().cString());
            if(isInternalBackgroundView)
            {
                this->thisView->internalBackgroundViewAdd(internalViewTmp);
            }
            else
            {
                this->thisView->internalForegroundViewAdd(internalViewTmp);
            }
        }
        else
        {
            exist->styleableCopyFrom(internalViewTmp);
        }
        return zftrue;
    }
    zfbool serializeInternalViewToCategoryData(ZF_IN zfbool isInternalBackgroundView,
                                               ZF_IN_OUT ZFSerializableData &ownerData,
                                               ZF_IN ZFUIView *ref,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
    {
        const ZFCoreArrayPOD<ZFUIView *> &views = (isInternalBackgroundView ? this->layerInternalBg.views : this->layerInternalFg.views);
        const ZFCoreArrayPOD<ZFUIView *> *viewsRef = zfnull;
        const zfchar *categoryTag = (isInternalBackgroundView
            ? ZFSerializableKeyword_ZFUIView_internalBackgroundView
            : ZFSerializableKeyword_ZFUIView_internalForegroundView);
        if(ref != zfnull)
        {
            viewsRef = (isInternalBackgroundView ? &(ref->d->layerInternalBg.views) : &(ref->d->layerInternalFg.views));
        }
        if(ref == zfnull || viewsRef->count() == 0)
        {
            for(zfindex i = 0; i < views.count(); ++i)
            {
                ZFUIView *tmp = views.get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToSerializableData(childData, tmp, outErrorHintToAppend))
                {
                    return zffalse;
                }
                childData.categorySet(categoryTag);
                ownerData.elementAdd(childData);
            }
        }
        else
        {
            for(zfindex i = 0; i < views.count(); ++i)
            {
                ZFUIView *tmp = views.get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                zfbool exist = zffalse;
                for(zfindex iRef = ref->d->layerInternalBg.views.count() - 1; iRef != zfindexMax; --iRef)
                {
                    if(ref->d->layerInternalBg.views.get(iRef)->to<ZFUIView *>()->viewId().compare(tmp->viewId().cString()) == 0)
                    {
                        exist = zftrue;
                        break;
                    }
                }
                if(exist)
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToSerializableData(childData, tmp, outErrorHintToAppend))
                {
                    return zffalse;
                }
                childData.categorySet(categoryTag);
                ownerData.elementAdd(childData);
            }
        }
        return zftrue;
    }
};

// ============================================================
// ZFUIView
ZFOBJECT_REGISTER(ZFUIView)

// ============================================================
// serialize
zfbool ZFUIView::serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryFromData(ownerData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    if(this->serializableOnCheckNeedSerializeChildren())
    {
        this->childRemoveAll();
    }
    d->layoutParamSet(zfnull);

    for(zfindex i = 0; i < ownerData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = ownerData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_child))
        {
            zfautoObject element;
            if(!ZFObjectFromSerializableData(element, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("null view"));
                return zffalse;
            }
            if(!element.toObject()->classData()->classIsSubclassOf(ZFUIView::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("%s not type of %s"),
                    element.toObject()->objectInfoOfInstance().cString(), ZFUIView::ClassData()->className());
                return zffalse;
            }
            ZFUIView *child = ZFCastZFObjectUnchecked(ZFUIView *, element);
            this->childAdd(child, child->layoutParam());

            categoryData.resolveMark();
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_layoutParam))
        {
            zfautoObject layoutParam;
            if(!ZFObjectFromSerializableData(layoutParam, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(layoutParam == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("null layoutParam"));
                return zffalse;
            }
            if(!layoutParam.toObject()->classData()->classIsSubclassOf(ZFUIViewLayoutParam::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("%s not type of %s"),
                    layoutParam.toObject()->objectInfoOfInstance().cString(), ZFUIViewLayoutParam::ClassData()->className());
                return zffalse;
            }

            d->layoutParamSet(ZFCastZFObjectUnchecked(ZFUIViewLayoutParam *, layoutParam));
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalBackgroundView))
        {
            if(!d->serializeInternalViewFromCategoryData(zftrue,
                categoryData,
                outErrorHintToAppend,
                outErrorPos))
            {
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalForegroundView))
        {
            if(!d->serializeInternalViewFromCategoryData(zffalse,
                categoryData,
                outErrorHintToAppend,
                outErrorPos))
            {
                return zffalse;
            }
        }
    }
    return zftrue;
}
zfbool ZFUIView::serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                       ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                       ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryToData(ownerData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    ZFUIViewLayoutParam *refLayoutParam = ((ref == zfnull) ? zfnull : ref->d->layoutParam);
    if(d->layoutParam != zfnull
        && this->viewParent() != zfnull
        && (refLayoutParam == zfnull || ZFObjectCompare(d->layoutParam, refLayoutParam) != ZFCompareTheSame))
    { // layoutParam
        ZFSerializableData categoryData;
        if(!ZFObjectToSerializableData(categoryData, d->layoutParam, outErrorHintToAppend, refLayoutParam))
        {
            return zffalse;
        }
        categoryData.categorySet(ZFSerializableKeyword_ZFUIView_layoutParam);
        ownerData.elementAdd(categoryData);
    }

    { // internal views
        if(!d->internalViewAutoSerializeTags.empty())
        {
            if(!d->serializeInternalViewToCategoryData(zftrue, ownerData, ref, outErrorHintToAppend))
            {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(zffalse, ownerData, ref, outErrorHintToAppend))
            {
                return zffalse;
            }
        }
    }

    // all children
    if(this->serializableOnCheckNeedSerializeChildren())
    {
        if(ref == zfnull || ref->childCount() == 0)
        {
            for(zfindex i = 0, count = this->childCount(); i < count; ++i)
            {
                ZFSerializableData childData;
                if(!ZFObjectToSerializableData(childData, this->childAtIndex(i), outErrorHintToAppend))
                {
                    return zffalse;
                }
                childData.categorySet(ZFSerializableKeyword_ZFUIView_child);
                ownerData.elementAdd(childData);
            }
        }
        else
        {
            if(!d->viewArrayTheSame(this, ref, ZFUIViewChildLayer::e_Normal))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                    zfText("child mismatch, this: %s, ref: %s"),
                    this->objectInfoOfInstance().cString(), ref->objectInfoOfInstance().cString());
                return zffalse;
            }
        }
    }
    return zftrue;
}
zfbool ZFUIView::serializableOnCheckNeedSerializeChildren(void)
{
    return zftrue;
}

// ============================================================
// styles
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, zfstring, viewDelegateClass)
{
    zfsuperI(ZFUIViewStyle)::viewDelegateClassSet(newValue);

    zfautoObject viewDelegateTmp = ZFClass::newInstanceForName(this->viewDelegateClass(),
        ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(!this->viewDelegateSupported() && viewDelegateTmp != zfautoObjectNull)
    {
        zfCoreCriticalMessage(zfText("viewDelegate not supported"));
        return ;
    }
    this->viewDelegateSet(ZFCastZFObject(ZFUIView *, viewDelegateTmp));
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, zfbool, viewVisible)
{
    zfbool changed = (this->viewVisible() != newValue);
    zfsuperI(ZFUIViewStyle)::viewVisibleSet(newValue);
    d->impl->viewVisibleSet(this, this->viewVisible());
    if(changed)
    {
        if(this->viewParent() != zfnull)
        {
            this->viewParent()->viewChildVisibleOnChange();
            if(this->viewParent()->d->viewDelegateParent != zfnull)
            {
                this->viewParent()->d->viewDelegateParent->viewChildVisibleOnChange();
            }
        }
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, zffloat, viewAlpha)
{
    zffloat tmp = zfmApplyRange<zffloat>(newValue, 0, 1);
    zfsuperI(ZFUIViewStyle)::viewAlphaSet(tmp);
    d->impl->viewAlphaSet(this, this->viewAlpha());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, zfbool, viewFocusable)
{
    zfsuperI(ZFUIViewStyle)::viewFocusableSet(newValue);
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        impl->viewFocusableSet(this, this->viewFocusable());
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, zfbool, viewUIEnable)
{
    zfsuperI(ZFUIViewStyle)::viewUIEnableSet(newValue);
    d->impl->viewUIEnableSet(this, this->viewUIEnable());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, zfbool, viewUIEnableTree)
{
    zfsuperI(ZFUIViewStyle)::viewUIEnableTreeSet(newValue);
    d->impl->viewUIEnableTreeSet(this, this->viewUIEnableTree());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, zfbool, viewMouseHoverEventEnable)
{
    zfsuperI(ZFUIViewStyle)::viewMouseHoverEventEnableSet(newValue);
    d->impl->viewMouseHoverEventEnableSet(this, this->viewMouseHoverEventEnable());
}

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, ZFUISize, viewSizeMin)
{
    zfbool changed = (this->viewSizeMin() != newValue);
    zfsuperI(ZFUIViewStyle)::viewSizeMinSet(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, ZFUISize, viewSizeMax)
{
    zfbool changed = (this->viewSizeMax() != newValue);
    zfsuperI(ZFUIViewStyle)::viewSizeMaxSet(newValue);
    if(changed)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIView, ZFUIColor, viewBackgroundColor)
{
    zfsuperI(ZFUIViewStyle)::viewBackgroundColorSet(newValue);
    d->impl->viewBackgroundColorSet(this, this->viewBackgroundColor());
}

// ============================================================
// init and dealloc
ZFObject *ZFUIView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIViewPrivate);
    d->thisView = this;

    d->nativeView = d->impl->nativeViewCreate(this);
    d->scaleForImpl = d->impl->nativeViewScaleForImpl(d->nativeView);
    d->scaleFixed = d->scaleForImpl;

    d->layoutParamClass = this->layoutParamClass();

    return this;
}
void ZFUIView::objectOnDealloc(void)
{
    if(d->viewDelegate != zfnull)
    {
        ZFUIView *viewDelegate = d->viewDelegate;
        d->viewDelegate = zfnull;
        viewDelegate->d->viewDelegateParent = zfnull;
        zfRelease(viewDelegate);
    }

    this->nativeImplViewSet(zfnull, zfnull);
    d->impl->nativeViewDestroy(this, d->nativeView);
    d->nativeView = zfnull;

    for(zfindex i = d->layerNormal.views.count() - 1; i != zfindexMax; --i)
    {
        zfRelease(d->layerNormal.views.get(i));
    }
    for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax; --i)
    {
        zfRelease(d->layerInternalBg.views.get(i));
    }
    for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax; --i)
    {
        zfRelease(d->layerInternalFg.views.get(i));
    }
    d->layoutParamSet(zfnull);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();

    this->observerAdd(
        ZFObject::EventObjectAfterAlloc(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder)->viewPropertyOnUpdateListener,
        this->objectHolder(),
        zfnull,
        zftrue);
}
void ZFUIView::objectOnDeallocPrepare(void)
{
    if(this->viewFocused())
    {
        this->viewFocusRequest(zffalse);
    }

    ZFThreadTaskCancelWithOwner(this);

    d->removeAllView(ZFUIViewChildLayer::e_Foreground, d->layerInternalFg);
    this->childRemoveAll();
    d->removeAllView(ZFUIViewChildLayer::e_Background, d->layerInternalBg);

    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFUIView::objectHash(void)
{
    zfidentity hash = zfidentityHash(zfidentityCalcString(this->classData()->className()) , this->childCount());
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        hash = zfidentityHash(hash, this->childAtIndex(i)->objectHash());
    }
    return hash;
}
ZFCompareResult ZFUIView::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->childCount() != another->childCount()
        || ZFObjectCompare(d->layoutParam, another->d->layoutParam) != ZFCompareTheSame
        || !ZFClassUtil::allPropertyIsEqual(this, another)
        || !d->viewArrayTheSame(this, another, ZFUIViewChildLayer::e_Background)
        || !d->viewArrayTheSame(this, another, ZFUIViewChildLayer::e_Foreground)
        || !d->viewArrayTheSame(this, another, ZFUIViewChildLayer::e_Normal))
    {
        return ZFCompareUncomparable;
    }
    return ZFCompareTheSame;
}

static void _ZFP_ZFUIView_objectInfo_viewDelegate(ZF_IN_OUT zfstring &ret, ZF_IN ZFUIView *view)
{
    if(view->viewDelegate() == zfnull)
    {
        return ;
    }
    zfindex count = 1;
    {
        ZFUIView *parent = view;
        while(parent->viewDelegateParent() != zfnull)
        {
            ++count;
            parent = parent->viewParent();
        }
    }
    ret += zfText("\n");
    for(zfindex i = 0; i < count; ++i)
    {
        ret += zfText("    ");
    }
    ret += zfText("(delegate) ");
    view->viewDelegate()->objectInfoT(ret);
}
void ZFUIView::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoT(ret);
    _ZFP_ZFUIView_objectInfo_viewDelegate(ret, this);
}
void ZFUIView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(!this->viewId().isEmpty())
    {
        zfstringAppend(ret, zfText(" [%s]"), this->viewId().cString());
    }

    ret += zfText(" ");
    ZFUIRectToString(ret, this->layoutedFrame());

    if(!this->viewVisible())
    {
        ret += zfText(" Hidden");
    }

    if(!this->viewUIEnableTree())
    {
        ret += zfText(" UITreeDisabled");
    }
    else if(!this->viewUIEnable())
    {
        ret += zfText(" UIDisabled");
    }
}

void ZFUIView::objectCachedOnChange(void)
{
    zfsuper::objectCachedOnChange();
    zfbool objectCached = this->objectCached();
    if(d->layoutParam)
    {
        d->layoutParam->objectCachedSet(objectCached);
    }
    if(d->viewDelegate)
    {
        d->viewDelegate->objectCachedSet(objectCached);
    }
    for(zfindex i = 0; i < d->layerInternalBg.views.count(); ++i)
    {
        d->layerInternalBg.views[i]->objectCachedSet(objectCached);
    }
    for(zfindex i = 0; i < d->layerNormal.views.count(); ++i)
    {
        d->layerNormal.views[i]->objectCachedSet(objectCached);
    }
    for(zfindex i = 0; i < d->layerInternalFg.views.count(); ++i)
    {
        d->layerInternalFg.views[i]->objectCachedSet(objectCached);
    }
}

void ZFUIView::nativeImplViewSet(ZF_IN void *nativeImplView,
                                 ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback)
{
    void *nativeImplViewOld = d->nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallbackOld = d->nativeImplViewDeleteCallback;
    d->nativeImplView = nativeImplView;
    d->nativeImplViewDeleteCallback = nativeImplViewDeleteCallback;
    d->impl->nativeImplViewSet(this, nativeImplViewOld, d->nativeImplView);
    if(nativeImplViewOld && nativeImplViewDeleteCallbackOld)
    {
        nativeImplViewDeleteCallbackOld(this, nativeImplViewOld);
    }
}
void *ZFUIView::nativeImplView(void)
{
    return d->nativeImplView;
}
void ZFUIView::_ZFP_ZFUIView_notifyLayoutNativeImplView(ZF_OUT ZFUIRect &result)
{
    ZFUIRect bounds = ZFUIRectGetBounds(this->layoutedFrame());
    result = bounds;
    this->nativeImplViewOnLayout(result, bounds);
}

// ============================================================
// parent
void ZFUIView::_ZFP_ZFUIView_parentChanged(ZF_IN ZFUIView *viewParent,
                                           ZF_IN ZFUIViewLayoutParam *layoutParam,
                                           ZF_IN ZFUIViewChildLayerEnum viewLayer)
{
    if(viewParent != zfnull && viewParent->d->viewDelegateParent != zfnull)
    {
        d->viewParent = viewParent->d->viewDelegateParent;
    }
    else
    {
        d->viewParent = viewParent;
    }
    if(layoutParam != zfnull)
    {
        // only change layoutParam if not null
        // keep old one for performance
        d->layoutParamSet(layoutParam);
    }
    d->viewLayer = viewLayer;
}
void *ZFUIView::nativeView(void)
{
    return d->nativeView;
}
void ZFUIView::nativeViewNotifyBeforeAdd(ZF_IN ZFUIView *view,
                                         ZF_IN void *nativeParentView)
{
    zfCoreAssert(view != zfnull && nativeParentView != zfnull);

    zfRetain(view);
    view->_ZFP_ZFUIView_scaleSetRecursively(view->scaleGet(), view->d->impl->nativeViewScaleForImpl(nativeParentView));
}
void ZFUIView::nativeViewNotifyAfterRemove(ZF_IN ZFUIView *view)
{
    zfCoreAssert(view != zfnull);
    zfRelease(view);
}

// ============================================================
// view delegate logic
void ZFUIView::viewDelegateSet(ZF_IN ZFUIView *viewDelegate)
{
    if(viewDelegate == d->viewDelegate)
    {
        return ;
    }
    zfCoreAssertWithMessage(viewDelegate != this, zfTextA("you must not set viewDelegate to self"));
    zfCoreAssertWithMessage(viewDelegate == zfnull || viewDelegate->viewParent() == zfnull,
        zfTextA("setting a viewDelegate which already has parent"));

    ZFUIView *viewDelegateOld = d->viewDelegate;
    d->viewDelegate = zfnull;
    zfblockedAllocWithoutLeakTest(ZFArrayEditable, children);
    zfblockedAllocWithoutLeakTest(ZFArrayEditable, childLayoutParams);
    if(viewDelegateOld != zfnull)
    {
        for(zfindex i = 0; i < viewDelegateOld->childCount(); ++i)
        {
            children->add(viewDelegateOld->childAtIndex(i));
            childLayoutParams->add(viewDelegateOld->childAtIndex(i)->layoutParam());
        }
        viewDelegateOld->childRemoveAll();
        viewDelegateOld->d->viewDelegateParent = zfnull;
        viewDelegateOld->viewRemoveFromParent();
    }

    if(viewDelegate != zfnull)
    {
        for(zfindex i = 0; i < this->childCount(); ++i)
        {
            children->add(this->childAtIndex(i));
            childLayoutParams->add(this->childAtIndex(i)->layoutParam());
        }
        this->childRemoveAll();

        d->viewDelegate = zfRetain(viewDelegate);
    }

    ZFUIView *viewToAdd = this;
    if(d->viewDelegate != zfnull)
    {
        this->internalForegroundViewAdd(d->viewDelegate,
            zfHint("layoutParam")zfnull,
            zfHint("addAsTopMost")zffalse);
        viewToAdd = d->viewDelegate;
        d->viewDelegate->d->viewDelegateParent = this;
    }
    for(zfindex i = 0; i < children->count(); ++i)
    {
        viewToAdd->childAdd(
            children->get<ZFUIView *>(i),
            childLayoutParams->get<ZFUIViewLayoutParam *>(i));
    }
    if(this->objectCached())
    {
        d->viewDelegate->objectCachedSet(zftrue);
        viewDelegateOld->objectCachedSet(zffalse);
    }
    this->viewDelegateOnDealloc(viewDelegateOld);
    this->viewDelegateOnInit(d->viewDelegate);
    zfRelease(viewDelegateOld);
}

ZFUIView *ZFUIView::viewDelegateParent(void)
{
    return d->viewDelegateParent;
}
ZFUIView *ZFUIView::viewDelegate(void)
{
    return d->viewDelegate;
}
void ZFUIView::viewDelegateLayoutOnMeasure(ZF_OUT ZFUISize &ret,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN const ZFUISizeParam &sizeParam)
{
    const ZFUIMargin &viewDelegateMargin = d->viewDelegate->layoutParam()->layoutMargin();
    ret = d->viewDelegate->layoutMeasure(
        ZFUIViewLayoutParam::sizeHintOffset(sizeHint, ZFUISizeMake(
            ZFUIMarginGetX(viewDelegateMargin),
            ZFUIMarginGetY(viewDelegateMargin))),
        sizeParam);
    ret = ZFUISizeApplyMarginReversely(ret, viewDelegateMargin);
}
void ZFUIView::viewDelegateLayoutOnMeasureFinish(ZF_IN_OUT ZFUISize &measuredSize,
                                                 ZF_IN const ZFUISize &sizeHint,
                                                 ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize tmp = ZFUISizeApplyMargin(measuredSize, d->viewDelegate->layoutParam()->layoutMargin());
    d->viewDelegate->layoutOnMeasureFinish(tmp, sizeHint, sizeParam);
    measuredSize = ZFUISizeApplyMarginReversely(tmp, d->viewDelegate->layoutParam()->layoutMargin());
}
void ZFUIView::viewDelegateLayoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    // nothing to do
}
void ZFUIView::viewDelegateLayoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    d->viewDelegate->layout(
        ZFUIRectApplyMargin(bounds, d->viewDelegate->layoutParam()->layoutMargin()));
}
void ZFUIView::viewDelegateLayoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds)
{
    // nothing to do
}

// ============================================================
// view focus
zfbool ZFUIView::viewFocused(void)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        return impl->viewFocused(this);
    }
    return zffalse;
}
void ZFUIView::viewFocusRequest(ZF_IN zfbool viewFocus)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        if(!viewFocus || this->viewFocusable())
        {
            impl->viewFocusRequest(this, viewFocus);
        }
    }
}
ZFUIView *ZFUIView::viewFocusedChild(void)
{
    if(!ZFPROTOCOL_IS_AVAILABLE(ZFUIViewFocus))
    {
        return zfnull;
    }
    if(this->viewFocused())
    {
        return this;
    }
    ZFUIView *ret = zfnull;
    for(zfindex i = this->childCount() - 1; i != zfindexMax; --i)
    {
        ret = this->childAtIndex(i)->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax; --i)
    {
        ret = d->layerInternalFg.views[i]->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax; --i)
    {
        ret = d->layerInternalBg.views[i]->viewFocusedChild();
        if(ret != zfnull)
        {
            return ret;
        }
    }
    return zfnull;
}

// ============================================================
// parent
ZFUIView *ZFUIView::viewParent(void)
{
    return d->viewParent;
}

void ZFUIView::viewRemoveFromParent(void)
{
    if(this->viewParent() != zfnull)
    {
        switch(this->viewLayer())
        {
            case ZFUIViewChildLayer::e_Normal:
                this->viewParent()->childRemove(this);
                break;
            case ZFUIViewChildLayer::e_Background:
                this->viewParent()->internalBackgroundViewRemove(this);
                break;
            case ZFUIViewChildLayer::e_Foreground:
                this->viewParent()->internalForegroundViewRemove(this);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
}

// ============================================================
// scale settings
zffloat ZFUIView::scaleGet(void)
{
    return d->scaleForApp;
}
zffloat ZFUIView::scaleGetForImpl(void)
{
    return d->scaleForImpl;
}
zffloat ZFUIView::scaleGetFixed(void)
{
    return d->scaleFixed;
}
zffloat ZFUIView::scaleGetForImplPhysicalPixel(void)
{
    return d->impl->nativeViewScaleForPhysicalPixel(d->nativeView);
}
void ZFUIView::scaleOnChange(void)
{
    this->observerNotify(ZFUIView::EventViewScaleOnChange());
}
void ZFUIView::_ZFP_ZFUIView_scaleSet(ZF_IN zffloat scaleForApp,
                                      ZF_IN zffloat scaleForImpl)
{
    d->scaleForApp = scaleForApp;
    d->scaleForImpl = scaleForImpl;
    zffloat scaleFixed = scaleForApp * scaleForImpl;
    if(zffloatNotEqual(d->scaleFixed, scaleFixed))
    {
        d->scaleFixed = scaleFixed;
        this->scaleOnChange();
    }
}
void ZFUIView::_ZFP_ZFUIView_scaleSetRecursively(ZF_IN zffloat scaleForApp,
                                                 ZF_IN zffloat scaleForImpl)
{
    if(zffloatNotEqual(d->scaleForApp, scaleForApp)
        || zffloatNotEqual(d->scaleForImpl, scaleForImpl))
    {
        this->_ZFP_ZFUIView_scaleSet(scaleForApp, scaleForImpl);
        for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax; --i)
        {
            d->layerInternalBg.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleForApp, scaleForImpl);
        }
        for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax; --i)
        {
            d->layerInternalFg.views.get(i)->to<ZFUIView *>()->_ZFP_ZFUIView_scaleSetRecursively(scaleForApp, scaleForImpl);
        }
        for(zfindex i = this->childCount() - 1; i != zfindexMax; --i)
        {
            this->childAtIndex(i)->_ZFP_ZFUIView_scaleSetRecursively(scaleForApp, scaleForImpl);
        }
    }
}

// ============================================================
// layout logic
zfautoObject ZFUIView::layoutParamCreate(void)
{
    zfautoObject layoutParam = d->layoutParamClass->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
    if(layoutParam == zfautoObjectNull || !layoutParam.toObject()->classData()->classIsSubclassOf(ZFUIViewLayoutParam::ClassData()))
    {
        return zfautoObjectNull;
    }
    this->layoutParamOnUpdate(layoutParam.to<ZFUIViewLayoutParam *>());
    return layoutParam;
}
const ZFClass *ZFUIView::layoutParamClass(void)
{
    return ZFUIViewLayoutParam::ClassData();
}

ZFUIViewLayoutParam *ZFUIView::layoutParam(void)
{
    return d->layoutParam;
}
void ZFUIView::layoutParamSet(ZF_IN ZFUIViewLayoutParam *layoutParam)
{
    d->layoutParamSet(layoutParam);
}

void ZFUIView::_ZFP_ZFUIView_notifyLayoutRootView(ZF_IN const ZFUIRect &bounds)
{
    ++_ZFP_ZFUIView_layoutRequestOverrideFlag;
    this->layoutMeasure(bounds.size, ZFUISizeParamFillWidthFillHeight);
    this->layout(bounds);
    --_ZFP_ZFUIView_layoutRequestOverrideFlag;
}

void ZFUIView::layoutRequest(void)
{
    if(!d->layoutRequested && _ZFP_ZFUIView_layoutRequestOverrideFlag == 0)
    {
        ZFUIView *view = this;
        while(view != zfnull)
        {
            view->d->layoutRequested = zftrue;
            view = view->viewParent();
        }
        d->impl->layoutRequest(this);
        this->layoutOnRequest();
        if(d->viewDelegate != zfnull)
        {
            d->viewDelegate->layoutRequest();
        }
    }
}
zfbool ZFUIView::layoutRequested(void)
{
    return d->layoutRequested;
}
zfbool ZFUIView::layouting(void)
{
    return d->layouting;
}
const ZFUISize &ZFUIView::layoutMeasure(ZF_IN const ZFUISize &sizeHint,
                                        ZF_IN const ZFUISizeParam &sizeParam)
{
    if(d->layoutRequested
        || !ZFUISizeIsEqual(d->lastMeasuredSizeHint, sizeHint)
        || !ZFUISizeParamIsEqual(d->lastMeasuredSizeParam, sizeParam))
    {
        d->lastMeasuredSizeHint = sizeHint;
        d->lastMeasuredSizeParam = sizeParam;
        if(sizeParam.width == ZFUISizeType::e_Fill && sizeParam.height == ZFUISizeType::e_Fill)
        {
            d->lastMeasuredSize = sizeHint;
            d->lastMeasuredSize.width = zfmMax(0, d->lastMeasuredSize.width);
            d->lastMeasuredSize.height = zfmMax(0, d->lastMeasuredSize.height);
        }
        else if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnMeasure(d->lastMeasuredSize, sizeHint, sizeParam);
        }
        else
        {
            this->layoutOnMeasure(d->lastMeasuredSize, sizeHint, sizeParam);
        }

        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnMeasureFinish(d->lastMeasuredSize, sizeHint, sizeParam);
        }
        this->layoutOnMeasureFinish(d->lastMeasuredSize, sizeHint, sizeParam);
        if(ZFOBSERVER_HAS_ADD(HasAdd_ViewLayoutOnMeasureFinish))
        {
            zfblockedAllocWithoutLeakTest(ZFUIViewMeasureResult, data);
            data->sizeHint = sizeHint;
            data->sizeParam = sizeParam;
            data->measuredSize = d->lastMeasuredSize;
            this->observerNotify(ZFUIView::EventViewLayoutOnMeasureFinish(), data);
            d->lastMeasuredSize = data->measuredSize;
        }

        d->lastMeasuredSize = ZFUISizeApplyRange(d->lastMeasuredSize, this->viewSizeMin(), this->viewSizeMax());
    }
    return d->lastMeasuredSize;
}
const ZFUISize &ZFUIView::layoutMeasuredSize(void)
{
    return d->lastMeasuredSize;
}
void ZFUIView::layout(ZF_IN const ZFUIRect &rect)
{
    if(d->layoutRequested
        || !ZFUISizeIsEqual(d->layoutedFrame.size, rect.size))
    {
        d->layoutedFramePrev = d->layoutedFrame;
        d->layoutedFrame = rect;
        d->impl->viewFrameSet(this, ZFUIRectApplyScale(rect, this->scaleGetFixed()));
        ZFUIRect bounds = ZFUIRectGetBounds(rect);

        d->layouting = zftrue;

        // layout prepare
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayoutPrepare(bounds);
        }
        this->layoutOnLayoutPrepare(bounds);
        if(ZFOBSERVER_HAS_ADD(HasAdd_ViewLayoutOnLayoutPrepare))
        {
            this->observerNotify(ZFUIView::EventViewLayoutOnLayoutPrepare());
        }

        // layout
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayout(bounds);
        }
        else
        {
            this->layoutOnLayout(bounds);
        }
        if(ZFOBSERVER_HAS_ADD(HasAdd_ViewLayoutOnLayout))
        {
            this->observerNotify(ZFUIView::EventViewLayoutOnLayout());
        }

        // internal views
        this->internalBackgroundViewOnLayout(bounds);
        this->internalForegroundViewOnLayout(bounds);

        // layout finish
        if(d->viewDelegate != zfnull)
        {
            this->viewDelegateLayoutOnLayoutFinish(bounds);
        }
        this->layoutOnLayoutFinish(bounds);
        if(ZFOBSERVER_HAS_ADD(HasAdd_ViewLayoutOnLayoutFinish))
        {
            this->observerNotify(ZFUIView::EventViewLayoutOnLayoutFinish());
        }

        d->layouting = zffalse;
    }
    else if(!ZFUIRectIsEqual(d->layoutedFrame, rect)) {
        // size not changed but point changed, notify impl to move the view is enough
        d->layoutedFramePrev = d->layoutedFrame;
        d->layoutedFrame = rect;
        d->impl->viewFrameSet(this, ZFUIRectApplyScale(rect, this->scaleGetFixed()));
    }
    d->layoutRequested = zffalse;
}
void ZFUIView::layoutIfNeed(void)
{
    this->layout(this->layoutedFrame());
}
const ZFUIRect &ZFUIView::layoutedFrame(void)
{
    return d->layoutedFrame;
}
const ZFUIRect &ZFUIView::layoutedFramePrev(void)
{
    return d->layoutedFramePrev;
}
void ZFUIView::layoutedFrameFixed(ZF_OUT ZFUIRect &ret)
{
    ret = d->layoutedFrame;
    if(d->viewDelegateParent != zfnull)
    {
        ZFUIView *viewDelegateParent = d->viewDelegateParent;
        do
        {
            ZFUIRect tmp = ret;
            viewDelegateParent->layoutedFrameFixedOnUpdateForChild(ret, tmp);
            viewDelegateParent = viewDelegateParent->d->viewDelegateParent;
        } while(viewDelegateParent != zfnull);
    }
    else if(d->viewParent != zfnull)
    {
        d->viewParent->layoutedFrameFixedOnUpdateForChild(ret, this->layoutedFrame());
    }
}

void ZFUIView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                               ZF_IN const ZFUISize &sizeHint,
                               ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = ZFUIViewLayoutParam::sizeHintApply(this->viewSizeMin(), sizeHint, sizeParam);
}
void ZFUIView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        child->layout(
            ZFUIViewLayoutParam::layoutParamApply(
                bounds,
                child,
                child->layoutParam()));
    }
}

// ============================================================
// child views
ZFUIView *ZFUIView::childFindById(ZF_IN const zfchar *viewId,
                                  ZF_IN_OPT zfbool findRecursively /* = zftrue */,
                                  ZF_IN_OPT zfbool includeInternalViews /* = zffalse */)
{
    if(viewId == zfnull || *viewId == '\0')
    {
        return zfnull;
    }

    ZFCoreArrayPOD<ZFUIView *> toFind;
    toFind.add(this);
    while(!toFind.isEmpty())
    {
        ZFUIView *view = toFind.get(0);
        toFind.remove(0);
        if(zfscmpTheSame(view->viewId().cString(), viewId))
        {
            return view;
        }
        if(!findRecursively)
        {
            continue;
        }
        toFind.addFrom(view->childArray());
        if(includeInternalViews)
        {
            toFind.addFrom(view->d->layerInternalBg.views);
            toFind.addFrom(view->d->layerInternalFg.views);
        }
    }
    return zfnull;
}

void ZFUIView::childAdd(ZF_IN ZFUIView *view,
                        ZF_IN_OPT ZFUIViewLayoutParam *layoutParam /* = zfnull */,
                        ZF_IN_OPT zfindex atIndex /* = zfindexMax */)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childAdd(view, layoutParam, atIndex);
    }
    else
    {
        d->childAdd(ZFUIViewChildLayer::e_Normal, d->layerNormal, view, layoutParam, atIndex);
    }
}
void ZFUIView::childRemove(ZF_IN ZFUIView *view)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemove(view);
    }
    else
    {
        d->childRemove(ZFUIViewChildLayer::e_Normal, d->layerNormal, view);
    }
}
void ZFUIView::childRemoveAtIndex(ZF_IN zfindex index)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemoveAtIndex(index);
    }
    else
    {
        d->childRemoveAtIndex(ZFUIViewChildLayer::e_Normal, d->layerNormal, index);
    }
}
void ZFUIView::childRemoveAll(void)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childRemoveAll();
    }
    else
    {
        d->removeAllView(ZFUIViewChildLayer::e_Normal, d->layerNormal);
    }
}

void ZFUIView::childMove(ZF_IN zfindex fromIndex, ZF_IN zfindex toIndexOrIndexMax)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childMove(fromIndex, toIndexOrIndexMax);
    }
    else
    {
        d->moveView(ZFUIViewChildLayer::e_Normal, d->layerNormal, fromIndex, toIndexOrIndexMax);
    }
}
void ZFUIView::childMove(ZF_IN ZFUIView *child, ZF_IN zfindex toIndexOrIndexMax)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childMove(child, toIndexOrIndexMax);
    }
    else
    {
        d->moveView(ZFUIViewChildLayer::e_Normal, d->layerNormal, child, toIndexOrIndexMax);
    }
}
void ZFUIView::childReplaceAtIndex(ZF_IN zfindex atIndex, ZF_IN ZFUIView *toReplace)
{
    if(d->viewDelegate)
    {
        d->viewDelegate->childReplaceAtIndex(atIndex, toReplace);
    }
    else
    {
        d->replaceView(ZFUIViewChildLayer::e_Normal, d->layerNormal, atIndex, toReplace);
    }
}

zfindex ZFUIView::childCount(void)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childCount();
    }
    else
    {
        return d->viewCount(d->layerNormal);
    }
}
ZFUIView *ZFUIView::childAtIndex(ZF_IN zfindex index)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childAtIndex(index);
    }
    else
    {
        return d->viewAtIndex(d->layerNormal, index);
    }
}
zfindex ZFUIView::childFind(ZF_IN ZFUIView *view)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childFind(view);
    }
    else
    {
        return d->indexOfView(d->layerNormal, view);
    }
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::childArray(void)
{
    if(d->viewDelegate)
    {
        return d->viewDelegate->childArray();
    }
    else
    {
        return d->layerNormal.views;
    }
}
ZFUIViewChildLayerEnum ZFUIView::viewLayer(void)
{
    return d->viewLayer;
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::childRawArray(void)
{
    ZFCoreArrayPOD<ZFUIView *> ret;
    ret.capacitySet(d->layerInternalBg.views.count() + this->childCount() + d->layerInternalFg.views.count());
    ret.addFrom(d->layerInternalBg.views);
    ret.addFrom(this->childCount());
    ret.addFrom(d->layerInternalFg.views);
    return ret;
}

// ============================================================
// internal background views
void ZFUIView::internalBackgroundViewAdd(ZF_IN ZFUIView *view,
                                         ZF_IN_OPT ZFUIViewLayoutParam *layoutParam /* = zfnull */,
                                         ZF_IN_OPT zfbool addAsTopMost /* = zftrue */)
{
    d->childAdd(ZFUIViewChildLayer::e_Background, d->layerInternalBg, view, layoutParam, (addAsTopMost ? zfindexMax : 0));
}
void ZFUIView::internalBackgroundViewRemove(ZF_IN ZFUIView *view)
{
    d->childRemove(ZFUIViewChildLayer::e_Background, d->layerInternalBg, view);
}
void ZFUIView::internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->viewCount(d->layerInternalBg); ++i)
    {
        ZFUIView *child = d->viewAtIndex(d->layerInternalBg, i);
        if(this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->viewLayoutParamAtIndex(d->layerInternalBg, i)));
        }
    }
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::internalBackgroundViewArray(void)
{
    return d->layerInternalBg.views;
}

// ============================================================
// internal foreground views
void ZFUIView::internalForegroundViewAdd(ZF_IN ZFUIView *view,
                                         ZF_IN_OPT ZFUIViewLayoutParam *layoutParam /* = zfnull */,
                                         ZF_IN_OPT zfbool addAsTopMost /* = zftrue */)
{
    d->childAdd(ZFUIViewChildLayer::e_Foreground, d->layerInternalFg, view, layoutParam, (addAsTopMost ? zfindexMax : 0));
}
void ZFUIView::internalForegroundViewRemove(ZF_IN ZFUIView *view)
{
    d->childRemove(ZFUIViewChildLayer::e_Foreground, d->layerInternalFg, view);
}
void ZFUIView::internalForegroundViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->viewCount(d->layerInternalFg); ++i)
    {
        ZFUIView *child = d->viewAtIndex(d->layerInternalFg, i);
        if(this->internalViewShouldLayout(child))
        {
            child->layout(
                ZFUIViewLayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->viewLayoutParamAtIndex(d->layerInternalFg, i)));
        }
    }
}
ZFCoreArrayPOD<ZFUIView *> ZFUIView::internalForegroundViewArray(void)
{
    return d->layerInternalFg.views;
}

// ============================================================
// other internal view logic
void ZFUIView::internalViewAutoSerializeTagAdd(ZF_IN const zfchar *tag)
{
    if(zfscmpTheSame(tag, zfText("")))
    {
        return ;
    }
    d->internalViewAutoSerializeTags[tag] = zftrue;
}
void ZFUIView::internalViewAutoSerializeTagRemove(ZF_IN const zfchar *tag)
{
    if(zfscmpTheSame(tag, zfText("")))
    {
        return ;
    }
    d->internalViewAutoSerializeTags.erase(tag);
}
void ZFUIView::internalViewAutoSerializeTagRemoveAll(void)
{
    d->internalViewAutoSerializeTags.clear();
}
ZFCoreArray<zfstring> ZFUIView::internalViewAutoSerializeTagGetAll(void)
{
    ZFCoreArray<zfstring> ret;
    ret.capacitySet((zfindex)d->internalViewAutoSerializeTags.size());
    for(_ZFP_ZFUIViewInternalViewAutoSerializeTagMapType::iterator it = d->internalViewAutoSerializeTags.begin();
        it != d->internalViewAutoSerializeTags.end();
        ++it)
    {
        ret.add(it->first.c_str());
    }
    return ret;
}

zfbool ZFUIView::internalViewShouldLayout(ZF_IN ZFUIView *internalView)
{
    return (internalView != d->viewDelegate);
}

// ============================================================
// events
void ZFUIView::viewEventSend(ZF_IN ZFUIEvent *event)
{
    if(event == zfnull)
    {
        return ;
    }

    zfRetainWithoutLeakTest(this);
    zfblockedReleaseWithoutLeakTest(this);
    ZFGlobalEventCenter *eventCenter = ZFGlobalEventCenter::instance();

    this->viewEventOnEventFilter(event);
    if(ZFOBSERVER_HAS_ADD(HasAdd_ViewOnEventFilter))
    {
        this->observerNotify(ZFUIView::EventViewOnEventFilter(), event);
    }
    eventCenter->observerNotifyWithCustomSender(this, ZFUIView::EventViewOnEventFilter(), event);

    if(event->eventResolved())
    {
        return ;
    }

    this->viewEventOnEvent(event);
    if(ZFOBSERVER_HAS_ADD(HasAdd_ViewOnEvent))
    {
        this->observerNotify(ZFUIView::EventViewOnEvent(), event);
    }
    eventCenter->observerNotifyWithCustomSender(this, ZFUIView::EventViewOnEvent(), event);
}
void ZFUIView::viewEventOnEvent(ZF_IN ZFUIEvent *event)
{
    const ZFClass *eventClass = event->classData();
    if(eventClass->classIsSubclassOf(ZFUIMouseEvent::ClassData()))
    {
        ZFUIMouseEvent *mouseEvent = ZFCastZFObjectUnchecked(ZFUIMouseEvent *, event);
        this->viewEventOnMouseEvent(mouseEvent);
        mouseEvent->eventResolvedSet(zftrue);
    }
    else if(eventClass->classIsSubclassOf(ZFUIKeyEvent::ClassData()))
    {
        ZFUIKeyEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIKeyEvent *, event);
        this->viewEventOnKeyEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnKeyEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
    else if(eventClass->classIsSubclassOf(ZFUIWheelEvent::ClassData()))
    {
        ZFUIWheelEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIWheelEvent *, event);
        this->viewEventOnWheelEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnWheelEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
}

void ZFUIView::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    if(this->viewUIEnable())
    {
        if(mouseEvent->mouseAction == ZFUIMouseAction::e_MouseDown
            && this->viewFocusable()
            && this->viewFocusObtainWhenClick())
        {
            this->viewFocusRequest(zftrue);
        }
        mouseEvent->eventResolvedSet(zftrue);
    }
}

void ZFUIView::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    this->viewEventOnKeyEventResolveFocus(keyEvent);
}
void ZFUIView::viewEventOnKeyEventResolveFocus(ZF_IN ZFUIKeyEvent *keyEvent)
{
    ZFUIViewFocusResolveKeyEvent(this, keyEvent);
}

void ZFUIView::viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent)
{
    // nothing to do
}

// ============================================================
// view property async update
void ZFUIView::viewPropertyUpdateRequest(void)
{
    ZFThreadTaskRequest(
        zfHint("task")ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder)->viewPropertyOnUpdateListener,
        zfHint("userData")this->objectHolder(),
        zfHint("param0")zfnull,
        zfHint("param1")zfnull,
        zfHint("owner")this,
        zfHint("mergeCallback")ZFThreadTaskRequestMergeCallbackIgnoreOldTask);
}

ZF_NAMESPACE_GLOBAL_END


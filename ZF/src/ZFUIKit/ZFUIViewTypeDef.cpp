/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIKit.hh"
#include "ZFUIViewTypeDef.h"
#include "ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIViewChildLayer)

ZFOBJECT_REGISTER(ZFUIViewMeasureResult)

// ============================================================
// ZFUIViewLayoutParam
ZFOBJECT_REGISTER(ZFUIViewLayoutParam)
ZFUIRect ZFUIViewLayoutParam::layoutParamApply(ZF_IN const ZFUIRect &rect,
                                               ZF_IN ZFUIView *child,
                                               ZF_IN ZFUIViewLayoutParam *lp)
{
    ZFUISize refSizeTmp = ZFUIRectApplyMargin(rect, lp->layoutMargin()).size;
    if(refSizeTmp.width < 0)
    {
        refSizeTmp.width = 0;
    }
    if(refSizeTmp.height < 0)
    {
        refSizeTmp.height = 0;
    }

    if(lp->sizeHint().width >= 0)
    {
        refSizeTmp.width = zfmMin(lp->sizeHint().width, refSizeTmp.width);
    }
    if(lp->sizeHint().height >= 0)
    {
        refSizeTmp.height = zfmMin(lp->sizeHint().height, refSizeTmp.height);
    }
    child->layoutMeasure(refSizeTmp, lp->sizeParam());
    return ZFUIAlignApply(lp->layoutAlign(), rect, child->layoutMeasuredSize(), lp->layoutMargin());
}

ZFUISize ZFUIViewLayoutParam::sizeHintApply(ZF_IN const ZFUISize &size,
                                            ZF_IN const ZFUISize &sizeHint,
                                            ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUISize ret = size;
    switch(sizeParam.width)
    {
        case ZFUISizeType::e_Wrap:
            if(sizeHint.width >= 0 && size.width > sizeHint.width)
            {
                ret.width = sizeHint.width;
            }
            break;
        case ZFUISizeType::e_Fill:
            if(sizeHint.width >= 0)
            {
                ret.width = sizeHint.width;
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ret;
    }
    switch(sizeParam.height)
    {
        case ZFUISizeType::e_Wrap:
            if(sizeHint.height >= 0 && size.height > sizeHint.height)
            {
                ret.height = sizeHint.height;
            }
            break;
        case ZFUISizeType::e_Fill:
            if(sizeHint.height >= 0)
            {
                ret.height = sizeHint.height;
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ret;
    }
    return ret;
}
zfint ZFUIViewLayoutParam::sizeHintMerge(ZF_IN zfint sizeHint0,
                                         ZF_IN zfint sizeHint1)
{
    if(sizeHint0 < 0 && sizeHint1 < 0)
    {
        return -1;
    }
    else if(sizeHint0 >= 0 && sizeHint1 >= 0)
    {
        return zfmMin(sizeHint0, sizeHint1);
    }
    else
    {
        return zfmMax(sizeHint0, sizeHint1);
    }
}
ZFUISize ZFUIViewLayoutParam::sizeHintMerge(ZF_IN const ZFUISize &sizeHint0,
                                            ZF_IN const ZFUISize &sizeHint1)
{
    return ZFUISizeMake(
        ZFUIViewLayoutParam::sizeHintMerge(sizeHint0.width, sizeHint1.width),
        ZFUIViewLayoutParam::sizeHintMerge(sizeHint0.height, sizeHint1.height));
}
zfint ZFUIViewLayoutParam::sizeHintOffset(ZF_IN zfint sizeHint,
                                          ZF_IN zfint offset)
{
    if(offset >= 0)
    {
        return ((sizeHint >= 0) ? sizeHint + offset : -1);
    }
    else
    {
        return ((sizeHint >= 0) ? zfmMax(0, sizeHint + offset) : -1);
    }
}
ZFUISize ZFUIViewLayoutParam::sizeHintOffset(ZF_IN const ZFUISize &sizeHint,
                                             ZF_IN const ZFUISize &offset)
{
    return ZFUISizeMake(
        ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, offset.width),
        ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, offset.height));
}
ZFUISize ZFUIViewLayoutParam::sizeHintOffset(ZF_IN const ZFUISize &sizeHint,
                                             ZF_IN zfint offset)
{
    return ZFUISizeMake(
        ZFUIViewLayoutParam::sizeHintOffset(sizeHint.width, offset),
        ZFUIViewLayoutParam::sizeHintOffset(sizeHint.height, offset));
}

ZF_NAMESPACE_GLOBAL_END


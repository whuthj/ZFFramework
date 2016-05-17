/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewTypeDef.h
 * @brief additional class for ZFUIView
 */

#ifndef _ZFI_ZFUIViewTypeDef_h_
#define _ZFI_ZFUIViewTypeDef_h_

#include "ZFUITypeDef.h"
#include "ZFUIStyle.h"
#include "ZFUIGlobalStyle.h"
#include "ZFUILayoutParam.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIViewChildLayerEnum value="value">
 *   </ZFUIViewChildLayerEnum>
 * @endcode
 */
#define ZFPropertyTypeId_ZFUIViewChildLayerEnum zfText("ZFUIViewChildLayerEnum")
/**
 * @brief three layer for ZFUIView
 */
ZFENUM_BEGIN(ZFUIViewChildLayer)
    ZFENUM_VALUE(Normal) /**< @brief added by #ZFUIView::childAdd */
    ZFENUM_VALUE(Background) /**< @brief added by #ZFUIView::internalBackgroundViewAdd */
    ZFENUM_VALUE(Foreground) /**< @brief added by #ZFUIView::internalForegroundViewAdd */
ZFENUM_SEPARATOR(ZFUIViewChildLayer)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(Background)
    ZFENUM_VALUE_REGISTER(Foreground)
ZFENUM_END(ZFUIViewChildLayer)

// ============================================================
// ZFUIViewMeasureResult
/**
 * @brief data used by #ZFUIView::EventViewLayoutOnMeasureFinish,
 *   you may modify the #ZFUIViewMeasureResult::measuredSize
 *   to override the measured result
 */
zfclass ZF_ENV_EXPORT ZFUIViewMeasureResult : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIViewMeasureResult, ZFObject)

public:
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize sizeHint;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISizeParam sizeParam;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize measuredSize;

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

// ============================================================
// ZFUIViewLayoutParam
zfclassFwd ZFUIView;
/**
 * @brief base class of layout params for ZFUIView
 *
 * ZFUIView supply basic but useful dynamic layout mechanism,
 * which use align, margin and measure logic to supply dynamic layout\n
 * while layout a child, ZFUIView would do those things:
 * -  measure child to see it's size depends on #sizeParam
 * -  layout child depends on #ZFUIAlignApply
 *
 * you should always use #ZFUIView::layoutParamCreate to create a layout param\n
 * subclass may override or define new params to supply more complex layout mechanism\n
 * \n
 * dynamic layout logic depends on #ZFUIViewLayoutParam::sizeHint and #ZFUIViewLayoutParam::sizeParam,
 * -  ZFUISizeType::e_Fill:
 *   means child should fill its parent or sizeHint
 *   -  if sizeHint is a non-negative value,
 *     means child would be scale to fill sizeHint
 *   -  if sizeHint is a negative value,
 *     means child would be scale to fill parent's size
 * -  ZFUISizeType::e_Wrap:
 *   means child should wrap by its content
 *   -  if sizeHint is a non-negative value,
 *     means child would use its desired size but should not exceeds sizeHint
 *   -  if sizeHint is a negative value,
 *     means child has no size limit and would use its desired size
 */
zfclass ZF_ENV_EXPORT ZFUIViewLayoutParam : zfextends ZFUILayoutParam
{
    ZFOBJECT_DECLARE(ZFUIViewLayoutParam, ZFUILayoutParam)

public:
    /**
     * @brief calculate single child's frame using logic of #ZFUIViewLayoutParam
     *
     * this is not necessary for subclass to declare its own one,
     * it's declared for convenient for subclass to layout child
     * using parent's layout logic
     */
    static ZFUIRect layoutParamApply(ZF_IN const ZFUIRect &rect,
                                     ZF_IN ZFUIView *child,
                                     ZF_IN ZFUIViewLayoutParam *lp);

    /**
     * @brief util method to apply sizeHint accorrding sizeParam
     */
    static ZFUISize sizeHintApply(ZF_IN const ZFUISize &size,
                                  ZF_IN const ZFUISize &sizeHint,
                                  ZF_IN const ZFUISizeParam &sizeParam);
    /**
     * @brief merge two size hint
     */
    static zfint sizeHintMerge(ZF_IN zfint sizeHint0,
                               ZF_IN zfint sizeHint1);
    /**
     * @brief merge two size hint
     */
    static ZFUISize sizeHintMerge(ZF_IN const ZFUISize &sizeHint0,
                                  ZF_IN const ZFUISize &sizeHint1);
    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    static zfint sizeHintOffset(ZF_IN zfint sizeHint,
                                ZF_IN zfint offset);
    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    static ZFUISize sizeHintOffset(ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISize &offset);
    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    static ZFUISize sizeHintOffset(ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN zfint offset);

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    /**
     * @brief see #ZFUIViewLayoutParam, default is #ZFUISizeInvalid
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, sizeHint,
                                ZFPropertyInitValue(ZFUISizeInvalid))
    /**
     * @brief see #ZFUIViewLayoutParam, default is #ZFUISizeParamWrapWidthWrapHeight
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISizeParam, sizeParam,
                                ZFPropertyInitValue(ZFUISizeParamWrapWidthWrapHeight))
    /**
     * @brief see #ZFUIViewLayoutParam, default is (ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, layoutAlign,
                                ZFPropertyInitValue(ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner))
    /**
     * @brief see #ZFUIViewLayoutParam, default is (0, 0, 0, 0)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, layoutMargin,
                                ZFPropertyInitValue(ZFUIMarginZero))
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewTypeDef_h_


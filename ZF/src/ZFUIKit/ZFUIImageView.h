/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIImageView.h
 * @brief image view
 */

#ifndef _ZFI_ZFUIImageView_h_
#define _ZFI_ZFUIImageView_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageViewStyle
/**
 * @brief style for #ZFUIImageView
 *
 * those property would be overrided:
 * -  ZFUIViewStyle::viewUIEnable: zffalse
 * -  ZFUIViewStyle::viewUIEnableTree: zffalse
 */
zfinterface ZF_ENV_EXPORT ZFUIImageViewStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIImageViewStyle, ZFUIViewStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIImageViewStyle, ZFUIViewStyle)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewUIEnable, zffalse)
    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewUIEnableTree, zffalse)

public:
    /**
     * @brief image for this view
     */
    ZFPROPERTY_RETAIN(ZFUIImage *, imageContent)

    /**
     * @brief image scale type, fill by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIContentScaleTypeEnum, imageScaleType,
                                ZFPropertyInitValue(ZFUIContentScaleType::e_Default()))

    /**
     * @brief margin of the content image
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, imageContentMargin)
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIImageViewStyle, ZFUIViewStyle)

// ============================================================
// ZFUIImageView
/**
 * @brief native image view
 */
zfclass ZF_ENV_EXPORT ZFUIImageView : zfextends ZFUIView, zfimplements ZFUIImageViewStyle
{
    ZFOBJECT_DECLARE(ZFUIImageView, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZFUIImageViewStyle)

public:
    // ============================================================
    // styles
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIImage *, imageContent);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIContentScaleTypeEnum, imageScaleType);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIMargin, imageContentMargin);

public:
    /**
     * @brief init with image
     */
    virtual ZFObject *objectOnInit(ZF_IN ZFUIImage *image);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

protected:
    /**
     * @brief for image view, we will update accorrding to #imageScaleType
     */
    zfoverride
    virtual void nativeImplViewOnLayout(ZF_OUT ZFUIRect &result,
                                        ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void nativeImplViewMarginOnUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin);
    /**
     * @brief image view would measure accorrding image size
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageView_h_


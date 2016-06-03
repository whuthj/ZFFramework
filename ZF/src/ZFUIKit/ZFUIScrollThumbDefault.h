/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIScrollThumbDefault.h
 * @brief default scroll thumb of #ZFUIScroller
 */

#ifndef _ZFI_ZFUIScrollThumbDefault_h_
#define _ZFI_ZFUIScrollThumbDefault_h_

#include "ZFUIScrollThumb.h"
#include "ZFUIScrollView.h"
#include "ZFUIImage.h"
#include "ZFUIImageView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief style for #ZFUIScrollThumbDefault
 */
zfinterface ZF_ENV_EXPORT ZFUIScrollThumbDefaultStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIScrollThumbDefaultStyle, ZFStyleable)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIScrollThumbDefaultStyle, ZFStyleable)

public:
    /**
     * @brief horizontal scroll thumb image, null to hide it
     */
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIImage *, scrollThumbImageHorizontal,
                                ZFPropertyInitValue(ZFUIImageResXml(zfText("ZFFramework/ZFUIKit/ZFUIScrollThumbDefault_thumb.xml")).to<ZFUIImage *>()))
    /**
     * @brief vertical scroll thumb image, null to hide it
     */
    ZFPROPERTY_RETAIN_WITH_INIT(ZFUIImage *, scrollThumbImageVertical,
                                ZFPropertyInitValue(ZFUIImageResXml(zfText("ZFFramework/ZFUIKit/ZFUIScrollThumbDefault_thumb.xml")).to<ZFUIImage *>()))

    /**
     * @brief delay to auto hide, #ZFUIGlobalStyle::aniDelayNormal by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, scrollThumbAutoHideDelayHorizontal,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->aniDelayNormal()))
    /**
     * @brief delay to auto hide, #ZFUIGlobalStyle::aniDelayNormal by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, scrollThumbAutoHideDelayVertical,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->aniDelayNormal()))

    /**
     * @brief time to auto hide, #ZFUIGlobalStyle::aniDurationNormal by default, 0 means never hide
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, scrollThumbAutoHideDurationHorizontal,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->aniDurationNormal()))
    /**
     * @brief time to auto hide, #ZFUIGlobalStyle::aniDurationNormal by default, 0 means never hide
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, scrollThumbAutoHideDurationVertical,
                                ZFPropertyInitValue(ZFUIGlobalStyle::DefaultStyle()->aniDurationNormal()))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIScrollThumbDefaultStyle, ZFStyleable)

zfclassFwd _ZFP_ZFUIScrollThumbDefaultPrivate;
/**
 * @brief default scroll thumb for #ZFUIScroller
 */
zfclass ZF_ENV_EXPORT ZFUIScrollThumbDefault : zfextends ZFStyleableObject, zfimplements ZFUIScrollThumbDefaultStyle, zfimplements ZFUIScrollThumb
{
    ZFOBJECT_DECLARE(ZFUIScrollThumbDefault, ZFStyleableObject)
    ZFIMPLEMENTS_DECLARE(ZFUIScrollThumbDefaultStyle, ZFUIScrollThumb)

    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIImage *, scrollThumbImageHorizontal);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIImage *, scrollThumbImageVertical);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    zfoverride
    virtual void scrollThumbInit(ZF_IN zfbool horizontal);
    zfoverride
    virtual void scrollThumbDealloc(void);

    zfoverride
    virtual void scrollThumbUpdate(void);

private:
    _ZFP_ZFUIScrollThumbDefaultPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollThumbDefault_h_


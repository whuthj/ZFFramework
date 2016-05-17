/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIAutoFitLayout.h
 * @brief a scroll container that automatically fits content's position
 *   accorrding #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFUIAutoFitLayout_h_
#define _ZFI_ZFUIAutoFitLayout_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIAutoFitLayoutStyle
/**
 * @brief style for #ZFUIAutoFitLayout
 */
zfinterface ZF_ENV_EXPORT ZFUIAutoFitLayoutStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIAutoFitLayoutStyle, ZFUIViewStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAutoFitLayoutStyle, ZFUIViewStyle)

public:
    /**
     * @brief whether enable auto fit, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitEnable,
                                ZFPropertyInitValue(zftrue))
    /**
     * @brief whether auto move focused view to visible position, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, autoFitFocusedViewToVisible,
                                ZFPropertyInitValue(zftrue))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIAutoFitLayoutStyle, ZFUIViewStyle)

// ============================================================
// ZFUIAutoFitLayout
zfclassFwd _ZFP_ZFUIAutoFitLayoutPrivate;
/**
 * @brief a scroll container that automatically fits content's position
 *   accorrding #ZFUIOnScreenKeyboardState
 */
zfclass ZF_ENV_EXPORT ZFUIAutoFitLayout : zfextends ZFUIView, zfimplements ZFUIAutoFitLayoutStyle
{
    ZFOBJECT_DECLARE(ZFUIAutoFitLayout, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZFUIAutoFitLayoutStyle)

public:
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, autoFitEnable);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, zfbool, autoFitFocusedViewToVisible);

public:
    /**
     * @brief current focused child view inside this layout, null if none
     */
    virtual ZFUIView *autoFitFocusedView(void);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

protected:
    zfoverride
    virtual zfbool viewDelegateSupported(void)
    {
        return zffalse;
    }

    zfoverride
    virtual void viewDelegateLayoutOnMeasure(ZF_OUT ZFUISize &ret,
                                             ZF_IN const ZFUISize &sizeHint,
                                             ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void viewDelegateLayoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);

private:
    _ZFP_ZFUIAutoFitLayoutPrivate *d;
    friend zfclassFwd _ZFP_ZFUIAutoFitLayoutPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAutoFitLayout_h_


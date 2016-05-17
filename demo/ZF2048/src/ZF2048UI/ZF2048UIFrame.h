/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZF2048UIFrame_h_
#define _ZFI_ZF2048UIFrame_h_

#include "ZF2048UIBlock.h"
#include "ZF2048UISkin.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZF2048UIFrameStyle
zfinterface ZF2048UIFrameStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZF2048UIFrameStyle, ZFUIViewStyle)
    ZFSTYLE_DEFAULT_DECLARE(ZF2048UIFrameStyle, ZFUIViewStyle)

    ZFPROPERTY_OVERRIDE_INIT_VALUE_ASSIGN(zfbool, viewFocusable, zftrue)

public:
    ZFPROPERTY_RETAIN(ZFUIImage *, frameBackgroundImage)
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, frameMargin, ZFPropertyInitValue(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemSpace())))
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZF2048UIFrameStyle, ZFUIViewStyle)

// ============================================================
// ZF2048UIFrame
zfclassFwd _ZFP_ZF2048UIFramePrivate;
/**
 * @brief linear container view that layout children in linear by specified orientation
 */
zfclass ZF2048UIFrame : zfextends ZFUIView, zfimplements ZF2048UIFrameStyle
{
    ZFOBJECT_DECLARE(ZF2048UIFrame, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZF2048UIFrameStyle)

    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIImage *, frameBackgroundImage);
    ZFPROPERTY_OVERRIDE_SETTER_DECLARE(public, ZFUIMargin, frameMargin);

    /**
     * called when swipe to move or press direction key to move,
     * param0 is the #ZFUIOrientation which indicates move direction
     */
    ZFOBSERVER_EVENT(FrameOnMove)

public:
    virtual void update(ZF_IN const ZF2048Value *data,
                        ZF_IN zfindex dataWidth,
                        ZF_IN zfindex dataHeight);
    virtual ZF2048UIBlock *blockAt(ZF_IN zfindex posX, ZF_IN zfindex posY);
    virtual ZFUIRect blockRectAt(ZF_IN zfindex posX, ZF_IN zfindex posY);

protected:
    virtual inline void frameOnMove(ZF_IN ZFUIOrientationEnum direction)
    {
        this->observerNotify(zfself::EventFrameOnMove(), zflineAlloc(ZFUIOrientation, direction));
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void internalBackgroundViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent);
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent);

private:
    _ZFP_ZF2048UIFramePrivate *d;
    friend zfclassFwd _ZFP_ZF2048UIFramePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048UIFrame_h_


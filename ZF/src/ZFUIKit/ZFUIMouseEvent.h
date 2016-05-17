/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIMouseEvent.h
 * @brief mouse event for ui elements
 */

#ifndef _ZFI_ZFUIMouseEvent_h_
#define _ZFI_ZFUIMouseEvent_h_

#include "ZFUIEvent.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIMouseActionEnum value="value">
 *   </ZFUIMouseActionEnum>
 * @endcode
 */
#define ZFPropertyTypeId_ZFUIMouseActionEnum zfText("ZFUIMouseActionEnum")
/**
 * @brief mouse action for ZFUIMouseEvent
 */
ZFENUM_BEGIN(ZFUIMouseAction)
    ZFENUM_VALUE(MouseDown) /**< @brief mouse down */
    ZFENUM_VALUE(MouseMove) /**< @brief mouse move */
    ZFENUM_VALUE(MouseUp) /**< @brief mouse up */
    ZFENUM_VALUE(MouseCancel) /**< @brief mouse cancel */
    ZFENUM_VALUE(MouseHoverEnter) /**< @brief mouse hover enter */
    ZFENUM_VALUE(MouseHover) /**< @brief mouse hover */
    ZFENUM_VALUE(MouseHoverExit) /**< @brief mouse hover exit */
ZFENUM_SEPARATOR(ZFUIMouseAction)
    ZFENUM_VALUE_REGISTER(MouseDown)
    ZFENUM_VALUE_REGISTER(MouseMove)
    ZFENUM_VALUE_REGISTER(MouseUp)
    ZFENUM_VALUE_REGISTER(MouseCancel)
    ZFENUM_VALUE_REGISTER(MouseHoverEnter)
    ZFENUM_VALUE_REGISTER(MouseHover)
    ZFENUM_VALUE_REGISTER(MouseHoverExit)
ZFENUM_END(ZFUIMouseAction)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUIMouseButtonEnum value="value">
 *   </ZFUIMouseButtonEnum>
 * @endcode
 */
#define ZFPropertyTypeId_ZFUIMouseButtonEnum zfText("ZFUIMouseButtonEnum")
/**
 * @brief mouse action for ZFUIMouseEvent
 */
ZFENUM_BEGIN(ZFUIMouseButton)
    ZFENUM_VALUE(LeftButton) /**< @brief left button */
    ZFENUM_VALUE(RightButton) /**< @brief right button */
    ZFENUM_VALUE(CenterButton) /**< @brief center button */
ZFENUM_SEPARATOR(ZFUIMouseButton)
    ZFENUM_VALUE_REGISTER(LeftButton)
    ZFENUM_VALUE_REGISTER(RightButton)
    ZFENUM_VALUE_REGISTER(CenterButton)
ZFENUM_END(ZFUIMouseButton)

// ============================================================
/**
 * @brief mouse event for ui elements
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIMouseEvent : zfextends ZFUIEvent
{
    ZFOBJECT_DECLARE(ZFUIMouseEvent, ZFUIEvent)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief a unique value to differ mouse from each other
     *
     * mouseId keeps the same from mouse down/move/up/cancel,
     * another mouse down would renew the mouseId\n
     * note, this value is invalid for mouse hover event,
     * and would always be 0
     */
    zfidentity mouseId;
    /**
     * @brief mouse action
     */
    ZFUIMouseActionEnum mouseAction;
    /**
     * @brief mouse point
     */
    ZFUIPoint mousePoint;
    /**
     * @brief mouse button
     *
     * note, this value is invalid for mouse hover event,
     * and would always be #ZFUIMouseButton::e_LeftButton
     */
    ZFUIMouseButtonEnum mouseButton;

public:
    ZFCACHEABLE_DECLARE(ZFUIMouseEvent)

protected:
    zfoverride
    virtual void cacheableOnReset(void);

public:
    zfoverride
    virtual void eventOnApplyScale(ZF_IN zffloat scale);
    zfoverride
    virtual void eventOnApplyScaleReversely(ZF_IN zffloat scale);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIMouseEvent_h_


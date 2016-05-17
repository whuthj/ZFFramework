/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUIKeyboardState.h
 * @brief protocol for #ZFUIKeyboardState
 */

#ifndef _ZFI_ZFProtocolZFUIKeyboardState_h_
#define _ZFI_ZFProtocolZFUIKeyboardState_h_

#include "../ZFUIKeyboardState.h"
#include "../ZFUIKeyEvent.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIKeyboardState
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIKeyboardState)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief see #ZFUIKeyboardState::keyPressed
     */
    virtual zfbool keyPressed(ZF_IN ZFUIKeyCodeEnum keyCode) = 0;
    /**
     * @brief see #ZFUIKeyboardState::keyPressedRaw
     */
    virtual zfbool keyPressedRaw(ZF_IN zfuint32 keyCodeRaw) = 0;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when keyboard's state changed
     */
    zffinal void notifyKeyPressedChange(void)
    {
        ZFUIKeyboardState::instance()->observerNotify(ZFUIKeyboardState::EventKeyPressedOnChange());
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(ZFUIKeyboardState::instance(), ZFUIKeyboardState::EventKeyPressedOnChange());
    }
ZFPROTOCOL_INTERFACE_END(ZFUIKeyboardState)

/**
 * @brief builtin impl with level of #ZFProtocolLevelDefault
 */
extern ZF_ENV_EXPORT void ZFUIKeyboardStateImplNotifyKeyDown(ZF_IN ZFUIKeyCodeEnum keyCode, ZF_IN zfuint32 keyCodeRaw);
/** @brief see #ZFUIKeyboardStateImplNotifyKeyDown */
extern ZF_ENV_EXPORT void ZFUIKeyboardStateImplNotifyKeyUp(ZF_IN ZFUIKeyCodeEnum keyCode, ZF_IN zfuint32 keyCodeRaw);

/** @brief util method to #ZFUIKeyboardStateImplNotifyKeyDown */
extern ZF_ENV_EXPORT void ZFUIKeyboardStateImplNotifyKeyEvent(ZF_IN ZFUIKeyEvent *event);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIKeyboardState_h_


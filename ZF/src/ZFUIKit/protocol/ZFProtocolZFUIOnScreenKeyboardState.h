/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFUIOnScreenKeyboardState.h
 * @brief protocol for #ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFProtocolZFUIOnScreenKeyboardState_h_
#define _ZFI_ZFProtocolZFUIOnScreenKeyboardState_h_

#include "../ZFUIOnScreenKeyboardState.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIOnScreenKeyboardState
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIOnScreenKeyboardState)
ZFPROTOCOL_INTERFACE_OPTIONAL
public:
    /**
     * @brief true if the keyboard currently showing
     */
    virtual zfbool keyboardShowing(ZF_IN ZFUIOnScreenKeyboardState *keyboardState) = 0;
    /**
     * @brief current key board frame, must return #ZFUIRectZero if not showing
     */
    virtual ZFUIRect keyboardFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState) = 0;

    /**
     * @brief see #ZFUIOnScreenKeyboardState::keyboardFixClientFrame
     */
    virtual void keyboardFixClientFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState,
                                        ZF_IN_OUT ZFUIMargin &margin)
    {
        margin = ZFUIMarginZero;
        if(keyboardState->keyboardShowing())
        {
            margin.bottom = this->keyboardFrame(keyboardState).size.height;
        }
    }

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementations must notify when keyboard's showing state or frame changed
     */
    zffinal void notifyKeyboardStateOnChange(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        zffloat scale = keyboardState->ownerSysWindow()->rootView()->scaleGetFixed();
        zfbool keyboardShowingNew = this->keyboardShowing(keyboardState);
        ZFUIRect keyboardFrameNew = ZFUIRectApplyScaleReversely(this->keyboardFrame(keyboardState), scale);
        if(keyboardShowingNew == keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardShowing
            && keyboardFrameNew == keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame)
        {
            return ;
        }
        keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFramePrev = keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame;
        keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardFrame = keyboardFrameNew;
        keyboardState->_ZFP_ZFUIOnScreenKeyboardState_keyboardShowing = this->keyboardShowing(keyboardState);
        keyboardState->observerNotify(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange());
        ZFGlobalEventCenter::instance()->observerNotifyWithCustomSender(keyboardState, ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange());
    }
ZFPROTOCOL_INTERFACE_END(ZFUIOnScreenKeyboardState)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIOnScreenKeyboardState_h_


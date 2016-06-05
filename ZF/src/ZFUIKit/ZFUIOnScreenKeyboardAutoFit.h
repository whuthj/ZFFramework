/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIOnScreenKeyboardAutoFit.h
 * @brief auto fit ZFUIWindow accorrding to ZFUIOnScreenKeyboardState
 */

#ifndef _ZFI_ZFUIOnScreenKeyboardAutoFit_h_
#define _ZFI_ZFUIOnScreenKeyboardAutoFit_h_

#include "ZFUIOnScreenKeyboardState.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief auto fit #ZFUIWindow accorrding to ZFUIOnScreenKeyboardState
 *
 * adjust window's layout param when on screen keyboard show or hide\n
 * you must not modify window's layout param's #ZFUIViewLayoutParam::layoutMargin
 * while the auto fit is activated,
 * it would be restored during #ZFUIOnScreenKeyboardAutoFitCancel
 * and your modification would be lost
 */
extern ZF_ENV_EXPORT void ZFUIOnScreenKeyboardAutoFit(ZF_IN ZFUIWindow *window);
/** @brief cancel #ZFUIOnScreenKeyboardAutoFit */
extern ZF_ENV_EXPORT void ZFUIOnScreenKeyboardAutoFitCancel(ZF_IN ZFUIWindow *window);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIOnScreenKeyboardAutoFit_h_


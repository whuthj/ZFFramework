/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFLISTENER_FUNCTION_DECLARE(ZFUIOnScreenKeyboardState_test_keyboardStateOnChange)
{
    zfLogTrimT() << zfText("[ZFUIOnScreenKeyboardState]") << listenerData.sender;
}
ZF_GLOBAL_INITIALIZER_INIT(ZFUIOnScreenKeyboardState_test)
{
    this->listener = ZFCallbackForMethod(ZFMethodAccessFunction(ZFUIOnScreenKeyboardState_test_keyboardStateOnChange));
    ZFUIOnScreenKeyboardState::instanceForSysWindow()->observerAdd(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), this->listener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardState_test)
{
    ZFUIOnScreenKeyboardState::instanceForSysWindow()->observerRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), this->listener);
}
private:
    ZFListener listener;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END


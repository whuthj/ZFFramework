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

ZFLISTENER_FUNCTION_DECLARE(ZFUIKeyboardState_test_keyPressedOnChange)
{
    zfLogTrimT() << zfText("[ZFUIKeyboardState]") << listenerData.sender;
}
ZF_GLOBAL_INITIALIZER_INIT(ZFUIKeyboardState_test)
{
    this->keyPressedOnChangeListener = ZFCallbackForMethod(ZFMethodAccessFunction(ZFUIKeyboardState_test_keyPressedOnChange));
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIKeyboardState::EventKeyPressedOnChange(), this->keyPressedOnChangeListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIKeyboardState_test)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIKeyboardState::EventKeyPressedOnChange(), this->keyPressedOnChangeListener);
}
private:
    ZFListener keyPressedOnChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardState_test)

ZF_NAMESPACE_GLOBAL_END


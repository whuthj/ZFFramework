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

#if 1
ZFLISTENER_FUNCTION_DECLARE(ZFUIViewFocusState_test_viewFocusOnChange)
{
    zfLogTrimT() << zfText("[ZFUIViewFocus]")
        << listenerData.sender
        << zfText("changed to") << listenerData.sender->to<ZFUIView *>()->viewFocused();
}
ZF_GLOBAL_INITIALIZER_INIT(ZFUIViewFocusState_test)
{
    this->listener = ZFCallbackForMethod(ZFMethodAccessFunction(ZFUIViewFocusState_test_viewFocusOnChange));
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIView::EventViewFocusOnChange(), this->listener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusState_test)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIView::EventViewFocusOnChange(), this->listener);
}
private:
    ZFListener listener;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusState_test)
#endif

ZF_NAMESPACE_GLOBAL_END


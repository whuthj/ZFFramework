/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUIKit.hh"
#include "ZFUIViewFocusAutoHighlight_AutoPause.h"

#include "ZFUIWindow.h"
#include "ZFUISysWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewFocusAutoHighlight_AutoPause_DataHolder, ZFLevelZFFrameworkLow)
{
    this->doActionListener = ZFCallbackForRawFunction(zfself::doAction);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFGlobalEventCenter::instance()->observerAdd(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewFocusAutoHighlight_AutoPause_DataHolder)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIWindow::EventWindowOnShow(), this->doActionListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUIWindow::EventWindowOnHide(), this->doActionListener);
    ZFGlobalEventCenter::instance()->observerRemove(ZFUISysWindow::EventSysWindowOnResume(), this->doActionListener);
}
ZFListener doActionListener;
static ZFLISTENER_PROTOTYPE_EXPAND(doAction)
{
    ZFUIViewFocusAutoHighlightPauseForTime();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewFocusAutoHighlight_AutoPause_DataHolder)

ZF_NAMESPACE_GLOBAL_END


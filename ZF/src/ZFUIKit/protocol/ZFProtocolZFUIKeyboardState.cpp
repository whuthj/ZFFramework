/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "../ZFPrivate_ZFUIKit.hh"
#include "ZFProtocolZFUIKeyboardState.h"
#include "../ZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFUIKeyboardState)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIKeyboardStateBuiltinImpl_DataHolder, ZFLevelZFFrameworkNormal)
{
    this->viewOnEventFilterListener = ZFCallbackForRawFunction(zfself::viewOnEventFilter);
}
ZFCoreArrayPOD<ZFUIKeyCodeEnum> keyPressed;
ZFCoreArrayPOD<zfuint32> keyPressedRaw;
ZFListener viewOnEventFilterListener;
static ZFLISTENER_PROTOTYPE_EXPAND(viewOnEventFilter)
{
    ZFUIKeyEvent *event = ZFCastZFObject(ZFUIKeyEvent *, listenerData.param0);
    if(event != zfnull)
    {
        ZFUIKeyboardStateBuiltinImplNotifyKeyEvent(event);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardStateBuiltinImpl_DataHolder)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIKeyboardStateBuiltinImpl, ZFUIKeyboardState, ZFProtocolLevelDefault)
public:
    virtual zfbool keyPressed(ZF_IN ZFUIKeyCodeEnum keyCode)
    {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->keyPressed.find(keyCode) != zfindexMax);
    }
    virtual zfbool keyPressedRaw(ZF_IN zfuint32 keyCodeRaw)
    {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->keyPressedRaw.find(keyCodeRaw) != zfindexMax);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIKeyboardStateBuiltinImpl)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIKeyboardStateBuiltinImpl)

// ============================================================
void ZFUIKeyboardStateBuiltinImplRegister(void)
{
    ZFGlobalEventCenter::instance()->observerAdd(
        ZFUIView::EventViewOnEventFilter(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->viewOnEventFilterListener);
}
void ZFUIKeyboardStateBuiltinImplUnregister(void)
{
    ZFGlobalEventCenter::instance()->observerRemove(
        ZFUIView::EventViewOnEventFilter(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder)->viewOnEventFilterListener);
}

void ZFUIKeyboardStateBuiltinImplNotifyKeyEvent(ZF_IN ZFUIKeyEvent *event)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIKeyboardStateBuiltinImpl_DataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateBuiltinImpl_DataHolder);
    switch(event->keyAction)
    {
        case ZFUIKeyAction::e_KeyDown:
        case ZFUIKeyAction::e_KeyRepeat:
        {
            d->keyPressed.add(event->keyCode);
            d->keyPressedRaw.add(event->keyCodeRaw);
        }
            break;
        case ZFUIKeyAction::e_KeyUp:
        case ZFUIKeyAction::e_KeyCancel:
        {
            zfindex index = 0;
            for( ; index < d->keyPressed.count(); ++index)
            {
                if(d->keyPressed[index] == event->keyCode
                    && d->keyPressedRaw[index] == event->keyCodeRaw)
                {
                    break;
                }
            }
            if(index < d->keyPressed.count())
            {
                d->keyPressed.remove(index);
                d->keyPressedRaw.remove(index);
            }
        }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

ZF_NAMESPACE_GLOBAL_END


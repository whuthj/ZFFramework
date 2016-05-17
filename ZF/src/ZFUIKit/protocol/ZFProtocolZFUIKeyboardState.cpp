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

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_INTERFACE_REGISTER(ZFUIKeyboardState)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIKeyboardStateImpl_default_DataHolder, ZFLevelZFFrameworkNormal)
{
}
ZFCoreArrayPOD<ZFUIKeyCodeEnum> keyPressed;
ZFCoreArrayPOD<zfuint32> keyPressedRaw;
ZF_GLOBAL_INITIALIZER_END(ZFUIKeyboardStateImpl_default_DataHolder)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIKeyboardStateImpl_default, ZFUIKeyboardState, ZFProtocolLevelDefault)
public:
    virtual zfbool keyPressed(ZF_IN ZFUIKeyCodeEnum keyCode)
    {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateImpl_default_DataHolder)->keyPressed.find(keyCode) != zfindexMax);
    }
    virtual zfbool keyPressedRaw(ZF_IN zfuint32 keyCodeRaw)
    {
        return (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateImpl_default_DataHolder)->keyPressedRaw.find(keyCodeRaw) != zfindexMax);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIKeyboardStateImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIKeyboardStateImpl_default)

// ============================================================
void ZFUIKeyboardStateImplNotifyKeyDown(ZF_IN ZFUIKeyCodeEnum keyCode, ZF_IN zfuint32 keyCodeRaw)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIKeyboardStateImpl_default_DataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateImpl_default_DataHolder);
    d->keyPressed.add(keyCode);
    d->keyPressedRaw.add(keyCodeRaw);
}
void ZFUIKeyboardStateImplNotifyKeyUp(ZF_IN ZFUIKeyCodeEnum keyCode, ZF_IN zfuint32 keyCodeRaw)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIKeyboardStateImpl_default_DataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIKeyboardStateImpl_default_DataHolder);
    zfindex index = 0;
    for( ; index < d->keyPressed.count(); ++index)
    {
        if(d->keyPressed[index] == keyCode
            && d->keyPressedRaw[index] == keyCodeRaw)
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

void ZFUIKeyboardStateImplNotifyKeyEvent(ZF_IN ZFUIKeyEvent *event)
{
    switch(event->keyAction)
    {
        case ZFUIKeyAction::e_KeyDown:
        case ZFUIKeyAction::e_KeyRepeat:
            ZFUIKeyboardStateImplNotifyKeyDown(event->keyCode, event->keyCodeRaw);
            break;
        case ZFUIKeyAction::e_KeyUp:
        case ZFUIKeyAction::e_KeyCancel:
            ZFUIKeyboardStateImplNotifyKeyUp(event->keyCode, event->keyCodeRaw);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

ZF_NAMESPACE_GLOBAL_END


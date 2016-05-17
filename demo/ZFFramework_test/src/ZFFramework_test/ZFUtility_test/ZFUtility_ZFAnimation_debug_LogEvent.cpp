/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
ZF_GLOBAL_INITIALIZER_INIT(ZFUtility_ZFAnimation_debug_LogEvent)
{
    ZFLISTENER_LOCAL(aniOnAlloc, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender << zfText("alloc");
    })
    ZFLISTENER_LOCAL(aniOnDealloc, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender << zfText("dealloc");
    })
    ZFLISTENER_LOCAL(aniOnInvalid, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender << zfText("invalid");
    })
    ZFLISTENER_LOCAL(aniOnStart, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender << zfText("start");
    })
    ZFLISTENER_LOCAL(aniOnStop, {
        zfLogTrimT() << zfLogCurTimeString() << listenerData.sender << zfText("stop");
    })
    this->debugTaskId = ZFObjectInstanceObserverStart(ZFAnimation::ClassData(), ZFCoreArrayCreate(ZFObjectInstanceObserverParam
        , ZFObjectInstanceObserverParam().eventIdSet(ZFObject::EventObjectAfterAlloc()).observerSet(aniOnAlloc)
        , ZFObjectInstanceObserverParam().eventIdSet(ZFObject::EventObjectBeforeDealloc()).observerSet(aniOnDealloc)
        , ZFObjectInstanceObserverParam().eventIdSet(ZFAnimation::EventAniOnInvalid()).observerSet(aniOnInvalid)
        , ZFObjectInstanceObserverParam().eventIdSet(ZFAnimation::EventAniOnStart()).observerSet(aniOnStart)
        , ZFObjectInstanceObserverParam().eventIdSet(ZFAnimation::EventAniOnStop()).observerSet(aniOnStop)
        ));
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUtility_ZFAnimation_debug_LogEvent)
{
    ZFObjectInstanceObserverStop(this->debugTaskId);
}
zfidentity debugTaskId;
ZF_GLOBAL_INITIALIZER_END(ZFUtility_ZFAnimation_debug_LogEvent)
#endif

ZF_NAMESPACE_GLOBAL_END


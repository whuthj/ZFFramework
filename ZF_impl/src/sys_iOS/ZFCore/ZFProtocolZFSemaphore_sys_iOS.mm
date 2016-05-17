/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFSemaphore.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFSemaphoreImpl_sys_iOS, ZFSemaphore, ZFProtocolLevelSystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:NSCondition"))
public:
    virtual void *nativeSemaphoreCreate(ZF_IN ZFSemaphore *semaphore)
    {
        return ZFCastStatic(void *, [NSCondition new]);
    }
    virtual void nativeSemaphoreDestroy(ZF_IN ZFSemaphore *semaphore,
                                        ZF_IN void *nativeSemaphore)
    {
        [ZFCastStatic(NSCondition *, nativeSemaphore) release];
    }

    virtual void semaphoreLock(ZF_IN ZFSemaphore *semaphore)
    {
        [ZFCastStatic(NSCondition *, semaphore->nativeSemaphore()) lock];
    }
    virtual void semaphoreUnlock(ZF_IN ZFSemaphore *semaphore)
    {
        [ZFCastStatic(NSCondition *, semaphore->nativeSemaphore()) unlock];
    }

    virtual void semaphoreSignal(ZF_IN ZFSemaphore *semaphore)
    {
        [ZFCastStatic(NSCondition *, semaphore->nativeSemaphore()) signal];
    }
    virtual void semaphoreBroadcast(ZF_IN ZFSemaphore *semaphore)
    {
        [ZFCastStatic(NSCondition *, semaphore->nativeSemaphore()) broadcast];
    }
    virtual void semaphoreWait(ZF_IN ZFSemaphore *semaphore)
    {
        [ZFCastStatic(NSCondition *, semaphore->nativeSemaphore()) wait];
    }
    virtual zfbool semaphoreWait(ZF_IN ZFSemaphore *semaphore,
                                 ZF_IN const zftimet &miliSecsTimeout)
    {
        return [ZFCastStatic(NSCondition *, semaphore->nativeSemaphore()) waitUntilDate:[NSDate dateWithTimeIntervalSinceNow:((NSTimeInterval)miliSecsTimeout / 1000)]];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFSemaphoreImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFSemaphoreImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


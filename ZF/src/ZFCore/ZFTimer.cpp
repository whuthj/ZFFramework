/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTimer.h"
#include "protocol/ZFProtocolZFTimer.h"

#include "ZFThread.h" // for timer thread register

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFTimerPrivate
zfclassNotPOD _ZFP_ZFTimerPrivate
{
public:
    void *nativeTimer;
    zfbool timerStarted;
    zftimet timerInterval;
    zftimet timerDelay;
    ZFObject *timerParam0;
    ZFObject *timerParam1;
    zfbool timerActivateInMainThread;
    zfindex timerActivatedCount;
    void *timerThreadToken;

public:
    _ZFP_ZFTimerPrivate(void)
    : nativeTimer(zfnull)
    , timerStarted(zffalse)
    , timerInterval(1000)
    , timerDelay(0)
    , timerParam0(zfnull)
    , timerParam1(zfnull)
    , timerActivateInMainThread(zffalse)
    , timerActivatedCount(0)
    , timerThreadToken(zfnull)
    {
    }
};

// ============================================================
// ZFTimer
ZFOBJECT_REGISTER(ZFTimer)
ZFObject *ZFTimer::objectOnInit(ZF_IN const zftimet &timerInterval,
                                ZF_IN_OPT ZFObject *timerParam0 /* = zfnull */,
                                ZF_IN_OPT ZFObject *timerParam1 /* = zfnull */,
                                ZF_IN_OPT const zftimet &timerDelay /* = 0 */,
                                ZF_IN_OPT zfbool timerActivateInMainThread /* = zffalse */)
{
    this->objectOnInit();
    zfself::timerIntervalSet(timerInterval);
    zfself::timerDelaySet(timerDelay);
    zfself::timerParam0Set(timerParam0);
    zfself::timerParam1Set(timerParam1);
    zfself::timerActivateInMainThreadSet(timerActivateInMainThread);
    return this;
}
ZFObject *ZFTimer::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTimerPrivate);
    d->nativeTimer = ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerCreate(this);
    return this;
}
void ZFTimer::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerDestroy(this, d->nativeTimer);
    d->nativeTimer = zfnull;
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFTimer::objectOnDeallocPrepare(void)
{
    this->timerStop();
    this->timerParam0Set(zfnull);
    this->timerParam1Set(zfnull);
    zfsuper::objectOnDeallocPrepare();
}

void *ZFTimer::nativeTimer(void)
{
    return d->nativeTimer;
}

void ZFTimer::timerIntervalSet(ZF_IN const zftimet &timerInterval)
{
    zfCoreAssert(!this->timerStarted());
    zfCoreAssert(timerInterval > 0);
    d->timerInterval = timerInterval;
}
const zftimet &ZFTimer::timerInterval(void)
{
    return d->timerInterval;
}

void ZFTimer::timerDelaySet(ZF_IN const zftimet &timerDelay)
{
    zfCoreAssert(!this->timerStarted());
    if(timerDelay < 10)
    {
        d->timerDelay = 0;
    }
    else
    {
        d->timerDelay = timerDelay;
    }
}
const zftimet &ZFTimer::timerDelay(void)
{
    return d->timerDelay;
}

void ZFTimer::timerParam0Set(ZF_IN ZFObject *timerParam0)
{
    zfCoreAssert(!this->timerStarted());
    ZFPropertyChange(d->timerParam0, timerParam0);
}
ZFObject *ZFTimer::timerParam0(void)
{
    return d->timerParam0;
}
void ZFTimer::timerParam1Set(ZF_IN ZFObject *timerParam1)
{
    zfCoreAssert(!this->timerStarted());
    ZFPropertyChange(d->timerParam1, timerParam1);
}
ZFObject *ZFTimer::timerParam1(void)
{
    return d->timerParam1;
}
void ZFTimer::timerActivateInMainThreadSet(ZF_IN zfbool timerActivateInMainThread)
{
    zfCoreAssert(!this->timerStarted());
    d->timerActivateInMainThread = timerActivateInMainThread;
}
zfbool ZFTimer::timerActivateInMainThread(void)
{
    return d->timerActivateInMainThread;
}

void ZFTimer::timerStart(void)
{
    if(d->timerStarted)
    {
        return ;
    }
    d->timerStarted = zftrue;

    zfRetain(this);

    d->timerActivatedCount = 0;
    ZFPROTOCOL_ACCESS(ZFTimer)->timerStart(this);
}
void ZFTimer::timerStop(void)
{
    if(d->timerStarted)
    {
        d->timerStarted = zffalse;
        ZFPROTOCOL_ACCESS(ZFTimer)->timerStop(this);
    }
}

zfbool ZFTimer::timerStarted(void)
{
    return d->timerStarted;
}

zfindex ZFTimer::timerActivatedCount(void)
{
    return d->timerActivatedCount;
}

void ZFTimer::_ZFP_ZFTimer_timerOnStart(void)
{
    if(ZFThread::currentThread() == zfnull)
    {
        d->timerThreadToken = ZFThread::nativeThreadRegister();
    }
    this->timerOnStart();
}
void ZFTimer::_ZFP_ZFTimer_timerOnActivate(void)
{
    zfRetain(this);
    {
        ++(d->timerActivatedCount);
        this->timerOnActivate();
    }
    zfRelease(this);
}
void ZFTimer::_ZFP_ZFTimer_timerOnStop(void)
{
    this->timerOnStop();
    if(d->timerThreadToken != zfnull)
    {
        ZFThread::nativeThreadUnregister(d->timerThreadToken);
        d->timerThreadToken = zfnull;
    }
    zfRelease(this);
}

ZF_NAMESPACE_GLOBAL_END


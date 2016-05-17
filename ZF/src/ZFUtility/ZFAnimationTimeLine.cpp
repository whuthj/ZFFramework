/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUtility.hh"
#include "ZFAnimationTimeLine.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAnimationTimeLinePrivate
zfclass _ZFP_ZFAnimationTimeLinePrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFAnimationTimeLinePrivate, ZFObject)

public:
    ZFAnimationTimeLine *owner;
    ZFTimer *timer;
    zftimet timeStart;
    zfbool aniTimeLineOnUpdateObserverHasAdd;

    ZFLISTENER_MEMBER_DECLARE(timerOnEvent)
    {
        zffloat progress = zfmApplyRange(
            (zffloat)(ZFTime::timestamp() - this->timeStart) / this->owner->aniDuration()
            , 0.0f, 1.0f);
        if(this->owner->aniTimeLineCurve() != zfnull)
        {
            progress = this->owner->aniTimeLineCurve()->progressUpdate(progress);
        }
        this->owner->aniTimeLineOnUpdate(progress);
        if(ZFTime::timestamp() - this->timeStart >= this->owner->aniDuration())
        {
            this->timer->timerStop();
            this->owner->aniImplNotifyStop();
        }
    }

public:
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->timer = zfAllocInternal(ZFTimer);
        this->timer->observerAdd(ZFTimer::EventTimerOnActivate(),
            ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(zfself, timerOnEvent)));
        this->aniTimeLineOnUpdateObserverHasAdd = zffalse;
        return this;
    }
    virtual void objectOnDealloc(void)
    {
        zfReleaseInternal(this->timer);
        zfsuper::objectOnDealloc();
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationTimeLine)

// ============================================================
// object
ZFObject *ZFAnimationTimeLine::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAllocInternal(_ZFP_ZFAnimationTimeLinePrivate);
    d->owner = this;
    return this;
}
void ZFAnimationTimeLine::objectOnDealloc(void)
{
    zfReleaseInternal(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfidentity ZFAnimationTimeLine::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , (zfidentity)this->aniTimeLineInterval()
        );
}
ZFCompareResult ZFAnimationTimeLine::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->aniTimeLineInterval() == another->aniTimeLineInterval())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

void ZFAnimationTimeLine::observerOnAddFirst(ZF_IN const zfidentity &eventId)
{
    zfsuper::observerOnAddFirst(eventId);
    if(eventId == ZFAnimationTimeLine::EventAniTimeLineOnUpdate())
    {
        d->aniTimeLineOnUpdateObserverHasAdd = zftrue;
    }
}
void ZFAnimationTimeLine::observerOnRemoveLast(ZF_IN const zfidentity &eventId)
{
    if(eventId == ZFAnimationTimeLine::EventAniTimeLineOnUpdate())
    {
        d->aniTimeLineOnUpdateObserverHasAdd = zffalse;
    }
    zfsuper::observerOnRemoveLast(eventId);
}

// ============================================================
// start stop
void ZFAnimationTimeLine::aniImplStart(void)
{
    zfsuper::aniImplStart();
    d->timeStart = ZFTime::timestamp();
    if(this->aniDuration() < this->aniTimeLineInterval())
    {
        d->timer->timerIntervalSet(this->aniDuration());
    }
    else
    {
        d->timer->timerIntervalSet(this->aniTimeLineInterval());
    }

    d->timer->timerActivateInMainThreadSet(this->aniTimeLineNotifyInMainThread());
    d->timer->timerStart();
}
void ZFAnimationTimeLine::aniImplStop(void)
{
    d->timer->timerStop();
    zfsuper::aniImplStop();
}

void ZFAnimationTimeLine::aniTimeLineOnUpdate(ZF_IN zffloat progress)
{
    if(d->aniTimeLineOnUpdateObserverHasAdd)
    {
        this->observerNotify(
            ZFAnimationTimeLine::EventAniTimeLineOnUpdate(),
            ZFValue::floatValueCreate(progress).toObject());
    }
}

ZF_NAMESPACE_GLOBAL_END


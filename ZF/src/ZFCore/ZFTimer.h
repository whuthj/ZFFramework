/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTimer.h
 * @brief timer utility
 */

#ifndef _ZFI_ZFTimer_h_
#define _ZFI_ZFTimer_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFTimerPrivate;
/**
 * @brief timer utility
 *
 * usage:
 * @code
 *   ZFTimer *timer = zfAlloc(ZFTimer, ...);
 *   timer->start();
 *   zfRelease(timer); // safe to release after start, timer would be auto released after stop
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFTimer : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFTimer, ZFObject)

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer started, ensured in the same thread of timer event
     */
    ZFOBSERVER_EVENT(TimerOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer activated
     * @note on what thread this event is fired,
     *   depends on #timerActivateInMainThread
     */
    ZFOBSERVER_EVENT(TimerOnActivate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when timer stopped, ensured in the same thread that stop the timer
     */
    ZFOBSERVER_EVENT(TimerOnStop)

public:
    /**
     * @brief init with timer settings
     *
     * you may change them after timer created,
     * but you must not if timer is started
     */
    virtual ZFObject *objectOnInit(ZF_IN const zftimet &timerInterval,
                                   ZF_IN_OPT ZFObject *timerParam0 = zfnull,
                                   ZF_IN_OPT ZFObject *timerParam1 = zfnull,
                                   ZF_IN_OPT const zftimet &timerDelay = 0,
                                   ZF_IN_OPT zfbool timerActivateInMainThread = zffalse);

public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    /** @brief for internal use only */
    virtual void *nativeTimer(void);

public:
    /**
     * @brief timer's interval when start a timer in mili seconds, default is 1000
     *
     * assert fail if interval is less than 0
     */
    virtual void timerIntervalSet(ZF_IN const zftimet &timerInterval);
    /**
     * @brief see #timerIntervalSet
     */
    virtual const zftimet &timerInterval(void);

    /**
     * @brief timer's delay when start a timer in mili seconds, default is 0
     *
     * if delay is less than 10, it's treated as 0\n
     * else, first timer event would be fired after (timerDelay + timerInterval)
     */
    virtual void timerDelaySet(ZF_IN const zftimet &timerDelay);
    /**
     * @brief see #timerDelaySet
     */
    virtual const zftimet &timerDelay(void);

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    virtual void timerParam0Set(ZF_IN ZFObject *timerParam0);
    /**
     * @brief see #timerParam0Set
     */
    virtual ZFObject *timerParam0(void);

    /**
     * @brief timer param, automatically retained
     *
     * the param is alive as long as the timer object is alive,
     * it won't be released after timer stop
     */
    virtual void timerParam1Set(ZF_IN ZFObject *timerParam1);
    /**
     * @brief see #timerParam1Set
     */
    virtual ZFObject *timerParam1(void);

    /**
     * @brief whether timer should be fired in main thread, false by default
     *
     * fired in main thread for convenience but may cause timer to be more inaccurate,
     * use only if necessary
     */
    virtual void timerActivateInMainThreadSet(ZF_IN zfbool timerActivateInMainThread);
    /**
     * @brief see #timerActivateInMainThreadSet
     */
    virtual zfbool timerActivateInMainThread(void);

public:
    /**
     * @brief start the timer
     */
    virtual void timerStart(void);
    /**
     * @brief stop the timer
     */
    virtual void timerStop(void);
    /**
     * @brief true if started
     */
    virtual zfbool timerStarted(void);

public:
    /**
     * @brief get timer's current fired count, with 1 as first fired time's value,
     *   keep after timer stop, but reset before timer start
     */
    virtual zfindex timerActivatedCount(void);

public:
    zffinal void _ZFP_ZFTimer_timerOnStart(void);
    zffinal void _ZFP_ZFTimer_timerOnActivate(void);
    zffinal void _ZFP_ZFTimer_timerOnStop(void);
protected:
    /** @brief see #EventTimerOnStart */
    virtual inline void timerOnStart(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnStart());
    }
    /** @brief see #EventTimerOnActivate */
    virtual inline void timerOnActivate(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnActivate());
    }
    /** @brief see #EventTimerOnStop */
    virtual inline void timerOnStop(void)
    {
        this->observerNotify(ZFTimer::EventTimerOnStop());
    }

private:
    _ZFP_ZFTimerPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimer_h_


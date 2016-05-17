/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFThread.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_h_
#define _ZFI_ZFThread_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// task request
/** @brief see #ZFThread::taskRequest */
zfclass ZF_ENV_EXPORT ZFThreadTaskRequestData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFThreadTaskRequestData, ZFObject)

public:
    /** @brief see #ZFThread::taskRequest */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFListener, taskCallback)
    /** @brief see #ZFThread::taskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskParam0)
    /** @brief see #ZFThread::taskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskParam1)
    /** @brief see #ZFThread::taskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskUserData)
    /** @brief see #ZFThread::taskRequest */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFObject *, taskOwner)
};
/** @brief see #ZFThread::taskRequest */
zfclass ZF_ENV_EXPORT ZFThreadTaskRequestMergeCallbackData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFThreadTaskRequestMergeCallbackData, ZFObject)

protected:
    ZFThreadTaskRequestMergeCallbackData(void)
    : taskRequestDataOld(zfnull)
    , taskRequestDataNew(zfnull)
    , taskRequestDataMerged(zfnull)
    {
    }

public:
    /** @brief see #ZFThread::taskRequest */
    ZFThreadTaskRequestData *taskRequestDataOld;
    /** @brief see #ZFThread::taskRequest */
    ZFThreadTaskRequestData *taskRequestDataNew;
    /** @brief see #ZFThread::taskRequest */
    ZFThreadTaskRequestData *taskRequestDataMerged;
};
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask(void);
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask(void);
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge(void);
/**
 * @brief see #ZFThread::taskRequest, simply ignore old task and replace with new task
 */
#define ZFThreadTaskRequestMergeCallbackIgnoreOldTask _ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask()
/**
 * @brief see #ZFThread::taskRequest, simply ignore new task and use the old one
 */
#define ZFThreadTaskRequestMergeCallbackIgnoreNewTask _ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask()
/**
 * @brief see #ZFThread::taskRequest, don't merge and add as new task
 */
#define ZFThreadTaskRequestMergeCallbackDoNotMerge _ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge()
/**
 * @brief see #ZFThread::taskRequest, #ZFThreadTaskRequestMergeCallbackIgnoreOldTask by default
 */
#define ZFThreadTaskRequestMergeCallbackDefault ZFThreadTaskRequestMergeCallbackIgnoreOldTask

// ============================================================
zfclassFwd _ZFP_ZFThreadPrivate;
/**
 * @brief thread utility
 *
 * you can use thread by one of these method:
 * -  inherit from ZFThread and override ZFThread::threadOnRun
 * -  declare a callback and set as thread's run loop by #ZFThread::threadRunnableSet
 *
 * @note try to use executeInNewThread is advised, since it may have thread pool
 */
zfclass ZF_ENV_EXPORT ZFThread : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFThread, ZFObject)

public:
    // ============================================================
    // observer events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0/param1 is the params passed to the thread,
     * called in the same thread of the thread task
     */
    ZFOBSERVER_EVENT(ThreadOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0/param1 is the params passed to the thread,
     * called in the same thread of the thread task
     */
    ZFOBSERVER_EVENT(ThreadOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0/param1 is the params passed to the thread,
     * called in the same thread that cancel the task
     */
    ZFOBSERVER_EVENT(ThreadOnCancel)

    // ============================================================
    // thread control
public:
    /**
     * @brief register a thread which is not started by ZFThread, assert fail if already registered
     *
     * return a token for #nativeThreadUnregister to unregister\n
     * \n
     * you must call nativeThreadUnregister before the thread ends\n
     * \n
     * you must take good care when calling this method\n
     * main thread has no need to register
     */
    static void *nativeThreadRegister(void);
    /**
     * @brief see #nativeThreadRegister
     *
     * it's safe to unregister in different thread using token
     */
    static void nativeThreadUnregister(ZF_IN void *token);

public:
    /**
     * @brief return main thread
     */
    static ZFThread *mainThread(void);
    /**
     * @brief return current thread,
     *   or zfnull if thread isn't started or registered by ZFThread
     *   or not registered by #ZFThread::nativeThreadRegister
     */
    static ZFThread *currentThread(void);

    /**
     * @brief make current thread sleep for miliSecs,
     *   note this method may be not accurate
     */
    static void sleep(ZF_IN const zftimet &miliSecs);

    /**
     * @brief execute in main thread
     *
     * automatically retain runnable's owner and params, and release them after finish\n
     * runnable won't be executed immediately even if current thread is main thread,
     * it's queued instead,
     * unless you call this function in main thread and set waitUntilDone to zftrue
     * @see executeInNewThread, executeCancel
     */
    static zfidentity executeInMainThread(ZF_IN const ZFListener &runnable,
                                          ZF_IN_OPT ZFObject *userData = zfnull,
                                          ZF_IN_OPT ZFObject *param0 = zfnull,
                                          ZF_IN_OPT ZFObject *param1 = zfnull,
                                          ZF_IN_OPT zfbool waitUntilDone = zffalse);
    /**
     * @brief util method for #executeInMainThread
     */
    static inline zfidentity executeInMainThreadWaitUntilDone(ZF_IN const ZFListener &runnable,
                                                              ZF_IN_OPT ZFObject *userData = zfnull,
                                                              ZF_IN_OPT ZFObject *param0 = zfnull,
                                                              ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        return ZFThread::executeInMainThread(runnable, userData, param0, param1, zftrue);
    }

    /**
     * @brief execute in new thread
     *
     * automatically retain runnable's owner and params, and release them after finish
     * @see executeInMainThread, executeCancel
     * @note always try this method first to achieve thread processing,
     *   instead of create new ZFThread instance,
     *   since we may have thread pool for performance
     */
    static zfidentity executeInNewThread(ZF_IN const ZFListener &runnable,
                                         ZF_IN_OPT ZFObject *userData = zfnull,
                                         ZF_IN_OPT ZFObject *param0 = zfnull,
                                         ZF_IN_OPT ZFObject *param1 = zfnull);

    /**
     * @brief exeute in main thread after delay, directly schedule an #executeInMainThread if (delay <= 0)
     */
    static zfidentity executeInMainThreadAfterDelay(ZF_IN zftimet delay,
                                                    ZF_IN const ZFListener &runnable,
                                                    ZF_IN_OPT ZFObject *userData = zfnull,
                                                    ZF_IN_OPT ZFObject *param0 = zfnull,
                                                    ZF_IN_OPT ZFObject *param1 = zfnull);

    /**
     * @brief cancel execute of task
     */
    static void executeCancel(ZF_IN zfidentity taskId);
    /**
     * @brief cancel execute task started by #executeInMainThread or #executeInNewThread
     *
     * cancel only if:
     * -  all the param is the same with the ones in executeInMaintThread or executeInNewThread
     *   (which are all compared by object compare)
     * -  task is queued but not actually start running
     */
    static void executeCancel(ZF_IN const ZFListener &runnable,
                              ZF_IN_OPT ZFObject *userData = zfnull,
                              ZF_IN_OPT ZFObject *param0 = zfnull,
                              ZF_IN_OPT ZFObject *param1 = zfnull);

    /**
     * @brief cancel all execute task of runnable, ignoring the params, see #executeCancel
     */
    static void executeCancelByTarget(ZF_IN const ZFListener &runnable);

    /**
     * @brief wait thread task until done
     *
     * return immediately if the task is running in main thread
     */
    static void executeWait(ZF_IN zfidentity taskId);
    /**
     * @brief wait thread task until done
     *
     * return ture if wait success,
     * or false if wait timeout or no such task\n
     * return false immediately if the task is running in main thread
     */
    static zfbool executeWait(ZF_IN zfidentity taskId,
                            ZF_IN const zftimet &miliSecs);

    /**
     * @brief see #observerNotify
     *
     * do nothing if task not exist, auto remove after task finished
     */
    static void executeObserverAdd(ZF_IN zfidentity taskId,
                                   ZF_IN const zfidentity &eventId,
                                   ZF_IN const ZFListener &callback);
    /**
     * @brief see #executeObserverAdd
     */
    static void executeObserverRemove(ZF_IN zfidentity taskId,
                                      ZF_IN const zfidentity &eventId,
                                      ZF_IN const ZFListener &callback);

public:
    // ============================================================
    // task request
    /**
     * @brief true if implementation for #taskRequest is available
     */
    static zfbool taskRequestImplAvailable(void);
    /**
     * @brief request a task that would be run on main thread after a proper time,
     *   safe to be called in background thread
     *
     * task's param is the params passed to this method,
     * and return value is ignored\n
     * \n
     * tasks are ensured scheduled in different CPU time\n
     * \n
     * if duplicated task is found, the mergeCallback would be called\n
     * two tasks are treated as duplicated task if task callback and owner are both the same,
     * while param0/param1/userData won't be compared
     *
     * \n
     * the mergeCallback's param0 is a #ZFThreadTaskRequestMergeCallbackData
     * contains old and new task data (#ZFThreadTaskRequestData),
     * you should decide merged task and set it to #ZFThreadTaskRequestMergeCallbackData::taskRequestDataMerged,
     * whether retain existing one or allocating new one,
     * or set it to null to show the task can not be merged\n
     * for example:\n
     * to ignore old task, you may retain new one and set it to taskRequestDataMerged\n
     * to ignore new task, you may retain the old one instead\n
     * to merge, allocate a new task data and setup necessary info\n
     * to disable merge, set taskRequestDataMerged to null, and old/new task would be scheduled separately\n
     * or, you may use the pre-defined callbacks such as #ZFThreadTaskRequestMergeCallbackDoNotMerge
     */
    static void taskRequest(ZF_IN ZFThreadTaskRequestData *taskRequestData,
                            ZF_IN_OPT const ZFListener &mergeCallback = ZFThreadTaskRequestMergeCallbackDefault);
    /** @brief see #taskRequest */
    static inline void taskRequest(ZF_IN const ZFListener &taskCallback,
                                   ZF_IN_OPT ZFObject *taskUserData = zfnull,
                                   ZF_IN_OPT ZFObject *taskParam0 = zfnull,
                                   ZF_IN_OPT ZFObject *taskParam1 = zfnull,
                                   ZF_IN_OPT ZFObject *taskOwner = zfnull,
                                   ZF_IN_OPT const ZFListener &taskMergeCallback = ZFThreadTaskRequestMergeCallbackDefault)
    {
        zfblockedAllocInternal(ZFThreadTaskRequestData, taskRequestData);
        taskRequestData->taskCallbackSet(taskCallback);
        taskRequestData->taskUserDataSet(taskUserData);
        taskRequestData->taskParam0Set(taskParam0);
        taskRequestData->taskParam1Set(taskParam1);
        taskRequestData->taskOwnerSet(taskOwner);
        ZFThread::taskRequest(taskRequestData, taskMergeCallback);
    }
    /**
     * @brief see #taskRequest
     */
    static void taskCancelExactly(ZF_IN const ZFListener &task,
                                  ZF_IN_OPT ZFObject *userData = zfnull,
                                  ZF_IN_OPT ZFObject *param0 = zfnull,
                                  ZF_IN_OPT ZFObject *param1 = zfnull);
    /**
     * @brief see #taskRequest
     */
    static void taskCancel(ZF_IN const ZFListener &task);
    /**
     * @brief see #taskRequest
     *
     * owner would be compared by pointer value
     */
    static void taskCancelWithOwner(ZF_IN ZFObject *owner);

    // ============================================================
    // thread instance method
public:
    /**
     * @brief init from listener
     */
    virtual ZFObject *objectOnInit(ZF_IN const ZFListener &runnable);
public:
    zfoverride
    virtual ZFObject *objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief the main run loop
     *
     * default is invalid callback, meaning the builtin #threadOnRun would be run\n
     * changing this callback to a valid runnable would cause the builtin #threadOnRun being replaced\n
     * you must make sure thread isn't running when setting new runnable\n
     * this value take effects only if you start thread by #threadStart
     * @see threadStart, threadOnRun, threadWait
     */
    ZFPROPERTY_ASSIGN_NOT_SERIALIZABLE(ZFListener, threadRunnable)

    /**
     * @brief start thread, do nothing if already started
     */
    virtual void threadStart(ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *param0 = zfnull,
                             ZF_IN_OPT ZFObject *param1 = zfnull);
    /**
     * @brief return true if start is called and hasn't been stopped or end
     */
    virtual zfbool threadStarted(void);
    /**
     * @brief return true only if thread is running
     */
    virtual zfbool threadRunning(void);
    /**
     * @brief request stop the thread, may not be stopped immediately
     *
     * if thread is started but not running, this function will cancel the thread's run\n
     * otherwise, whether the thread is continue running or stopped, is up to the runnable
     * @see threadStopRequested
     */
    virtual void threadStop(void);
    /**
     * @brief return true if stop is called after start
     */
    virtual zfbool threadStopRequested(void);

    /**
     * @brief wait until thread finished running
     *
     * return immediately if thread isn't running
     */
    virtual void threadWait(void);

    /**
     * @brief wait until thread finished running, or timeout
     *
     * return immediately if thread isn't running
     * @return true if wait success or thread isn't running\n
     *         false if wait timeout or error
     */
    virtual zfbool threadWait(ZF_IN const zftimet &miliSecs);

    /**
     * @brief return true if current thread is main thread
     *
     * subclass should not override this method
     */
    virtual zfbool isMainThread(void);
    /**
     * @brief add object to auto release pool attached to this thread
     *
     * usually this method is called by #zfautoRelease
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolAdd(ZF_IN ZFObject *obj,
                                    ZF_IN_OPT zfbool enableLeakTest = zftrue);
    /**
     * @brief add object to auto release pool attached to this thread with location info,
     *   used by ZFLeakTest
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolAdd(ZF_IN ZFObject *obj,
                                    ZF_IN const zfcharA *callerFile,
                                    ZF_IN const zfcharA *callerFunction,
                                    ZF_IN zfindex callerLine,
                                    ZF_IN_OPT zfbool enableLeakTest = zftrue);
    /**
     * @brief manually drain auto release pool
     *
     * must ensure that all objects in pool are safe to release\n
     * called automatically after each time that runnable ends
     * @see ZFAutoReleasePool
     */
    virtual void autoReleasePoolDrain(void);
    zffinal void _ZFP_ZFThreadAutoReleasePoolAdd(ZF_IN ZFObject *obj,
                                                 ZF_IN const zfcharA *callerFile,
                                                 ZF_IN const zfcharA *callerFunction,
                                                 ZF_IN zfindex callerLine,
                                                 ZF_IN zfbool enableLeakTest);
    zffinal void _ZFP_ZFThreadAutoReleasePoolMarkResolved(void);

protected:
    /**
     * @brief internal runnable, do nothing by default, you may override it
     *
     * see #threadRunnable
     */
    ZFMETHOD_MEMBER_DECLARE_DETAIL_2(protected, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst,
                                     void, threadOnRun, ZFMethodNoId,
                                     const ZFListenerData &, listenerData, ZFMethodNoDefaultParam,
                                     ZFObject *, userData, ZFMethodNoDefaultParam);

public:
    zffinal void _ZFP_ZFThread_threadOnStart(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->threadOnStart(param0, param1);
    }
    zffinal void _ZFP_ZFThread_threadOnStop(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->threadOnStop(param0, param1);
    }
    zffinal void _ZFP_ZFThread_threadOnCancel(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->threadOnCancel(param0, param1);
    }
protected:
    /** @brief see #EventThreadOnStart */
    virtual inline void threadOnStart(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->observerNotify(ZFThread::EventThreadOnStart(), param0, param1);
    }
    /** @brief see #EventThreadOnStop */
    virtual inline void threadOnStop(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->observerNotify(ZFThread::EventThreadOnStop(), param0, param1);
    }
    /** @brief see #EventThreadOnCancel */
    virtual inline void threadOnCancel(ZF_IN ZFObject *param0, ZF_IN ZFObject *param1)
    {
        this->observerNotify(ZFThread::EventThreadOnCancel(), param0, param1);
    }

private:
    _ZFP_ZFThreadPrivate *d;
};
zfclassFwd ZFMutex;
extern ZF_ENV_EXPORT ZFMutex *_ZFP_ZFThread_mutex;

// ============================================================
extern ZF_ENV_EXPORT void _ZFP_zfautoReleaseAction(ZF_IN ZFObject *obj,
                                                   ZF_IN const zfcharA *callerFile,
                                                   ZF_IN const zfcharA *callerFunction,
                                                   ZF_IN zfindex callerLine,
                                                   ZF_IN zfbool enableLeakTest);
template<typename T_ZFObject>
inline T_ZFObject *_ZFP_zfautoRelease(ZF_IN T_ZFObject *obj,
                                      ZF_IN const zfcharA *callerFile,
                                      ZF_IN const zfcharA *callerFunction,
                                      ZF_IN zfindex callerLine,
                                      ZF_IN zfbool enableLeakTest)
{
    if(obj)
    {
        _ZFP_zfautoReleaseAction(ZFCastZFObjectUnchecked(ZFObject *, obj), callerFile, callerFunction, callerLine, enableLeakTest);
    }
    return obj;
}
inline const ZFAny &_ZFP_zfautoRelease(ZF_IN const ZFAny &obj,
                                       ZF_IN const zfcharA *callerFile,
                                       ZF_IN const zfcharA *callerFunction,
                                       ZF_IN zfindex callerLine,
                                       ZF_IN zfbool enableLeakTest)
{
    if(obj.toObject())
    {
        _ZFP_zfautoReleaseAction(obj.toObject(), callerFile, callerFunction, callerLine, enableLeakTest);
    }
    return obj;
}
template<typename T_ZFObject>
inline const ZFAnyT<T_ZFObject *> &_ZFP_zfautoRelease(ZF_IN const ZFAnyT<T_ZFObject *> &obj,
                                                      ZF_IN const zfcharA *callerFile,
                                                      ZF_IN const zfcharA *callerFunction,
                                                      ZF_IN zfindex callerLine,
                                                      ZF_IN zfbool enableLeakTest)
{
    ZFObject *t = obj.toObject();
    if(t)
    {
        _ZFP_zfautoReleaseAction(t, callerFile, callerFunction, callerLine, enableLeakTest);
    }
    return obj;
}
/**
 * @brief ensured be logged by ZFLeakTest
 * @see zfautoRelease
 */
#define zfautoReleaseWithLeakTest(obj) _ZFP_zfautoRelease(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, zftrue)
/**
 * @brief ensured not be logged by ZFLeakTest
 * @see zfautoRelease
 */
#define zfautoReleaseWithoutLeakTest(obj) _ZFP_zfautoRelease(obj, ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE, zffalse)
/**
 * @brief make object autorelease, which would be released by the owner thread
 *
 * calling this function will add the object to current thread's autorelease pool\n
 * assert failure if current thread isn't started or registered by ZFThread\n
 * note that which time to release the objects in pool, is depends on implementation
 * @see zfRetain, zfRelease
 *
 * ADVANCED:\n
 * this method depends on ZFThread's implementation\n
 * if no ZFThread's implementation found,
 * we'll try to add to global memory pool (see #ZFAutoReleasePool::instance),
 * and try to schedule a drain task by #ZFThread::taskRequest\n
 * if even taskRequest not be implemented,
 * then the pool won't be drained automatically,
 * you should do it manually if necessary\n
 * \n
 * so, try not to abuse this method, since:
 * -  the actual behavior depends on ZFThread's implementation
 * -  it have lower performance than other release methods
 *   (see #ZFObject for more info about other release methods)
 */
#if ZF_LEAKTEST_ENABLE
    #define zfautoRelease(obj) zfautoReleaseWithLeakTest(obj)
#else
    #define zfautoRelease(obj) zfautoReleaseWithoutLeakTest(obj)
#endif

/**
 * @brief log leak test if ZF_LEAKTEST_ENABLE_INTERNAL,
 *   internal use only
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zfautoReleaseInternal(obj) zfautoReleaseWithLeakTest(obj)
#else
    #define zfautoReleaseInternal(obj) zfautoReleaseWithoutLeakTest(obj)
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_h_


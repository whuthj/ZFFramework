/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore.hh"
#include "ZFThread.h"
#include "protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFArray.h"
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// task processing
ZFLISTENER_FUNCTION_DECLARE(_ZFP_ZFThreadTaskRequestCallback);
ZFLISTENER_FUNCTION_DECLARE(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask);
ZFLISTENER_FUNCTION_DECLARE(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask);
ZFLISTENER_FUNCTION_DECLARE(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge);

static ZFListener *_ZFP_ZFThread_wrappedTaskCallback = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackDoNotMerge = zfnull;
static zfbool _ZFP_ZFThread_taskRunning = zffalse;
static ZFArrayEditable *_ZFP_ZFThread_taskDatas = zfnull;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadTaskRequestDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFThread_wrappedTaskCallback = zfnew(ZFListener);
    *_ZFP_ZFThread_wrappedTaskCallback = ZFCallbackForMethod(ZFMethodAccessFunction(_ZFP_ZFThreadTaskRequestCallback));
    _ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackIgnoreOldTask = ZFCallbackForMethod(ZFMethodAccessFunction(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask));
    _ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackIgnoreNewTask = ZFCallbackForMethod(ZFMethodAccessFunction(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask));
    _ZFP_ZFThread_mergeCallbackDoNotMerge = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackDoNotMerge = ZFCallbackForMethod(ZFMethodAccessFunction(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge));
    _ZFP_ZFThread_taskDatas = zfAllocInternal(ZFArrayEditable);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadTaskRequestDataHolder)
{
    zfdelete(_ZFP_ZFThread_wrappedTaskCallback);
    _ZFP_ZFThread_wrappedTaskCallback = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackIgnoreOldTask);
    _ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackIgnoreNewTask);
    _ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackDoNotMerge);
    _ZFP_ZFThread_mergeCallbackDoNotMerge = zfnull;
    _ZFP_ZFThread_taskRunning = zffalse;
    zfReleaseInternal(_ZFP_ZFThread_taskDatas);
    _ZFP_ZFThread_taskDatas = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadTaskRequestDataHolder)

ZFLISTENER_FUNCTION_DEFINE(_ZFP_ZFThreadTaskRequestCallback)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }
    if(_ZFP_ZFThread_taskDatas != zfnull && !_ZFP_ZFThread_taskDatas->isEmpty())
    {
        // take and remove a task
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->getFirst<ZFThreadTaskRequestData *>();
        zfRetainWithLeakTest(taskData);
        _ZFP_ZFThread_taskDatas->removeFirst();
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }

        // run
        taskData->taskCallback().execute(
            ZFListenerData(zfidentityInvalid, zfnull, taskData->taskParam0(), taskData->taskParam1()),
            taskData->taskUserData());
        zfReleaseWithLeakTest(taskData);

        // schedule next task or quit
        if(lockAvailable)
        {
            zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
        }
        if(!_ZFP_ZFThread_taskDatas->isEmpty())
        {
            ZFPROTOCOL_ACCESS(ZFThreadTaskRequest)->taskRequest(*_ZFP_ZFThread_wrappedTaskCallback, zfnull, zfnull);
        }
        else
        {
            _ZFP_ZFThread_taskRunning = zffalse;
        }
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
    else
    {
        _ZFP_ZFThread_taskRunning = zffalse;
        if(lockAvailable)
        {
            zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
ZFLISTENER_FUNCTION_DEFINE(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask)
{
    ZFThreadTaskRequestMergeCallbackData *mergeCallbackData = listenerData.param0->to<ZFThreadTaskRequestMergeCallbackData *>();
    mergeCallbackData->taskRequestDataMerged = zfRetainWithLeakTest(mergeCallbackData->taskRequestDataNew);
}
ZFLISTENER_FUNCTION_DEFINE(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask)
{
    ZFThreadTaskRequestMergeCallbackData *mergeCallbackData = listenerData.param0->to<ZFThreadTaskRequestMergeCallbackData *>();
    mergeCallbackData->taskRequestDataMerged = zfRetainWithLeakTest(mergeCallbackData->taskRequestDataOld);
}
ZFLISTENER_FUNCTION_DEFINE(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge)
{
    // leave empty merged data, nothing to do
}

const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask(void)
{
    return *_ZFP_ZFThread_mergeCallbackIgnoreOldTask;
}
const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask(void)
{
    return *_ZFP_ZFThread_mergeCallbackIgnoreNewTask;
}
const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge(void)
{
    return *_ZFP_ZFThread_mergeCallbackDoNotMerge;
}

zfbool ZFThread::taskRequestImplAvailable(void)
{
    return ZFPROTOCOL_IS_AVAILABLE(ZFThreadTaskRequest);
}
void ZFThread::taskRequest(ZF_IN ZFThreadTaskRequestData *taskRequestData,
                           ZF_IN_OPT const ZFListener &mergeCallback /* = ZFThreadTaskRequestMergeCallbackDefault */)
{
    if(taskRequestData == zfnull || !taskRequestData->taskCallback().callbackIsValid())
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    zfindex oldTaskIndex = zfindexMax;
    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *existing = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(existing->taskCallback().objectCompare(taskRequestData->taskCallback()) == ZFCompareTheSame
            && existing->taskOwner() == taskRequestData->taskOwner())
        {
            oldTaskIndex = i;
            break;
        }
    }
    if(oldTaskIndex != zfindexMax)
    {
        zfblockedAllocInternal(ZFThreadTaskRequestMergeCallbackData, mergeCallbackData);
        mergeCallbackData->taskRequestDataOld = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(oldTaskIndex);
        mergeCallbackData->taskRequestDataNew = taskRequestData;
        mergeCallback.execute(ZFListenerData(zfidentityInvalid, zfnull, mergeCallbackData));
        if(mergeCallbackData->taskRequestDataMerged != zfnull)
        {
            _ZFP_ZFThread_taskDatas->remove(oldTaskIndex);
            _ZFP_ZFThread_taskDatas->add(mergeCallbackData->taskRequestDataMerged);
            zfReleaseWithLeakTest(mergeCallbackData->taskRequestDataMerged);
            mergeCallbackData->taskRequestDataMerged = zfnull;
        }
        else
        {
            _ZFP_ZFThread_taskDatas->add(taskRequestData);
        }
    }
    else
    {
        _ZFP_ZFThread_taskDatas->add(taskRequestData);
    }

    if(!_ZFP_ZFThread_taskRunning)
    {
        _ZFP_ZFThread_taskRunning = zftrue;
        ZFPROTOCOL_ACCESS(ZFThreadTaskRequest)->taskRequest(*_ZFP_ZFThread_wrappedTaskCallback, zfnull, zfnull);
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThread::taskCancelExactly(ZF_IN const ZFListener &task,
                                 ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                 ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                 ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    if(!task.callbackIsValid())
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskCallback().objectCompare(task) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskUserData(), userData) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskParam0(), param0) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskParam1(), param1) == ZFCompareTheSame)
        {
            _ZFP_ZFThread_taskDatas->remove(i);
            break;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThread::taskCancel(ZF_IN const ZFListener &task)
{
    if(!task.callbackIsValid())
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskCallback().objectCompare(task) == ZFCompareTheSame)
        {
            _ZFP_ZFThread_taskDatas->remove(i);
            --i;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThread::taskCancelWithOwner(ZF_IN ZFObject *owner)
{
    if(owner == zfnull)
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizedObjectLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskOwner() == owner)
        {
            _ZFP_ZFThread_taskDatas->remove(i);
            --i;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizedObjectUnlock(_ZFP_ZFThread_mutex);
    }
}

ZF_NAMESPACE_GLOBAL_END


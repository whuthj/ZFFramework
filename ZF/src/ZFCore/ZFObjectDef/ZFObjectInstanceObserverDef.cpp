/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore_ZFObjectDef.hh"
#include "ZFObjectDef.h"
#include "ZFListenerDeclareDef.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
_ZFP_ZFCORE_PARAM_RETAIN_DEFINE(ZFObjectInstanceObserverParam, ZFObject *, userData, zfnull)

zfclass _ZFP_I_ZFObjectInstanceObserverData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFObjectInstanceObserverData, ZFObject)

public:
    zfidentity taskId;
    const ZFClass *cls;
    ZFCoreArray<ZFObjectInstanceObserverParam> params;

private:
    ZFCoreArrayPOD<ZFObject *> observedObjects;
    ZFListener interceptedAllocListener;
    ZFListener interceptedDeallocListener;

public:
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->interceptedAllocListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(zfself, interceptedAlloc));
        this->interceptedDeallocListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(zfself, interceptedDealloc));
        return this;
    }
    virtual void objectOnDeallocPrepare(void)
    {
        this->cls->instanceObserverRemove(this->interceptedAllocListener);

        for(zfindex iObj = 0; iObj < this->observedObjects.count(); ++iObj)
        {
            ZFObject *obj = this->observedObjects[iObj];
            for(zfindex iObserver = 0; iObserver < this->params.count(); ++iObserver)
            {
                const ZFObjectInstanceObserverParam &param = this->params[iObserver];
                obj->observerRemove(param.eventId(), param.observer());
            }
            obj->observerRemove(ZFObject::EventObjectBeforeDealloc(), this->interceptedDeallocListener);
        }
        for(zfindex i = 0; i < this->params.count(); ++i)
        {
            ZFObject *userData = this->params[i].userData();
            if(userData != zfnull)
            {
                ZFLeakTestLogReleaseVerbose(userData, ZF_CALLER_FILE, zfTextA("ZFObjectInstanceObserverStop"), ZF_CALLER_LINE);
            }
        }

        zfsuper::objectOnDeallocPrepare();
    }

public:
    void taskStart(void)
    {
        this->cls->instanceObserverAdd(this->interceptedAllocListener);
    }

public:
    ZFLISTENER_MEMBER_DECLARE(interceptedAlloc)
    {
        this->observedObjects.add(listenerData.sender);
        listenerData.sender->observerAddWithLevel(ZFLevelAppLow,
            ZFObject::EventObjectBeforeDealloc(),
            this->interceptedDeallocListener);
        for(zfindex i = 0; i < this->params.count(); ++i)
        {
            const ZFObjectInstanceObserverParam &param = this->params[i];
            listenerData.sender->observerAddWithLevel(ZFLevelAppLow,
                param.eventId(),
                param.observer(),
                param.userData());
        }
    }
    ZFLISTENER_MEMBER_DECLARE(interceptedDealloc)
    {
        this->observedObjects.removeElement(listenerData.sender);
    }
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectInstanceObserverDataHolder, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObjectInstanceObserverDataHolder)
{
    if(!this->tasks.isEmpty())
    {
        ZFCoreArrayPOD<_ZFP_I_ZFObjectInstanceObserverData *> tmp = this->tasks;
        this->tasks = ZFCoreArrayPOD<_ZFP_I_ZFObjectInstanceObserverData *>();
        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            zfReleaseInternal(tmp[i]);
        }
    }
}
ZFCoreArrayPOD<_ZFP_I_ZFObjectInstanceObserverData *> tasks;
ZFIdentityGenerator taskIdGenerator;
ZF_GLOBAL_INITIALIZER_END(ZFObjectInstanceObserverDataHolder)
zfidentity ZFObjectInstanceObserverStart(ZF_IN const ZFClass *cls,
                                         ZF_IN const ZFCoreArray<ZFObjectInstanceObserverParam> &params)
{
    if(cls == zfnull || params.isEmpty())
    {
        return zfidentityInvalid;
    }
    ZFCoreMutexLock();
    _ZFP_I_ZFObjectInstanceObserverData *task = zfAllocInternal(_ZFP_I_ZFObjectInstanceObserverData);
    task->taskId = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectInstanceObserverDataHolder)->taskIdGenerator.nextMarkUsed();
    task->cls = cls;
    task->params.addFrom(params);
    for(zfindex i = 0; i < params.count(); ++i)
    {
        zfRetainWithLeakTest(params[i].userData());
    }
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectInstanceObserverDataHolder)->tasks.add(task);

    ZFCoreMutexUnlock();
    task->taskStart();
    return task->taskId;
}
zfidentity ZFObjectInstanceObserverStart(ZF_IN const ZFClass *cls,
                                         ZF_IN const ZFObjectInstanceObserverParam &param)
{
    ZFCoreArray<ZFObjectInstanceObserverParam> params;
    params.add(param);
    return ZFObjectInstanceObserverStart(cls, params);
}
zfidentity ZFObjectInstanceObserverStart(ZF_IN const ZFClass *cls,
                                         ZF_IN zfidentity eventId,
                                         ZF_IN const ZFListener &observer,
                                         ZF_IN_OPT ZFObject *userData /* = zfnull */)
{
    ZFCoreArray<ZFObjectInstanceObserverParam> params;
    params.add(ZFObjectInstanceObserverParam()
        .eventIdSet(eventId)
        .observerSet(observer)
        .userDataSet(userData)
        );
    return ZFObjectInstanceObserverStart(cls, params);
}
static ZFCompareResult _ZFP_ZFObjectInstanceObserverCompareByTaskId(ZF_IN _ZFP_I_ZFObjectInstanceObserverData * const &e0,
                                                                    ZF_IN zfidentity const &e1)
{
    if(e0->taskId == e1)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}
void ZFObjectInstanceObserverStop(ZF_IN zfidentity taskId)
{
    if(taskId == zfidentityInvalid)
    {
        return ;
    }

    ZFCoreMutexLocker();
    ZFCoreArrayPOD<_ZFP_I_ZFObjectInstanceObserverData *> &tasks = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObjectInstanceObserverDataHolder)->tasks;
    zfindex index = tasks.find(taskId, _ZFP_ZFObjectInstanceObserverCompareByTaskId);
    if(index != zfindexMax)
    {
        _ZFP_I_ZFObjectInstanceObserverData *task = tasks[index];
        tasks.remove(index);
        zfReleaseInternal(task);
    }
}

ZF_NAMESPACE_GLOBAL_END


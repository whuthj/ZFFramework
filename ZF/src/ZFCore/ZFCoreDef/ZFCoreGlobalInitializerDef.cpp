/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore_ZFCoreDef.hh"
#include "ZFCoreGlobalInitializerDef.h"
#include "../ZFGlobalEventCenter_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFGlobalInitializerData
{
public:
    zfindex refCount;
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;
    ZFFrameworkState state;
    zfchar *name;
    ZFLevel level;
    _ZFP_ZFGlobalInitializerDummyBase *instance;
    _ZFP_ZFGlobalInitializerConstructor constructor;

public:
    _ZFP_ZFGlobalInitializerData(void)
    : refCount(1)
    , ZFCoreLibDestroyFlag()
    , state(ZFFrameworkStateNotAvailable)
    , name(zfnull)
    , level(ZFLevelAppLow)
    , instance(zfnull)
    , constructor(zfnull)
    {
    }
    ~_ZFP_ZFGlobalInitializerData(void)
    {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i)
        {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
        if(this->instance != zfnull)
        {
            zfdelete(this->instance);
            this->instance = zfnull;
        }
        zffree(this->name);
    }
};
#define _ZFP_ZFGlobalInitializerDataArrayType ZFCoreArrayPOD<_ZFP_ZFGlobalInitializerData *>

static zfstring _ZFP_ZFGlobalInitializerKey(ZF_IN const zfchar *name,
                                            ZF_IN ZFLevel level)
{
    return zfstringWithFormat(zfText("%d_%s"), (zfint)level, name);
}

static void _ZFP_ZFGlobalInitializerInstanceInit(_ZFP_ZFGlobalInitializerDataArrayType &list)
{
    if(!list.isEmpty())
    {
        // array may be changed during init step, copy it first
        _ZFP_ZFGlobalInitializerDataArrayType tmp;
        tmp.copyFrom(list);
        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            _ZFP_ZFGlobalInitializerData *data = tmp.get(i);
            if(data->instance == zfnull)
            {
                data->instance = data->constructor();
            }
        }
    }
}
static void _ZFP_ZFGlobalInitializerInstanceDealloc(_ZFP_ZFGlobalInitializerDataArrayType &list)
{
    for(zfindex i = list.count() - 1; i != zfindexMax; --i)
    {
        _ZFP_ZFGlobalInitializerData *data = list.get(i);
        if(data->instance != zfnull)
        {
            zfdelete(data->instance);
            data->instance = zfnull;
        }
    }
}

zfclassNotPOD _ZFP_ZFGlobalInitializerDataContainer
{
public:
    ZFFrameworkState state;

    ZFFrameworkState stateZFFrameworkEssential;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkEssential;
    ZFCoreMap dataMapLevelZFFrameworkEssential; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateZFFrameworkHigh;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkHigh;
    ZFCoreMap dataMapLevelZFFrameworkHigh; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateZFFrameworkNormal;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkNormal;
    ZFCoreMap dataMapLevelZFFrameworkNormal; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateZFFrameworkLow;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkLow;
    ZFCoreMap dataMapLevelZFFrameworkLow; // _ZFP_ZFGlobalInitializerData *


    ZFFrameworkState stateAppEssential;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelAppEssential;
    ZFCoreMap dataMapLevelAppEssential; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateAppHigh;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelAppHigh;
    ZFCoreMap dataMapLevelAppHigh; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateAppNormal;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelAppNormal;
    ZFCoreMap dataMapLevelAppNormal; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateAppLow;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelAppLow;
    ZFCoreMap dataMapLevelAppLow; // _ZFP_ZFGlobalInitializerData *


    ZFFrameworkState stateZFFrameworkPostLow;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkPostLow;
    ZFCoreMap dataMapLevelZFFrameworkPostLow; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateZFFrameworkPostNormal;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkPostNormal;
    ZFCoreMap dataMapLevelZFFrameworkPostNormal; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateZFFrameworkPostHigh;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkPostHigh;
    ZFCoreMap dataMapLevelZFFrameworkPostHigh; // _ZFP_ZFGlobalInitializerData *

    ZFFrameworkState stateZFFrameworkPostEssential;
    _ZFP_ZFGlobalInitializerDataArrayType dataLevelZFFrameworkPostEssential;
    ZFCoreMap dataMapLevelZFFrameworkPostEssential; // _ZFP_ZFGlobalInitializerData *

public:
    _ZFP_ZFGlobalInitializerDataArrayType &dataListForLevel(ZF_IN ZFLevel level)
    {
        switch(level)
        {
            case ZFLevelZFFrameworkEssential:
                return this->dataLevelZFFrameworkEssential;
            case ZFLevelZFFrameworkHigh:
                return this->dataLevelZFFrameworkHigh;
            case ZFLevelZFFrameworkNormal:
                return this->dataLevelZFFrameworkNormal;
            case ZFLevelZFFrameworkLow:
                return this->dataLevelZFFrameworkLow;

            case ZFLevelAppEssential:
                return this->dataLevelAppEssential;
            case ZFLevelAppHigh:
                return this->dataLevelAppHigh;
            case ZFLevelAppNormal:
                return this->dataLevelAppNormal;
            case ZFLevelAppLow:
                return this->dataLevelAppLow;

            case ZFLevelZFFrameworkPostLow:
                return this->dataLevelZFFrameworkPostLow;
            case ZFLevelZFFrameworkPostNormal:
                return this->dataLevelZFFrameworkPostNormal;
            case ZFLevelZFFrameworkPostHigh:
                return this->dataLevelZFFrameworkPostHigh;
            case ZFLevelZFFrameworkPostEssential:
                return this->dataLevelZFFrameworkPostEssential;

            default:
                zfCoreCriticalShouldNotGoHere();
                return this->dataLevelAppLow;
        }
    }
    ZFCoreMap &dataMapForLevel(ZF_IN ZFLevel level)
    {
        switch(level)
        {
            case ZFLevelZFFrameworkEssential:
                return this->dataMapLevelZFFrameworkEssential;
            case ZFLevelZFFrameworkHigh:
                return this->dataMapLevelZFFrameworkHigh;
            case ZFLevelZFFrameworkNormal:
                return this->dataMapLevelZFFrameworkNormal;
            case ZFLevelZFFrameworkLow:
                return this->dataMapLevelZFFrameworkLow;

            case ZFLevelAppEssential:
                return this->dataMapLevelAppEssential;
            case ZFLevelAppHigh:
                return this->dataMapLevelAppHigh;
            case ZFLevelAppNormal:
                return this->dataMapLevelAppNormal;
            case ZFLevelAppLow:
                return this->dataMapLevelAppLow;

            case ZFLevelZFFrameworkPostLow:
                return this->dataMapLevelZFFrameworkPostLow;
            case ZFLevelZFFrameworkPostNormal:
                return this->dataMapLevelZFFrameworkPostNormal;
            case ZFLevelZFFrameworkPostHigh:
                return this->dataMapLevelZFFrameworkPostHigh;
            case ZFLevelZFFrameworkPostEssential:
                return this->dataMapLevelZFFrameworkPostEssential;

            default:
                zfCoreCriticalShouldNotGoHere();
                return this->dataMapLevelAppLow;
        }
    }

public:
    _ZFP_ZFGlobalInitializerDataContainer(void)
    : state(ZFFrameworkStateNotAvailable)

    , stateZFFrameworkEssential(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkEssential()
    , stateZFFrameworkHigh(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkHigh()
    , stateZFFrameworkNormal(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkNormal()
    , stateZFFrameworkLow(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkLow()

    , stateAppEssential(ZFFrameworkStateNotAvailable)
    , dataLevelAppEssential()
    , stateAppHigh(ZFFrameworkStateNotAvailable)
    , dataLevelAppHigh()
    , stateAppNormal(ZFFrameworkStateNotAvailable)
    , dataLevelAppNormal()
    , stateAppLow(ZFFrameworkStateNotAvailable)
    , dataLevelAppLow()

    , stateZFFrameworkPostLow(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostLow()
    , stateZFFrameworkPostNormal(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostNormal()
    , stateZFFrameworkPostHigh(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostHigh()
    , stateZFFrameworkPostEssential(ZFFrameworkStateNotAvailable)
    , dataLevelZFFrameworkPostEssential()
    {
    }
};
static _ZFP_ZFGlobalInitializerDataContainer &_ZFP_ZFGlobalInitializerDataContainerInstance_(void)
{
    static _ZFP_ZFGlobalInitializerDataContainer _instance;
    return _instance;
}
#define _ZFP_ZFGlobalInitializerDataContainerInstance (_ZFP_ZFGlobalInitializerDataContainerInstance_())

zfclassLikePOD _ZFP_ZFFrameworkAutoCleanupHolder
{
public:
    ~_ZFP_ZFFrameworkAutoCleanupHolder(void)
    {
        ZFFrameworkCleanup();
    }
};
void ZFFrameworkInit(void)
{
    static _ZFP_ZFFrameworkAutoCleanupHolder _holder;

    _ZFP_ZFGlobalInitializerDataContainer &d = _ZFP_ZFGlobalInitializerDataContainerInstance;
    if(d.state == ZFFrameworkStateNotAvailable)
    {
        d.state = ZFFrameworkStateInitProcessing;

        d.stateZFFrameworkEssential = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkEssential);
        d.stateZFFrameworkEssential = ZFFrameworkStateAvailable;

        d.stateZFFrameworkHigh = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkHigh);
        d.stateZFFrameworkHigh = ZFFrameworkStateAvailable;

        d.stateZFFrameworkNormal = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkNormal);
        d.stateZFFrameworkNormal = ZFFrameworkStateAvailable;

        d.stateZFFrameworkLow = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkLow);
        d.stateZFFrameworkLow = ZFFrameworkStateAvailable;


        d.stateAppEssential = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelAppEssential);
        d.stateAppEssential = ZFFrameworkStateAvailable;

        d.stateAppHigh = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelAppHigh);
        d.stateAppHigh = ZFFrameworkStateAvailable;

        d.stateAppNormal = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelAppNormal);
        d.stateAppNormal = ZFFrameworkStateAvailable;

        d.stateAppLow = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelAppLow);
        d.stateAppLow = ZFFrameworkStateAvailable;


        d.stateZFFrameworkPostLow = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkPostLow);
        d.stateZFFrameworkPostLow = ZFFrameworkStateAvailable;

        d.stateZFFrameworkPostNormal = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkPostNormal);
        d.stateZFFrameworkPostNormal = ZFFrameworkStateAvailable;

        d.stateZFFrameworkPostHigh = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkPostHigh);
        d.stateZFFrameworkPostHigh = ZFFrameworkStateAvailable;

        d.stateZFFrameworkPostEssential = ZFFrameworkStateInitProcessing;
        _ZFP_ZFGlobalInitializerInstanceInit(d.dataLevelZFFrameworkPostEssential);
        d.stateZFFrameworkPostEssential = ZFFrameworkStateAvailable;


        d.state = ZFFrameworkStateAvailable;

        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnCreate());
    }
}
void ZFFrameworkCleanup(void)
{
    _ZFP_ZFGlobalInitializerDataContainer &d = _ZFP_ZFGlobalInitializerDataContainerInstance;
    if(d.state == ZFFrameworkStateAvailable)
    {
        d.state = ZFFrameworkStateCleanupProcessing;

        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnDestroy());

        d.stateZFFrameworkPostEssential = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkPostEssential);
        d.stateZFFrameworkPostEssential = ZFFrameworkStateNotAvailable;

        d.stateZFFrameworkPostHigh = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkPostHigh);
        d.stateZFFrameworkPostHigh = ZFFrameworkStateNotAvailable;

        d.stateZFFrameworkPostNormal = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkPostNormal);
        d.stateZFFrameworkPostNormal = ZFFrameworkStateNotAvailable;

        d.stateZFFrameworkPostLow = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkPostLow);
        d.stateZFFrameworkPostLow = ZFFrameworkStateNotAvailable;


        d.stateAppLow = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelAppLow);
        d.stateAppLow = ZFFrameworkStateNotAvailable;

        d.stateAppNormal = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelAppNormal);
        d.stateAppNormal = ZFFrameworkStateNotAvailable;

        d.stateAppHigh = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelAppHigh);
        d.stateAppHigh = ZFFrameworkStateNotAvailable;

        d.stateAppEssential = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelAppEssential);
        d.stateAppEssential = ZFFrameworkStateNotAvailable;


        d.stateZFFrameworkLow = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkLow);
        d.stateZFFrameworkLow = ZFFrameworkStateNotAvailable;

        d.stateZFFrameworkNormal = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkNormal);
        d.stateZFFrameworkNormal = ZFFrameworkStateNotAvailable;

        d.stateZFFrameworkHigh = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkHigh);
        d.stateZFFrameworkHigh = ZFFrameworkStateNotAvailable;

        d.stateZFFrameworkEssential = ZFFrameworkStateCleanupProcessing;
        _ZFP_ZFGlobalInitializerInstanceDealloc(d.dataLevelZFFrameworkEssential);
        d.stateZFFrameworkEssential = ZFFrameworkStateNotAvailable;

        d.state = ZFFrameworkStateNotAvailable;
    }
}

void ZFFrameworkAssertInit(void)
{
    zfCoreAssertWithMessage(ZFFrameworkStateCheck() == ZFFrameworkStateAvailable, zfTextA("ZFFramework hasn't been initialized"));
}
ZFFrameworkState ZFFrameworkStateCheck(void)
{
    return _ZFP_ZFGlobalInitializerDataContainerInstance.state;
}
ZFFrameworkState ZFFrameworkStateCheck(ZF_IN ZFLevel level)
{
    switch(level)
    {
        case ZFLevelZFFrameworkEssential:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkEssential;
        case ZFLevelZFFrameworkHigh:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkHigh;
        case ZFLevelZFFrameworkNormal:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkNormal;
        case ZFLevelZFFrameworkLow:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkLow;

        case ZFLevelAppEssential:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateAppEssential;
        case ZFLevelAppHigh:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateAppHigh;
        case ZFLevelAppNormal:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateAppNormal;
        case ZFLevelAppLow:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateAppLow;

        case ZFLevelZFFrameworkPostLow:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkPostLow;
        case ZFLevelZFFrameworkPostNormal:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkPostNormal;
        case ZFLevelZFFrameworkPostHigh:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkPostHigh;
        case ZFLevelZFFrameworkPostEssential:
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateZFFrameworkPostEssential;

        default:
            zfCoreCriticalShouldNotGoHere();
            return _ZFP_ZFGlobalInitializerDataContainerInstance.stateAppLow;
    }
}

void _ZFP_ZFGlobalInitializerDataRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                          ZF_IN const zfchar *name,
                                          ZF_IN ZFLevel level,
                                          ZF_IN _ZFP_ZFGlobalInitializerConstructor constructor)
{
    _ZFP_ZFGlobalInitializerDataContainer &holder = _ZFP_ZFGlobalInitializerDataContainerInstance;
    _ZFP_ZFGlobalInitializerDataArrayType &dataList = holder.dataListForLevel(level);
    ZFCoreMap &dataMap = holder.dataMapForLevel(level);
    zfstring key = _ZFP_ZFGlobalInitializerKey(name, level);

    _ZFP_ZFGlobalInitializerData *data = dataMap.get<_ZFP_ZFGlobalInitializerData *>(key.cString());
    if(data != zfnull)
    {
        ++(data->refCount);
    }
    else
    {
        data = zfnew(_ZFP_ZFGlobalInitializerData);
        data->name = zfsCopy(name);
        data->level = level;
        data->constructor = constructor;

        dataList.add(data);
        dataMap.set(key.cString(), ZFCorePointerForObject<_ZFP_ZFGlobalInitializerData *>(data));
    }
    data->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);
}
void _ZFP_ZFGlobalInitializerDataUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag,
                                            ZF_IN const zfchar *name,
                                            ZF_IN ZFLevel level)
{
    _ZFP_ZFGlobalInitializerDataContainer &holder = _ZFP_ZFGlobalInitializerDataContainerInstance;
    _ZFP_ZFGlobalInitializerDataArrayType &dataList = holder.dataListForLevel(level);
    ZFCoreMap &dataMap = holder.dataMapForLevel(level);
    zfstring key = _ZFP_ZFGlobalInitializerKey(name, level);

    zfiterator it = dataMap.iteratorForKey(key.cString());
    if(!dataMap.iteratorIsValid(it))
    {
        zfCoreCriticalShouldNotGoHere();
        return ;
    }
    _ZFP_ZFGlobalInitializerData *data = dataMap.iteratorGetValue<_ZFP_ZFGlobalInitializerData *>(it);
    data->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    --(data->refCount);
    if(data->refCount == 0)
    {
        for(zfiterator itList = dataList.iterator(); dataList.iteratorIsValid(itList); dataList.iteratorNext(itList))
        {
            if(dataList.iteratorGet(itList) == data)
            {
                dataList.iteratorRemove(itList);
                break;
            }
        }
        dataMap.iteratorRemove(it);
    }
}

static void _ZFP_ZFGlobalInitializerNotifyInstanceCreated(ZF_IN const _ZFP_ZFGlobalInitializerData *data);
_ZFP_ZFGlobalInitializerDummyBase *&_ZFP_ZFGlobalInitializerInstanceHolderAccess(ZF_IN const zfchar *name,
                                                                                 ZF_IN ZFLevel level)
{
    static _ZFP_ZFGlobalInitializerDummyBase *dummy = zfnull;
    ZFCoreMutexLocker();

    if(ZFFrameworkStateCheck(level) == ZFFrameworkStateCleanupProcessing)
    {
        zfCoreCriticalMessageTrim(
            zfTextA("try to reenter global initializer during ZFFrameworkCleanup, name: %s, level: %s, ")
            zfTextA("typically due to invalid global initializer dependency"),
            zfsCoreZ2A(name), zfsCoreZ2A(ZFLevelToString(level).cString()));
        return dummy;
    }

    _ZFP_ZFGlobalInitializerDataContainer &holder = _ZFP_ZFGlobalInitializerDataContainerInstance;
    ZFCoreMap &dataMap = holder.dataMapForLevel(level);
    zfstring key = _ZFP_ZFGlobalInitializerKey(name, level);

    _ZFP_ZFGlobalInitializerData *data = dataMap.get<_ZFP_ZFGlobalInitializerData *>(key.cString());
    if(data == zfnull)
    {
        zfCoreCriticalShouldNotGoHere();
        return dummy;
    }

    if(data->instance == zfnull)
    {
        data->instance = data->constructor();
        _ZFP_ZFGlobalInitializerNotifyInstanceCreated(data);
    }

    return data->instance;
}

static const _ZFP_ZFGlobalInitializerData *_ZFP_ZFGlobalInitializerCheckDependency(_ZFP_ZFGlobalInitializerDataArrayType &data)
{
    for(zfindex i = 0; i < data.count(); ++i)
    {
        if(data[i]->instance == zfnull)
        {
            return data[i];
        }
    }
    return zfnull;
}
void _ZFP_ZFGlobalInitializerNotifyInstanceCreated(ZF_IN const _ZFP_ZFGlobalInitializerData *data)
{
    _ZFP_ZFGlobalInitializerDataContainer &d = _ZFP_ZFGlobalInitializerDataContainerInstance;
    // check higher level initialized?
    const _ZFP_ZFGlobalInitializerData *dependency = zfnull;
    do
    {
        if(data->level > ZFLevelZFFrameworkEssential)
        {
            if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkEssential)) != zfnull) {break;}
            if(data->level > ZFLevelZFFrameworkHigh)
            {
                if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkHigh)) != zfnull) {break;}
                if(data->level > ZFLevelZFFrameworkNormal)
                {
                    if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkNormal)) != zfnull) {break;}
                    if(data->level > ZFLevelZFFrameworkLow)
                    {
                        if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkLow)) != zfnull) {break;}
                    }
                }
            }
        }
        if(data->level > ZFLevelAppEssential)
        {
            if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelAppEssential)) != zfnull) {break;}
            if(data->level > ZFLevelAppHigh)
            {
                if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelAppHigh)) != zfnull) {break;}
                if(data->level > ZFLevelAppNormal)
                {
                    if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelAppNormal)) != zfnull) {break;}
                    if(data->level > ZFLevelAppLow)
                    {
                        if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelAppLow)) != zfnull) {break;}
                    }
                }
            }
        }
        if(data->level > ZFLevelZFFrameworkPostLow)
        {
            if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkPostLow)) != zfnull) {break;}
            if(data->level > ZFLevelZFFrameworkPostNormal)
            {
                if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkPostNormal)) != zfnull) {break;}
                if(data->level > ZFLevelZFFrameworkPostHigh)
                {
                    if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkPostHigh)) != zfnull) {break;}
                    if(data->level > ZFLevelZFFrameworkPostEssential)
                    {
                        if((dependency = _ZFP_ZFGlobalInitializerCheckDependency(d.dataLevelZFFrameworkPostEssential)) != zfnull) {break;}
                    }
                }
            }
        }
    } while(zffalse);
    if(dependency != zfnull)
    {
        // dependency hasn't initialized
        zfCoreCriticalMessageTrim(zfTextA("ZFGlobalInitializer %s depends on %s, while it hasn't been initialized or already deallocated"),
            zfsCoreZ2A(data->name), zfsCoreZ2A(dependency->name));
        return ;
    }

    // reorder in same level
    _ZFP_ZFGlobalInitializerDataArrayType &dataList = d.dataListForLevel(data->level);
    zfindex prevNull = zfindexMax;
    zfindex self = 0;
    for(zfindex i = 0; i < dataList.count(); ++i)
    {
        if(dataList[i] == data)
        {
            self = i;
        }
        else if(prevNull == zfindexMax && dataList[i]->instance == zfnull)
        {
            prevNull = i;
        }
    }
    if(prevNull < self)
    {
        dataList.add(prevNull, dataList[self]);
        dataList.remove(self + 1);
    }
}

ZF_NAMESPACE_GLOBAL_END


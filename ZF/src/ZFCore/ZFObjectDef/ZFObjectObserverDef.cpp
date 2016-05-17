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
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFListenerData
void ZFListenerData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfstringAppend(ret, zfText("ZFListenerData(%p)"), this);
    const zfchar *eventName = ZFObserverEventGetName(this->eventId);
    if(eventName != zfnull)
    {
        ret += zfText(", event: ");
        ret += eventName;
    }
    if(this->sender != zfnull)
    {
        ret += zfText(", sender: ");
        ZFobjectInfoT(ret, this->sender);
    }
    if(this->param0 != zfnull)
    {
        ret += zfText(", param0: ");
        ZFobjectInfoT(ret, this->param0);
    }
    if(this->param1 != zfnull)
    {
        ret += zfText(", param1: ");
        ZFobjectInfoT(ret, this->param1);
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
// observer
zfclassLikePOD _ZFP_ZFObserverEventData
{
public:
    zfindex refCount;
    zfidentity eventId;
    zfstring eventName;
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;

public:
    _ZFP_ZFObserverEventData(void)
    : refCount(1)
    , eventId(zfidentityInvalid)
    , eventName()
    , ZFCoreLibDestroyFlag()
    {
    }
    ~_ZFP_ZFObserverEventData(void)
    {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i)
        {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
    }
};
typedef zfstlmap<zfidentity, _ZFP_ZFObserverEventData *> _ZFP_ZFObserverEventIdMapType;
typedef zfstlmap<const zfchar *, _ZFP_ZFObserverEventData *, zfcharConst_zfstlComparer> _ZFP_ZFObserverEventNameMapType;
ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(ZFObserverEventDataHolder, ZFLevelZFFrameworkEssential)
{
}
_ZFP_ZFObserverEventIdMapType eventIdMap;
_ZFP_ZFObserverEventNameMapType eventNameMap;
ZFIdentityGenerator eventIdGenerator;
ZF_STATIC_INITIALIZER_END(ZFObserverEventDataHolder)
#define _ZFP_ZFObserver_eventIdMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFObserverEventDataHolder)->eventIdMap)
#define _ZFP_ZFObserver_eventNameMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFObserverEventDataHolder)->eventNameMap)
#define _ZFP_ZFObserver_eventIdGenerator (ZF_STATIC_INITIALIZER_INSTANCE(ZFObserverEventDataHolder)->eventIdGenerator)

const zfidentity *_ZFP_ZFObserverEventIdRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *name)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObserverEventIdMapType &eventIdMap = _ZFP_ZFObserver_eventIdMap;
    _ZFP_ZFObserverEventNameMapType &eventNameMap = _ZFP_ZFObserver_eventNameMap;
    _ZFP_ZFObserverEventData *eventData = zfnull;
    for(_ZFP_ZFObserverEventIdMapType::iterator it = eventIdMap.begin(); it != eventIdMap.end(); ++it)
    {
        if(zfscmpTheSame(it->second->eventName.cString(), name))
        {
            eventData = it->second;
            ++eventData->refCount;
            break;
        }
    }
    if(eventData == zfnull)
    {
        eventData = zfnew(_ZFP_ZFObserverEventData);
        eventData->eventId = _ZFP_ZFObserver_eventIdGenerator.nextMarkUsed();
        eventData->eventName = name;

        eventIdMap[eventData->eventId] = eventData;
        eventNameMap[eventData->eventName.cString()] = eventData;
    }
    eventData->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);

    return &(eventData->eventId);
}
void _ZFP_ZFObserverEventIdUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN zfidentity eventId)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObserverEventIdMapType &eventIdMap = _ZFP_ZFObserver_eventIdMap;
    _ZFP_ZFObserverEventNameMapType &eventNameMap = _ZFP_ZFObserver_eventNameMap;
    _ZFP_ZFObserverEventIdMapType::iterator it = eventIdMap.find(eventId);
    if(it == eventIdMap.end())
    {
        zfCoreCriticalShouldNotGoHere();
        return ;
    }
    _ZFP_ZFObserverEventData *eventData = it->second;
    eventData->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    --(eventData->refCount);
    if(eventData->refCount == 0)
    {
        eventIdMap.erase(eventData->eventId);
        eventNameMap.erase(eventData->eventName.cString());
        zfdelete(eventData);
    }
}
const zfchar *ZFObserverEventGetName(ZF_IN const zfidentity &eventId)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObserverEventIdMapType &m = _ZFP_ZFObserver_eventIdMap;
    _ZFP_ZFObserverEventIdMapType::const_iterator it = m.find(eventId);
    if(it != m.end())
    {
        return it->second->eventName.cString();
    }
    return zfnull;
}
zfidentity ZFObserverEventGetId(ZF_IN const zfchar *name)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObserverEventNameMapType &m = _ZFP_ZFObserver_eventNameMap;
    _ZFP_ZFObserverEventNameMapType::const_iterator it = m.find(name);
    if(it != m.end())
    {
        return it->second->eventId;
    }
    return zfidentityInvalid;
}

ZF_NAMESPACE_GLOBAL_END


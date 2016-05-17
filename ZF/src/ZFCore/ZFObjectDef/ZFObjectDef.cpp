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
#include "ZFObjectSmartPointerDef.h"
#include "ZFSynchronizedDef.h"
#include "ZFClassUtilDef.h"
#include "ZFPropertyDef.h"
#include "ZFObjectUtilTypeDef.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"
#include "../ZFSTLWrapper/zfstl_deque.h"
#include "../ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static ZFObjectMutexImplCheckCallback _ZFP_ZFObjectMutexImplCheckCallbackRef = zfnull;
static ZFObjectMutexImplInitCallback _ZFP_ZFObjectMutexImplInitCallbackRef = zfnull;
static ZFObjectMutexImplCleanupCallback _ZFP_ZFObjectMutexImplCleanupCallbackRef = zfnull;
void ZFObjectMutexImplSet(ZF_IN_OPT ZFObjectMutexImplCheckCallback checkCallback /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplInitCallback initCallback /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCleanupCallback cleanupCallback /* = zfnull */)
{
    _ZFP_ZFObjectMutexImplCheckCallbackRef = checkCallback;
    _ZFP_ZFObjectMutexImplInitCallbackRef = initCallback;
    _ZFP_ZFObjectMutexImplCleanupCallbackRef = cleanupCallback;
}

// ============================================================
// _ZFP_ZFObjectPrivate
zfclassLikePOD _ZFP_ZFObjectObserverListData
{
public:
    zfidentity eventId;
    ZFListener observer;
    ZFObject *userData; // no auto retain, manually retain or release for readable leak test
    ZFObject *owner;
    zfbool autoRemoveAfterActivated;
    ZFLevel observerLevel;

public:
    _ZFP_ZFObjectObserverListData(void)
    : eventId(zfidentityInvalid)
    , observer()
    , userData(zfnull)
    , owner(zfnull)
    , autoRemoveAfterActivated(zffalse)
    , observerLevel(ZFLevelAppNormal)
    {
    }
    _ZFP_ZFObjectObserverListData(ZF_IN zfidentity eventId,
                                  ZF_IN const ZFListener &observer,
                                  ZF_IN_OPT ZFObject *userData = zfnull,
                                  ZF_IN_OPT ZFObject *owner = zfnull,
                                  ZF_IN_OPT zfbool autoRemoveAfterActivated = zffalse,
                                  ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal)
    : eventId(eventId)
    , observer(observer)
    , userData(userData)
    , owner(owner)
    , autoRemoveAfterActivated(autoRemoveAfterActivated)
    , observerLevel(observerLevel)
    {
    }
    _ZFP_ZFObjectObserverListData(ZF_IN const _ZFP_ZFObjectObserverListData &ref)
    : eventId(ref.eventId)
    , observer(ref.observer)
    , userData(ref.userData)
    , owner(ref.owner)
    , autoRemoveAfterActivated(ref.autoRemoveAfterActivated)
    , observerLevel(ref.observerLevel)
    {
    }
    _ZFP_ZFObjectObserverListData &operator = (ZF_IN const _ZFP_ZFObjectObserverListData &ref)
    {
        this->eventId = ref.eventId;
        this->observer = ref.observer;
        this->userData = ref.userData;
        this->owner = ref.owner;
        this->autoRemoveAfterActivated = ref.autoRemoveAfterActivated;
        this->observerLevel = ref.observerLevel;
        return *this;
    }
};
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFObjectTagMapType;
typedef zfstldeque<_ZFP_ZFObjectObserverListData> _ZFP_ZFObjectObserverListType;
typedef zfstlmap<zfidentity, _ZFP_ZFObjectObserverListType> _ZFP_ZFObjectObserverMapType;
typedef enum {
    _ZFP_ZFObject_observerHasAdd_objectAfterAlloc                = (1 << 0),
    _ZFP_ZFObject_observerHasAdd_objectAfterRetain               = (1 << 1),
    _ZFP_ZFObject_observerHasAdd_objectBeforeRelease             = (1 << 2),
    _ZFP_ZFObject_observerHasAdd_objectBeforeDealloc             = (1 << 3),
    _ZFP_ZFObject_observerHasAdd_objectPropertyValueBeforeChange = (1 << 4),
    _ZFP_ZFObject_observerHasAdd_objectPropertyValueAfterChange  = (1 << 5),
} _ZFP_ZFObject_observerHasAddFlagType;
zfclassNotPOD _ZFP_ZFObjectPrivate
{
public:
    zfindex objectRetainCount;
    ZFObject::ObjectInstanceState objectInstanceState;
    ZFObjectHolder *objectHolder;
    zfbool mutexImplAvailable;
    ZFObjectMutexImpl *mutexImpl;
    _ZFP_ZFObjectTagMapType tagMap;
    _ZFP_ZFObjectObserverMapType observerMap;
    zfchar *objectDebugInfo;
    zfbool deallocating;
    zfindex observerNotifyingFlag;
    zfflags observerHasAddFlag;
    zfindex objectCachedFlag;

public:
    _ZFP_ZFObjectObserverListType *observerForId(ZF_IN const zfidentity &eventId)
    {
        _ZFP_ZFObjectObserverMapType::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end())
        {
            return &(it->second);
        }
        else
        {
            return zfnull;
        }
    }

public:
    _ZFP_ZFObjectPrivate(void)
    : objectRetainCount(1)
    , objectInstanceState(ZFObject::ObjectInstanceStateOnInit)
    , objectHolder(zfnull)
    , mutexImplAvailable(_ZFP_ZFObjectMutexImplCheckCallbackRef != zfnull && _ZFP_ZFObjectMutexImplCheckCallbackRef())
    , mutexImpl(zfnull)
    , tagMap()
    , observerMap()
    , objectDebugInfo(zfnull)
    , deallocating(zffalse)
    , observerNotifyingFlag(0)
    , observerHasAddFlag(0)
    , objectCachedFlag(0)
    {
    }
};

// ============================================================
// ZFObject
ZFOBJECT_REGISTER(ZFObject)

zfindex ZFObject::objectRetainCount(void)
{
    return d->objectRetainCount;
}

ZFObjectHolder *ZFObject::objectHolder(void)
{
    if(d->objectHolder == zfnull)
    {
        d->objectHolder = zfAllocInternal(ZFObjectHolder, this);
    }
    return d->objectHolder;
}

void ZFObject::objectInfoOfInstanceT(ZF_IN_OUT zfstring &ret)
{
    ret += this->classData()->className();
    ret += zfText("(");
    zfsFromPointerT(ret, this);
    ret += zfText(")");
}
void ZFObject::objectInfoForDebugT(ZF_IN_OUT zfstring &ret)
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    this->objectInfoOfInstanceT(ret);
    if(this->objectDebugInfo() != zfnull)
    {
        ret += zfText(" (");
        ret += this->objectDebugInfo();
        ret += zfText(")");
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}
void ZFObject::objectInfoT(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOnAppendTokenLeft(ret);
    this->objectInfoOnAppend(ret);
    this->objectInfoOnAppendTokenRight(ret);
}
void ZFObject::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    this->objectInfoOfInstanceT(ret);
}
zfidentity ZFObject::objectHash(void)
{
    return zfidentityCalcPointer(this);
}
ZFCompareResult ZFObject::objectCompare(ZF_IN ZFObject *anotherObj)
{
    return ((this == anotherObj) ? ZFCompareTheSame : ZFCompareUncomparable);
}

zfbool ZFObject::tagHasSet(void)
{
    ZFCoreMutexLocker();
    return !(d->tagMap.empty());
}
void ZFObject::tagSet(ZF_IN const zfchar *key,
                      ZF_IN ZFObject *tag,
                      ZF_IN_OPT zfbool autoMarkCached /* = zffalse */)
{
    ZFCoreMutexLocker();
    if(d->deallocating && tag != zfnull)
    {
        zfCoreCriticalMessage(zfTextA("you must not set tag while object is deallocating"));
        return ;
    }
    if(key == zfnull)
    {
        return ;
    }
    _ZFP_ZFObjectTagMapType &m = d->tagMap;
    _ZFP_ZFObjectTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            if(autoMarkCached)
            {
                m[key] = zflockfree_zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                m[key] = zflockfree_zfautoObjectCreateWithLeakTest(tag);
            }
        }
    }
    else
    {
        if(tag == zfnull)
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            if(autoMarkCached)
            {
                it->second = zflockfree_zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                it->second = zflockfree_zfautoObjectCreateWithLeakTest(tag);
            }
        }
    }
}
ZFObject *ZFObject::tagGet(ZF_IN const zfchar *key)
{
    if(key == zfnull)
    {
        return zfnull;
    }
    ZFCoreMutexLocker();
    _ZFP_ZFObjectTagMapType::iterator it = d->tagMap.find(key);
    if(it != d->tagMap.end())
    {
        return it->second.toObject();
    }
    else
    {
        return zfnull;
    }
}
void ZFObject::tagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                 ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectTagMapType &m = d->tagMap;
    for(_ZFP_ZFObjectTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFObject::tagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        ZFCoreMutexLocker();
        _ZFP_ZFObjectTagMapType::iterator it = d->tagMap.find(key);
        if(it != d->tagMap.end())
        {
            zfautoObject ret = zfautoObjectCreateWithLeakTest(it->second.toObject());
            d->tagMap.erase(it);
            return ret;
        }
    }
    return zfautoObjectNull;
}
void ZFObject::tagRemoveAll(void)
{
    ZFCoreMutexLocker();
    if(!d->tagMap.empty())
    {
        _ZFP_ZFObjectTagMapType tmp;
        tmp.swap(d->tagMap);
    }
}

void ZFObject::objectDebugInfoSet(ZF_IN const zfchar *debugInfo)
{
    zfsChange(d->objectDebugInfo, debugInfo);
}
const zfchar *ZFObject::objectDebugInfo(void)
{
    return d->objectDebugInfo;
}

void ZFObject::observerAdd(ZF_IN const zfidentity &eventId,
                           ZF_IN const ZFListener &observer,
                           ZF_IN_OPT ZFObject *userData /* = zfnull */,
                           ZF_IN_OPT ZFObject *owner /* = zfnull */,
                           ZF_IN_OPT zfbool autoRemoveAfterActivate /* = zffalse */,
                           ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */)
{
    if(!zfidentityIsValid(eventId)
        || !observer.callbackIsValid())
    {
        return ;
    }

    if(d->deallocating)
    {
        zfCoreCriticalMessage(zfTextA("you must not add observer while object is deallocating"));
        return ;
    }

    if(userData != zfnull)
    {
        userData->objectCachedSet(zftrue);
    }

    ZFCoreMutexLocker();
    _ZFP_ZFObjectObserverListType *observerList = d->observerForId(eventId);
    if(observerList != zfnull)
    {
        zfstlsize index = observerList->size();
        while(index > 0 && observerList->at(index - 1).observerLevel > observerLevel)
        {
            --index;
        }
        observerList->insert(observerList->begin() + index, _ZFP_ZFObjectObserverListData(
            eventId
            , observer
            , zflockfree_zfRetainWithLeakTest(userData)
            , owner
            , autoRemoveAfterActivate
            , observerLevel
            ));
        this->observerOnAdd(eventId, observer);
    }
    else
    {
        _ZFP_ZFObjectObserverListType observerListTmp;
        observerListTmp.push_back(_ZFP_ZFObjectObserverListData(
            eventId
            , observer
            , zflockfree_zfRetainWithLeakTest(userData)
            , owner
            , autoRemoveAfterActivate
            , observerLevel
            ));
        d->observerMap.insert(zfstlpair<zfidentity, _ZFP_ZFObjectObserverListType>(eventId, observerListTmp));

        this->observerOnAdd(eventId, observer);
        this->observerOnAddFirst(eventId);
    }
}
void ZFObject::observerRemove(ZF_IN const zfidentity &eventId,
                              ZF_IN const ZFListener &callback)
{
    ZFCoreMutexLocker();
    _ZFP_ZFObjectObserverMapType::iterator it = d->observerMap.find(eventId);
    if(it != d->observerMap.end())
    {
        for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
        {
            if(it->second[iObserver].observer.objectCompareByInstance(callback) == ZFCompareTheSame)
            {
                ZFObject *toRemove = it->second[iObserver].userData;
                it->second.erase(it->second.begin() + iObserver);

                zfbool removedLast = zffalse;
                if(it->second.empty())
                {
                    d->observerMap.erase(it);
                    removedLast = zftrue;
                }
                this->observerOnRemove(eventId, callback);
                if(removedLast)
                {
                    this->observerOnRemoveLast(eventId);
                }
                if(toRemove != zfnull)
                {
                    toRemove->objectCachedSet(zffalse);
                    zflockfree_zfReleaseWithLeakTest(toRemove);
                }
                break;
            }
        }
    }
}
void ZFObject::observerRemoveByOwner(ZF_IN ZFObject *owner)
{
    ZFCoreMutexLocker();
    if(owner != zfnull && !d->observerMap.empty())
    {
        zfstldeque<_ZFP_ZFObjectObserverListData> removed;
        zfstldeque<zfidentity> removedLast;
        for(_ZFP_ZFObjectObserverMapType::iterator it = d->observerMap.begin();
            it != d->observerMap.end();)
        {
            for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
            {
                if(it->second[iObserver].owner == owner)
                {
                    removed.push_back(it->second[iObserver]);
                    it->second.erase(it->second.begin() + iObserver);
                    --iObserver;
                }
            }
            if(it->second.empty())
            {
                removedLast.push_back(it->first);
                d->observerMap.erase(it++);
            }
            else
            {
                ++it;
            }
        }
        for(zfstlsize i = 0; i < removed.size(); ++i)
        {
            this->observerOnRemove(removed[i].eventId, removed[i].observer);
        }
        for(zfstlsize i = 0; i < removedLast.size(); ++i)
        {
            this->observerOnRemoveLast(removedLast[i]);
        }
        if(!removed.empty())
        {
            for(zfstlsize i = 0; i < removed.size(); ++i)
            {
                ZFObject *userDataTmp = removed[i].userData;
                if(userDataTmp != zfnull)
                {
                    userDataTmp->objectCachedSet(zffalse);
                    zflockfree_zfReleaseWithLeakTest(userDataTmp);
                }
            }
        }
    }
}
void ZFObject::observerRemoveAll(ZF_IN const zfidentity &eventId)
{
    ZFCoreMutexLocker();
    if(!d->observerMap.empty())
    {
        _ZFP_ZFObjectObserverMapType::iterator it = d->observerMap.find(eventId);
        if(it != d->observerMap.end())
        {
            _ZFP_ZFObjectObserverListType removed = it->second;
            d->observerMap.erase(it);
            for(zfstlsize i = 0; i < removed.size(); ++i)
            {
                this->observerOnRemove(eventId, removed[i].observer);
            }
            this->observerOnRemoveLast(eventId);
            if(!removed.empty())
            {
                for(zfstlsize i = 0; i < removed.size(); ++i)
                {
                    ZFObject *userDataTmp = removed[i].userData;
                    if(userDataTmp != zfnull)
                    {
                        userDataTmp->objectCachedSet(zffalse);
                        zflockfree_zfReleaseWithLeakTest(userDataTmp);
                    }
                }
            }
        }
    }
}
void ZFObject::observerRemoveAll(void)
{
    ZFCoreMutexLocker();
    if(!d->observerMap.empty())
    {
        _ZFP_ZFObjectObserverMapType tmp = d->observerMap;
        d->observerMap.clear();

        for(_ZFP_ZFObjectObserverMapType::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
            {
                this->observerOnRemove(it->first, it->second[iObserver].observer);
            }
            this->observerOnRemoveLast(it->first);
        }

        for(_ZFP_ZFObjectObserverMapType::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
            {
                ZFObject *userDataTmp = it->second[iObserver].userData;
                if(userDataTmp != zfnull)
                {
                    userDataTmp->objectCachedSet(zffalse);
                    zflockfree_zfReleaseWithLeakTest(userDataTmp);
                }
            }
        }
    }
}
zfbool ZFObject::observerHasAdded(void)
{
    ZFCoreMutexLocker();
    return !d->observerMap.empty();
}
zfbool ZFObject::observerHasAdded(ZF_IN const zfidentity &eventId)
{
    ZFCoreMutexLocker();
    return (d->observerForId(eventId) != zfnull);
}
void ZFObject::observerNotifyWithCustomSender(ZF_IN ZFObject *customSender,
                                              ZF_IN const zfidentity &eventId,
                                              ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                              ZF_IN_OPT ZFObject *param1 /* = zfnull */)
{
    if(!zfidentityIsValid(eventId))
    {
        return ;
    }
    ZFCoreMutexLock();
    ++(d->observerNotifyingFlag);
    this->observerOnEvent(eventId, param0, param1);
    if(!d->observerMap.empty())
    {
        _ZFP_ZFObjectObserverMapType::iterator it = d->observerMap.find(eventId);
        if(it != d->observerMap.end())
        {
            _ZFP_ZFObjectObserverListType toNotify;
            ZFCoreArrayPOD<ZFObject *> toRelease;
            for(zfstlsize i = 0; i < it->second.size(); ++i)
            {
                _ZFP_ZFObjectObserverListData &observerData = it->second[i];
                toNotify.push_back(observerData);
                if(observerData.autoRemoveAfterActivated)
                {
                    if(observerData.userData != zfnull)
                    {
                        toRelease.add(observerData.userData);
                    }
                    it->second.erase(it->second.begin() + i);
                    --i;

                    this->observerOnRemove(observerData.eventId, observerData.observer);
                }
            }
            if(it->second.empty())
            {
                d->observerMap.erase(it);
                this->observerOnRemoveLast(eventId);
            }

            if(!toNotify.empty())
            {
                ZFCoreMutexUnlock();
                ZFListenerData listenerData(eventId, customSender, param0, param1);
                for(zfstlsize i = 0; i < toNotify.size(); ++i)
                {
                    const _ZFP_ZFObjectObserverListData &observerData = toNotify[i];
                    observerData.observer.execute(listenerData, observerData.userData);
                }
                ZFCoreMutexLock();

                for(zfindex i = 0; i < toRelease.count(); ++i)
                {
                    ZFObject *userDataTmp = toRelease[i];
                    userDataTmp->objectCachedSet(zffalse);
                    zflockfree_zfReleaseWithLeakTest(userDataTmp);
                }
            }
        }
    }
    --(d->observerNotifyingFlag);
    ZFCoreMutexUnlock();
}
void ZFObject::observerInfo(ZF_OUT zfstring &ret)
{
    ZFCoreMutexLocker();
    zfbool hasPrev = zffalse;
    for(_ZFP_ZFObjectObserverMapType::iterator it = d->observerMap.begin();
        it != d->observerMap.end();
        ++it)
    {
        if(hasPrev)
        {
            ret += zfText("\n");
        }
        hasPrev = zftrue;

        ret += zfText("  ");
        ret += ZFObserverEventGetName(it->first);
        ret += zfText(":");

        for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
        {
            ret += zfText("\n    ");
            ret += it->second[iObserver].observer.objectInfo();
        }
    }
}

void ZFObject::observerOnAddFirst(ZF_IN const zfidentity &eventId)
{
    if(eventId == ZFObject::EventObjectPropertyValueAfterChange())
    {
        ZFBitSet(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectPropertyValueAfterChange);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueBeforeChange())
    {
        ZFBitSet(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectPropertyValueBeforeChange);
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc())
    {
        ZFBitSet(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectAfterRetain())
    {
        ZFBitSet(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectAfterRetain);
    }
    else if(eventId == ZFObject::EventObjectBeforeRelease())
    {
        ZFBitSet(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectBeforeRelease);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc())
    {
        ZFBitSet(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectBeforeDealloc);
    }
}
void ZFObject::observerOnRemoveLast(ZF_IN const zfidentity &eventId)
{
    if(eventId == ZFObject::EventObjectPropertyValueAfterChange())
    {
        ZFBitUnset(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectPropertyValueAfterChange);
    }
    else if(eventId == ZFObject::EventObjectPropertyValueBeforeChange())
    {
        ZFBitUnset(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectPropertyValueBeforeChange);
    }
    else if(eventId == ZFObject::EventObjectAfterAlloc())
    {
        ZFBitUnset(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectAfterAlloc);
    }
    else if(eventId == ZFObject::EventObjectAfterRetain())
    {
        ZFBitUnset(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectAfterRetain);
    }
    else if(eventId == ZFObject::EventObjectBeforeRelease())
    {
        ZFBitUnset(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectBeforeRelease);
    }
    else if(eventId == ZFObject::EventObjectBeforeDealloc())
    {
        ZFBitUnset(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectBeforeDealloc);
    }
}

zfbool ZFObject::_ZFP_ZFObjectLockIsAvailable(void)
{
    return d->mutexImplAvailable;
}
void ZFObject::_ZFP_ZFObjectLock(void)
{
    if(d->mutexImpl)
    {
        d->mutexImpl->mutexImplLock();
    }
    else if(d->mutexImplAvailable)
    {
        d->mutexImpl = _ZFP_ZFObjectMutexImplInitCallbackRef();
        d->mutexImpl->mutexImplLock();
    }
}
void ZFObject::_ZFP_ZFObjectUnlock(void)
{
    if(d->mutexImpl)
    {
        d->mutexImpl->mutexImplUnlock();
    }
    // else should not go here
}
zfbool ZFObject::_ZFP_ZFObjectTryLock(void)
{
    if(d->mutexImpl)
    {
        return d->mutexImpl->mutexImplTryLock();
    }
    else if(d->mutexImplAvailable)
    {
        d->mutexImpl = _ZFP_ZFObjectMutexImplInitCallbackRef();
        return d->mutexImpl->mutexImplTryLock();
    }
    else
    {
        return zffalse;
    }
}

ZFObject *ZFObject::_ZFP_ZFObjectAlloc(ZFObject *obj)
{
    obj->classData()->_ZFP_ZFClass_internalTypesRegister();
    return obj;
}
ZFObject *ZFObject::_ZFP_ZFObjectCheckOnInit(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnInit() not called"));
        return zfnull;
    }
    this->classData()->_ZFP_ZFClass_propertyInitStepCheckAutoInit(this);
    this->_ZFP_ZFObject_objectInstanceStateSet(ZFObject::ObjectInstanceStateOnInitFinish);
    this->_ZFP_ZFObject_interfaceOnInit();
    this->objectOnInitFinish();
    this->_ZFP_ZFObject_objectInstanceStateSet(ZFObject::ObjectInstanceStateIdle);

    this->classData()->_ZFP_ZFClass_instanceObserverNotify(this);
    if(ZFBitTest(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectAfterAlloc))
    {
        this->observerNotify(ZFObject::EventObjectAfterAlloc());
    }

    return this;
}
void ZFObject::_ZFP_ZFObjectDealloc(ZFObject *obj)
{
    if(ZFBitTest(obj->d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectBeforeDealloc))
    {
        obj->observerNotify(ZFObject::EventObjectBeforeDealloc());
    }
    if(obj->d->observerNotifyingFlag > 0)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] deallocating an object during observerNotify, you may want to use zfautoRelease?"));
        return ;
    }
    obj->d->deallocating = zftrue;
    obj->_ZFP_ZFObject_objectInstanceStateSet(ZFObject::ObjectInstanceStateOnDeallocPrepare);
    obj->objectOnDeallocPrepare();
    obj->_ZFP_ZFObject_interfaceOnDealloc();
    obj->_ZFP_ZFObject_objectInstanceStateSet(ZFObject::ObjectInstanceStateOnDealloc);
    zflockfree_ZFLeakTestLogDealloc(obj);
    obj->objectOnDealloc();
    if(obj->d != zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnDealloc() not called"));
        return ;
    }
    obj->classData()->_ZFP_ZFClass_objectDesctuct(obj);
}

ZFObject *ZFObject::objectOnInit(void)
{
    if(d != zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnInit() called twice"));
        return zfnull;
    }

    // note that (d != zfnull) is also used to check whether ZFObject::objectOnInit() is called
    d = zfpoolNew(_ZFP_ZFObjectPrivate);

    return this;
}
void ZFObject::objectOnDealloc(void)
{
    if(d == zfnull)
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] ZFObject::objectOnDealloc() called twice"));
        return ;
    }

    if(this->objectCached())
    {
        zfCoreLogTrim(zfText("[ZFObject] you should reset objectCached before destroy the object: %s, current cached count: %zi"),
            this->objectInfoOfInstance().cString(), this->objectCachedCount());
    }

    this->observerRemoveAll();

    zfsChange(d->objectDebugInfo, (const zfchar *)zfnull);

    if(d->mutexImpl)
    {
        _ZFP_ZFObjectMutexImplCleanupCallbackRef(d->mutexImpl);
        d->mutexImpl = zfnull;
    }

    if(d->objectHolder)
    {
        d->objectHolder->holdedObj = (ZFObject *)zfnull;
        zfReleaseInternal(d->objectHolder);
    }

    zfpoolDelete(d);
    // note that (d == zfnull) is also used to check whether ZFObject::objectOnDealloc() is called
    d = zfnull;
}
void ZFObject::objectOnInitFinish(void)
{
}
void ZFObject::objectOnDeallocPrepare(void)
{
    this->tagRemoveAll();
    this->observerRemoveAll();
}
void ZFObject::objectOnRetain(void)
{
    zfCoreAssertWithMessageTrim(d->objectRetainCount > 0,
        zfTextA("retain an object while deallocating: %s"), zfsCoreZ2A(this->objectInfoOfInstance().cString()));
    ++(d->objectRetainCount);
    if(ZFBitTest(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectAfterRetain))
    {
        this->observerNotify(ZFObject::EventObjectAfterRetain());
    }
}
void ZFObject::objectOnRelease(void)
{
    if(ZFBitTest(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectBeforeRelease))
    {
        this->observerNotify(ZFObject::EventObjectBeforeRelease());
    }
    --(d->objectRetainCount);
}

ZFObject::ObjectInstanceState ZFObject::objectInstanceState(void)
{
    return d->objectInstanceState;
}
void ZFObject::_ZFP_ZFObject_objectInstanceStateSet(ZF_IN ZFObject::ObjectInstanceState objectInstanceState)
{
    d->objectInstanceState = objectInstanceState;
}

void ZFObject::_ZFP_ZFObject_objectPropertyValueCheckMarkCache(ZF_IN const ZFProperty *property,
                                                               ZF_IN zfbool objectCached)
{
    if(this->objectCached() && property->propertyIsRetainProperty())
    {
        ZFObject *value = property->callbackRetainGet(property, this);
        if(value != zfnull)
        {
            value->objectCachedSet(zftrue);
        }
    }
}
void ZFObject::_ZFP_ZFObject_objectPropertyValueBeforeChange(ZF_IN const ZFProperty *property)
{
    if(this->objectCached() && property->propertyIsRetainProperty())
    {
        ZFObject *value = property->callbackRetainGet(property, this);
        if(value != zfnull)
        {
            value->objectCachedSet(zffalse);
        }
    }
    this->objectPropertyValueBeforeChange(property);
}
void ZFObject::_ZFP_ZFObject_objectPropertyValueAfterChange(ZF_IN const ZFProperty *property)
{
    if(this->objectCached() && property->propertyIsRetainProperty())
    {
        ZFObject *value = property->callbackRetainGet(property, this);
        if(value != zfnull)
        {
            value->objectCachedSet(zftrue);
        }
    }
    this->objectPropertyValueAfterChange(property);
}

void ZFObject::objectPropertyValueBeforeChange(ZF_IN const ZFProperty *property)
{
    if(ZFBitTest(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectPropertyValueBeforeChange))
    {
        ZFPointerHolder *holder = ZFPointerHolder::cacheAccess();
        holder->holdedData = property;
        this->observerNotify(ZFObject::EventObjectPropertyValueBeforeChange(), holder);
        ZFPointerHolder::cacheRelease(holder);
    }
}
void ZFObject::objectPropertyValueAfterChange(ZF_IN const ZFProperty *property)
{
    if(ZFBitTest(d->observerHasAddFlag, _ZFP_ZFObject_observerHasAdd_objectPropertyValueAfterChange))
    {
        ZFPointerHolder *holder = ZFPointerHolder::cacheAccess();
        holder->holdedData = property;
        this->observerNotify(ZFObject::EventObjectPropertyValueAfterChange(), holder);
        ZFPointerHolder::cacheRelease(holder);
    }
}

zfclass _ZFP_ZFObject_objectAllRetainPropertyCache : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFObject_objectAllRetainPropertyCache, ZFObject)

public:
    ZFCoreArrayPOD<const ZFProperty *> allProperty;
};
ZFOBJECT_REGISTER(_ZFP_ZFObject_objectAllRetainPropertyCache)
static void _ZFP_ZFObject_objectCachedMarkAllProperty(ZF_IN ZFObject *ownerObject, ZF_IN zfbool objectCached)
{
    _ZFP_ZFObject_objectAllRetainPropertyCache *holder = ownerObject->classData()->classTagGet<_ZFP_ZFObject_objectAllRetainPropertyCache *>(zfText("_ZFP_ZFObject_objectAllRetainPropertyCache"));
    if(holder == zfnull)
    {
        holder = zflockfree_zfAllocWithoutLeakTest(_ZFP_ZFObject_objectAllRetainPropertyCache);
        ownerObject->classData()->classTagSet(zfText("_ZFP_ZFObject_objectAllRetainPropertyCache"), holder);
        holder->allProperty = ZFClassUtil::allProperty(ownerObject->classData());
        for(zfindex i = 0; i < holder->allProperty.count(); ++i)
        {
            if(!holder->allProperty[i]->propertyIsRetainProperty())
            {
                holder->allProperty.remove(i);
                --i;
            }
        }
        zflockfree_zfReleaseWithoutLeakTest(holder);
    }
    ZFCoreArrayPOD<const ZFProperty *> &allProperty = holder->allProperty;
    for(zfindex i = allProperty.count() - 1; i != zfindexMax; --i)
    {
        const ZFProperty *property = allProperty[i];
        if(property->callbackIsValueAccessed(property, ownerObject))
        {
            ZFObject *value = property->callbackRetainGet(property, ownerObject);
            if(value != zfnull)
            {
                value->objectCachedSet(objectCached);
            }
        }
    }
}
void ZFObject::objectCachedSet(ZF_IN zfbool objectCached)
{
    if(objectCached)
    {
        ++(d->objectCachedFlag);
        _ZFP_ZFLeakTestLogCacheAdd(this);

        if(d->objectCachedFlag == 1)
        {
            _ZFP_ZFObject_objectCachedMarkAllProperty(this, zftrue);
            this->objectCachedOnChange();
        }
    }
    else
    {
        zfCoreAssert(d->objectCachedFlag > 0);
        _ZFP_ZFLeakTestLogCacheRemove(this);
        --(d->objectCachedFlag);

        if(d->objectCachedFlag == 0)
        {
            _ZFP_ZFObject_objectCachedMarkAllProperty(this, zffalse);
            this->objectCachedOnChange();
        }
    }
}
zfbool ZFObject::objectCached(void)
{
    return (d->objectCachedFlag > 0);
}
zfindex ZFObject::objectCachedCount(void)
{
    return d->objectCachedFlag;
}
void ZFObject::objectCachedOnChange(void)
{
    this->observerNotify(ZFObject::EventObjectCachedOnChange());
}

ZF_NAMESPACE_GLOBAL_END


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUtility.hh"
#include "ZFAnimationGroup.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFAnimationGroupChildData
ZFOBJECT_REGISTER(ZFAnimationGroupChildData)

zfidentity ZFAnimationGroupChildData::objectHash(void)
{
    return zfidentityHash(ZFObjectHash(this->childAni())
        , (zfidentity)this->childDelayBeforeStart()
        , (zfidentity)this->childAutoCopyTarget()
    );
}
ZFCompareResult ZFAnimationGroupChildData::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(ZFObjectCompare(this->childAni(), another->childAni()) == ZFCompareTheSame
        && this->childDelayBeforeStart() == another->childDelayBeforeStart()
        && this->childAutoCopyTarget() == another->childAutoCopyTarget())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
// _ZFP_ZFAnimationGroupPrivate
zfclass _ZFP_ZFAnimationGroupPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFAnimationGroupPrivate, ZFObject)

public:
    ZFAnimationGroup *owner;
    ZFArrayEditable *childAnis; // ZFAnimationGroupChildData *
private:
    /*
     * ZFAnimationGroupChildData *
     * for parallel group, stores all running child
     * for serial group, stores running child and all child to run
     */
    ZFArrayEditable *childBuf;
    zfbool cachedParallel;
    ZFListener cachedOnStartListener;
    ZFListener cachedOnStopListener;
    ZFCoreArrayPOD<zfidentity> childDelayTaskIds;

public:
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->childAnis = zfAllocInternal(ZFArrayEditable);
        this->childBuf = zfAllocInternal(ZFArrayEditable);
        return this;
    }
    virtual void objectOnDealloc(void)
    {
        zfReleaseInternal(this->childAnis);
        zfReleaseInternal(this->childBuf);
        zfsuper::objectOnDealloc();
    }

public:
    void doStart(void)
    {
        if(this->childAnis->isEmpty())
        {
            this->owner->aniImplNotifyStop();
            return ;
        }
        if(this->cachedParallel != this->owner->aniParallel() || !this->cachedOnStartListener.callbackIsValid())
        {
            this->cachedParallel = this->owner->aniParallel();
            if(this->cachedParallel)
            {
                this->cachedOnStartListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(_ZFP_ZFAnimationGroupPrivate, onChildStart_parallel));
                this->cachedOnStopListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(_ZFP_ZFAnimationGroupPrivate, onChildStop_parallel));
            }
            else
            {
                this->cachedOnStartListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(_ZFP_ZFAnimationGroupPrivate, onChildStart_serial));
                this->cachedOnStopListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(_ZFP_ZFAnimationGroupPrivate, onChildStop_serial));
            }
        }

        if(this->cachedParallel)
        {
            this->doStartParallel();
        }
        else
        {
            this->doStartSerial();
        }
    }
    void doStop(void)
    {
        if(!this->childDelayTaskIds.isEmpty())
        {
            for(zfindex i = 0; i < this->childDelayTaskIds.count(); ++i)
            {
                ZFThread::executeCancel(this->childDelayTaskIds[i]);
            }
            this->childDelayTaskIds.removeAll();
        }
        if(!this->childBuf->isEmpty())
        {
            zfblockedAllocInternal(ZFArrayEditable, childToStop, this->childBuf);
            this->childBuf->removeAll();
            if(this->cachedParallel)
            {
                for(zfindex i = 0; i < childToStop->count(); ++i)
                {
                    ZFAnimationGroupChildData *childData = childToStop->get<ZFAnimationGroupChildData *>(i);
                    this->childCleanup(childData);
                    childData->childAni()->aniStop();
                }
            }
            else
            {
                for(zfindex i = 0; i < childToStop->count(); ++i)
                {
                    ZFAnimationGroupChildData *childData = childToStop->get<ZFAnimationGroupChildData *>(i);
                    this->childCleanup(childData);
                }
                childToStop->getFirst<ZFAnimationGroupChildData *>()->childAni()->aniStop();
            }
        }
    }

private:
    void doStartParallel(void)
    {
        zfidentity aniId = this->owner->aniId();
        zfblockedAllocInternal(ZFArrayEditable, tmpArray, this->childAnis);
        this->childBuf->addFrom(this->childAnis);
        for(zfindex i = 0; aniId == this->owner->aniId() && i < tmpArray->count(); ++i)
        {
            ZFAnimationGroupChildData *childData = tmpArray->get<ZFAnimationGroupChildData *>(i);
            this->childSetup(childData);
            this->childStart(childData, aniId);
        }
    }
    void doStartSerial(void)
    {
        this->childBuf->addFrom(this->childAnis);
        ZFAnimationGroupChildData *childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
        this->childSetup(childData);
        this->childStart(childData, this->owner->aniId());
    }

private:
    void childSetup(ZF_IN ZFAnimationGroupChildData *childData)
    {
        childData->childAni()->observerAdd(ZFAnimation::EventAniOnStart(), this->cachedOnStartListener);
        childData->childAni()->observerAdd(ZFAnimation::EventAniOnStop(), this->cachedOnStopListener);
    }
    void childCleanup(ZF_IN ZFAnimationGroupChildData *childData)
    {
        childData->childAni()->observerRemove(ZFAnimation::EventAniOnStart(), this->cachedOnStartListener);
        childData->childAni()->observerRemove(ZFAnimation::EventAniOnStop(), this->cachedOnStopListener);
    }
private:
    void childStart(ZF_IN ZFAnimationGroupChildData *childData, ZF_IN zfidentity aniId)
    {
        if(childData->childDelayBeforeStart() <= 0)
        {
            childData->childAni()->aniStart();
        }
        else
        {
            zfidentity childDelayTaskId = ZFThread::executeInMainThreadAfterDelay(
                childData->childDelayBeforeStart(),
                ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(_ZFP_ZFAnimationGroupPrivate, onChildStartDelay)),
                ZFValue::identityValueCreate(aniId).toObject(),
                childData);
            if(childDelayTaskId != zfidentityInvalid)
            {
                this->childDelayTaskIds.add(childDelayTaskId);
            }
        }
    }
    ZFLISTENER_MEMBER_DECLARE(onChildStartDelay)
    {
        ZFAnimationGroupChildData *childData = ZFCastZFObjectUnchecked(ZFAnimationGroupChildData *, listenerData.param0);
        zfidentity aniId = ZFCastZFObjectUnchecked(ZFValue *, userData)->identityValueAccess();
        if(aniId == this->owner->aniId())
        {
            childData->childAni()->aniStart();
        }
    }
private:
    ZFAnimationGroupChildData *checkChild(ZF_IN ZFObject *childAni, ZF_IN zfbool autoRemove = zffalse)
    {
        for(zfindex i = 0; i < this->childBuf->count(); ++i)
        {
            ZFAnimationGroupChildData *childData = this->childBuf->get<ZFAnimationGroupChildData *>(i);
            if(childData->childAni() == childAni)
            {
                if(autoRemove)
                { // retained in childAnis, safe to be removed directly
                    this->childBuf->remove(i);
                }
                return childData;
            }
        }
        return zfnull;
    }
private:
    ZFLISTENER_MEMBER_DECLARE(onChildStart_parallel)
    {
        ZFAnimationGroupChildData *childData = this->checkChild(listenerData.sender);
        if(childData == zfnull)
        {
            return ;
        }
        this->owner->aniGroupOnChildStart(childData->childAni());
    }
    ZFLISTENER_MEMBER_DECLARE(onChildStop_parallel)
    {
        ZFAnimationGroupChildData *childData = this->checkChild(listenerData.sender, zfHint("autoRemove")zftrue);
        if(childData == zfnull)
        {
            return ;
        }
        this->childCleanup(childData);
        this->owner->aniGroupOnChildStop(childData->childAni());

        if(this->childBuf->isEmpty())
        {
            this->owner->aniImplNotifyStop();
        }
    }
    ZFLISTENER_MEMBER_DECLARE(onChildStart_serial)
    {
        ZFAnimationGroupChildData *childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
        if(childData == zfnull || childData->childAni() != listenerData.sender)
        {
            return ;
        }
        this->owner->aniGroupOnChildStart(childData->childAni());
    }
    ZFLISTENER_MEMBER_DECLARE(onChildStop_serial)
    {
        ZFAnimationGroupChildData *childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
        if(childData == zfnull || childData->childAni() != listenerData.sender)
        {
            return ;
        }
        this->childBuf->removeFirst();
        this->childCleanup(childData);
        this->owner->aniGroupOnChildStop(childData->childAni());

        if(this->childBuf->isEmpty())
        {
            this->owner->aniImplNotifyStop();
        }
        else
        {
            childData = this->childBuf->getFirst<ZFAnimationGroupChildData *>();
            this->childSetup(childData);
            this->childStart(childData, this->owner->aniId());
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationGroup)

// ============================================================
// serialize
zfbool ZFAnimationGroup::serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                                 ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                                 ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryFromData(ownerData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    this->childAniRemoveAll();

    for(zfindex i = 0; i < ownerData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = ownerData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFAnimationGroup_child))
        {
            zfautoObject element;
            if(!ZFObjectFromSerializableData(element, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("null child"));
                return zffalse;
            }
            if(!element.toObject()->classData()->classIsSubclassOf(ZFAnimationGroupChildData::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("object %s not type of %s"), element.toObject()->objectInfoOfInstance().cString(), ZFAnimationGroupChildData::ClassData());
                return zffalse;
            }
            ZFAnimationGroupChildData *childData = ZFCastZFObjectUnchecked(ZFAnimationGroupChildData *, element.toObject());
            if(childData->childAni() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("null child animation"));
                return zffalse;
            }
            this->childAniAdd(childData);

            categoryData.resolveMark();
        }
    }
    return zftrue;
}
zfbool ZFAnimationGroup::serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                               ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryToData(ownerData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfindex i = 0; i < d->childAnis->count(); ++i)
        {
            ZFSerializableData elementData;
            if(!ZFObjectToSerializableData(elementData, d->childAnis->get(i), outErrorHintToAppend))
            {
                return zffalse;
            }
            elementData.categorySet(ZFSerializableKeyword_ZFAnimationGroup_child);
            ownerData.elementAdd(elementData);
        }
    }
    else
    {
        zfbool mismatch = zffalse;
        if(this->childAniCount() != ref->childAniCount())
        {
            mismatch = zftrue;
        }
        else
        {
            for(zfindex i = 0; i < d->childAnis->count(); ++i)
            {
                if(ZFObjectCompare(d->childAnis->get(i), ref->d->childAnis->get(i)) != ZFCompareTheSame)
                {
                    mismatch = zftrue;
                    break;
                }
            }
        }
        if(mismatch)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                zfText("animation group contents mismatch, this: %s, ref: %s"),
                d->childAnis->objectInfoOfContent().cString(), ref->d->childAnis->objectInfoOfContent().cString());
            return zffalse;
        }
    }

    return zftrue;
}

// ============================================================
// object
ZFObject *ZFAnimationGroup::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAllocInternal(_ZFP_ZFAnimationGroupPrivate);
    d->owner = this;
    return this;
}
void ZFAnimationGroup::objectOnDealloc(void)
{
    zfReleaseInternal(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAnimationGroup::objectOnDeallocPrepare(void)
{
    this->aniStop();
    this->childAniRemoveAll();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFAnimationGroup::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , d->childAnis->objectHash()
        , (zfidentity)this->aniParallel()
    );
}
ZFCompareResult ZFAnimationGroup::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(ZFObjectCompare(d->childAnis, another->d->childAnis) == ZFCompareTheSame
       && this->aniParallel() == another->aniParallel())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
// child animation control
void ZFAnimationGroup::childAniAdd(ZF_IN ZFAnimation *ani)
{
    zfblockedAllocInternal(ZFAnimationGroupChildData, childData);
    childData->childAniSet(ani);
    this->childAniAdd(childData);
}
void ZFAnimationGroup::childAniAdd(ZF_IN ZFAnimation *ani,
                                   ZF_IN zfbool childAutoCopyTarget)
{
    zfblockedAllocInternal(ZFAnimationGroupChildData, childData);
    childData->childAniSet(ani);
    childData->childAutoCopyTargetSet(childAutoCopyTarget);
    this->childAniAdd(childData);
}
void ZFAnimationGroup::childAniAdd(ZF_IN ZFAnimationGroupChildData *childData)
{
    zfCoreAssert(childData != zfnull);
    zfCoreAssert(childData->childAni() != zfnull);
    zfCoreAssertWithMessage(!this->aniRunning(), zfTextA("you must not modify child animation while group is running"));
    d->childAnis->add(childData);
}
zfindex ZFAnimationGroup::childAniCount(void)
{
    return d->childAnis->count();
}
ZFAnimation *ZFAnimationGroup::childAniGet(ZF_IN zfindex index)
{
    return d->childAnis->get<ZFAnimationGroupChildData *>(index)->childAni();
}
ZFAnimationGroupChildData *ZFAnimationGroup::childAniDataGet(ZF_IN zfindex index)
{
    return d->childAnis->get<ZFAnimationGroupChildData *>(index);
}
void ZFAnimationGroup::childAniRemove(ZF_IN zfindex index)
{
    d->childAnis->remove(index);
}
void ZFAnimationGroup::childAniRemoveAll(void)
{
    d->childAnis->removeAll();
}

// ============================================================
// start stop
zfbool ZFAnimationGroup::aniImplCheckValid(void)
{
    if(!zfsuper::aniImplCheckValid())
    {
        return zffalse;
    }
    for(zfindex i = 0; i < d->childAnis->count(); ++i)
    {
        ZFAnimationGroupChildData *childData = d->childAnis->get<ZFAnimationGroupChildData *>(i);
        if(childData->childAutoCopyTarget())
        {
            childData->childAni()->aniTargetSet(this->aniTarget());
        }
        if(!childData->childAni()->aniValid())
        {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFAnimationGroup::aniImplStart(void)
{
    zfsuper::aniImplStart();
    d->doStart();
}
void ZFAnimationGroup::aniImplStop(void)
{
    d->doStop();
    zfsuper::aniImplStop();
}

ZF_NAMESPACE_GLOBAL_END


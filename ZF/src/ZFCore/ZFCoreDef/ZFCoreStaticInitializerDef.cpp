/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreStaticInitializerDef.h"
#include "ZFCoreArrayDef.h"
#include "ZFCoreMutexDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassLikePOD _ZFP_ZFStaticInitializerItemData
{
public:
    zfstring name;
    _ZFP_ZFStaticInitializerDummyBase *instance;
};
#define _ZFP_ZFStaticInitializerDataArrayType ZFCoreArray<_ZFP_ZFStaticInitializerItemData>
static void _ZFP_ZFStaticInitializerInstanceDealloc(_ZFP_ZFStaticInitializerDataArrayType &list)
{
    if(!list.isEmpty())
    {
        for(zfindex i = list.count() - 1; i != zfindexMax; --i)
        {
            zfdelete(list.get(i).instance);
        }
        list.removeAll();
    }
}

zfclassNotPOD _ZFP_ZFStaticInitializerData
{
public:
    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkEssential;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkHigh;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkNormal;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkLow;

    _ZFP_ZFStaticInitializerDataArrayType dataLevelAppEssential;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelAppHigh;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelAppNormal;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelAppLow;

    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkPostLow;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkPostNormal;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkPostHigh;
    _ZFP_ZFStaticInitializerDataArrayType dataLevelZFFrameworkPostEssential;

public:
    ~_ZFP_ZFStaticInitializerData(void)
    {
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkPostEssential);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkPostHigh);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkPostNormal);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkPostLow);

        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelAppLow);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelAppNormal);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelAppHigh);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelAppEssential);

        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkLow);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkNormal);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkHigh);
        _ZFP_ZFStaticInitializerInstanceDealloc(this->dataLevelZFFrameworkEssential);
    }
public:
    _ZFP_ZFStaticInitializerDummyBase *access(ZF_IN_OUT _ZFP_ZFStaticInitializerDataArrayType &levelData,
                                              const zfchar *name,
                                              _ZFP_ZFStaticInitializerConstructor constructor)
    {
        for(zfindex i = 0; i < levelData.count(); ++i)
        {
            if(zfscmpTheSame(levelData.get(i).name.cString(), name))
            {
                return levelData.get(i).instance;
            }
        }
        _ZFP_ZFStaticInitializerItemData itemData;
        itemData.name = name;
        itemData.instance = constructor();
        levelData.add(itemData);
        return itemData.instance;
    }
};
static _ZFP_ZFStaticInitializerData &_ZFP_ZFStaticInitializerDataRef_(void)
{
    static _ZFP_ZFStaticInitializerData d;
    return d;
}
#define _ZFP_ZFStaticInitializerDataRef _ZFP_ZFStaticInitializerDataRef_()

_ZFP_ZFStaticInitializerDummyBase *_ZFP_ZFStaticInitializerAccessInstance(const zfchar *name,
                                                                          ZFLevel level,
                                                                          _ZFP_ZFStaticInitializerConstructor constructor)
{
    ZFCoreMutexLocker();
    _ZFP_ZFStaticInitializerData &instance = _ZFP_ZFStaticInitializerDataRef;
    switch(level)
    {
        case ZFLevelZFFrameworkEssential:
            return instance.access(instance.dataLevelZFFrameworkEssential, name, constructor);
        case ZFLevelZFFrameworkHigh:
            return instance.access(instance.dataLevelZFFrameworkHigh, name, constructor);
        case ZFLevelZFFrameworkNormal:
            return instance.access(instance.dataLevelZFFrameworkNormal, name, constructor);
        case ZFLevelZFFrameworkLow:
            return instance.access(instance.dataLevelZFFrameworkLow, name, constructor);

        case ZFLevelAppEssential:
            return instance.access(instance.dataLevelAppEssential, name, constructor);
        case ZFLevelAppHigh:
            return instance.access(instance.dataLevelAppHigh, name, constructor);
        case ZFLevelAppNormal:
            return instance.access(instance.dataLevelAppNormal, name, constructor);
        case ZFLevelAppLow:
            return instance.access(instance.dataLevelAppLow, name, constructor);

        case ZFLevelZFFrameworkPostLow:
            return instance.access(instance.dataLevelZFFrameworkPostLow, name, constructor);
        case ZFLevelZFFrameworkPostNormal:
            return instance.access(instance.dataLevelZFFrameworkPostNormal, name, constructor);
        case ZFLevelZFFrameworkPostHigh:
            return instance.access(instance.dataLevelZFFrameworkPostHigh, name, constructor);
        case ZFLevelZFFrameworkPostEssential:
            return instance.access(instance.dataLevelZFFrameworkPostEssential, name, constructor);

        default:
            zfCoreCriticalShouldNotGoHere();
            return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END


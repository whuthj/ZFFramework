/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore.hh"
#include "ZFHashMap.h"
#include "ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFHashMapPrivate
zfclassNotPOD _ZFP_ZFHashMapKeyHasher
{
public:
    zfstlsize operator()(ZFObject *const &v) const
    {
        return (zfstlsize)v->objectHash();
    }
};
zfclassNotPOD _ZFP_ZFHashMapKeyComparer
{
public:
    zfbool operator()(ZFObject * const &v0, ZFObject * const &v1) const
    {
        return (v0->objectCompare(v1) == ZFCompareTheSame);
    }
};

zfclassNotPOD _ZFP_ZFHashMapPrivate
{
public:
    typedef zfstlhashmap<ZFObject *, ZFObject *, _ZFP_ZFHashMapKeyHasher, _ZFP_ZFHashMapKeyComparer> MapType;

public:
    MapType data;

public:
    _ZFP_ZFHashMapPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFHashMap
ZFOBJECT_REGISTER(ZFHashMap)

zfbool ZFHashMap::serializableOnSerializeCategoryToDataWithRef(ZF_IN_OUT ZFSerializableData &ownerData,
                                                               ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    ZFKeyValueContainer *ref = ZFCastZFObject(ZFKeyValueContainer *, referencedOwnerOrNull);
    if(ref == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("%s not type of %s"),
            referencedOwnerOrNull->toObject()->objectInfoOfInstance().cString(), ZFKeyValueContainer::ClassData()->className());
        return zffalse;
    }

    zfblockedAllocInternal(zfself, tmp, ref);
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        ZFKeyValuePair pair = this->iteratorNextPair(it);
        zfiterator itTmp = tmp->iteratorFind(pair.key);
        if(ref->iteratorIsValid(itTmp))
        {
            if(ZFObjectCompare(pair.value, tmp->iteratorGetValue(itTmp)))
            {
                tmp->iteratorRemove(itTmp);
                continue;
            }
            else
            {
                tmp->iteratorRemove(itTmp);
            }
        }

        ZFSerializableData keyData;
        if(!ZFObjectToSerializableData(keyData, pair.key, outErrorHintToAppend))
        {
            return zffalse;
        }
        ZFSerializableData valueData;
        if(!ZFObjectToSerializableData(valueData, pair.value, outErrorHintToAppend))
        {
            return zffalse;
        }

        keyData.categorySet(ZFSerializableKeyword_ZFKeyValueContainer_key);
        ownerData.elementAdd(keyData);
        valueData.categorySet(ZFSerializableKeyword_ZFKeyValueContainer_value);
        ownerData.elementAdd(valueData);
    }

    if(!tmp->isEmpty())
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("missing elements from referenced container: %s"), tmp->objectInfoOfContent().cString());
        return zffalse;
    }

    return zftrue;
}

ZFObject *ZFHashMap::objectOnInit(ZF_IN ZFKeyValueContainer *another)
{
    this->objectOnInit();
    if(another != zfnull)
    {
        zfself::addFrom(another);
    }
    return this;
}
ZFObject *ZFHashMap::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFHashMapPrivate);
    return this;
}
void ZFHashMap::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfindex ZFHashMap::count(void)
{
    return d->data.size();
}

zfbool ZFHashMap::isEmpty(void)
{
    return d->data.empty();
}

zfbool ZFHashMap::isContain(ZF_IN ZFObject *pKey)
{
    return (pKey != zfnull && d->data.find(pKey) != d->data.end());
}

ZFObject *ZFHashMap::get(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            return it->second;
        }
    }
    return zfnull;
}
ZFKeyValuePair ZFHashMap::getPair(ZF_IN ZFObject *pKey)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ret.key = it->first;
            ret.value = it->second;
        }
    }
    return ret;
}

ZFCoreArrayPOD<ZFObject *> ZFHashMap::allKey(void)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    if(!this->isEmpty())
    {
        ret.capacitySet(this->count());
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->first);
        }
    }
    return ret;
}
ZFCoreArrayPOD<ZFObject *> ZFHashMap::allValue(void)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    if(!this->isEmpty())
    {
        ret.capacitySet(this->count());
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->second);
        }
    }
    return ret;
}
ZFCoreArrayPOD<ZFKeyValuePair> ZFHashMap::allPair(void)
{
    ZFCoreArrayPOD<ZFKeyValuePair> ret;
    if(!this->isEmpty())
    {
        ret.capacitySet(this->count());
        ZFKeyValuePair pair;
        for(_ZFP_ZFHashMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            pair.key = it->first;
            pair.value = it->second;
            ret.add(pair);
        }
    }
    return ret;
}

void ZFHashMap::addFrom(ZF_IN ZFKeyValueContainer *another)
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    ZFKeyValuePair pair = ZFKeyValuePairZero;
    for(zfiterator it = another->iterator(); another->iteratorIsValid(it); )
    {
        pair = another->iteratorNextPair(it);

        _ZFP_ZFHashMapPrivate::MapType::iterator itExisting = d->data.find(pair.key);
        if(itExisting != d->data.end())
        {
            this->contentOnRemove(itExisting->first, itExisting->second);
            ZFObject *tmp = itExisting->second;
            zfRetainWithLeakTest(pair.value);
            itExisting->second = pair.value;
            this->contentOnAdd(pair.key, pair.value);
            zfReleaseWithLeakTest(tmp);
        }
        else
        {
            zfRetainWithLeakTest(pair.key);
            zfRetainWithLeakTest(pair.value);
            (d->data)[pair.key] = pair.value;
            this->contentOnAdd(pair.key, pair.value);
        }
    }

    if(pair.key != zfnull)
    {
        this->contentOnChange();
    }
}

void ZFHashMap::set(ZF_IN ZFObject *pKey,
                    ZF_IN ZFObject *pValue)
{
    if(pKey == zfnull)
    {
        return ;
    }
    if(pValue == zfnull)
    {
        this->remove(pKey);
        return ;
    }

    _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
    if(it != d->data.end())
    {
        this->contentOnRemove(it->first, it->second);
        ZFObject *tmp = it->second;
        zfRetainWithLeakTest(pValue);
        it->second = pValue;
        this->contentOnAdd(it->first, it->second);
        zfReleaseWithLeakTest(tmp);
    }
    else
    {
        zfRetainWithLeakTest(pKey);
        zfRetainWithLeakTest(pValue);
        (d->data)[pKey] = pValue;
        this->contentOnAdd(it->first, it->second);
    }

    this->contentOnChange();
}

void ZFHashMap::remove(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);
            this->contentOnRemove(tmpKey, tmpValue);
            zfReleaseWithLeakTest(tmpKey);
            zfReleaseWithLeakTest(tmpValue);

            this->contentOnChange();
        }
    }
}
zfautoObject ZFHashMap::removeAndGet(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);

            this->contentOnRemove(tmpKey, tmpValue);
            this->contentOnChange();

            zfReleaseWithLeakTest(tmpKey);
            zfblockedReleaseWithLeakTest(tmpValue);
            return zfautoObjectCreateWithLeakTest(tmpValue);
        }
    }
    return zfautoObjectNull;
}
ZFKeyValuePairAutoRelease ZFHashMap::removeAndGetPair(ZF_IN ZFObject *pKey)
{
    ZFKeyValuePairAutoRelease ret;
    if(pKey != zfnull)
    {
        _ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            zfblockedReleaseWithLeakTest(it->first);
            zfblockedReleaseWithLeakTest(it->second);
            ret.key = zfautoObjectCreateWithLeakTest(it->first);
            ret.value = zfautoObjectCreateWithLeakTest(it->second);
            d->data.erase(it);

            this->contentOnRemove(ret.key.toObject(), ret.value.toObject());
            this->contentOnChange();

            return ret;
        }
    }
    return ret;
}
void ZFHashMap::removeAll(void)
{
    if(!d->data.empty())
    {
        ZFObject **tmp = (ZFObject **)zfmalloc(sizeof(ZFObject *) * d->data.size() * 2);
        zfindex count = 0;
        for(_ZFP_ZFHashMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            tmp[count++] = it->first;
            tmp[count++] = it->second;
        }
        d->data.clear();

        for(zfindex i = 0; i < count; i += 2)
        {
            this->contentOnRemove(tmp[i], tmp[i + 1]);
            zfReleaseWithLeakTest(tmp[i]);
            zfReleaseWithLeakTest(tmp[i + 1]);
        }
        zffree(tmp);

        this->contentOnChange();
    }
}

// ============================================================
static void _ZFP_ZFHashMap_iteratorDeleteCallback(ZF_IN void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFHashMapPrivate::MapType::iterator *, data));
}
static void *_ZFP_ZFHashMap_iteratorCopyCallback(ZF_IN void *data)
{
    return zfnew(_ZFP_ZFHashMapPrivate::MapType::iterator,
        *ZFCastStatic(_ZFP_ZFHashMapPrivate::MapType::iterator *, data));
}
zfiterator ZFHashMap::iterator(void)
{
    return zfiterator(zfnew(_ZFP_ZFHashMapPrivate::MapType::iterator, d->data.begin()),
        _ZFP_ZFHashMap_iteratorDeleteCallback,
        _ZFP_ZFHashMap_iteratorCopyCallback);
}

zfbool ZFHashMap::iteratorIsValid(ZF_IN const zfiterator &it)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    return (data != zfnull && *data != d->data.end());
}
zfbool ZFHashMap::iteratorIsEqual(ZF_IN const zfiterator &it0,
                                  ZF_IN const zfiterator &it1)
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFHashMapPrivate::MapType::iterator *>(it0, it1);
}

void ZFHashMap::iteratorSet(ZF_IN_OUT zfiterator &it,
                            ZF_IN ZFObject *value)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        this->contentOnRemove((*data)->first, (*data)->second);
        ZFObject *tmp = (*data)->second;
        zfRetainWithLeakTest(value);
        (*data)->second = value;
        this->contentOnAdd((*data)->first, value);
        this->contentOnChange();
        zfReleaseWithLeakTest(tmp);
    }
}
void ZFHashMap::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *tmpKey = (*data)->first;
        ZFObject *tmpValue = (*data)->second;
        d->data.erase((*data)++);

        this->contentOnRemove(tmpKey, tmpValue);
        this->contentOnChange();
        zfReleaseWithLeakTest(tmpKey);
        zfReleaseWithLeakTest(tmpValue);
    }
}
zfiterator ZFHashMap::iteratorForKey(ZF_IN ZFObject *key)
{
    return zfiterator(zfnew(_ZFP_ZFHashMapPrivate::MapType::iterator, d->data.find(key)),
        _ZFP_ZFHashMap_iteratorDeleteCallback,
        _ZFP_ZFHashMap_iteratorCopyCallback);
}
ZFObject *ZFHashMap::iteratorGetKey(ZF_IN const zfiterator &it)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->first : zfnull);
}
ZFObject *ZFHashMap::iteratorGetValue(ZF_IN const zfiterator &it)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->second : zfnull);
}
ZFKeyValuePair ZFHashMap::iteratorGetPair(ZF_IN const zfiterator &it)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
    }
    return ret;
}
ZFObject *ZFHashMap::iteratorNextKey(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->first;
        data->operator ++();
        return ret;
    }
    return zfnull;
}
ZFObject *ZFHashMap::iteratorNextValue(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->second;
        data->operator ++();
        return ret;
    }
    return zfnull;
}
ZFKeyValuePair ZFHashMap::iteratorNextPair(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    _ZFP_ZFHashMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFHashMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator ++();
        return ret;
    }
    return ret;
}
ZFObject *ZFHashMap::iteratorPrevKey(ZF_IN_OUT zfiterator &it)
{
    zfCoreCriticalNotSupported();
    return zfnull;
}
ZFObject *ZFHashMap::iteratorPrevValue(ZF_IN_OUT zfiterator &it)
{
    zfCoreCriticalNotSupported();
    return zfnull;
}
ZFKeyValuePair ZFHashMap::iteratorPrevPair(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    zfCoreCriticalNotSupported();
    return ret;
}
void ZFHashMap::iteratorAddKeyValue(ZF_IN ZFObject *key,
                                    ZF_IN ZFObject *value,
                                    ZF_IN_OPT const zfiterator &it /* = zfiteratorInvalid */)
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END


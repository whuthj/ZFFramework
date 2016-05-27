/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMap.h"
#include "ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFMapPrivate
zfclassNotPOD _ZFP_ZFMapKeyComparer
{
public:
    inline zfbool operator()(ZFObject *obj1,
                             ZFObject *obj2) const
    {
        ZFCompareResult cmp = ZFObjectCompare(obj1, obj2);
        zfCoreAssertWithMessageTrim(cmp != ZFCompareUncomparable, zfTextA("[ZFMap] key must comparable: %s, %s"),
            zfsCoreZ2A(ZFObjectInfo(obj1).cString()),
            zfsCoreZ2A(ZFObjectInfo(obj2).cString()));
        return (cmp == ZFCompareSmaller);
    }
};
zfclassNotPOD _ZFP_ZFMapPrivate
{
public:
    typedef zfstlmap<ZFObject *, ZFObject *, _ZFP_ZFMapKeyComparer> MapType;

public:
    MapType data;

public:
    _ZFP_ZFMapPrivate(void)
    : data()
    {
    }
};

// ============================================================
// ZFMap
ZFOBJECT_REGISTER(ZFMap)

zfbool ZFMap::serializableOnSerializeCategoryToDataWithRef(ZF_IN_OUT ZFSerializableData &ownerData,
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

    zfblockedAllocWithoutLeakTest(zfself, tmp, ref);
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

ZFObject *ZFMap::objectOnInit(ZF_IN ZFKeyValueContainer *another)
{
    this->objectOnInit();
    if(another != zfnull)
    {
        zfself::addFrom(another);
    }
    return this;
}
ZFObject *ZFMap::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFMapPrivate);
    return this;
}
void ZFMap::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFCompareResult ZFMap::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
       || this->objectHash() != another->objectHash())
    {
        return ZFCompareUncomparable;
    }

    // ZFMap is sorted, compare by order for performance
    for(zfiterator it0 = this->iterator(),
        it1 = another->iterator();
        this->iteratorIsValid(it0);)
    {
        ZFKeyValuePair pair0 = this->iteratorNextPair(it0);
        ZFKeyValuePair pair1 = another->iteratorNextPair(it1);
        if(ZFObjectCompare(pair0.key, pair1.key) != ZFCompareTheSame
            || ZFObjectCompare(pair0.value, pair1.value) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

zfindex ZFMap::count(void)
{
    return d->data.size();
}

zfbool ZFMap::isEmpty(void)
{
    return d->data.empty();
}

zfbool ZFMap::isContain(ZF_IN ZFObject *pKey)
{
    return (pKey != zfnull && d->data.find(pKey) != d->data.end());
}

ZFObject *ZFMap::get(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            return it->second;
        }
    }
    return zfnull;
}
ZFKeyValuePair ZFMap::getPair(ZF_IN ZFObject *pKey)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ret.key = it->first;
            ret.value = it->second;
        }
    }
    return ret;
}

ZFCoreArrayPOD<ZFObject *> ZFMap::allKey(void)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    if(!this->isEmpty())
    {
        ret.capacitySet(this->count());
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->first);
        }
    }
    return ret;
}
ZFCoreArrayPOD<ZFObject *> ZFMap::allValue(void)
{
    ZFCoreArrayPOD<ZFObject *> ret;
    if(!this->isEmpty())
    {
        ret.capacitySet(this->count());
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            ret.add(it->second);
        }
    }
    return ret;
}
ZFCoreArrayPOD<ZFKeyValuePair> ZFMap::allPair(void)
{
    ZFCoreArrayPOD<ZFKeyValuePair> ret;
    if(!this->isEmpty())
    {
        ret.capacitySet(this->count());
        ZFKeyValuePair pair;
        for(_ZFP_ZFMapPrivate::MapType::const_iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            pair.key = it->first;
            pair.value = it->second;
            ret.add(pair);
        }
    }
    return ret;
}

void ZFMap::addFrom(ZF_IN ZFKeyValueContainer *another)
{
    if(another == this || another == zfnull)
    {
        return ;
    }

    ZFKeyValuePair pair = ZFKeyValuePairZero;
    for(zfiterator it = another->iterator(); another->iteratorIsValid(it); )
    {
        pair = another->iteratorNextPair(it);

        _ZFP_ZFMapPrivate::MapType::iterator itExisting = d->data.find(pair.key);
        if(itExisting != d->data.end())
        {
            this->contentOnRemove(itExisting->first, itExisting->second);
            ZFObject *tmp = itExisting->second;
            zfRetain(pair.value);
            itExisting->second = pair.value;
            this->contentOnAdd(itExisting->first, itExisting->second);
            zfRelease(tmp);
        }
        else
        {
            zfRetain(pair.key);
            zfRetain(pair.value);
            (d->data)[pair.key] = pair.value;
        }
    }

    if(pair.key != zfnull)
    {
        this->contentOnChange();
    }
}

void ZFMap::set(ZF_IN ZFObject *pKey,
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

    _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
    if(it != d->data.end())
    {
        this->contentOnRemove(it->first, it->second);
        ZFObject *tmp = it->second;
        zfRetain(pValue);
        it->second = pValue;
        this->contentOnAdd(it->first, it->second);
        zfRelease(tmp);
    }
    else
    {
        zfRetain(pKey);
        zfRetain(pValue);
        (d->data)[pKey] = pValue;
    }

    this->contentOnChange();
}

void ZFMap::remove(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);
            this->contentOnRemove(tmpKey, tmpValue);
            zfRelease(tmpKey);
            zfRelease(tmpValue);

            this->contentOnChange();
        }
    }
}
zfautoObject ZFMap::removeAndGet(ZF_IN ZFObject *pKey)
{
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            ZFObject *tmpKey = it->first;
            ZFObject *tmpValue = it->second;
            d->data.erase(it);

            this->contentOnRemove(tmpKey, tmpValue);
            this->contentOnChange();

            zfRelease(tmpKey);
            zfblockedRelease(tmpValue);
            return zfautoObjectCreate(tmpValue);
        }
    }
    return zfautoObjectNull;
}
ZFKeyValuePairAutoRelease ZFMap::removeAndGetPair(ZF_IN ZFObject *pKey)
{
    ZFKeyValuePairAutoRelease ret;
    if(pKey != zfnull)
    {
        _ZFP_ZFMapPrivate::MapType::iterator it = d->data.find(pKey);
        if(it != d->data.end())
        {
            zfblockedRelease(it->first);
            zfblockedRelease(it->second);
            ret.key = zfautoObjectCreate(it->first);
            ret.value = zfautoObjectCreate(it->second);
            d->data.erase(it);

            this->contentOnRemove(ret.key.toObject(), ret.value.toObject());
            this->contentOnChange();
            return ret;
        }
    }
    return ret;
}
void ZFMap::removeAll(void)
{
    if(!d->data.empty())
    {
        ZFObject **tmp = (ZFObject **)zfmalloc(sizeof(ZFObject *) * d->data.size() * 2);
        zfindex count = 0;
        for(_ZFP_ZFMapPrivate::MapType::iterator it = d->data.begin(); it != d->data.end(); ++it)
        {
            tmp[count++] = it->first;
            tmp[count++] = it->second;
        }
        d->data.clear();

        for(zfindex i = 0; i < count; i += 2)
        {
            this->contentOnRemove(tmp[i], tmp[i + 1]);
            zfRelease(tmp[i]);
            zfRelease(tmp[i + 1]);
        }
        zffree(tmp);

        this->contentOnChange();
    }
}

// ============================================================
static void _ZFP_ZFMap_iteratorDeleteCallback(ZF_IN void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFMapPrivate::MapType::iterator *, data));
}
static void *_ZFP_ZFMap_iteratorCopyCallback(ZF_IN void *data)
{
    return zfnew(_ZFP_ZFMapPrivate::MapType::iterator,
        *ZFCastStatic(_ZFP_ZFMapPrivate::MapType::iterator *, data));
}
zfiterator ZFMap::iterator(void)
{
    return zfiterator(zfnew(_ZFP_ZFMapPrivate::MapType::iterator, d->data.begin()),
        _ZFP_ZFMap_iteratorDeleteCallback,
        _ZFP_ZFMap_iteratorCopyCallback);
}

zfbool ZFMap::iteratorIsValid(ZF_IN const zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return (data != zfnull && *data != d->data.end());
}
zfbool ZFMap::iteratorIsEqual(ZF_IN const zfiterator &it0,
                              ZF_IN const zfiterator &it1)
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFMapPrivate::MapType::iterator *>(it0, it1);
}

void ZFMap::iteratorSet(ZF_IN_OUT zfiterator &it,
                        ZF_IN ZFObject *value)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        this->contentOnRemove((*data)->first, (*data)->second);
        ZFObject *tmp = (*data)->second;
        zfRetain(value);
        (*data)->second = value;
        this->contentOnAdd((*data)->first, (*data)->second);
        zfRelease(tmp);

        this->contentOnChange();
    }
}
void ZFMap::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *tmpKey = (*data)->first;
        ZFObject *tmpValue = (*data)->second;
        d->data.erase((*data)++);

        this->contentOnRemove(tmpKey, tmpValue);
        this->contentOnChange();

        zfRelease(tmpKey);
        zfRelease(tmpValue);
    }
}
zfiterator ZFMap::iteratorForKey(ZF_IN ZFObject *key)
{
    return zfiterator(zfnew(_ZFP_ZFMapPrivate::MapType::iterator, d->data.find(key)),
        _ZFP_ZFMap_iteratorDeleteCallback,
        _ZFP_ZFMap_iteratorCopyCallback);
}
ZFObject *ZFMap::iteratorGetKey(ZF_IN const zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->first : zfnull);
}
ZFObject *ZFMap::iteratorGetValue(ZF_IN const zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    return ((data != zfnull && *data != d->data.end()) ? (*data)->second : zfnull);
}
ZFKeyValuePair ZFMap::iteratorGetPair(ZF_IN const zfiterator &it)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
    }
    return ret;
}
ZFObject *ZFMap::iteratorNextKey(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->first;
        data->operator ++();
        return ret;
    }
    return zfnull;
}
ZFObject *ZFMap::iteratorNextValue(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->second;
        data->operator ++();
        return ret;
    }
    return zfnull;
}
ZFKeyValuePair ZFMap::iteratorNextPair(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator ++();
        return ret;
    }
    return ret;
}
ZFObject *ZFMap::iteratorPrevKey(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->first;
        data->operator --();
        return ret;
    }
    return zfnull;
}
ZFObject *ZFMap::iteratorPrevValue(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ZFObject *ret = (*data)->second;
        data->operator --();
        return ret;
    }
    return zfnull;
}
ZFKeyValuePair ZFMap::iteratorPrevPair(ZF_IN_OUT zfiterator &it)
{
    ZFKeyValuePair ret = ZFKeyValuePairZero;
    _ZFP_ZFMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFMapPrivate::MapType::iterator *>();
    if(data != zfnull && *data != d->data.end())
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator --();
        return ret;
    }
    return ret;
}
void ZFMap::iteratorAddKeyValue(ZF_IN ZFObject *key,
                                ZF_IN ZFObject *value,
                                ZF_IN_OPT const zfiterator &it /* = zfiteratorInvalid */)
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END


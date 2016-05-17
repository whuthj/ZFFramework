/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore_ZFCoreDef.hh"
#include "ZFCoreMapDef.h"

#define _ZFP_ZFCoreMap_USE_HASHMAP 1

#if _ZFP_ZFCoreMap_USE_HASHMAP
    #include "../ZFSTLWrapper/zfstl_hashmap.h"
#else
    #include "../ZFSTLWrapper/zfstl_map.h"
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFCoreMapPair ZFCoreMapPairZero = {0, 0};

// ============================================================
zfclassNotPOD _ZFP_ZFCoreMapPrivate
{
public:
#if _ZFP_ZFCoreMap_USE_HASHMAP
    typedef zfstlhashmap<zfchar *, ZFCorePointerBase *, zfchar_zfstlHasher, zfchar_zfstlHashComparer> MapType;
#else
    typedef zfstlmap<zfchar *, ZFCorePointerBase *, zfchar_zfstlComparer> MapType;
#endif

public:
    zfindex refCount;
    _ZFP_ZFCoreMapPrivate::MapType m;

public:
    static void releaseRef(ZF_IN _ZFP_ZFCoreMapPrivate *&d)
    {
        if(d != zfnull)
        {
            --(d->refCount);
            if(d->refCount == 0)
            {
                zfdelete(d);
            }
            d = zfnull;
        }
    }

    void removeAll(void)
    {
        if(!this->m.empty())
        {
            ZFCoreArrayPOD<void *> allKey;
            ZFCoreArrayPOD<ZFCorePointerBase *> allValue;
            for(_ZFP_ZFCoreMapPrivate::MapType::iterator it = this->m.begin();
                it != this->m.end();
                ++it)
            {
                allKey.add(it->first);
                allValue.add(it->second);
            }
            this->m.clear();
            for(zfindex i = 0; i < allKey.count(); ++i)
            {
                zffree(allKey[i]);
                zfdelete(allValue[i]);
            }
        }
    }

public:
    _ZFP_ZFCoreMapPrivate(void)
    : refCount(1)
    {
    }
    ~_ZFP_ZFCoreMapPrivate(void)
    {
        this->removeAll();
    }
};

// ============================================================
ZFCoreMap::ZFCoreMap(void)
{
    d = zfnew(_ZFP_ZFCoreMapPrivate);
}
ZFCoreMap::ZFCoreMap(ZF_IN const ZFCoreMap &ref)
{
    d = zfnull;
    this->operator=(ref);
}
ZFCoreMap &ZFCoreMap::operator =(ZF_IN const ZFCoreMap &ref)
{
    ++(ref.d->refCount);
    _ZFP_ZFCoreMapPrivate::releaseRef(d);
    d = ref.d;
    return *this;
}
ZFCoreMap::~ZFCoreMap(void)
{
    _ZFP_ZFCoreMapPrivate::releaseRef(d);
}

void ZFCoreMap::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    this->objectInfoOfContentT(ret, 5);
}
void ZFCoreMap::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                     ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                     ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault */) const
{
    zfindex count = 0;
    ret += token.tokenLeft;
    if(!this->isEmpty())
    {
        for(zfiterator it = this->iterator();
            this->iteratorIsValid(it) && count < maxCount;
            this->iteratorNext(it), ++count)
        {
            if(count > 0)
            {
                ret += token.tokenSeparator;
            }

            ret += token.tokenPairLeft;
            {
                ret += token.tokenKeyLeft;
                ret += this->iteratorGetKey(it);
                ret += token.tokenKeyRight;
            }
            ret += token.tokenPairSeparator;
            {
                ret += token.tokenValueLeft;
                ZFCorePointerBase *value = this->iteratorGetValue(it);
                if(value == zfnull)
                {
                    ret += ZFTOKEN_zfnull;
                }
                else
                {
                    value->objectInfoOfContentT(ret);
                }
                ret += token.tokenValueRight;
            }
            ret += token.tokenPairRight;
        }
    }
    if(count < this->count())
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenEtc;
    }
    ret += token.tokenRight;
}

void ZFCoreMap::copyFrom(ZF_IN const ZFCoreMap &ref)
{
    if(d != ref.d)
    {
        this->removeAll();
        this->addFrom(ref);
    }
}

zfindex ZFCoreMap::objectRetainCount(void) const
{
    return d->refCount;
}

zfindex ZFCoreMap::count(void) const
{
    return d->m.size();
}

zfbool ZFCoreMap::isEmpty(void) const
{
    return d->m.empty();
}

zfbool ZFCoreMap::isContain(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = zfText("");
    }
    return (d->m.find((zfchar *)key) != d->m.end());
}

void ZFCoreMap::addFrom(ZF_IN const ZFCoreMap &ref)
{
    if(!ref.isEmpty())
    {
        if(d != ref.d)
        {
            for(zfiterator it = ref.iterator(); this->iteratorIsValid(it); this->iteratorNext(it))
            {
                this->set(this->iteratorGetKey(it), *(this->iteratorGetValue(it)));
            }
        }
        else
        {
            ZFCoreMap tmp;
            tmp.addFrom(ref);
            this->addFrom(tmp);
        }
    }
}

void ZFCoreMap::set(ZF_IN const zfchar *key,
                    ZF_IN const ZFCorePointerBase &value)
{
    if(key == zfnull)
    {
        key = zfText("");
    }

    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find((zfchar *)key);
    if(it == d->m.end())
    {
        d->m[zfsCopy(key)] = value.refNew();
    }
    else
    {
        ZFCorePointerBase *toDelete = it->second;
        it->second = value.refNew();
        toDelete->refDelete();
    }
}
ZFCorePointerBase *ZFCoreMap::get(ZF_IN const zfchar *key) const
{
    if(key == zfnull)
    {
        key = zfText("");
    }
    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find((zfchar *)key);
    if(it == d->m.end())
    {
        return zfnull;
    }
    return it->second;
}

ZFCoreArrayPOD<const zfchar *> ZFCoreMap::allKey(void) const
{
    ZFCoreArrayPOD<const zfchar *> ret;
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        ret.add(it->first);
    }
    return ret;
}
ZFCoreArrayPOD<ZFCorePointerBase *> ZFCoreMap::allValue(void) const
{
    ZFCoreArrayPOD<ZFCorePointerBase *> ret;
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        ret.add(it->second);
    }
    return ret;
}
ZFCoreArrayPOD<ZFCoreMapPair> ZFCoreMap::allPair(void) const
{
    ZFCoreArrayPOD<ZFCoreMapPair> ret;
    ZFCoreMapPair tmp;
    for(_ZFP_ZFCoreMapPrivate::MapType::const_iterator it = d->m.begin();
        it != d->m.end();
        ++it)
    {
        tmp.key = it->first;
        tmp.value = it->second;
        ret.add(tmp);
    }
    return ret;
}

void ZFCoreMap::remove(ZF_IN const zfchar *key)
{
    if(key == zfnull)
    {
        key = zfText("");
    }
    _ZFP_ZFCoreMapPrivate::MapType::iterator it = d->m.find((zfchar *)key);
    if(it != d->m.end())
    {
        zfchar *savedKey = it->first;
        ZFCorePointerBase *savedValue = it->second;
        d->m.erase(it);
        zffree(savedKey);
        savedValue->refDelete();
    }
}

void ZFCoreMap::removeAll(void)
{
    d->removeAll();
}

// ============================================================
// iterator
static void _ZFP_ZFCoreMap_iteratorDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFCoreMapPrivate::MapType::iterator *, data));
}
static void *_ZFP_ZFCoreMap_iteratorCopyCallback(void *data)
{
    return zfnew(_ZFP_ZFCoreMapPrivate::MapType::iterator,
        *ZFCastStatic(_ZFP_ZFCoreMapPrivate::MapType::iterator *, data));
}
zfiterator ZFCoreMap::iterator(void) const
{
    return zfiterator(
        zfnew(_ZFP_ZFCoreMapPrivate::MapType::iterator, d->m.begin()),
        _ZFP_ZFCoreMap_iteratorDeleteCallback,
        _ZFP_ZFCoreMap_iteratorCopyCallback);
}

zfiterator ZFCoreMap::iteratorForKey(ZF_IN const zfchar *key) const
{
    return zfiterator(
        zfnew(_ZFP_ZFCoreMapPrivate::MapType::iterator, d->m.find((zfchar *)key)),
        _ZFP_ZFCoreMap_iteratorDeleteCallback,
        _ZFP_ZFCoreMap_iteratorCopyCallback);
}

zfbool ZFCoreMap::iteratorIsValid(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    return (data != zfnull && *data != d->m.end());
}
zfbool ZFCoreMap::iteratorIsEqual(ZF_IN const zfiterator &it0,
                                  ZF_IN const zfiterator &it1) const
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFCoreMapPrivate::MapType::iterator *>(it0, it1);
}

void ZFCoreMap::iteratorSet(ZF_IN_OUT zfiterator &it,
                            ZF_IN const ZFCorePointerBase &newValue)
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        ZFCorePointerBase *toDelete = (*data)->second;
        (*data)->second = newValue.refNew();
        toDelete->refDelete();
    }
}
void ZFCoreMap::iteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data != zfnull)
    {
        zfchar *savedKey = (*data)->first;
        ZFCorePointerBase *savedValue = (*data)->second;
        d->m.erase((*data)++);
        zffree(savedKey);
        savedValue->refDelete();
    }
}

const zfchar *ZFCoreMap::iteratorGetKey(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    return ((data != zfnull) ? (*data)->first : zfnull);
}
ZFCorePointerBase *ZFCoreMap::iteratorGetValue(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    return ((data != zfnull) ? (*data)->second : zfnull);
}
ZFCoreMapPair ZFCoreMap::iteratorGetPair(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    ZFCoreMapPair ret = ZFCoreMapPairZero;
    if(data)
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
    }
    return ret;
}

const zfchar *ZFCoreMap::iteratorNextKey(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        const zfchar *ret = (*data)->first;
        data->operator ++();
        return ret;
    }
    return zfnull;
}
ZFCorePointerBase *ZFCoreMap::iteratorNextValue(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        ZFCorePointerBase *ret = (*data)->second;
        data->operator ++();
        return ret;
    }
    return zfnull;
}
ZFCoreMapPair ZFCoreMap::iteratorNextPair(ZF_IN_OUT zfiterator &it) const
{
    ZFCoreMapPair ret = ZFCoreMapPairZero;
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator ++();
    }
    return ret;
}

const zfchar *ZFCoreMap::iteratorPrevKey(ZF_IN_OUT zfiterator &it) const
{
#if _ZFP_ZFCoreMap_USE_HASHMAP
    zfCoreCriticalNotSupported();
#else
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        const zfchar *ret = (*data)->first;
        data->operator --();
        return ret;
    }
#endif
    return zfnull;
}
ZFCorePointerBase *ZFCoreMap::iteratorPrevValue(ZF_IN_OUT zfiterator &it) const
{
#if _ZFP_ZFCoreMap_USE_HASHMAP
    zfCoreCriticalNotSupported();
#else
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        ZFCorePointerBase *ret = (*data)->second;
        data->operator --();
        return ret;
    }
#endif
    return zfnull;
}
ZFCoreMapPair ZFCoreMap::iteratorPrevPair(ZF_IN_OUT zfiterator &it) const
{
    ZFCoreMapPair ret = ZFCoreMapPairZero;
#if _ZFP_ZFCoreMap_USE_HASHMAP
    zfCoreCriticalNotSupported();
#else
    _ZFP_ZFCoreMapPrivate::MapType::iterator *data = it.data<_ZFP_ZFCoreMapPrivate::MapType::iterator *>();
    if(data)
    {
        ret.key = (*data)->first;
        ret.value = (*data)->second;
        data->operator --();
    }
#endif
    return ret;
}

void ZFCoreMap::iteratorAddKeyValue(ZF_IN const zfchar *key,
                                    ZF_IN const ZFCorePointerBase &value,
                                    ZF_IN_OPT const zfiterator &it /* = zfiteratorInvalid */)
{
    this->set(key, value);
}

ZF_NAMESPACE_GLOBAL_END


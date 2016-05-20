/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore_ZFObjectDef.hh"
#include "ZFSerializableDataDef.h"
#include "ZFSerializableUtilDef.h"
#include "ZFObjectSmartPointerDef.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_deque.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFSerializableDataPrivate
zfclassNotPOD _ZFP_ZFSerializableDataAttributeData
{
public:
    zfstlstringZ attrValue;
    zfbool resolved;
public:
    _ZFP_ZFSerializableDataAttributeData(void)
    : attrValue()
    , resolved(zffalse)
    {
    }
    _ZFP_ZFSerializableDataAttributeData(ZF_IN const zfchar *attrValue,
                                         ZF_IN_OPT zfbool resolved = zffalse)
    : attrValue(attrValue)
    , resolved(resolved)
    {
    }
};
typedef zfstlmap<zfstlstringZ, _ZFP_ZFSerializableDataAttributeData> _ZFP_ZFSerializableDataAttributeMapType;
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFSerializableDataTagMapType;
zfclassNotPOD _ZFP_ZFSerializableDataPrivate
{
public:
    zfindex refCount;
    _ZFP_ZFSerializableDataPrivate *serializableDataParent;
    zfbool resolved;
    zfstlstringZ className;
    zfchar *referenceRefType;
    zfchar *referenceRefData;
    _ZFP_ZFSerializableDataAttributeMapType attributes;
    zfstldeque<ZFSerializableData> elements;
    _ZFP_ZFSerializableDataTagMapType serializableDataTagMap;

public:
    void removeAll(void)
    {
        this->className.clear();
        this->attributes.clear();
        if(!this->elements.empty())
        {
            for(zfstlsize i = 0; i < this->elements.size(); ++i)
            {
                this->elements[i].d->serializableDataParent = zfnull;
            }
            this->elements.clear();
        }

        zffree(this->referenceRefType);
        this->referenceRefType = zfnull;
        zffree(this->referenceRefData);
        this->referenceRefData = zfnull;
    }

public:
    _ZFP_ZFSerializableDataPrivate(void)
    : refCount(1)
    , serializableDataParent(zfnull)
    , resolved(zffalse)
    , className()
    , referenceRefType(zfnull)
    , referenceRefData(zfnull)
    , attributes()
    , elements()
    , serializableDataTagMap()
    {
    }
    ~_ZFP_ZFSerializableDataPrivate(void)
    {
        this->removeAll();
    }
};

// ============================================================
// ZFSerializableData
/** @cond ZFPrivateDoc */
ZFSerializableData::ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d)
: d(d)
{
    ++(d->refCount);
}
/** @endcond */
ZFSerializableData::ZFSerializableData(void)
: d(zfnew(_ZFP_ZFSerializableDataPrivate))
{
}
ZFSerializableData::ZFSerializableData(ZF_IN const ZFSerializableData &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFSerializableData &ZFSerializableData::operator =(ZF_IN const ZFSerializableData &ref)
{
    ++(ref.d->refCount);

    {
        --(d->refCount);
        if(d->refCount == 0)
        {
            zfdelete(d);
        }
        d = zfnull;
    }

    d = ref.d;
    return *this;
}
ZFSerializableData::~ZFSerializableData(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

void ZFSerializableData::copyFrom(ZF_IN const ZFSerializableData &ref)
{
    if(d == ref.d)
    {
        return ;
    }

    d->className = ref.d->className;

    d->attributes = ref.d->attributes;
    d->elements = ref.d->elements;
    for(zfstlsize i = 0; i < d->elements.size(); ++i)
    {
        d->elements[i] = ref.elementAtIndex(i).copy();
        d->elements[i].d->serializableDataParent = d;
    }

    zfsChange(d->referenceRefType, ref.d->referenceRefType);
    zfsChange(d->referenceRefData, ref.d->referenceRefData);

    d->serializableDataTagMap = ref.d->serializableDataTagMap;
}

zfindex ZFSerializableData::objectRetainCount(void) const
{
    return d->refCount;
}

// ============================================================
// reference logic
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSerializableDataReferenceTypeDataHolder, ZFLevelZFFrameworkNormal)
{
}
ZFCoreMap refTypeMap; // _ZFP_ZFSerializableDataReferenceCallback *
ZF_GLOBAL_INITIALIZER_END(ZFSerializableDataReferenceTypeDataHolder)
#define _ZFP_ZFSerializableData_refTypeMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableDataReferenceTypeDataHolder)->refTypeMap)

ZFCoreArrayPOD<const zfchar *> ZFSerializableDataRefTypeGetAll(void)
{
    return _ZFP_ZFSerializableData_refTypeMap.allKey();
}

void _ZFP_ZFSerializableDataReferenceTypeRegister(ZF_IN const zfchar *referenceType,
                                                  ZF_IN _ZFP_ZFSerializableDataReferenceCallback callback)
{
    _ZFP_ZFSerializableDataReferenceCallback *tmp = (_ZFP_ZFSerializableDataReferenceCallback *)zfmalloc(sizeof(_ZFP_ZFSerializableDataReferenceCallback));
    *tmp = callback;
    _ZFP_ZFSerializableData_refTypeMap.set(
        referenceType,
        ZFCorePointerForObject<_ZFP_ZFSerializableDataReferenceCallback *>(tmp));
}
void _ZFP_ZFSerializableDataReferenceTypeUnregister(ZF_IN const zfchar *referenceType)
{
    _ZFP_ZFSerializableData_refTypeMap.remove(referenceType);
}
void ZFSerializableData::referenceRefTypeSet(ZF_IN const zfchar *refType)
{
    zfsChange(d->referenceRefType, refType);
}
const zfchar *ZFSerializableData::referenceRefType(void) const
{
    return d->referenceRefType;
}
void ZFSerializableData::referenceRefDataSet(ZF_IN const zfchar *refData)
{
    zfsChange(d->referenceRefData, refData);
}
const zfchar *ZFSerializableData::referenceRefData(void) const
{
    return d->referenceRefData;
}
static void _ZFP_ZFSerializableData_clearNestedReferenceInfo(ZF_IN_OUT ZFSerializableData &data)
{
    data.referenceRefTypeSet(zfnull);
    data.referenceRefDataSet(zfnull);
    for(zfindex i = 0; i < data.elementCount(); ++i)
    {
        _ZFP_ZFSerializableData_clearNestedReferenceInfo(data.elementAtIndex(i));
    }
}
zfbool ZFSerializableData::referenceCheckLoad(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                                              ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(outErrorPos != zfnull)
    {
        *outErrorPos = zfnull;
    }

    if(d->referenceRefType == zfnull && d->referenceRefData == zfnull)
    {
        return zftrue;
    }

    // load self
    {
        if(d->referenceRefType == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, *this,
                zfText("missing \"%s\""), ZFSerializableKeyword_refType);
            return zffalse;
        }
        if(d->referenceRefData == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, *this,
                zfText("missing \"%s\""), ZFSerializableKeyword_refData);
            return zffalse;
        }

        // backup
        zfstlstringZ serializableClassSaved = d->className;
        d->className.clear();
        _ZFP_ZFSerializableDataAttributeMapType attributesSaved;
        attributesSaved.swap(d->attributes);
        zfstldeque<ZFSerializableData> elementsSaved;
        elementsSaved.swap(d->elements);

        // load reference to self
        _ZFP_ZFSerializableDataReferenceCallback *callback = _ZFP_ZFSerializableData_refTypeMap.get<_ZFP_ZFSerializableDataReferenceCallback *>(d->referenceRefType);
        if(callback == zfnull || !(*callback)(*this, this->referenceRefData(), outErrorHintToAppend))
        {
            if(outErrorPos != zfnull)
            {
                *outErrorPos = this;
            }
            return zffalse;
        }

        // clear nested reference info loaded from parent referenced data
        for(zfindex i = 0; i < this->elementCount(); ++i)
        {
            _ZFP_ZFSerializableData_clearNestedReferenceInfo(this->elementAtIndex(i));
        }

        // restore the serializable class
        d->className = serializableClassSaved;

        // copy all attributes and children back to the newly created tmpData
        for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = attributesSaved.begin(); it != attributesSaved.end(); ++it)
        {
            d->attributes[it->first] = it->second;
        }
        d->elements.insert(d->elements.end(), elementsSaved.begin(), elementsSaved.end());
    }

    // check all children
    for(zfstlsize i = 0; i < d->elements.size(); ++i)
    {
        d->elements[i].referenceCheckLoad();
    }

    return zftrue;
}

zfbool ZFSerializableData::referenceInfoExist(void) const
{
    return (d->referenceRefType != zfnull || d->referenceRefData != zfnull);
}
zfbool ZFSerializableData::referenceInfoExistRecursively(void) const
{
    if(this->referenceInfoExist())
    {
        return zftrue;
    }
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        if(this->elementAtIndex(i).referenceInfoExistRecursively())
        {
            return zftrue;
        }
    }
    return zffalse;
}
ZFSerializableData ZFSerializableData::referenceInfoCopy(void) const
{
    ZFSerializableData ret;
    ret.referenceRefTypeSet(d->referenceRefType);
    ret.referenceRefDataSet(d->referenceRefData);
    return ret;
}
ZFSerializableData ZFSerializableData::referenceInfoCopyRecursively(void) const
{
    ZFSerializableData ret;
    ret.referenceRefTypeSet(d->referenceRefType);
    ret.referenceRefDataSet(d->referenceRefData);
    const zfchar *tmp = zfnull;
    if((tmp = this->propertyName()) != zfnull)
    {
        ret.propertyNameSet(tmp);
    }
    if((tmp = this->category()) != zfnull)
    {
        ret.categorySet(tmp);
    }

    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        const ZFSerializableData &child = this->elementAtIndex(i);
        if(child.referenceInfoExistRecursively())
        {
            ret.elementAdd(child.referenceInfoCopyRecursively());
        }
    }
    return ret;
}

// ============================================================
// parent
zfbool ZFSerializableData::serializableDataParent(ZF_OUT ZFSerializableData &ret) const
{
    if(d->serializableDataParent != zfnull)
    {
        ret = ZFSerializableData(d->serializableDataParent);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

// ============================================================
// class
void ZFSerializableData::itemClassSet(ZF_IN const zfchar *clsName)
{
    if(clsName == zfnull)
    {
        d->className.clear();
    }
    else
    {
        d->className = clsName;
    }
}
const zfchar *ZFSerializableData::itemClass(void) const
{
    return d->className.empty() ? zfnull : d->className.c_str();
}

// ============================================================
// other types
void ZFSerializableData::removeAll(void)
{
    d->removeAll();
}

// ============================================================
void ZFSerializableData::serializableDataTagSet(ZF_IN const zfchar *key,
                                                ZF_IN ZFObject *tag,
                                                ZF_IN_OPT zfbool autoMarkCached /* = zffalse */)
{
    if(key == zfnull)
    {
        return ;
    }

    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            if(autoMarkCached)
            {
                m[key] = zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                m[key] = zfautoObjectCreate(tag);
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
                it->second = zfautoObjectCreateMarkCached(tag);
            }
            else
            {
                it->second = zfautoObjectCreate(tag);
            }
        }
    }
}
ZFObject *ZFSerializableData::serializableDataTagGet(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFSerializableData::serializableDataTagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                                           ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue) const
{
    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    for(_ZFP_ZFSerializableDataTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
zfautoObject ZFSerializableData::serializableDataTagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret = zfautoObjectCreate(it->second.toObject());
            m.erase(it);
            return ret;
        }
    }
    return zfautoObjectNull;
}
void ZFSerializableData::serializableDataTagRemoveAll(void)
{
    if(!d->serializableDataTagMap.empty())
    {
        _ZFP_ZFSerializableDataTagMapType tmp;
        tmp.swap(d->serializableDataTagMap);
    }
}

// ============================================================
// attribute
void ZFSerializableData::attributeSet(ZF_IN const zfchar *name,
                                      ZF_IN const zfchar *value)
{
    if(name != zfnull)
    {
        if(value != zfnull)
        {
            _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
            if(it != d->attributes.end())
            {
                it->second = _ZFP_ZFSerializableDataAttributeData(value);
            }
            else
            {
                d->attributes[name] = _ZFP_ZFSerializableDataAttributeData(value);
            }
        }
        else
        {
            d->attributes.erase(name);
        }
    }
}
const zfchar *ZFSerializableData::attributeForName(ZF_IN const zfchar *name) const
{
    if(name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            return it->second.attrValue.c_str();
        }
    }
    return zfnull;
}

zfindex ZFSerializableData::attributeCount(void) const
{
    return (zfindex)d->attributes.size();
}
void ZFSerializableData::attributeRemove(ZF_IN const zfchar *name)
{
    if(name != zfnull)
    {
        d->attributes.erase(name);
    }
}
void ZFSerializableData::attributeRemoveAll(void)
{
    d->attributes.clear();
}

static void _ZFP_ZFSerializableData_iteratorDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFSerializableDataAttributeMapType::iterator *, data));
}
static void *_ZFP_ZFSerializableData_iteratorCopyCallback(void *data)
{
    return zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator,
        *ZFCastStatic(_ZFP_ZFSerializableDataAttributeMapType::iterator *, data));
}
zfiterator ZFSerializableData::attributeIteratorForName(ZF_IN const zfchar *name) const
{
    return zfiterator(
        zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator, name ? d->attributes.find(name) : d->attributes.end()),
        _ZFP_ZFSerializableData_iteratorDeleteCallback,
        _ZFP_ZFSerializableData_iteratorCopyCallback);
}
zfiterator ZFSerializableData::attributeIterator(void) const
{
    return zfiterator(
        zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator, d->attributes.begin()),
        _ZFP_ZFSerializableData_iteratorDeleteCallback,
        _ZFP_ZFSerializableData_iteratorCopyCallback);
}
zfbool ZFSerializableData::attributeIteratorIsValid(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    return (data != zfnull && *data != d->attributes.end());
}
zfbool ZFSerializableData::attributeIteratorIsEqual(ZF_IN const zfiterator &it0,
                                                    ZF_IN const zfiterator &it1) const
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFSerializableDataAttributeMapType::iterator *>(it0, it1);
}
void ZFSerializableData::attributeIteratorSet(ZF_IN_OUT zfiterator &it,
                                              ZF_IN const zfchar *value)
{
    if(value == zfnull)
    {
        this->attributeIteratorRemove(it);
        return ;
    }
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data)
    {
        (*data)->second = _ZFP_ZFSerializableDataAttributeData(value);
    }
}
void ZFSerializableData::attributeIteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        d->attributes.erase((*data)++);
    }
}
const zfchar *ZFSerializableData::attributeIteratorGetKey(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->first.c_str();
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorGet(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->second.attrValue.c_str();
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorNextKey(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        const zfchar *ret = (*data)->first.c_str();
        ++(*data);
        return ret;
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorNext(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeData &ret = (*data)->second;
        ++(*data);
        return ret.attrValue.c_str();
    }
    return zfnull;
}

zfbool ZFSerializableData::attributeIteratorResolved(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->second.resolved;
    }
    return zffalse;
}
void ZFSerializableData::attributeIteratorResolveMark(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        (*data)->second.resolved = zftrue;
    }
}
void ZFSerializableData::attributeIteratorResolveUnmark(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        (*data)->second.resolved = zftrue;
    }
}

// ============================================================
// element
void ZFSerializableData::elementAdd(ZF_IN const ZFSerializableData &element)
{
    zfCoreAssertWithMessage(d != element.d, zfTextA("adding self is not allowed"));
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, zfTextA("adding a data which already has parent"));
    d->elements.push_back(element);
    element.d->serializableDataParent = d;
}
void ZFSerializableData::elementAdd(ZF_IN const ZFSerializableData &element,
                                    ZF_IN zfindex atIndex)
{
    zfCoreAssertWithMessage(d != element.d, zfTextA("adding self is not allowed"));
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, zfTextA("adding a data which already has parent"));
    if(atIndex == zfindexMax)
    {
        atIndex = (zfindex)d->elements.size();
    }
    if(atIndex > (zfindex)d->elements.size())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, (zfindex)(d->elements.size() + 1));
        return ;
    }

    d->elements.insert(d->elements.begin() + atIndex, element);
    element.d->serializableDataParent = d;
}

void ZFSerializableData::elementSetAtIndex(ZF_IN zfindex index,
                                           ZF_IN const ZFSerializableData &element)
{
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements[(zfstlsize)index] = element;
    element.d->serializableDataParent = d;
}

zfindex ZFSerializableData::elementFindByName(ZF_IN const zfchar *name,
                                              ZF_IN_OPT zfbool skipResolved /* = zffalse */) const
{
    if(name != zfnull && *name != '\0')
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            if(zfscmpTheSame(d->elements[i].propertyName(), name)
                && (!skipResolved || !d->elements[i].resolved()))
            {
                return i;
            }
        }
    }
    return zfindexMax;
}
zfindex ZFSerializableData::elementFindByCategory(ZF_IN const zfchar *category,
                                                  ZF_IN_OPT zfbool skipResolved /* = zffalse */) const
{
    if(category != zfnull && *category != '\0')
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            if(zfscmpTheSame(d->elements[i].category(), category)
                && (!skipResolved || !d->elements[i].resolved()))
            {
                return i;
            }
        }
    }
    return zfindexMax;
}

zfindex ZFSerializableData::elementCount(void) const
{
    return (zfindex)d->elements.size();
}
const ZFSerializableData &ZFSerializableData::elementAtIndex(ZF_IN zfindex index) const
{
    return d->elements[(zfstlsize)index];
}
ZFSerializableData &ZFSerializableData::elementAtIndex(ZF_IN zfindex index)
{
    return d->elements[(zfstlsize)index];
}

void ZFSerializableData::elementRemoveAtIndex(ZF_IN zfindex index)
{
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements.erase(d->elements.begin() + (zfstlsize)index);
}
void ZFSerializableData::elementRemoveAll(void)
{
    if(!d->elements.empty())
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            d->elements[i].d->serializableDataParent = zfnull;
        }
        d->elements.clear();
    }
}

// ============================================================
// resolved state
zfbool ZFSerializableData::resolved(void) const
{
    return d->resolved;
}
void ZFSerializableData::resolveMark(void) const
{
    d->resolved = zftrue;
}
void ZFSerializableData::resolveUnmark(void) const
{
    d->resolved = zffalse;
}

zfbool ZFSerializableData::resolvedAttribute(ZF_IN const zfchar *name) const
{
    if(name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            return it->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::resolveAttributeMark(ZF_IN const zfchar *name) const
{
    if(name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttributeUnmark(ZF_IN const zfchar *name) const
{
    if(name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            it->second.resolved = zffalse;
        }
    }
}

zfbool ZFSerializableData::resolvedAll(ZF_OUT_OPT const ZFSerializableData **firstNotResolvedElement /* = zfnull */,
                                       ZF_OUT_OPT zfstring *firstNotResolvedAttribute /* = zfnull */) const
{
    if(!this->resolved())
    {
        if(firstNotResolvedElement != zfnull)
        {
            *firstNotResolvedElement = this;
        }
        return zffalse;
    }
    for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
    {
        if(!it->second.resolved)
        {
            if(firstNotResolvedElement != zfnull)
            {
                *firstNotResolvedElement = this;
            }
            if(firstNotResolvedAttribute != zfnull)
            {
                *firstNotResolvedAttribute += it->first.c_str();
            }
            return zffalse;
        }
    }
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        if(!this->elementAtIndex(i).resolvedAll(firstNotResolvedElement, firstNotResolvedAttribute))
        {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFSerializableData::resolveMarkAll(void) const
{
    this->resolveMark();
    this->resolveAttributeMarkAll();
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        this->elementAtIndex(i).resolveMarkAll();
    }
}
void ZFSerializableData::resolveUnmarkAll(void) const
{
    this->resolveUnmark();
    this->resolveAttributeUnmarkAll();
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        this->elementAtIndex(i).resolveUnmarkAll();
    }
}
void ZFSerializableData::resolveAttributeMarkAll(void) const
{
    for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
    {
        it->second.resolved = zftrue;
    }
}
void ZFSerializableData::resolveAttributeUnmarkAll(void) const
{
    for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
    {
        it->second.resolved = zftrue;
    }
}

// ============================================================
// other functions
void ZFSerializableData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '{';

    if(this->itemClass() == zfnull)
    {
        ret += ZFTOKEN_zfnull;
    }
    else
    {
        ret += this->itemClass();
    }

    if(this->attributeCount() > 0
        || this->referenceRefType() != zfnull || this->referenceRefData() != zfnull)
    {
        ret += '(';
        if(this->referenceRefType() != zfnull)
        {
            ret += ZFSerializableKeyword_refType;
            ret += '=';
            ret += '"';
            ret += this->referenceRefType();
            ret += '"';
            ret += ';';
        }
        if(this->referenceRefData() != zfnull)
        {
            ret += ZFSerializableKeyword_refData;
            ret += '=';
            ret += '"';
            ret += this->referenceRefData();
            ret += '"';
            ret += ';';
        }

        for(zfiterator it = this->attributeIterator(); this->attributeIteratorIsValid(it); this->attributeIteratorNext(it))
        {
            ret += this->attributeIteratorGetKey(it);
            ret += '=';
            ret += '"';
            ret += this->attributeIteratorGet(it);
            ret += '"';
            ret += ';';
        }
        ret += ')';
    }

    if(this->elementCount() > 0)
    {
        ret += zfText(" [\n");
        for(zfindex i = 0; i < this->elementCount(); ++i)
        {
            zfstring tmpInfo = this->elementAtIndex(i).objectInfo();
            ret += zfText("    ");
            for(zfindex i = 0; i < tmpInfo.length(); ++i)
            {
                ret += tmpInfo[i];
                if(tmpInfo[i] == '\n')
                {
                    ret += zfText("    ");
                }
            }
            ret += zfText("\n");
        }
        ret += ']';
    }

    ret += '}';
}

ZFCompareResult ZFSerializableData::objectCompare(ZF_IN const ZFSerializableData &another) const
{
    if(d == another.d)
    {
        return ZFCompareTheSame;
    }

    if(!zfscmpTheSame(this->itemClass(), another.itemClass())
        || this->attributeCount() != another.attributeCount()
        || this->elementCount() != another.elementCount())
    {
        return ZFCompareUncomparable;
    }

    for(zfiterator it = another.attributeIterator(); another.attributeIteratorIsValid(it); another.attributeIteratorNext(it))
    {
        const zfchar *value = this->attributeForName(another.attributeIteratorGetKey(it));
        if(value == zfnull || !zfscmpTheSame(another.attributeIteratorGet(it), value))
        {
            return ZFCompareUncomparable;
        }
    }
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        if(this->elementAtIndex(i).objectCompare(another.elementAtIndex(i)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

zfbool ZFSerializableData::isEmpty(void) const
{
    return (
        d->className.empty()
        && d->attributes.empty()
        && d->elements.empty()
        && d->referenceRefType == zfnull
        && d->referenceRefData == zfnull
        );
}

ZF_NAMESPACE_GLOBAL_END


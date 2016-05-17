/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore.hh"
#include "ZFJson.h"
#include "protocol/ZFProtocolZFJson.h"
#include "protocol/ZFProtocolZFJsonEscapeChar.h"

#include "ZFSTLWrapper/zfstl_string.h"
#include "ZFSTLWrapper/zfstl_deque.h"
#include "ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFJsonType)

// ============================================================
static ZFJsonOutputFlags _ZFP_ZFJsonOutputFlagsDefaultInstance;
const ZFJsonOutputFlags &_ZFP_ZFJsonOutputFlagsDefault(void)
{
    return _ZFP_ZFJsonOutputFlagsDefaultInstance;
}
static ZFJsonOutputFlags _ZFP_ZFJsonOutputFlagsTrimInstance;
const ZFJsonOutputFlags &_ZFP_ZFJsonOutputFlagsTrim(void)
{
    return _ZFP_ZFJsonOutputFlagsTrimInstance;
}
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFJsonOutputFlagsInit, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFJsonOutputFlagsTrimInstance.jsonToken.jsonNewLineToken.removeAll();
    _ZFP_ZFJsonOutputFlagsTrimInstance.jsonToken.jsonIndentToken.removeAll();
    _ZFP_ZFJsonOutputFlagsTrimInstance.jsonToken.jsonValueSeparatorToken = zfText(":");
    _ZFP_ZFJsonOutputFlagsTrimInstance.jsonToken.jsonSeparatorInSameLineToken = zfText(",");
    _ZFP_ZFJsonOutputFlagsTrimInstance.jsonObjectAddNewLineForContent = zffalse;
    _ZFP_ZFJsonOutputFlagsTrimInstance.jsonArrayAddNewLineForContent = zffalse;
}
ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFJsonOutputFlagsInit)

// ============================================================
static void _ZFP_ZFJsonToOutput_outputIndent(ZF_IN_OUT const ZFOutputCallback &output,
                                             ZF_IN const ZFJsonOutputFlags &outputFlags,
                                             ZF_IN zfindex indentLevel)
{
    for(zfindex i = 0; i < indentLevel; ++i)
    {
        output << outputFlags.jsonToken.jsonIndentToken;
    }
}
static void _ZFP_ZFJsonToOutput_output(ZF_IN_OUT const ZFOutputCallback &output,
                                       ZF_IN const ZFJsonItem *jsonItem,
                                       ZF_IN const ZFJsonOutputFlags &outputFlags,
                                       ZF_IN zfindex indentLevel);
static void _ZFP_ZFJsonToOutput_output_JsonValue(ZF_IN_OUT const ZFOutputCallback &output,
                                                 ZF_IN const ZFJsonItem *jsonValue,
                                                 ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                 ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonValueTagLeft;
    ZFJsonEscapeCharEncode(output, jsonValue->jsonValue());
    output << outputFlags.jsonToken.jsonValueTagRight;
}
static void _ZFP_ZFJsonToOutput_output_JsonObject(ZF_IN_OUT const ZFOutputCallback &output,
                                                  ZF_IN const ZFJsonItem *jsonObject,
                                                  ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                  ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonObjectTagLeft;
    zfbool first = zftrue;
    for(zfiterator it = jsonObject->jsonItemIterator(); jsonObject->jsonItemIteratorIsValid(it); jsonObject->jsonItemIteratorNext(it))
    {
        if(outputFlags.jsonObjectAddNewLineForContent)
        {
            if(first)
            {
                first = zffalse;
            }
            else
            {
                output << outputFlags.jsonToken.jsonSeparatorToken;
            }
            output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
            _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel + 1);
        }
        else
        {
            if(first)
            {
                first = zffalse;
            }
            else
            {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
        }
        output
            << outputFlags.jsonToken.jsonKeyTagLeft
            << jsonObject->jsonItemIteratorGetKey(it)
            << outputFlags.jsonToken.jsonKeyTagRight;
        output << outputFlags.jsonToken.jsonValueSeparatorToken;
        _ZFP_ZFJsonToOutput_output(output, jsonObject->jsonItemIteratorGet(it), outputFlags, indentLevel + 1);
    }
    if(outputFlags.jsonObjectAddNewLineForContent
        && !(outputFlags.jsonObjectTagInSameLineIfNoContent && jsonObject->jsonItemCount() == 0))
    {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonObjectTagRight;
}
static void _ZFP_ZFJsonToOutput_output_JsonArray(ZF_IN_OUT const ZFOutputCallback &output,
                                                 ZF_IN const ZFJsonItem *jsonArray,
                                                 ZF_IN const ZFJsonOutputFlags &outputFlags,
                                                 ZF_IN zfindex indentLevel)
{
    output << outputFlags.jsonToken.jsonArrayTagLeft;
    for(zfindex i = 0; i < jsonArray->jsonObjectCount(); ++i)
    {
        if(outputFlags.jsonArrayAddNewLineForContent)
        {
            if(i != 0)
            {
                if(outputFlags.jsonArrayContentTagInSameLine)
                {
                    output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
                }
                else
                {
                    output << outputFlags.jsonToken.jsonSeparatorToken;
                }
            }

            if(!outputFlags.jsonArrayContentTagInSameLine)
            {
                output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
                _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel);
            }
            _ZFP_ZFJsonToOutput_output(output, jsonArray->jsonObjectAtIndex(i), outputFlags, indentLevel + 1);
        }
        else
        {
            if(i != 0)
            {
                output << outputFlags.jsonToken.jsonSeparatorInSameLineToken;
            }
            _ZFP_ZFJsonToOutput_output(output, jsonArray->jsonObjectAtIndex(i), outputFlags, indentLevel + 1);
        }
    }
    if(outputFlags.jsonArrayAddNewLineForContent
        && !(outputFlags.jsonArrayTagInSameLineIfNoContent && jsonArray->jsonObjectCount() == 0))
    {
        output << outputFlags.jsonToken.jsonNewLineToken << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonToOutput_outputIndent(output, outputFlags, indentLevel);
    }
    output << outputFlags.jsonToken.jsonArrayTagRight;
}
static void _ZFP_ZFJsonToOutput_output(ZF_IN_OUT const ZFOutputCallback &output,
                                       ZF_IN const ZFJsonItem *jsonItem,
                                       ZF_IN const ZFJsonOutputFlags &outputFlags,
                                       ZF_IN zfindex indentLevel)
{
    switch(jsonItem->jsonType())
    {
        case ZFJsonType::e_JsonValue:
            _ZFP_ZFJsonToOutput_output_JsonValue(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonObject:
            _ZFP_ZFJsonToOutput_output_JsonObject(output, jsonItem, outputFlags, indentLevel);
            break;
        case ZFJsonType::e_JsonArray:
            _ZFP_ZFJsonToOutput_output_JsonArray(output, jsonItem, outputFlags, indentLevel);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}

// ============================================================
typedef zfstlmap<zfstlstringZ, zfstlstringZ> _ZFP_ZFJsonItemImplTagMapType;
typedef zfstlmap<zfstlstringZ, ZFJsonItem> _ZFP_ZFJsonItemMapType;
typedef zfstldeque<ZFJsonItem> _ZFP_ZFJsonObjectArrayType;
zfclassNotPOD _ZFP_ZFJsonItemPrivate
{
public:
    zfindex refCount;
    _ZFP_ZFJsonItemImplTagMapType implTag;
    ZFJsonTypeEnum jsonType;
    _ZFP_ZFJsonItemPrivate *jsonParent;
    zfstlstringZ jsonValue;
    _ZFP_ZFJsonItemMapType jsonItemMap;
    _ZFP_ZFJsonObjectArrayType jsonObjectArray;

public:
    _ZFP_ZFJsonItemPrivate(void)
    : refCount(1)
    , implTag()
    , jsonType(ZFJsonType::e_JsonNull)
    , jsonParent(zfnull)
    , jsonValue()
    , jsonItemMap()
    , jsonObjectArray()
    {
    }
};

// ============================================================
// ZFJsonItem
void ZFJsonItem::_ZFP_ZFJsonItemOnDelete(void)
{
    this->jsonItemRemoveAll();
    this->jsonObjectRemoveAll();

    zfpoolDelete(d);
    d = zfnull;
}

/** @cond ZFPrivateDoc */
ZFJsonItem::ZFJsonItem(ZF_IN _ZFP_ZFJsonItemPrivate *d)
: d(d)
{
    ++(d->refCount);
}
/** @endcond */
ZFJsonItem::ZFJsonItem(void)
: d(zfpoolNew(_ZFP_ZFJsonItemPrivate))
{
}
ZFJsonItem::ZFJsonItem(ZF_IN ZFJsonTypeEnum jsonType)
: d(zfpoolNew(_ZFP_ZFJsonItemPrivate))
{
    d->jsonType = jsonType;
}
ZFJsonItem::ZFJsonItem(ZF_IN const ZFJsonItem &ref)
{
    d = ref.d;
    ++(d->refCount);
}
ZFJsonItem::ZFJsonItem(ZF_IN const ZFJsonItem *ref)
{
    if(ref == zfnull)
    {
        d = zfpoolNew(_ZFP_ZFJsonItemPrivate);
    }
    else
    {
        d = ref->d;
        ++(d->refCount);
    }
}
ZFJsonItem::~ZFJsonItem(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        _ZFP_ZFJsonItemOnDelete();
    }
}

/** @cond ZFPrivateDoc */
ZFJsonItem &ZFJsonItem::operator = (ZF_IN const ZFJsonItem &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        _ZFP_ZFJsonItemOnDelete();
    }
    d = ref.d;
    return *this;
}
ZFJsonItem &ZFJsonItem::operator = (ZF_IN const ZFJsonItem *ref)
{
    if(ref == zfnull)
    {
        this->operator = (ZFJsonItem());
    }
    else
    {
        this->operator = (*ref);
    }
    return *this;
}
zfbool ZFJsonItem::operator == (ZF_IN const ZFJsonItem &ref) const
{
    return (d == ref.d || (d->jsonType == ZFJsonType::e_JsonNull && ref.d->jsonType == ZFJsonType::e_JsonNull));
}
zfbool ZFJsonItem::operator == (ZF_IN const ZFJsonItem *ref) const
{
    if(ref == zfnull)
    {
        return (d->jsonType == ZFJsonType::e_JsonNull);
    }
    else
    {
        return (d == ref->d);
    }
}
/** @endcond */

// ============================================================
void ZFJsonItem::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    ret += ZFJsonType::EnumNameForValue(this->jsonType());
    switch(this->jsonType())
    {
        case ZFJsonType::e_JsonNull:
            break;
        case ZFJsonType::e_JsonValue:
            zfstringAppend(ret, zfText("\"%s\""), this->jsonValue());
            break;
        case ZFJsonType::e_JsonObject:
            if(this->jsonItemCount() > 0)
            {
                zfstringAppend(ret, zfText(" %zi elements"), this->jsonItemCount());
            }
            break;
        case ZFJsonType::e_JsonArray:
            if(this->jsonObjectCount() > 0)
            {
                zfstringAppend(ret, zfText(" %zi elements"), this->jsonObjectCount());
            }
            break;
        default:
            break;
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

zfindex ZFJsonItem::objectRetainCount(void) const
{
    return d->refCount;
}

// ============================================================
void ZFJsonItem::implTagSet(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    zfCoreAssert(this->jsonType() != ZFJsonType::e_JsonNull);
    if(key == zfnull)
    {
        return ;
    }
    if(value == zfnull)
    {
        d->implTag.erase(key);
    }
    else
    {
        _ZFP_ZFJsonItemImplTagMapType::iterator it = d->implTag.find(key);
        if(it != d->implTag.end())
        {
            it->second = value;
        }
        else
        {
            d->implTag[key] = value;
        }
    }
}
const zfchar *ZFJsonItem::implTag(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        _ZFP_ZFJsonItemImplTagMapType::iterator it = d->implTag.find(key);
        if(it != d->implTag.end())
        {
            return it->second.c_str();
        }
    }
    return zfnull;
}

// ============================================================
ZFJsonTypeEnum ZFJsonItem::jsonType(void) const
{
    return d->jsonType;
}

zfbool ZFJsonItem::jsonParent(ZF_OUT ZFJsonItem &ret) const
{
    if(d->jsonParent != zfnull)
    {
        ret = ZFJsonItem(d->jsonParent);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

void ZFJsonItem::jsonValueSet(ZF_IN const zfchar *value)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonValue);
    d->jsonValue = value;
}
const zfchar *ZFJsonItem::jsonValue(void) const
{
    return (d->jsonValue.empty() ? zfnull : d->jsonValue.c_str());
}

// ============================================================
ZFJsonItem ZFJsonItem::jsonCloneTree(void) const
{
    ZFJsonItem ret(this->jsonType());
    switch(this->jsonType())
    {
        case ZFJsonType::e_JsonNull:
            break;
        case ZFJsonType::e_JsonValue:
            ret.jsonValueSet(this->jsonValue());
            break;
        case ZFJsonType::e_JsonObject:
            for(zfiterator it = this->jsonItemIterator(); this->jsonItemIteratorIsValid(it); this->jsonItemIteratorNext(it))
            {
                ret.jsonItemSet(this->jsonItemIteratorGetKey(it), this->jsonItemIteratorGet(it)->jsonCloneTree());
            }
            break;
        case ZFJsonType::e_JsonArray:
            for(zfindex i = 0; i < this->jsonObjectCount(); ++i)
            {
                ret.jsonObjectAdd(this->jsonObjectAtIndex(i)->jsonCloneTree());
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
    return ret;
}

// ============================================================
// for object type
ZFJsonItem *ZFJsonItem::jsonItem(ZF_IN const zfchar *key) const
{
    _ZFP_ZFJsonItemMapType::iterator it = d->jsonItemMap.find(key);
    if(it != d->jsonItemMap.end())
    {
        return it->second;
    }
    return zfnull;
}
void ZFJsonItem::jsonItemSet(ZF_IN const zfchar *key,
                             ZF_IN ZFJsonItem *jsonItem)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    if(key == zfnull)
    {
        return ;
    }
    _ZFP_ZFJsonItemMapType::iterator it = d->jsonItemMap.find(key);
    if(it != d->jsonItemMap.end())
    {
        it->second.d->jsonParent = zfnull;
        if(jsonItem != zfnull)
        {
            it->second = jsonItem;
            jsonItem->d->jsonParent = d;
        }
        else
        {
            d->jsonItemMap.erase(it);
        }
    }
    else
    {
        if(jsonItem != zfnull)
        {
            d->jsonItemMap[key] = ZFJsonItem(jsonItem);
            jsonItem->d->jsonParent = d;
        }
    }
}

const zfchar *ZFJsonItem::jsonItemValue(ZF_IN const zfchar *key) const
{
    const ZFJsonItem *jsonItem = this->jsonItem(key);
    return (jsonItem ? jsonItem->jsonValue() : zfnull);
}
void ZFJsonItem::jsonItemValueSet(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    if(value == zfnull)
    {
        this->jsonItemSet(key, zfnull);
    }
    else
    {
        ZFJsonItem jsonValue(ZFJsonType::e_JsonValue);
        jsonValue.jsonValueSet(value);
        this->jsonItemSet(key, jsonValue);
    }
}

void ZFJsonItem::jsonItemRemoveAll(void)
{
    for(_ZFP_ZFJsonItemMapType::iterator it = d->jsonItemMap.begin(); it != d->jsonItemMap.end(); ++it)
    {
        it->second->d->jsonParent = zfnull;
    }
    d->jsonItemMap.clear();
}

zfindex ZFJsonItem::jsonItemCount(void) const
{
    return (zfindex)d->jsonItemMap.size();
}

static void _ZFP_ZFJsonItem_iteratorDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFJsonItemMapType::iterator *, data));
}
static void *_ZFP_ZFJsonItem_iteratorCopyCallback(void *data)
{
    return zfnew(_ZFP_ZFJsonItemMapType::iterator,
        *ZFCastStatic(_ZFP_ZFJsonItemMapType::iterator *, data));
}
zfiterator ZFJsonItem::jsonItemIterator(void) const
{
    return zfiterator(
        zfnew(_ZFP_ZFJsonItemMapType::iterator, d->jsonItemMap.begin()),
        _ZFP_ZFJsonItem_iteratorDeleteCallback,
        _ZFP_ZFJsonItem_iteratorCopyCallback);
}

zfiterator ZFJsonItem::jsonItemIteratorForKey(ZF_IN const zfchar *key) const
{
    return zfiterator(
        zfnew(_ZFP_ZFJsonItemMapType::iterator, d->jsonItemMap.find(key)),
        _ZFP_ZFJsonItem_iteratorDeleteCallback,
        _ZFP_ZFJsonItem_iteratorCopyCallback);
}

zfbool ZFJsonItem::jsonItemIteratorIsValid(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    return (data != zfnull && *data != d->jsonItemMap.end());
}

zfbool ZFJsonItem::jsonItemIteratorIsEqual(ZF_IN const zfiterator &it0,
                                           ZF_IN const zfiterator &it1) const
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFJsonItemMapType::iterator *>(it0, it1);
}

void ZFJsonItem::jsonItemIteratorSet(ZF_IN_OUT zfiterator &it, ZF_IN ZFJsonItem *jsonItem)
{
    if(jsonItem == zfnull)
    {
        this->jsonItemIteratorRemove(it);
        return ;
    }
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonObject);
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data)
    {
        (*data)->second->d->jsonParent = zfnull;
        (*data)->second = ZFJsonItem(jsonItem);
        jsonItem->d->jsonParent = d;
    }
}

void ZFJsonItem::jsonItemIteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data != zfnull)
    {
        (*data)->second->d->jsonParent = zfnull;
        d->jsonItemMap.erase((*data)++);
    }
}

const zfchar *ZFJsonItem::jsonItemIteratorGetKey(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->first.c_str();
    }
    return zfnull;
}
ZFJsonItem *ZFJsonItem::jsonItemIteratorGet(ZF_IN const zfiterator &it)
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->second;
    }
    return zfnull;
}
const ZFJsonItem *ZFJsonItem::jsonItemIteratorGet(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->second;
    }
    return zfnull;
}

const zfchar *ZFJsonItem::jsonItemIteratorNextKey(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data != zfnull)
    {
        const zfchar *ret = (*data)->first.c_str();
        ++(*data);
        return ret;
    }
    return zfnull;
}
ZFJsonItem *ZFJsonItem::jsonItemIteratorNext(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data != zfnull)
    {
        ZFJsonItem *ret = (*data)->second;
        ++(*data);
        return ret;
    }
    return zfnull;
}
const ZFJsonItem *ZFJsonItem::jsonItemIteratorNext(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFJsonItemMapType::iterator *data = it.data<_ZFP_ZFJsonItemMapType::iterator *>();
    if(data != zfnull)
    {
        ZFJsonItem *ret = (*data)->second;
        ++(*data);
        return ret;
    }
    return zfnull;
}

// ============================================================
zfindex ZFJsonItem::jsonObjectCount(void) const
{
    return (zfindex)d->jsonObjectArray.size();
}
ZFJsonItem *ZFJsonItem::jsonObjectAtIndex(ZF_IN zfindex index)
{
    if(index >= (zfindex)d->jsonObjectArray.size())
    {
        zfCoreCriticalIndexOutOfRange(index, (zfindex)d->jsonObjectArray.size());
        return zfnull;
    }
    return d->jsonObjectArray[index];
}
const ZFJsonItem *ZFJsonItem::jsonObjectAtIndex(ZF_IN zfindex index) const
{
    if(index >= (zfindex)d->jsonObjectArray.size())
    {
        zfCoreCriticalIndexOutOfRange(index, (zfindex)d->jsonObjectArray.size());
        return zfnull;
    }
    return d->jsonObjectArray[index];
}

void ZFJsonItem::jsonObjectAdd(ZF_IN ZFJsonItem *jsonObject)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonArray);
    zfCoreAssertWithMessage(jsonObject != zfnull, zfTextA("add null jsonObject"));
    zfCoreAssertWithMessage(jsonObject->jsonType() == ZFJsonType::e_JsonObject, zfTextA("item not type of %s"),
        zfsCoreZ2A(ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonObject)));
    d->jsonObjectArray.push_back(ZFJsonItem(jsonObject));
    jsonObject->d->jsonParent = d;
}
void ZFJsonItem::jsonObjectAdd(ZF_IN ZFJsonItem *jsonObject,
                               ZF_IN zfindex atIndexOrZFIndexMax)
{
    zfCoreAssert(this->jsonType() == ZFJsonType::e_JsonArray);
    if(atIndexOrZFIndexMax == zfindexMax)
    {
        atIndexOrZFIndexMax = (zfindex)d->jsonObjectArray.size();
    }
    else if(atIndexOrZFIndexMax > (zfindex)d->jsonObjectArray.size())
    {
        zfCoreCriticalIndexOutOfRange(atIndexOrZFIndexMax, (zfindex)(d->jsonObjectArray.size() + 1));
        return ;
    }
    zfCoreAssertWithMessage(jsonObject != zfnull, zfTextA("add null jsonObject"));
    zfCoreAssertWithMessage(jsonObject->jsonType() == ZFJsonType::e_JsonObject, zfTextA("item not type of %s"),
        zfsCoreZ2A(ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonObject)));
    d->jsonObjectArray.insert(d->jsonObjectArray.begin() + atIndexOrZFIndexMax,
        ZFJsonItem(jsonObject));
    jsonObject->d->jsonParent = d;
}
void ZFJsonItem::jsonObjectRemove(ZF_IN ZFJsonItem *jsonObject)
{
    if(jsonObject == zfnull || jsonObject->d->jsonParent != d || jsonObject->d->jsonType != ZFJsonType::e_JsonObject)
    {
        return ;
    }

    for(zfstlsize i = 0; i < d->jsonObjectArray.size(); ++i)
    {
        if(jsonObject == d->jsonObjectArray[i])
        {
            jsonObject->d->jsonParent = zfnull;
            d->jsonObjectArray.erase(d->jsonObjectArray.begin() + i);
            break;
        }
    }
}
void ZFJsonItem::jsonObjectRemoveAtIndex(ZF_IN zfindex index)
{
    if(index >= (zfindex)d->jsonObjectArray.size())
    {
        zfCoreCriticalIndexOutOfRange(index, (zfindex)d->jsonObjectArray.size());
        return ;
    }
    d->jsonObjectArray[index].d->jsonParent = zfnull;
    d->jsonObjectArray.erase(d->jsonObjectArray.begin() + index);
}
void ZFJsonItem::jsonObjectRemoveAll(void)
{
    for(_ZFP_ZFJsonObjectArrayType::iterator it = d->jsonObjectArray.begin(); it != d->jsonObjectArray.end(); ++it)
    {
        (*it).d->jsonParent = zfnull;
    }
    d->jsonObjectArray.clear();
}
zfindex ZFJsonItem::jsonObjectFind(ZF_IN const ZFJsonItem *jsonObject) const
{
    if(jsonObject != zfnull && jsonObject->d->jsonParent == d && jsonObject->d->jsonType == ZFJsonType::e_JsonObject)
    {
        for(zfstlsize i = 0; i < d->jsonObjectArray.size(); ++i)
        {
            if(jsonObject == d->jsonObjectArray[i])
            {
                return (zfindex)i;
            }
        }
    }
    return zfindexMax;
}

// ============================================================
ZFJsonItem ZFJsonFromInput(ZF_IN const ZFInputCallback &input)
{
    ZFJsonItem jsonItem(ZFJsonType::e_JsonObject);
    if(!ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(jsonItem, input))
    {
        return ZFJsonItem();
    }
    return jsonItem;
}
ZFJsonItem ZFJsonFromString(ZF_IN const zfchar *src,
                            ZF_IN_OPT zfindex length /* = zfindexMax */)
{
    ZFJsonItem jsonItem(ZFJsonType::e_JsonObject);
    if(!ZFPROTOCOL_ACCESS(ZFJson)->jsonParse(jsonItem, src, length))
    {
        return ZFJsonItem();
    }
    return jsonItem;
}

zfbool ZFJsonToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                      ZF_IN const ZFJsonItem *jsonItem,
                      ZF_IN_OPT const ZFJsonOutputFlags &outputFlags /* = ZFJsonOutputFlagsDefault */)
{
    if(output.callbackIsValid() && jsonItem != zfnull)
    {
        output << outputFlags.jsonGlobalLineBeginToken;
        _ZFP_ZFJsonToOutput_output(output, jsonItem, outputFlags, 0);
        return zftrue;
    }
    return zffalse;
}

// ============================================================
// escape chars
void ZFJsonEscapeCharEncode(ZF_OUT zfstring &dst,
                            ZF_IN const zfchar *src,
                            ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFJsonEscapeCharEncode(ZFOutputCallbackForString(dst), src, count);
}
void ZFJsonEscapeCharEncode(ZF_OUT const ZFOutputCallback &dst,
                            ZF_IN const zfchar *src,
                            ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFPROTOCOL_ACCESS(ZFJsonEscapeChar)->jsonEscapeCharEncode(dst, src, count);
}

void ZFJsonEscapeCharDecode(ZF_OUT zfstring &dst,
                            ZF_IN const zfchar *src,
                            ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFJsonEscapeCharDecode(ZFOutputCallbackForString(dst), src, count);
}
void ZFJsonEscapeCharDecode(ZF_OUT const ZFOutputCallback &dst,
                            ZF_IN const zfchar *src,
                            ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    ZFPROTOCOL_ACCESS(ZFJsonEscapeChar)->jsonEscapeCharDecode(dst, src, count);
}

ZF_NAMESPACE_GLOBAL_END


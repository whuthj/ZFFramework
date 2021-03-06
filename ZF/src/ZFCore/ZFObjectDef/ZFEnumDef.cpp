/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFEnumDef.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(ZFEnumDataHolder, ZFLevelZFFrameworkHigh)
{
}
ZFCoreMap enumDataMap;
ZF_STATIC_INITIALIZER_END(ZFEnumDataHolder)
#define _ZFP_ZFEnumDataMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFEnumDataHolder)->enumDataMap)

// ============================================================
// ZFEnum
ZFOBJECT_REGISTER(ZFEnum)

zfbool ZFEnum::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                               ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    const zfchar *value = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(value != zfnull)
    {
        ZFEnumValue enumValue = ZFEnumValueInvalid;
        if(!zfscmpTheSame(value, ZFEnumValueNameInvalid))
        {
            enumValue = this->enumValueForName(value);
            if(enumValue == ZFEnumValueInvalid)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                    zfText("invalid value %s for enum %s"), value, this->classData()->className());
                return zffalse;
            }
        }
        this->enumValueSet(enumValue);
    }
    else
    {
        this->enumValueSet(ZFEnumValueInvalid);
    }
    return zftrue;
}
zfbool ZFEnum::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                             ZF_IN ZFSerializable *referencedOwnerOrNull,
                                             ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull || ref->enumValue() != this->enumValue())
    {
        serializableData.propertyValueSet(this->enumName());
    }
    return zftrue;
}

ZFObject *ZFEnum::objectOnInit(ZF_IN ZFEnumValue value)
{
    this->objectOnInit();
    zfself::enumValueSet(value);
    return this;
}
ZFObject *ZFEnum::objectOnInit(ZF_IN ZFEnum *another)
{
    this->objectOnInit();
    if(another != zfnull && another->classData()->classIsSubclassOf(this->classData()))
    {
        zfself::enumValueSet(another->enumValue());
    }
    return this;
}
ZFObject *ZFEnum::objectOnInit(void)
{
    zfsuper::objectOnInit();
    _ZFP_ZFEnum_value = ZFEnumValueInvalid;
    return this;
}
void ZFEnum::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}

void ZFEnum::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += this->classData()->className();
    ret += zfText("::");
    ret += this->enumName();
}

ZFCompareResult ZFEnum::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj)
    {
        return ZFCompareTheSame;
    }
    if(anotherObj == zfnull || anotherObj->classData() != this->classData())
    {
        return ZFCompareUncomparable;
    }

    zfself *another = ZFCastZFObjectUnchecked(zfself *, anotherObj);
    if(this->enumValue() == ZFEnumValueInvalid
        || another->enumValue() == ZFEnumValueInvalid)
    {
        return ZFCompareUncomparable;
    }
    zfint n = ZFCastStatic(zfint, this->enumValue()) - ZFCastStatic(zfint, another->enumValue());
    if(n < 0)
    {
        return ZFCompareSmaller;
    }
    else if(n == 0)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareGreater;
    }
}

zfclassPOD _ZFP_ZFEnumDataPrivateNameData
{
public:
    zfchar *name;
    zfchar *fullName;
};
zfclassNotPOD _ZFP_ZFEnumDataPrivate
{
public:
    typedef zfstlmap<ZFEnumValue, _ZFP_ZFEnumDataPrivateNameData> DataType;
    _ZFP_ZFEnumDataPrivate::DataType d;
public:
    ~_ZFP_ZFEnumDataPrivate(void)
    {
        for(_ZFP_ZFEnumDataPrivate::DataType::iterator it = d.begin(); it != d.end(); ++it)
        {
            zfsChange(it->second.name, (const zfchar *)zfnull);
            zfsChange(it->second.fullName, (const zfchar *)zfnull);
        }
        d.clear();
    }
};
_ZFP_ZFEnumData::_ZFP_ZFEnumData(void)
: needInitFlag(zftrue)
{
    d = zfnew(_ZFP_ZFEnumDataPrivate);
}
_ZFP_ZFEnumData::~_ZFP_ZFEnumData(void)
{
    zfdelete(d);
    d = zfnull;
}
void _ZFP_ZFEnumData::add(ZF_IN zfbool isEnableDuplicateValue,
                          ZF_IN ZFEnumValue value,
                          ZF_IN const zfchar *name,
                          ZF_IN const zfchar *fullName,
                          ZF_IN const ZFClass *ownerClass,
                          ZF_IN const zfcharA *callerFile,
                          zfindex callerLine)
{
    zfCoreAssert(value != ZFEnumValueInvalid);
    _ZFP_ZFEnumDataPrivate::DataType::iterator it = d->d.find(value);
    if(it != d->d.end())
    {
        zfCoreAssertWithMessageTrim(isEnableDuplicateValue,
            zfTextA("[%s (%s)] duplicate value %s (new: %s, old: %s) when define %s"),
            ZF_CALLER_PATH_TO_NAME(callerFile),
            zfsFromInt<zfstringA>(callerLine).cString(),
            zfsFromInt<zfstringA>(value).cString(),
            zfsCoreZ2A(name),
            zfsCoreZ2A(it->second.name),
            zfsCoreZ2A(ownerClass->className()));
        zfsChange(it->second.name, (const zfchar *)zfnull);
        zfsChange(it->second.fullName, (const zfchar *)zfnull);
    }
    _ZFP_ZFEnumDataPrivateNameData nameData;
    nameData.name = zfsCopy(name);
    nameData.fullName = zfsCopy(fullName);
    (d->d)[value] = nameData;
}
zfindex _ZFP_ZFEnumData::enumCount(void) const
{
    return d->d.size();
}
zfindex _ZFP_ZFEnumData::enumIndexForValue(ZF_IN ZFEnumValue value) const
{
    _ZFP_ZFEnumDataPrivate::DataType::iterator it = d->d.find(value);
    if(it != d->d.end())
    {
        zfindex ret = 0;
        for(_ZFP_ZFEnumDataPrivate::DataType::iterator itTmp = d->d.begin(); itTmp != it; ++itTmp, ++ret)
        {
            // nothing to do
        }
        return ret;
    }
    return zfindexMax;
}
ZFEnumValue _ZFP_ZFEnumData::enumValueAtIndex(ZF_IN zfindex index) const
{
    if(index >= d->d.size())
    {
        return ZFEnumValueInvalid;
    }
    _ZFP_ZFEnumDataPrivate::DataType::const_iterator it = d->d.begin();
    for(zfindex i = 0; i < index && it != d->d.end(); ++it, ++i)
    {
        // nothing to do
    }
    if(it == d->d.end())
    {
        return ZFEnumValueInvalid;
    }
    return it->first;
}
const zfchar *_ZFP_ZFEnumData::enumNameAtIndex(ZF_IN zfindex index) const
{
    if(index >= d->d.size())
    {
        return ZFEnumValueNameInvalid;
    }
    _ZFP_ZFEnumDataPrivate::DataType::const_iterator it = d->d.begin();
    for(zfindex i = 0; i < index && it != d->d.end(); ++it, ++i);
    if(it == d->d.end())
    {
        return ZFEnumValueNameInvalid;
    }
    return it->second.name;
}
const zfchar *_ZFP_ZFEnumData::enumFullNameAtIndex(ZF_IN zfindex index) const
{
    if(index >= d->d.size())
    {
        return ZFEnumValueNameInvalid;
    }
    _ZFP_ZFEnumDataPrivate::DataType::const_iterator it = d->d.begin();
    for(zfindex i = 0; i < index && it != d->d.end(); ++it, ++i);
    if(it == d->d.end())
    {
        return ZFEnumValueNameInvalid;
    }
    return it->second.fullName;
}
zfbool _ZFP_ZFEnumData::enumContainValue(ZF_IN ZFEnumValue value) const
{
    return (d->d.find(value) != d->d.end());
}
ZFEnumValue _ZFP_ZFEnumData::enumValueForName(ZF_IN const zfchar *name) const
{
    if(name == zfnull)
    {
        return ZFEnumValueInvalid;
    }
    for(_ZFP_ZFEnumDataPrivate::DataType::const_iterator it = d->d.begin();
        it != d->d.end();
        ++it)
    {
        if(zfscmpTheSame(name, it->second.name))
        {
            return it->first;
        }
    }
    return ZFEnumValueInvalid;
}
const zfchar *_ZFP_ZFEnumData::enumNameForValue(ZF_IN ZFEnumValue value) const
{
    _ZFP_ZFEnumDataPrivate::DataType::const_iterator it = d->d.find(value);
    if(it != d->d.end())
    {
        return it->second.name;
    }
    return ZFEnumValueNameInvalid;
}

_ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(const zfchar *name)
{
    ZFCoreMutexLocker();
    _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataMap.get<_ZFP_ZFEnumData *>(name);
    if(d != zfnull)
    {
        return d;
    }
    d = zfnew(_ZFP_ZFEnumData);
    _ZFP_ZFEnumDataMap.set(name, ZFCorePointerForObject<_ZFP_ZFEnumData *>(d));
    return d;
}

// ============================================================
// zfflags conversion
void zfflagsToString(ZF_IN_OUT zfstring &ret,
                     ZF_IN ZFEnum *enumObj,
                     ZF_IN zfflags const &value,
                     ZF_IN_OPT zfbool includeNotConverted /* = zftrue */,
                     ZF_IN_OPT zfbool exclusiveMode /* = zffalse */,
                     ZF_OUT_OPT zfflags *notConverted /* = zfnull */,
                     ZF_IN_OPT zfchar separatorToken /* = '|' */)
{
    if(enumObj->enumCount() > 0)
    {
        zfflags *flagList = (zfflags *)zfmalloc(sizeof(zfflags) * enumObj->enumCount());
        const zfchar **nameList = (const zfchar **)zfmalloc(sizeof(const zfchar *) * enumObj->enumCount());
        for(zfindex i = 0; i < enumObj->enumCount(); ++i)
        {
            flagList[i] = enumObj->enumValueAtIndex(i);
            nameList[i] = enumObj->enumNameAtIndex(i);
        }
        zfflagsToString(ret,
            flagList, nameList, enumObj->enumCount(),
            value,
            includeNotConverted, exclusiveMode, notConverted, separatorToken);
        zffree(flagList);
        zffree(nameList);
    }
    else
    {
        zfflagsToString(ret,
            zfnull, zfnull, 0,
            value,
            includeNotConverted, exclusiveMode, notConverted, separatorToken);
    }
}
const zfchar *zfflagsFromString(ZF_OUT zfflags &ret,
                                ZF_IN ZFEnum *enumObj,
                                ZF_IN const zfchar *src,
                                ZF_IN_OPT zfindex srcLen /* = zfindexMax */,
                                ZF_IN_OPT zfchar separatorToken /* = '|' */)
{
    if(enumObj->enumCount() > 0)
    {
        zfflags *flagList = (zfflags *)zfmalloc(sizeof(zfflags) * enumObj->enumCount());
        const zfchar **nameList = (const zfchar **)zfmalloc(sizeof(const zfchar *) * enumObj->enumCount());
        for(zfindex i = 0; i < enumObj->enumCount(); ++i)
        {
            flagList[i] = enumObj->enumValueAtIndex(i);
            nameList[i] = enumObj->enumNameAtIndex(i);
        }
        const zfchar *errPosTmp = zfflagsFromString(
            ret,
            flagList, nameList, enumObj->enumCount(),
            src, srcLen, separatorToken);
        zffree(flagList);
        zffree(nameList);
        return errPosTmp;
    }
    else
    {
        return zfflagsFromString(
            ret,
            zfnull, zfnull, 0,
            src, srcLen, separatorToken);
    }
}

ZFOUTPUT_TYPE_DEFINE(ZFEnumFlagsBase, {output << v.objectInfo();})

ZF_NAMESPACE_GLOBAL_END


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFRegExp.h"
#include "protocol/ZFProtocolZFRegExp.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFRegExpResult::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    if(this->matched)
    {
        ret += zfText("matched");
        if(this->matchedRange != zfindexRangeZero)
        {
            ret += zfText(" in ");
            zfindexRangeToString(ret, this->matchedRange);
        }
        if(!this->namedGroups.isEmpty())
        {
            ret += zfText(", named groups: ");
            for(zfindex i = 0; i < this->namedGroups.count(); ++i)
            {
                if(i != 0)
                {
                    ret += zfText(", ");
                }
                zfindexRangeToString(ret, this->namedGroups[i]);
            }
        }
    }
    else
    {
        ret += zfText("no match");
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
// global
ZFENUM_DEFINE(ZFRegExpOption)
ZFENUM_FLAGS_DEFINE(ZFRegExpOption, ZFRegExpOptionFlags)

// ============================================================
// _ZFP_ZFRegExpPrivate
zfclassNotPOD _ZFP_ZFRegExpPrivate
{
public:
    void *nativeRegExp;
    zfstring pattern;
    ZFRegExpOptionFlags flag;

public:
    _ZFP_ZFRegExpPrivate(void)
    : nativeRegExp(zfnull)
    , pattern()
    , flag()
    {
    }
};

// ============================================================
// ZFRegExp
ZFOBJECT_REGISTER(ZFRegExp)

zfbool ZFRegExp::serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryFromData(ownerData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    const ZFSerializableData *parttenData = ZFSerializableUtil::checkElementByCategory(ownerData, ZFSerializableKeyword_ZFRegExp_pattern);
    const ZFSerializableData *flagData = ZFSerializableUtil::checkElementByCategory(ownerData, ZFSerializableKeyword_ZFRegExp_flag);
    if(parttenData == zfnull && flagData == zfnull)
    {
        this->regExpCompile(zfText(""));
    }

    // partten
    const zfchar *partten = zfnull;
    if(parttenData != zfnull)
    {
        if(!zfstringFromSerializableData(partten, *parttenData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
        if(parttenData->referenceInfoExist())
        {
            this->serializableRefInfoSetForCategory(ZFSerializableKeyword_ZFRegExp_pattern, parttenData->referenceInfoCopy());
        }
    }

    // flag
    ZFRegExpOptionFlags flag = ZFRegExpOptionFlags::e_Default();
    if(flagData != zfnull)
    {
        if(!ZFRegExpOptionFlagsFromSerializableData(flag, *flagData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
        if(flagData->referenceInfoExist())
        {
            this->serializableRefInfoSetForCategory(ZFSerializableKeyword_ZFRegExp_flag, flagData->referenceInfoCopy());
        }
    }

    this->regExpCompile(partten, flag);

    return zftrue;
}
zfbool ZFRegExp::serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                       ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                       ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryToData(ownerData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    { // partten
        ZFSerializableData categoryData;
        if(!zfstringToSerializableData(categoryData, this->regExpPattern(), outErrorHintToAppend))
        {
            return zffalse;
        }
        categoryData.categorySet(ZFSerializableKeyword_ZFRegExp_pattern);

        zfbool needAdd = zftrue;
        if(ref != zfnull)
        {
            if(zfscmpTheSame(ref->regExpPattern(), this->regExpPattern()))
            {
                needAdd = zffalse;
            }
        }
        if(needAdd)
        {
            const ZFSerializableData *refInfo = this->serializableRefInfoGetForCategory(ZFSerializableKeyword_ZFRegExp_pattern);
            if(refInfo != zfnull)
            {
                ZFSerializableData tmpData;
                if(!refInfo->referenceLoadTo(tmpData, outErrorHintToAppend))
                {
                    return zffalse;
                }
                ZFSerializableUtil::resolveReferencedData(categoryData, *refInfo);
            }
            else if(zfscmpTheSame(this->regExpPattern(), zfText("")))
            {
                needAdd = zffalse;
            }
        }
        if(needAdd)
        {
            ownerData.elementAdd(categoryData);
        }
    }

    { // flag
        ZFSerializableData categoryData;
        if(!ZFRegExpOptionFlagsToSerializableData(categoryData, this->regExpFlag(), outErrorHintToAppend))
        {
            return zffalse;
        }
        categoryData.categorySet(ZFSerializableKeyword_ZFRegExp_flag);

        zfbool needAdd = zftrue;
        if(ref != zfnull)
        {
            if(ref->regExpFlag() == this->regExpFlag())
            {
                needAdd = zffalse;
            }
        }
        if(needAdd)
        {
            const ZFSerializableData *refInfo = this->serializableRefInfoGetForCategory(ZFSerializableKeyword_ZFRegExp_flag);
            if(refInfo != zfnull)
            {
                ZFSerializableData tmpData;
                if(!refInfo->referenceLoadTo(tmpData, outErrorHintToAppend))
                {
                    return zffalse;
                }
                ZFSerializableUtil::resolveReferencedData(categoryData, *refInfo);
            }
            else if(this->regExpFlag() == ZFRegExpOptionFlags::e_Default())
            {
                needAdd = zffalse;
            }
        }

        if(needAdd)
        {
            ownerData.elementAdd(categoryData);
        }
    }
    return zftrue;
}

ZFObject *ZFRegExp::objectOnInit(ZF_IN const zfchar *pattern,
                                 ZF_IN_OPT ZFRegExpOptionFlags flag /* = ZFRegExpOptionFlags::e_Default() */)
{
    this->objectOnInit();
    zfself::regExpCompile(pattern, flag);
    return this;
}
ZFObject *ZFRegExp::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFRegExpPrivate);
    d->nativeRegExp = ZFPROTOCOL_ACCESS(ZFRegExp)->nativeRegExpCreate(this);
    return this;
}
void ZFRegExp::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFRegExp)->nativeRegExpDestroy(this, d->nativeRegExp);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFRegExp::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ZFClassUtil::objectPropertyInfo(ret, this);
}

zfidentity ZFRegExp::objectHash(void)
{
    ZFRegExpOptionFlags flag = this->regExpFlag();
    return zfidentityHash(zfidentityCalcString(this->regExpPattern()),
                         zfidentityCalcPOD(flag));
}
ZFCompareResult ZFRegExp::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zfscmpTheSame(this->regExpPattern(), another->regExpPattern())
        && this->regExpFlag() == another->regExpFlag())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

void *ZFRegExp::nativeRegExp(void)
{
    return d->nativeRegExp;
}

const zfchar *ZFRegExp::regExpPattern(void)
{
    return (d->pattern.isEmpty() ? zfnull : d->pattern.cString());
}
ZFRegExpOptionFlags ZFRegExp::regExpFlag(void)
{
    return d->flag;
}

void ZFRegExp::regExpCompile(ZF_IN const zfchar *pattern,
                             ZF_IN_OPT ZFRegExpOptionFlags flag /* = ZFRegExpOptionFlags::e_Default() */)
{
    if(pattern == zfnull)
    {
        pattern = zfText("");
    }
    d->pattern = pattern;
    d->flag = flag;
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpCompile(this, pattern, flag);
}
zfindex ZFRegExp::regExpNamedGroupIndexForName(ZF_IN const zfchar *name)
{
    if(name == zfnull)
    {
        return zfindexMax;
    }
    return ZFPROTOCOL_ACCESS(ZFRegExp)->regExpNamedGroupIndexForName(this, name);
}
void ZFRegExp::regExpMatch(ZF_OUT ZFRegExpResult &result,
                           ZF_IN const zfchar *src,
                           ZF_IN_OPT zfindex srcLength /* = zfindexMax */)
{
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpMatch(this, result, src, srcLength);
}
void ZFRegExp::regExpMatchExact(ZF_OUT ZFRegExpResult &result,
                                ZF_IN const zfchar *src,
                                ZF_IN_OPT zfindex srcLength /* = zfindexMax */)
{
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpMatchExact(this, result, src, srcLength);
}
void ZFRegExp::regExpReplace(ZF_OUT zfstring &ret,
                             ZF_OUT ZFRegExpResult &result,
                             ZF_IN const zfchar *src,
                             ZF_IN const zfchar *replacePattern,
                             ZF_IN_OPT zfindex maxReplaceCount /* = zfindexMax */,
                             ZF_IN_OPT zfindex srcLength /* = zfindexMax */)
{
    result.namedGroups.removeAll();
    ZFPROTOCOL_ACCESS(ZFRegExp)->regExpReplace(this, ret, result, src, replacePattern, maxReplaceCount, srcLength);
}
ZF_NAMESPACE_GLOBAL_END


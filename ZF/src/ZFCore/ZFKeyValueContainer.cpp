/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFKeyValueContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFKeyValueContainer)

zfbool ZFKeyValueContainer::serializableOnCheck(void)
{
    if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}

    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); this->iteratorNext(it))
    {
        if(!ZFObjectIsSerializable(this->iteratorGetKey(it))
            || !ZFObjectIsSerializable(this->iteratorGetValue(it)))
        {
            return zffalse;
        }
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryFromData(ownerData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    this->removeAll();

    zfautoObject key;
    zfautoObject value;
    for(zfindex i = 0; i < ownerData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = ownerData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFKeyValueContainer_key))
        {
            if(key != zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData,
                    zfText("missing value for key %s (%s)"),
                    key.toObject()->objectInfoOfInstance().cString(),
                    key.toObject()->objectInfo().cString());
                return zffalse;
            }
            if(!ZFObjectFromSerializableData(key, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(key == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData, zfText("null key"));
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFKeyValueContainer_value))
        {
            if(key == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData,
                    zfText("missing key"));
                return zffalse;
            }
            if(!ZFObjectFromSerializableData(value, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(value == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData, zfText("null value"));
                return zffalse;
            }
            this->iteratorAddKeyValue(key.toObject(), value.toObject());
            key = zfautoObjectNull;
            value = zfautoObjectNull;
        }
    }
    if(key != zfautoObjectNull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData,
            zfText("missing value for key %s (%s)"),
            key.toObject()->objectInfoOfInstance().cString(),
            key.toObject()->objectInfo().cString());
        return zffalse;
    }

    return zftrue;
}
zfbool ZFKeyValueContainer::serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                                  ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryToData(ownerData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ZFKeyValuePair pair = this->iteratorNextPair(it);

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
    }
    else
    {
        return this->serializableOnSerializeCategoryToDataWithRef(ownerData, ref, outErrorHintToAppend);
    }

    return zftrue;
}

void ZFKeyValueContainer::objectOnDeallocPrepare(void)
{
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

void ZFKeyValueContainer::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                               ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                               ZF_IN_OPT const ZFTokenForKeyValueContainer &token /* = ZFTokenForKeyValueContainerDefault */)
{
    zfindex count = 0;
    ret += token.tokenLeft;
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it) && count < maxCount; ++count)
    {
        ZFKeyValuePair pair = this->iteratorNextPair(it);

        if(count > 0)
        {
            ret += token.tokenSeparator;
        }

        ret += token.tokenPairLeft;
        {
            ret += token.tokenKeyLeft;
            pair.key->objectInfoT(ret);
            ret += token.tokenKeyRight;
        }
        ret += token.tokenPairSeparator;
        {
            ret += token.tokenValueLeft;
            pair.value->objectInfoT(ret);
            ret += token.tokenValueRight;
        }
        ret += token.tokenPairRight;
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

zfidentity ZFKeyValueContainer::objectHash(void)
{
    zfiterator it = this->iterator();
    if(this->iteratorIsValid(it))
    {
        return ~((zfidentity)this->count()
            | ((ZFObjectHash(this->iteratorGetKey(it)) << 16) & 0x00FF0000)
            | ((ZFObjectHash(this->iteratorGetValue(it)) << 24) & 0xFF000000));
    }
    else
    {
        return 0;
    }
}
ZFCompareResult ZFKeyValueContainer::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
        || this->objectHash() != another->objectHash())
    {
        return ZFCompareUncomparable;
    }

    for(zfiterator it0 = this->iterator(); this->iteratorIsValid(it0);)
    {
        ZFKeyValuePair pair0 = this->iteratorNextPair(it0);
        zfiterator it1 = another->iteratorForKey(pair0.key);
        if(!another->iteratorIsValid(it1))
        {
            return ZFCompareUncomparable;
        }
        if(ZFObjectCompare(pair0.value, another->iteratorNextValue(it1)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

void ZFKeyValueContainer::objectCachedOnChange(void)
{
    zfsuper::objectCachedOnChange();
    zfbool objectCached = this->objectCached();
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        ZFKeyValuePair pair = this->iteratorNextPair(it);
        pair.key->objectCachedSet(objectCached);
        pair.value->objectCachedSet(objectCached);
    }
}

ZF_NAMESPACE_GLOBAL_END


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore.hh"
#include "ZFContainer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFContainer)

zfbool ZFContainer::serializableOnCheck(void)
{
    if(!zfsuperI(ZFSerializable)::serializableOnCheck()) {return zffalse;}
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        if(!ZFObjectIsSerializable(this->iteratorNext(it)))
        {
            return zffalse;
        }
    }
    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                            ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                            ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryFromData(ownerData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    this->removeAll();

    for(zfindex i = 0; i < ownerData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = ownerData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFContainer_element))
        {
            zfautoObject element;
            if(!ZFObjectFromSerializableData(element, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("null element"));
                return zffalse;
            }
            this->iteratorAdd(element.toObject());

            categoryData.resolveMark();
        }
    }
    return zftrue;
}
zfbool ZFContainer::serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                          ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                          ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeCategoryToData(ownerData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
        {
            ZFSerializableData elementData;
            if(!ZFObjectToSerializableData(elementData, this->iteratorNext(it), outErrorHintToAppend))
            {
                return zffalse;
            }
            elementData.categorySet(ZFSerializableKeyword_ZFContainer_element);
            ownerData.elementAdd(elementData);
        }
    }
    else
    {
        return this->serializableOnSerializeCategoryToDataWithRef(ownerData, ref, outErrorHintToAppend);
    }

    return zftrue;
}

void ZFContainer::objectOnDeallocPrepare(void)
{
    this->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

void ZFContainer::objectInfoOfContentT(ZF_IN_OUT zfstring &ret,
                                       ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                       ZF_IN_OPT const ZFTokenForContainer &token /* = ZFTokenForContainerDefault */)
{
    zfindex count = 0;
    ret += token.tokenLeft;
    zfiterator it = this->iterator();
    for(; this->iteratorIsValid(it) && count < maxCount; ++count)
    {
        if(count > 0)
        {
            ret += token.tokenSeparator;
        }
        ret += token.tokenValueLeft;
        this->iteratorNext(it)->objectInfoT(ret);
        ret += token.tokenValueRight;
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

zfidentity ZFContainer::objectHash(void)
{
    ZFObject *first = zfnull;
    {
        zfiterator it = this->iterator();
        if(this->iteratorIsValid(it))
        {
            first = this->iteratorGet(it);
        }
    }
    if(first != zfnull)
    {
        return zfidentityHash(this->count(), first->objectHash());
    }
    else
    {
        return 0;
    }
}
ZFCompareResult ZFContainer::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->count() != another->count()
        || this->objectHash() != another->objectHash())
    {
        return ZFCompareUncomparable;
    }
    for(zfiterator it = this->iterator(), itRef = another->iterator(); this->iteratorIsValid(it);)
    {
        if(ZFObjectCompare(this->iteratorNext(it), another->iteratorNext(itRef)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

void ZFContainer::objectCachedOnChange(void)
{
    zfsuper::objectCachedOnChange();
    zfbool objectCached = this->objectCached();
    for(zfiterator it = this->iterator(); this->iteratorIsValid(it); )
    {
        this->iteratorNext(it)->objectCachedSet(objectCached);
    }
}

ZF_NAMESPACE_GLOBAL_END


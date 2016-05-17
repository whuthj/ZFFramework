/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore_ZFObjectDef.hh"
#include "ZFSerializableUtilDef.h"
#include "ZFCoreType_IODef.h"
#include "ZFPropertyTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSerializableUtil)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSerializableUtilErrorOutputCallbacksHolder, ZFLevelZFFrameworkNormal)
{

}
ZFCoreArray<ZFOutputCallback> callbacks;
ZF_GLOBAL_INITIALIZER_END(ZFSerializableUtilErrorOutputCallbacksHolder)

ZFCoreArray<ZFOutputCallback> &_ZFP_ZFSerializableUtilErrorOutputCallbacks(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableUtilErrorOutputCallbacksHolder)->callbacks;
}

void errorOccurred(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                   ZF_OUT_OPT const ZFSerializableData **outErrorPos,
                   ZF_IN const ZFSerializableData &errorPos,
                   ZF_IN const zfchar *fmt,
                   ...)
{
    if(outErrorPos != zfnull)
    {
        *outErrorPos = &errorPos;
    }

    zfstring errorString;
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(errorString, fmt, vaList);
    va_end(vaList);
    zfstringAppend(errorString, zfText(", at:\n    %s"), errorPos.objectInfo().cString());

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}

void errorOccurred(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                   ZF_IN const zfchar *fmt,
                   ...)
{
    zfstring errorString;
    va_list vaList;
    va_start(vaList, fmt);
    zfstringAppendV(errorString, fmt, vaList);
    va_end(vaList);

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}

void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                        ZF_OUT_OPT const ZFSerializableData **outErrorPos,
                        ZF_IN const ZFSerializableData &errorPos,
                        ZF_IN const zfchar *serializingName,
                        ZF_IN const zfchar *errorValue)
{
    if(outErrorPos != zfnull)
    {
        *outErrorPos = &errorPos;
    }

    zfstring errorString;
    zfstringAppend(errorString, zfText("failed to serialize \"%s\" with value \"%s\""),
        serializingName, errorValue);
    zfstringAppend(errorString, zfText(", at:\n    %s"), errorPos.objectInfo().cString());

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}
void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                        ZF_IN const zfchar *serializingName,
                        ZF_IN const zfchar *errorValue)
{
    zfstring errorString;
    zfstringAppend(errorString, zfText("failed to serialize \"%s\" with value \"%s\""),
        serializingName, errorValue);

    if(outErrorHintToAppend != zfnull)
    {
        *outErrorHintToAppend += errorString;
    }
    ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
    for(zfindex i = 0; i < callbacks.count(); ++i)
    {
        callbacks[i].execute(errorString.cString());
    }
}

const zfchar *checkSerializableClass(ZF_IN const zfchar *desiredClass,
                                     ZF_IN const ZFSerializableData &serializableData)
{
    const zfchar *serializableClass = serializableData.itemClass();
    if(desiredClass == zfnull || *desiredClass == '\0')
    {
        return serializableClass;
    }
    else
    {
        if(zfscmpTheSame(serializableClass, desiredClass))
        {
            return serializableClass;
        }
        else
        {
            return zfnull;
        }
    }
}
const zfchar *requireSerializableClass(ZF_IN const zfchar *desiredClass,
                                       ZF_IN const ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                       ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    const zfchar *ret = checkSerializableClass(desiredClass, serializableData);
    if(ret == zfnull)
    {
        if(desiredClass == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("missing serializable class"));
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("serializable class must be \"%s\""), desiredClass);
        }
    }
    return ret;
}

const zfchar *checkAttribute(ZF_IN const ZFSerializableData &serializableData,
                             ZF_IN const zfchar *desiredAttribute)
{
    zfiterator it = serializableData.attributeIteratorForName(desiredAttribute);
    if(!serializableData.attributeIteratorIsValid(it))
    {
        return zfnull;
    }
    serializableData.attributeIteratorResolveMark(it);
    return serializableData.attributeIteratorGet(it);
}
const zfchar *requireAttribute(ZF_IN const ZFSerializableData &serializableData,
                               ZF_IN const zfchar *desiredAttribute,
                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                               ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    const zfchar *ret = checkAttribute(serializableData, desiredAttribute);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("missing attribute \"%s\""), desiredAttribute);
    }
    return ret;
}

const ZFSerializableData *checkElementByName(ZF_IN const ZFSerializableData &ownerData,
                                             ZF_IN const zfchar *desiredElementName)
{
    zfindex index = ownerData.elementFindByName(desiredElementName, zfHint("skipResolved")zftrue);
    if(index == zfindexMax)
    {
        return zfnull;
    }
    ownerData.elementAtIndex(index).resolvePropertyNameMark();
    return &(ownerData.elementAtIndex(index));
}
const ZFSerializableData *requireElementByName(ZF_IN const ZFSerializableData &ownerData,
                                               ZF_IN const zfchar *desiredElementName,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                               ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    const ZFSerializableData *ret = checkElementByName(ownerData, desiredElementName);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData,
            zfText("missing element with name \"%s\""),
            desiredElementName);
    }
    return ret;
}

const ZFSerializableData *checkElementByCategory(ZF_IN const ZFSerializableData &ownerData,
                                                 ZF_IN const zfchar *desiredElementCategory)
{
    zfindex index = ownerData.elementFindByCategory(desiredElementCategory);
    if(index == zfindexMax)
    {
        return zfnull;
    }
    ownerData.elementAtIndex(index).resolveCategoryMark();
    return &(ownerData.elementAtIndex(index));
}
const ZFSerializableData *requireElementByCategory(ZF_IN const ZFSerializableData &ownerData,
                                                   ZF_IN const zfchar *desiredElementCategory,
                                                   ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                   ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    const ZFSerializableData *ret = checkElementByCategory(ownerData, desiredElementCategory);
    if(ret == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, ownerData,
            zfText("missing element with category \"%s\""),
            desiredElementCategory);
    }
    return ret;
}

// ============================================================
zfbool copyRefInfo(ZF_IN_OUT ZFSerializableData &outData,
                   ZF_IN const ZFSerializableData &data)
{
    zfbool hasRefInfo = zffalse;

    if(data.referenceRefType() != zfnull)
    {
        hasRefInfo = zftrue;
        outData.referenceRefTypeSet(data.referenceRefType());
    }
    if(data.referenceRefData() != zfnull)
    {
        hasRefInfo = zftrue;
        outData.referenceRefDataSet(data.referenceRefData());
    }
    const zfchar *name = data.propertyName();
    if(name != zfnull)
    {
        outData.propertyNameSet(name);
    }

    for(zfindex i = 0; i < data.elementCount(); ++i)
    {
        ZFSerializableData tmp;
        if(ZFSerializableUtil::copyRefInfo(tmp, data.elementAtIndex(i)))
        {
            hasRefInfo = zftrue;
            outData.elementAdd(tmp);
        }
    }

    return hasRefInfo;
}

void resolveReferencedData(ZF_IN_OUT ZFSerializableData &data,
                           ZF_IN const ZFSerializableData &referencedData)
{
    data.referenceRefTypeSet(referencedData.referenceRefType());
    data.referenceRefDataSet(referencedData.referenceRefData());

    for(zfiterator itRef = referencedData.attributeIterator();
        referencedData.attributeIteratorIsValid(itRef);
        referencedData.attributeIteratorNext(itRef))
    {
        const zfchar *refAttrName = referencedData.attributeIteratorGetKey(itRef);
        const zfchar *refAttrValue = referencedData.attributeIteratorGet(itRef);
        if(!zfscmpTheSame(refAttrName, ZFSerializableKeyword_name)
            && !zfscmpTheSame(refAttrValue, ZFSerializableKeyword_category))
        {
            zfiterator it = data.attributeIteratorForName(refAttrName);
            if(data.attributeIteratorIsValid(it)
               && zfscmpTheSame(data.attributeIteratorGet(it), refAttrValue))
            {
                data.attributeIteratorRemove(it);
            }
        }
    }
    for(zfindex i = 0; i < referencedData.elementCount(); ++i)
    {
        zfindex index = zfindexMax;
        const zfchar *tmpName = referencedData.elementAtIndex(i).propertyName();
        if(tmpName != zfnull)
        {
            index = data.elementFindByName(tmpName);
        }
        else
        {
            tmpName = referencedData.elementAtIndex(i).category();
            index = data.elementFindByCategory(tmpName);
        }
        if(index != zfindexMax)
        {
            ZFSerializableData &childData = data.elementAtIndex(index);
            ZFSerializableUtil::resolveReferencedData(childData, referencedData.elementAtIndex(i));
            zfbool shouldRemove = (childData.referenceRefType() == zfnull
                && childData.referenceRefData() == zfnull
                && childData.elementCount() == 0);
            if(shouldRemove)
            {
                if(childData.attributeCount() > 1)
                {
                    shouldRemove = zffalse;
                }
                else if(childData.attributeCount() == 1)
                {
                    zfiterator it = childData.attributeIterator();
                    const zfchar *attrName = childData.attributeIteratorGetKey(it);
                    shouldRemove = (zfscmpTheSame(attrName, ZFSerializableKeyword_name)
                        || zfscmpTheSame(attrName, ZFSerializableKeyword_category));
                }
            }
            if(shouldRemove)
            {
                data.elementRemoveAtIndex(index);
            }
        }
    }
}

static zfbool _ZFP_ZFSerializableUtilPrintResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                                                        ZF_IN const ZFOutputCallback &outputCallback,
                                                        ZF_IN zfindex level)
{
    if(!serializableData.resolvedAll())
    {
        for(zfindex i = 0; i < level; ++i)
        {
            outputCallback << zfText("    ");
        }
        outputCallback << serializableData.itemClass();
        if(!serializableData.resolved())
        {
            outputCallback << zfText("(unresolved)");
        }

        zfbool hasUnresolvedAttribute = zffalse;
        for(zfiterator it = serializableData.attributeIterator();
            serializableData.attributeIteratorIsValid(it);
            serializableData.attributeIteratorNext(it))
        {
            if(!serializableData.attributeIteratorResolved(it))
            {
                if(!hasUnresolvedAttribute)
                {
                    hasUnresolvedAttribute = zftrue;
                    outputCallback << zfText(" < ");
                }
                else
                {
                    outputCallback << zfText("; ");
                }
                outputCallback
                    << serializableData.attributeIteratorGetKey(it)
                    << zfText("=")
                    << serializableData.attributeIteratorGet(it);
            }
        }
        if(hasUnresolvedAttribute)
        {
            outputCallback << zfText(" >");
        }

        outputCallback << zfText("\n");

        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData.elementAtIndex(i), outputCallback, level + 1);
        }
        return zftrue;
    }
    return zffalse;
}
zfbool printResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                          ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */)
{
    zfstring tmp = zfText("not all resolved:\n");
    zfbool ret = _ZFP_ZFSerializableUtilPrintResolveStatus(serializableData, ZFOutputCallbackForString(tmp), 1);
    if(ret)
    {
        outputCallback.execute(tmp.cString());
    }
    return ret;
}

ZF_NAMESPACE_END(ZFSerializableUtil)
ZF_NAMESPACE_GLOBAL_END


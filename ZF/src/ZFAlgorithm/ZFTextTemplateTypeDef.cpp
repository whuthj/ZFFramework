/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTextTemplateTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFTextTemplateIndexFlag)

// ============================================================
void ZFTextTemplateIndexData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;

    zfstringAppend(ret, zfText("%%%zi[%zi%s]"),
        this->indexWidth,
        this->indexRadix,
        this->indexUpperCase ? zfText("x") : zfText("X"));

    if(this->indexOffset >= 0)
    {
        zfstringAppend(ret, zfText("[%zi, %d)"), this->indexStart, this->indexOffset);
    }
    else
    {
        zfstringAppend(ret, zfText("(%d, %zi]"), this->indexOffset, this->indexStart);
    }

    ret += zfText(" ");
    ZFTextTemplateIndexFlagEnumToString(ret, this->indexFlag);

    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFTextTemplateIndexData, ZFTextTemplateIndexData)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(ZFTextTemplateIndexData, ZFTextTemplateIndexData)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFTextTemplateIndexData, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }

    result.indexStart = 0;
    result.indexOffset = 1;
    result.indexRadix = 10;
    result.indexUpperCase = zftrue;
    result.indexWidth = 0;
    result.indexFlag = ZFTextTemplateIndexFlag::e_Default();

    const zfchar *element = zfnull;

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart);
    if(element != zfnull && zfindexFromString(result.indexStart, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset);
    if(element != zfnull && zfintFromString(result.indexOffset, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix);
    if(element != zfnull && zfindexFromString(result.indexRadix, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase);
    if(element != zfnull && zfboolFromString(result.indexUpperCase, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth);
    if(element != zfnull && zfindexFromString(result.indexWidth, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, element);
        return zffalse;
    }

    element = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag);
    if(element != zfnull && ZFTextTemplateIndexFlagEnumFromString(result.indexFlag, element) != zfnull)
    {
        ZFSerializableUtil::errorOccurredWhile(outErrorHintToAppend, outErrorPos, serializableData, ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, element);
        return zffalse;
    }

    serializableData.resolveMark();
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(ZFTextTemplateIndexData, ZFTextTemplateIndexData)
{
    serializableData.itemClassSet(ZFPropertyTypeId_ZFTextTemplateIndexData);

    if(v.indexStart != 0)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart, zfindexToString(v.indexStart));
    }

    if(v.indexOffset != 1)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset, zfintToString(v.indexOffset));
    }

    if(v.indexRadix != 10)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix, zfindexToString(v.indexRadix));
    }

    if(!v.indexUpperCase)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase, zfboolToString(v.indexUpperCase));
    }

    if(v.indexWidth != 0)
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth, zfindexToString(v.indexWidth));
    }

    if(v.indexFlag != ZFTextTemplateIndexFlag::e_Default())
    {
        serializableData.attributeSet(ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag, ZFTextTemplateIndexFlagEnumToString(v.indexFlag));
    }

    return zftrue;
}

// ============================================================
void ZFTextTemplateParam::copyFrom(ZF_IN const ZFTextTemplateParam &ref)
{
    this->replaceDatas.removeAll();
    for(zfiterator it = ref.replaceDatas.iterator(); ref.replaceDatas.iteratorIsValid(it); )
    {
        ZFCoreMapPair pair = ref.replaceDatas.iteratorNextPair(it);
        this->replaceDatas.set(pair.key, ZFCorePointerForObject<zfstring *>(zfnew(zfstring, *(pair.value->pointerValueT<const zfstring *>()))));
    }

    this->enableIfDatas.removeAll();
    for(zfiterator it = ref.enableIfDatas.iterator(); ref.enableIfDatas.iteratorIsValid(it); )
    {
        ZFCoreMapPair pair = ref.enableIfDatas.iteratorNextPair(it);
        this->enableIfDatas.set(pair.key, ZFCorePointerForObject<zfbool *>(zfnew(zfbool, *(pair.value->pointerValueT<const zfbool *>()))));
    }

    this->enableIfByDefault = ref.enableIfByDefault;

    this->indexDatas.removeAll();
    for(zfiterator it = ref.indexDatas.iterator(); ref.indexDatas.iteratorIsValid(it); )
    {
        ZFCoreMapPair pair = ref.indexDatas.iteratorNextPair(it);
        ZFTextTemplateIndexData *indexData = zfnew(ZFTextTemplateIndexData);
        indexData->copyFrom(*(pair.value->pointerValueT<const ZFTextTemplateIndexData *>()));
        this->indexDatas.set(pair.key, ZFCorePointerForObject<ZFTextTemplateIndexData *>(indexData));
    }

    this->indexDataDefault.copyFrom(ref.indexDataDefault);
}
void ZFTextTemplateParam::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfbool first = zftrue;

    if(!this->replaceDatas.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("replace: ");
        this->replaceDatas.objectInfoOfContentT(ret);
    }

    if(!this->enableIfDatas.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("enableIf: ");
        this->enableIfDatas.objectInfoOfContentT(ret);
    }

    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("enableIfByDefault: ");
        zfboolToString(ret, this->enableIfByDefault);
    }

    if(!this->indexDatas.isEmpty())
    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("index: ");
        this->indexDatas.objectInfoOfContentT(ret);
    }

    {
        if(first) {first = zffalse;}
        else {ret += zfText(", ");}

        ret += zfText("indexDataDefault: ");
        this->indexDataDefault.objectInfoT(ret);
    }

    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
ZFPROPERTY_TYPE_DEFINE(ZFTextTemplateParam, ZFTextTemplateParam)
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(ZFTextTemplateParam, ZFTextTemplateParam)
{
    if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_ZFTextTemplateParam, serializableData, outErrorHintToAppend, outErrorPos) == zfnull)
    {
        return zffalse;
    }

    result.replaceDatas.removeAll();
    result.enableIfDatas.removeAll();
    result.enableIfByDefault = zffalse;
    result.indexDatas.removeAll();
    result.indexDataDefault = ZFTextTemplateIndexData();

    const ZFSerializableData *element = zfnull;

    element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_replace);
    if(element != zfnull)
    {
        for(zfindex i = 0; i < element->elementCount(); ++i)
        {
            const ZFSerializableData &item = element->elementAtIndex(i);
            const zfchar *key = item.propertyName();
            if(key == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, item, zfText("missing item name"));
                return zffalse;
            }
            zfstring value;
            if(!zfstringFromSerializableData(value, item, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            result.replaceDatas.set(key, ZFCorePointerForObject<zfstring *>(zfnew(zfstring, value)));
        }
    }

    element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_enableIf);
    if(element != zfnull)
    {
        for(zfindex i = 0; i < element->elementCount(); ++i)
        {
            const ZFSerializableData &item = element->elementAtIndex(i);
            const zfchar *key = item.propertyName();
            if(key == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, item, zfText("missing item name"));
                return zffalse;
            }
            zfbool value = zffalse;
            if(!zfboolFromSerializableData(value, item, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            result.replaceDatas.set(key, ZFCorePointerForObject<zfbool *>(zfnew(zfbool, value)));
        }
    }

    element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_enableIfByDefault);
    if(element != zfnull)
    {
        if(!zfboolFromSerializableData(result.enableIfByDefault, *element, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
    }

    element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_index);
    if(element != zfnull)
    {
        for(zfindex i = 0; i < element->elementCount(); ++i)
        {
            const ZFSerializableData &item = element->elementAtIndex(i);
            const zfchar *key = item.propertyName();
            if(key == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, item, zfText("missing item name"));
                return zffalse;
            }
            ZFTextTemplateIndexData value;
            if(!ZFTextTemplateIndexDataFromSerializableData(value, item, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            result.replaceDatas.set(key, ZFCorePointerForObject<ZFTextTemplateIndexData *>(zfnew(ZFTextTemplateIndexData, value)));
        }
    }

    element = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault);
    if(element != zfnull)
    {
        if(!ZFTextTemplateIndexDataFromSerializableData(result.indexDataDefault, *element, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
    }

    serializableData.resolveMark();
    return zftrue;
}
ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(ZFTextTemplateParam, ZFTextTemplateParam)
{
    serializableData.itemClassSet(ZFPropertyTypeId_ZFTextTemplateParam);

    if(!v.replaceDatas.isEmpty())
    {
        ZFSerializableData nodeData;
        for(zfiterator it = v.replaceDatas.iterator(); v.replaceDatas.iteratorIsValid(it); )
        {
            ZFCoreMapPair pair = v.replaceDatas.iteratorNextPair(it);
            ZFSerializableData itemData;
            if(!zfstringToSerializableData(itemData, *(pair.value->pointerValueT<const zfstring *>()), outErrorHintToAppend))
            {
                return zffalse;
            }
            itemData.propertyNameSet(pair.key);
            nodeData.elementAdd(itemData);
        }

        nodeData.itemClassSet(ZFSerializableKeyword_node);
        nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_replace);
        serializableData.elementAdd(nodeData);
    }

    if(!v.enableIfDatas.isEmpty())
    {
        ZFSerializableData nodeData;
        for(zfiterator it = v.enableIfDatas.iterator(); v.enableIfDatas.iteratorIsValid(it); )
        {
            ZFCoreMapPair pair = v.enableIfDatas.iteratorNextPair(it);
            ZFSerializableData itemData;
            if(!zfboolToSerializableData(itemData, *(pair.value->pointerValueT<const zfbool *>()), outErrorHintToAppend))
            {
                return zffalse;
            }
            itemData.propertyNameSet(pair.key);
            nodeData.elementAdd(itemData);
        }

        nodeData.itemClassSet(ZFSerializableKeyword_node);
        nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_enableIf);
        serializableData.elementAdd(nodeData);
    }

    if(v.enableIfByDefault)
    {
        ZFSerializableData nodeData;
        if(!zfboolToSerializableData(nodeData, v.enableIfByDefault, outErrorHintToAppend))
        {
            return zffalse;
        }
        nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_enableIfByDefault);
        serializableData.elementAdd(nodeData);
    }

    if(!v.indexDatas.isEmpty())
    {
        ZFSerializableData nodeData;
        for(zfiterator it = v.indexDatas.iterator(); v.indexDatas.iteratorIsValid(it); )
        {
            ZFCoreMapPair pair = v.indexDatas.iteratorNextPair(it);
            ZFSerializableData itemData;
            if(!ZFTextTemplateIndexDataToSerializableData(itemData, *(pair.value->pointerValueT<const ZFTextTemplateIndexData *>()), outErrorHintToAppend))
            {
                return zffalse;
            }
            itemData.propertyNameSet(pair.key);
            nodeData.elementAdd(itemData);
        }

        nodeData.itemClassSet(ZFSerializableKeyword_node);
        nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_index);
        serializableData.elementAdd(nodeData);
    }

    {
        ZFSerializableData nodeData;
        if(!ZFTextTemplateIndexDataToSerializableData(nodeData, v.indexDataDefault, outErrorHintToAppend))
        {
            return zffalse;
        }
        if(nodeData.attributeCount() > 0 || nodeData.elementCount() > 0)
        {
            nodeData.categorySet(ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault);
            serializableData.elementAdd(nodeData);
        }
    }

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END


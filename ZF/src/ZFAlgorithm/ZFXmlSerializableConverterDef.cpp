/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFXmlSerializableConverterDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(ZFSerializableDataRefType_xml, serializableData, data, outErrorHintToAppend)
{
    ZFXmlItem xmlElement = ZFXmlParseFirstElement(ZFInputCallbackForFileDescriptor(data));
    if(xmlElement.xmlType() == ZFXmlType::e_XmlNull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("failed to load xml element from \"%s\""), data);
        return zffalse;
    }
    return ZFXmlToSerializableData(serializableData, xmlElement);
}

ZFOBJECT_CREATOR_DEFINE(ZFObjectCreatorType_xml, data)
{
    ZFXmlItem xmlElement = ZFXmlParseFirstElement(ZFInputCallbackForFileDescriptor(data));
    if(xmlElement == zfnull)
    {
        return zfautoObjectNull;
    }
    ZFSerializableData serializableData;
    if(ZFXmlToSerializableData(serializableData, xmlElement))
    {
        return ZFObjectFromSerializableData(serializableData);
    }
    return zfautoObjectNull;
}

zfbool ZFXmlToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                               ZF_IN const ZFXmlItem *xmlElement,
                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                               ZF_OUT_OPT const ZFXmlItem **outErrorPos /* = zfnull */)
{
    if(xmlElement == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("null xml element"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }
    if(xmlElement->xmlType() != ZFXmlType::e_XmlElement)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("param not type of xml element"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }

    if(xmlElement->xmlName() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("missing xml node name"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }
    serializableData.itemClassSet(xmlElement->xmlName());

    const ZFXmlItem *attribute = xmlElement->xmlAttributeFirst();
    while(attribute != zfnull)
    {
        if(attribute->xmlName() == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("missing xml attribute name"));
            if(outErrorPos != zfnull)
            {
                *outErrorPos = attribute;
            }
            return zffalse;
        }
        if(zfscmpTheSame(attribute->xmlName(), ZFSerializableKeyword_refType))
        {
            serializableData.referenceRefTypeSet(attribute->xmlValue());
        }
        else if(zfscmpTheSame(attribute->xmlName(), ZFSerializableKeyword_refData))
        {
            serializableData.referenceRefDataSet(attribute->xmlValue());
        }
        else
        {
            serializableData.attributeSet(attribute->xmlName(), attribute->xmlValue());
        }

        attribute = attribute->xmlAttributeNext();
    }

    const ZFXmlItem *element = xmlElement->xmlChildElementFirst();
    while(element != zfnull)
    {
        ZFSerializableData childData;
        if(!ZFXmlToSerializableData(childData, element, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
        serializableData.elementAdd(childData);
        element = element->xmlSiblingElementNext();
    }

    if(outErrorPos != zfnull)
    {
        *outErrorPos = zfnull;
    }
    return serializableData.referenceCheckLoad(outErrorHintToAppend, zfnull);
}
ZFSerializableData ZFXmlToSerializableData(ZF_IN const ZFXmlItem *xmlElement,
                                           ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                           ZF_OUT_OPT const ZFXmlItem **outErrorPos /* = zfnull */)
{
    ZFSerializableData ret;
    if(ZFXmlToSerializableData(ret, xmlElement, outErrorHintToAppend, outErrorPos))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
zfbool ZFXmlFromSerializableData(ZF_OUT ZFXmlItem &xmlElement,
                                 ZF_IN const ZFSerializableData &serializableData,
                                 ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                 ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    xmlElement = ZFXmlFromSerializableData(serializableData, outErrorHintToAppend, outErrorPos);
    return (xmlElement.xmlType() != ZFXmlType::e_XmlNull);
}
ZFXmlItem ZFXmlFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData, zfText("missing serializable class"));
        return ZFXmlItem();
    }

    ZFXmlItem ret(ZFXmlType::e_XmlElement);
    ret.xmlNameSet(serializableData.itemClass());

    if(serializableData.referenceRefType() != zfnull)
    {
        ret.xmlAttributeAdd(ZFSerializableKeyword_refType, serializableData.referenceRefType());
    }
    if(serializableData.referenceRefData() != zfnull)
    {
        ret.xmlAttributeAdd(ZFSerializableKeyword_refData, serializableData.referenceRefData());
    }

    for(zfiterator it = serializableData.attributeIterator();
        serializableData.attributeIteratorIsValid(it);
        serializableData.attributeIteratorNext(it))
    {
        ret.xmlAttributeAdd(serializableData.attributeIteratorGetKey(it),
            serializableData.attributeIteratorGet(it));
    }

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        ZFXmlItem child = ZFXmlFromSerializableData(serializableData.elementAtIndex(i), outErrorHintToAppend, outErrorPos);
        if(child.xmlType() == ZFXmlType::e_XmlNull)
        {
            return ZFXmlItem();
        }
        ret->xmlChildAdd(child);
    }

    return ret;
}

void ZFXmlPrint(ZF_IN const ZFSerializableData &serializableData,
                ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */,
                ZF_IN_OPT const ZFXmlOutputFlags &flags /* = ZFXmlOutputFlagsDefault */)
{
    ZFXmlItem xmlElement = ZFXmlFromSerializableData(serializableData);
    if(xmlElement.xmlType() != ZFXmlType::e_XmlNull)
    {
        xmlElement->xmlAttributeSortRecursively();
        xmlElement->xmlVisit(ZFXmlVisitCallbackForOutput(outputCallback, flags));
        outputCallback.execute(zfText("\n"));
    }
}
void ZFXmlPrint(ZF_IN ZFObject *obj,
                ZF_IN_OPT const ZFOutputCallback &outputCallback /* = ZFOutputCallbackDefault */,
                ZF_IN_OPT const ZFXmlOutputFlags &flags /* = ZFXmlOutputFlagsDefault */)
{
    ZFXmlPrint(ZFObjectToSerializableData(obj), outputCallback, flags);
}

// ============================================================
zfbool ZFSerializableDataParseXml(ZF_OUT ZFSerializableData &ret, ZF_IN const ZFInputCallback &input)
{
    if(!input.callbackIsValid())
    {
        return zffalse;
    }
    ZFXmlItem xmlElement = ZFXmlParseFirstElement(input);
    if(xmlElement.xmlType() == ZFXmlType::e_XmlNull)
    {
        return zffalse;
    }
    if(!ZFXmlToSerializableData(ret, xmlElement))
    {
        return zffalse;
    }

    {
        ZFString *v = input.callbackTagGet<ZFString *>(ZFCallbackTagKeyword_resPath);
        if(v != zfnull)
        {
            ret.serializableDataTagSetMarkCached(ZFSerializableDataTagKeyword_resPath, v);
        }
        else
        {
            v = input.callbackTagGet<ZFString *>(ZFCallbackTagKeyword_filePath);
            if(v != zfnull)
            {
                ret.serializableDataTagSetMarkCached(ZFSerializableDataTagKeyword_filePath, v);
            }
        }
    }
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFXmlSerializableConverterDef.h
 * @brief serializable convertion between xml and serializalbe
 */

#ifndef _ZFI_ZFXmlSerializableConverterDef_h_
#define _ZFI_ZFXmlSerializableConverterDef_h_

#include "ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// xml serializable conversion

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <Node refType="xml" refData="file descriptor, see ZFInputCallbackForFileDescriptor" />
 * @endcode
 */
#define ZFSerializableDataRefType_xml xml
/** @brief see #ZFSerializableDataRefType_xml */
#define ZFSerializableDataRefTypeName_xml ZFMACRO_TOSTRING(ZFSerializableDataRefType_xml)

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data:
 * @code
 *   "file descriptor, see ZFInputCallbackForFileDescriptor"
 * @endcode
 * data is a file descriptor to a xml file,
 * which should contain a serializable object
 */
#define ZFObjectCreatorType_xml xml
/** @brief see #ZFSerializableDataRefType_xml */
#define ZFObjectCreatorTypeName_xml ZFMACRO_TOSTRING(ZFObjectCreatorType_xml)

/**
 * @brief convert serializable data from xml
 */
extern ZF_ENV_EXPORT zfbool ZFXmlToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                                    ZF_IN const ZFXmlItem *xmlElement,
                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                    ZF_OUT_OPT const ZFXmlItem **outErrorPos = zfnull);
/**
 * @brief see #ZFXmlToSerializableData
 */
extern ZF_ENV_EXPORT ZFSerializableData ZFXmlToSerializableData(ZF_IN const ZFXmlItem *xmlElement,
                                                                ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                ZF_OUT_OPT const ZFXmlItem **outErrorPos = zfnull);
/**
 * @brief convert serializable data from xml, see #ZFXmlToSerializableData
 */
extern ZF_ENV_EXPORT zfbool ZFXmlFromSerializableData(ZF_OUT ZFXmlItem &xmlElement,
                                                      ZF_IN const ZFSerializableData &serializableData,
                                                      ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                      ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
/**
 * @brief see #ZFXmlFromSerializableData
 */
extern ZF_ENV_EXPORT ZFXmlItem ZFXmlFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);

/**
 * @brief util method to print serializable data in xml format, usually for debug use only
 */
extern ZF_ENV_EXPORT void ZFXmlPrint(ZF_IN const ZFSerializableData &serializableData,
                                     ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault,
                                     ZF_IN_OPT const ZFXmlOutputFlags &flags = ZFXmlOutputFlagsDefault);
/**
 * @brief util method to print serializable object in xml format, usually for debug use only
 */
extern ZF_ENV_EXPORT void ZFXmlPrint(ZF_IN ZFObject *obj,
                                     ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault,
                                     ZF_IN_OPT const ZFXmlOutputFlags &flags = ZFXmlOutputFlagsDefault);

ZFOUTPUT_TYPE(ZFXmlItem, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFXmlItem *, {if(v) {output << v->objectInfo();} else {output << (const void *)zfnull;}})
ZFOUTPUT_TYPE(ZFXmlItem *, {output << (const ZFXmlItem *)v;})

// ============================================================
/**
 * @brief util method to parse serializable data from input with xml format
 *
 * this method would automatically setup
 * #ZFSerializableDataTagKeyword_filePath/#ZFSerializableDataTagKeyword_resPath
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataParseXml(ZF_OUT ZFSerializableData &ret, ZF_IN const ZFInputCallback &input);
/** @brief see #ZFSerializableDataParseXml */
inline ZFSerializableData ZFSerializableDataParseXml(ZF_IN const ZFInputCallback &input)
{
    ZFSerializableData ret;
    if(ZFSerializableDataParseXml(ret, input))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFXmlSerializableConverterDef_h_


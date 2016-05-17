/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFJsonSerializableConverterDef.h
 * @brief serializable convertion between json and serializalbe
 */

#ifndef _ZFI_ZFJsonSerializableConverterDef_h_
#define _ZFI_ZFJsonSerializableConverterDef_h_

#include "ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// json serializable conversion

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <Node refType="json" refData="file descriptor, see ZFInputCallbackForFileDescriptor" />
 * @endcode
 */
#define ZFSerializableDataRefType_json json
/** @brief see #ZFSerializableDataRefType_json */
#define ZFSerializableDataRefTypeName_json ZFMACRO_TOSTRING(ZFSerializableDataRefType_json)

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data:
 * @code
 *   "file descriptor, see ZFInputCallbackForFileDescriptor"
 * @endcode
 * data is a file descriptor to a json file,
 * which should contain a serializable object
 */
#define ZFObjectCreatorType_json json
/** @brief see #ZFSerializableDataRefType_json */
#define ZFObjectCreatorTypeName_json ZFMACRO_TOSTRING(ZFObjectCreatorType_json)

/**
 * @brief convert serializable data from json
 */
extern ZF_ENV_EXPORT zfbool ZFJsonToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                                     ZF_IN const ZFJsonItem *jsonObject,
                                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                     ZF_OUT_OPT const ZFJsonItem **outErrorPos = zfnull);
/**
 * @brief see #ZFJsonToSerializableData
 */
extern ZF_ENV_EXPORT ZFSerializableData ZFJsonToSerializableData(ZF_IN const ZFJsonItem *jsonObject,
                                                                 ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                 ZF_OUT_OPT const ZFJsonItem **outErrorPos = zfnull);
/**
 * @brief convert serializable data from json, see #ZFJsonToSerializableData
 */
extern ZF_ENV_EXPORT zfbool ZFJsonFromSerializableData(ZF_OUT ZFJsonItem &jsonObject,
                                                       ZF_IN const ZFSerializableData &serializableData,
                                                       ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                       ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
/**
 * @brief see #ZFJsonFromSerializableData
 */
extern ZF_ENV_EXPORT ZFJsonItem ZFJsonFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                                           ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                           ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);

/**
 * @brief util method to print serializable data in json format, usually for debug use only
 */
extern ZF_ENV_EXPORT void ZFJsonPrint(ZF_IN const ZFSerializableData &serializableData,
                                      ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault,
                                      ZF_IN_OPT const ZFJsonOutputFlags &flags = ZFJsonOutputFlagsDefault);
/**
 * @brief util method to print serializable object in json format, usually for debug use only
 */
extern ZF_ENV_EXPORT void ZFJsonPrint(ZF_IN ZFObject *obj,
                                      ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault,
                                      ZF_IN_OPT const ZFJsonOutputFlags &flags = ZFJsonOutputFlagsDefault);

ZFOUTPUT_TYPE(ZFJsonItem, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFJsonItem *, {if(v) {output << v->objectInfo();} else {output << (const void *)zfnull;}})
ZFOUTPUT_TYPE(ZFJsonItem *, {output << (const ZFJsonItem *)v;})

// ============================================================
/**
 * @brief util method to parse serializable data from input with json format
 *
 * this method would automatically setup
 * #ZFSerializableDataTagKeyword_filePath/#ZFSerializableDataTagKeyword_resPath
 */
extern ZF_ENV_EXPORT zfbool ZFSerializableDataParseJson(ZF_OUT ZFSerializableData &ret, ZF_IN const ZFInputCallback &input);
/** @brief see #ZFSerializableDataParseJson */
inline ZFSerializableData ZFSerializableDataParseJson(ZF_IN const ZFInputCallback &input)
{
    ZFSerializableData ret;
    if(ZFSerializableDataParseJson(ret, input))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFJsonSerializableConverterDef_h_


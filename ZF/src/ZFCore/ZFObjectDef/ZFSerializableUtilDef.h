/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableUtilDef.h
 * @brief utilities for ZFSerializable's implementation
 */

#ifndef _ZFI_ZFSerializableUtilDef_h_
#define _ZFI_ZFSerializableUtilDef_h_

#include "ZFSerializableDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN
ZF_NAMESPACE_BEGIN(ZFSerializableUtil)

extern ZF_ENV_EXPORT ZFCoreArray<ZFOutputCallback> &_ZFP_ZFSerializableUtilErrorOutputCallbacks(void);
/**
 * @brief global output callback that would be called when any serialization error occurred,
 *   empty by default, usually for debug use only
 *
 * note: add this may cause performance issue, recommended used for debug only
 */
#define ZFSerializableUtilErrorOutputCallbacks ZFSerializableUtil::_ZFP_ZFSerializableUtilErrorOutputCallbacks()

/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurred(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                                        ZF_OUT_OPT const ZFSerializableData **outErrorPos,
                                        ZF_IN const ZFSerializableData &errorPos,
                                        ZF_IN const zfchar *fmt,
                                        ...);

/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurred(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                                        ZF_IN const zfchar *fmt,
                                        ...);

/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                                             ZF_OUT_OPT const ZFSerializableData **outErrorPos,
                                             ZF_IN const ZFSerializableData &errorPos,
                                             ZF_IN const zfchar *serializingName,
                                             ZF_IN const zfchar *errorValue);
/**
 * @brief util method to set error
 */
extern ZF_ENV_EXPORT void errorOccurredWhile(ZF_OUT_OPT zfstring *outErrorHintToAppend,
                                             ZF_IN const zfchar *serializingName,
                                             ZF_IN const zfchar *errorValue);

/**
 * @brief util to check whether the serializable data contains desired attribute
 *
 * use null desiredClass shows that any class name is allowed except empty\n
 * return null if no match
 */
extern ZF_ENV_EXPORT const zfchar *checkSerializableClass(ZF_IN const zfchar *desiredClass,
                                                          ZF_IN const ZFSerializableData &serializableData);
/**
 * @brief see #checkSerializableClass, output error hint if failed
 */
extern ZF_ENV_EXPORT const zfchar *requireSerializableClass(ZF_IN const zfchar *desiredClass,
                                                            ZF_IN const ZFSerializableData &serializableData,
                                                            ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                            ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains desired attribute,
 *   auto mark as resolved
 *
 * return null if not exist
 */
extern ZF_ENV_EXPORT const zfchar *checkAttribute(ZF_IN const ZFSerializableData &serializableData,
                                                  ZF_IN const zfchar *desiredAttribute);
/**
 * @brief see #checkAttribute, output error hint if failed,
 *   auto mark as resolved
 */
extern ZF_ENV_EXPORT const zfchar *requireAttribute(ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_IN const zfchar *desiredAttribute,
                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto skip resolved data,
 *   auto mark name as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *checkElementByName(ZF_IN const ZFSerializableData &ownerData,
                                                                  ZF_IN const zfchar *desiredElementName);
/**
 * @brief see #checkElementByName, output error hint if failed,
 *   auto skip resolved data,
 *   auto mark name as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *requireElementByName(ZF_IN const ZFSerializableData &ownerData,
                                                                    ZF_IN const zfchar *desiredElementName,
                                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains desired element,
 *   auto skip resolved data,
 *   auto mark category as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *checkElementByCategory(ZF_IN const ZFSerializableData &ownerData,
                                                                      ZF_IN const zfchar *desiredElementCategory);
/**
 * @brief see #checkElementByCategory, output error hint if failed,
 *   auto skip resolved data,
 *   auto mark category as resolved
 */
extern ZF_ENV_EXPORT const ZFSerializableData *requireElementByCategory(ZF_IN const ZFSerializableData &ownerData,
                                                                        ZF_IN const zfchar *desiredElementCategory,
                                                                        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                        ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);

/**
 * @brief util to check whether the serializable data contains property name attribute,
 *   auto mark as resolved
 */
inline const zfchar *checkPropertyName(ZF_IN const ZFSerializableData &serializableData)
{
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_name);
}
/**
 * @brief see #checkPropertyName, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requirePropertyName(ZF_IN const ZFSerializableData &serializableData,
                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull)
{
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_name, outErrorHintToAppend, outErrorPos);
}

/**
 * @brief util to check whether the serializable data contains property value attribute,
 *   auto mark as resolved
 */
inline const zfchar *checkPropertyValue(ZF_IN const ZFSerializableData &serializableData)
{
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_value);
}
/**
 * @brief see #checkPropertyValue, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requirePropertyValue(ZF_IN const ZFSerializableData &serializableData,
                                          ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                          ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull)
{
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_value, outErrorHintToAppend, outErrorPos);
}

/**
 * @brief util to check whether the serializable data contains category,
 *   auto mark as resolved
 */
inline const zfchar *checkCategory(ZF_IN const ZFSerializableData &serializableData)
{
    return ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_category);
}
/**
 * @brief see #checkCategory, output error hint if failed,
 *   auto mark as resolved
 */
inline const zfchar *requireCategory(ZF_IN const ZFSerializableData &serializableData,
                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                     ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull)
{
    return ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_category, outErrorHintToAppend, outErrorPos);
}

/**
 * @brief util method to copy ref info, return true if any ref info found
 */
extern ZF_ENV_EXPORT zfbool copyRefInfo(ZF_IN_OUT ZFSerializableData &outData,
                                        ZF_IN const ZFSerializableData &data);

/**
 * @brief util method to remove unnecessary nodes in data, accorrding referencedData
 *
 * the referencedData must have #ZFSerializableData::referenceCheckLoad called before invoke this method\n
 * \n
 * this method would first remove all duplicated attributes in data accorrding referencedData,
 * then remove all nodes that have no child and no attributes (or only have attribute "name" and/or "category")\n
 * \n
 * for example, assume we have this data
 * @code
 *   <Owner>
 *       <Type0 name="name0">
 *           <Type1 name="name1" value="value1" />
 *           <Type2 name="name2" value="value2" />
 *       </Type0>
 *   </Owner>
 * @endcode
 * and it has this related referenced data:
 * @code
 *   <null>
 *       <null ref="ref datas..." >
 *           <Type1 name="name1" value="value1" />
 *       </null>
 *   </null>
 * @endcode
 * then the resolved result data would be like:
 * @code
 *   <Owner>
 *       <Type0 name="name0" ref="ref datas..." >
 *           <Type2 name="name2" value="value2" />
 *       </Type0>
 *   </Owner>
 * @endcode
 */
extern ZF_ENV_EXPORT void resolveReferencedData(ZF_IN_OUT ZFSerializableData &data,
                                                ZF_IN const ZFSerializableData &referencedData);

/**
 * @brief util method to print resolve status of ZFSerializableData, for debug use only,
 *   output nothing if all resolved
 *
 * return true if anything not resolved, or false if all resolved
 */
extern ZF_ENV_EXPORT zfbool printResolveStatus(ZF_IN const ZFSerializableData &serializableData,
                                               ZF_IN_OPT const ZFOutputCallback &outputCallback = ZFOutputCallbackDefault);

ZF_NAMESPACE_END(ZFSerializableUtil)
ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableUtilDef_h_


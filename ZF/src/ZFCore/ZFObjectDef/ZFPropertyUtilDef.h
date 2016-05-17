/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyUtilDef.h
 * @brief utility for ZFProperty
 */

#ifndef _ZFI_ZFPropertyUtilDef_h_
#define _ZFI_ZFPropertyUtilDef_h_

#include "ZFPropertyDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFPropertyIsValueAccessed
/**
 * @brief check whether the property is in init value state,
 *   see #ZFPropertyCallbackIsValueAccessed
 */
extern ZF_ENV_EXPORT zfbool ZFPropertyIsValueAccessed(ZF_IN const ZFProperty *propertyInfo,
                                                      ZF_IN ZFObject *ownerObj);

// ============================================================
// ZFPropertyIsInitValue
/**
 * @brief check whether the property is in init value state,
 *   see #ZFPropertyCallbackIsInitValue
 */
extern ZF_ENV_EXPORT zfbool ZFPropertyIsInitValue(ZF_IN const ZFProperty *propertyInfo,
                                                  ZF_IN ZFObject *ownerObj);

// ============================================================
// ZFPropertyResetInitValue
/**
 * @brief reset the property to its init value state,
 *   see #ZFPropertyCallbackResetInitValue
 */
extern ZF_ENV_EXPORT void ZFPropertyResetInitValue(ZF_IN const ZFProperty *propertyInfo,
                                                   ZF_IN ZFObject *ownerObj);

// ============================================================
// ZFPropertyCompare
/**
 * @brief compare two object's property value,
 *   see #ZFPropertyCallbackCompare
 */
extern ZF_ENV_EXPORT ZFCompareResult ZFPropertyCompare(ZF_IN const ZFProperty *propertyInfo,
                                                       ZF_IN ZFObject *obj0,
                                                       ZF_IN ZFObject *obj1,
                                                       ZF_OUT_OPT zfbool *successfullyCompared = zfnull);

// ============================================================
// ZFPropertyCompare
/**
 * @brief copy property value to dstObj from srcObj
 *   see #ZFPropertyCallbackCopy
 */
extern ZF_ENV_EXPORT zfbool ZFPropertyCopy(ZF_IN const ZFProperty *propertyInfo,
                                           ZF_IN ZFObject *dstObj,
                                           ZF_IN ZFObject *srcObj);

// ============================================================
// retain property reflect
/**
 * @brief set a retain property without knowing the type,
 *   return false if src's type not match the property's type
 *
 * see #ZFPropertyCallbackRetainSet, #ZFProperty::callbackRetainSet
 */
extern ZF_ENV_EXPORT zfbool ZFPropertyRetainSet(ZF_IN const ZFProperty *propertyInfo,
                                                ZF_IN ZFObject *dstObj,
                                                ZF_IN ZFObject *src);
/** @brief see #ZFPropertyRetainSet */
extern ZF_ENV_EXPORT ZFObject *ZFPropertyRetainGet(ZF_IN const ZFProperty *propertyInfo,
                                                   ZF_IN ZFObject *ownerObj,
                                                   ZF_OUT_OPT zfbool *success = zfnull);

// ============================================================
// assign property reflect
/**
 * @brief set a assign property
 *
 * you must ensure the src's type is correct
 * (or safe and valid to cast to desired type by reinterpret pointer cast),
 * otherwise, data may be invalid and app may crash
 *
 * see #ZFPropertyCallbackAssignSet, #ZFProperty::callbackAssignSet
 */
extern ZF_ENV_EXPORT zfbool ZFPropertyAssignSet(ZF_IN const ZFProperty *propertyInfo,
                                                ZF_IN ZFObject *dstObj,
                                                ZF_IN void *src,
                                                ZF_IN const zfchar *srcTypeName = ZFPropertyTypeId_none);
/** @brief see #ZFPropertyAssignSet */
extern ZF_ENV_EXPORT const void *ZFPropertyAssignGet(ZF_IN const ZFProperty *propertyInfo,
                                                     ZF_IN ZFObject *ownerObj);
/** @brief see #ZFPropertyAssignSet */
template<typename T_Type>
const T_Type *ZFPropertyAssignGet(ZF_IN const ZFProperty *propertyInfo,
                                  ZF_IN ZFObject *ownerObj)
{
    const void *ret = ZFPropertyAssignGet(propertyInfo, ownerObj);
    if(ret == zfnull)
    {
        return zfnull;
    }
    return ZFCastStatic(const T_Type *, ret);
}

// ============================================================
// ZFPropertyCopyAll
/**
 * @brief copy all properties from srcObj to dstObj, filtered by filter
 *
 * this function would go through all properties in dstObj,
 * then search whether there are same property in srcObj,
 * if there are, copy them from srcObj to dstObj by the reflectable getter and setter\n
 * search is done by property's name compare and type compare,
 * and type is compared by ZFProperty::propertyTypeIdName\n
 * it's your responsibility to make sure the copy is valid
 * @note this function may cost much time if there are many properties in the inherit tree,
 *   you may want to use #ZFProperty::callbackCopy directly for performance
 */
extern ZF_ENV_EXPORT void ZFPropertyCopyAll(ZF_IN ZFObject *dstObj,
                                            ZF_IN ZFObject *srcObj,
                                            ZF_IN_OPT const ZFPropertyFilter *filter = zfnull,
                                            ZF_OUT_OPT ZFCoreArrayPOD<const ZFProperty *> *copiedProperties = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyUtilDef_h_


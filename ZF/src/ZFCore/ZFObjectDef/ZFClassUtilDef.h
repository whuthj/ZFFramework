/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFClassUtilDef.h
 * @brief util for ZFClass
 */

#ifndef _ZFI_ZFClassUtilDef_h_
#define _ZFI_ZFClassUtilDef_h_

#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFObject;
zfclassFwd ZFClassFilter;
zfclassFwd ZFMethodFilter;
zfclassFwd ZFPropertyFilter;

ZF_NAMESPACE_BEGIN(ZFClassUtil)

/**
 * @brief util to get all parent class
 */
extern ZF_ENV_EXPORT ZFCoreArrayPOD<const ZFClass *> allClass(ZF_IN const ZFClass *cls,
                                                              ZF_IN_OPT const ZFClassFilter *filter = zfnull);

/**
 * @brief util to get all method in inherit tree
 */
extern ZF_ENV_EXPORT ZFCoreArrayPOD<const ZFMethod *> allMethod(ZF_IN const ZFClass *cls,
                                                                ZF_IN_OPT const ZFMethodFilter *filter = zfnull);

/**
 * @brief util to get all property in inherit tree
 */
extern ZF_ENV_EXPORT ZFCoreArrayPOD<const ZFProperty *> allProperty(ZF_IN const ZFClass *cls,
                                                                    ZF_IN_OPT const ZFPropertyFilter *filter = zfnull);

/**
 * @brief compare all properties of obj0 with obj1's property by reflect
 *
 * if both object is null, return true\n
 * two object have no need to be same class,
 * but must be ensured that all property are safe to be compared,
 * i.e. each property's type must match
 */
extern ZF_ENV_EXPORT zfbool allPropertyIsEqual(ZF_IN ZFObject *obj0,
                                               ZF_IN ZFObject *obj1,
                                               ZF_IN_OPT const ZFPropertyFilter *filter = zfnull);

/**
 * @brief use reflect to print all property if the property is not #ZFPropertyIsInitValue
 */
extern ZF_ENV_EXPORT void objectPropertyInfo(ZF_IN_OUT zfstring &ret,
                                             ZF_IN ZFObject *obj,
                                             ZF_IN_OPT zfindex maxCount = zfindexMax,
                                             ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault);
/** @brief see #objectPropertyInfo */
inline zfstring objectPropertyInfo(ZF_IN ZFObject *obj,
                                   ZF_IN_OPT zfindex maxCount = zfindexMax,
                                   ZF_IN_OPT const ZFTokenForKeyValueContainer &token = ZFTokenForKeyValueContainerDefault)
{
    zfstring ret;
    ZFClassUtil::objectPropertyInfo(ret, obj, maxCount, token);
    return ret;
}

/**
 * @brief get short info of object, see #objectPropertyInfo
 */
extern ZF_ENV_EXPORT void objectInfoT(ZF_IN_OUT zfstring &ret,
                                      ZF_IN ZFObject *obj);
/** @brief see #objectInfo */
inline zfstring objectInfo(ZF_IN ZFObject *obj)
{
    zfstring ret;
    ZFClassUtil::objectInfoT(ret, obj);
    return ret;
}

ZF_NAMESPACE_END(ZFClassUtil)
ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFClassUtilDef_h_


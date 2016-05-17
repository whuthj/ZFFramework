/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectCreator_common.h
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 */

#ifndef _ZFI_ZFObjectCreator_common_h_
#define _ZFI_ZFObjectCreator_common_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data is a string containing class name and method name,
 * which should be like this:\n
 *   "YourClass::YourMethod"\n
 * \n
 * YourClass must be a ZFObject or ZFInterface type (accessed by #ZFClass::classForName)\n
 * YourMethod must be a #ZFMethod with this proto type:\n
 *   static zfautoObject yourMethod(void);\n
 * no method id supported
 */
#define ZFObjectCreatorType_memberMethod memberMethod
/** @brief see #ZFObjectCreatorType_memberMethod */
#define ZFObjectCreatorTypeName_memberMethod ZFMACRO_TOSTRING(ZFObjectCreatorType_memberMethod)

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * similar to #ZFObjectCreatorType_memberMethod,
 * but method should be declared by #ZFMETHOD_FUNCTION_DECLARE_0,
 * and method is reflected by #ZFMethodAccessFunction\n
 * data is "MethodNamespace::methodName:methodId",
 * "MethodNamespace" is optional (while "::" should be reserved),
 * ":methodId" is optional (":" can be omitted
 */
#define ZFObjectCreatorType_localMethod localMethod
/** @brief see #ZFObjectCreatorType_localMethod */
#define ZFObjectCreatorTypeName_localMethod ZFMACRO_TOSTRING(ZFObjectCreatorType_localMethod)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCreator_common_h_


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCallbackSerializableDef.h
 * @brief serialization for ZFCallback
 */

#ifndef _ZFI_ZFCallbackSerializableDef_h_
#define _ZFI_ZFCallbackSerializableDef_h_

#include "ZFCallbackDef.h"
#include "ZFSerializableDef.h"
#include "ZFPropertyTypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   // for #ZFCallbackNull
 *   <null />
 *
 *   // for #ZFCallbackForMethod
 *   <ZFCallback>
 *       <ZFMethod name="method" ... />
 *   </ZFCallback>
 *
 *   // for #ZFCallbackForMemberMethod
 *   <ZFCallback>
 *       <zfstring name="owner" ... />
 *       <ZFMethod name="method" ... />
 *   </ZFCallback>
 *
 *   // for #ZFCallbackForRawFunction
 *   // not supported
 * @endcode
 * \n
 * ADVANCED:\n
 * since class member method's callback need a object instance to create,
 * we use a special "owner" node to store <string, ownerObject> map,
 * you should properly set the owner map by #ZFSerializableOwnerTagSet,
 * before serializing the callback,
 * and the callback should have same owner tag set by #ZFCallback::callbackSerializableOwnerTagSet\n
 * \n
 * ADVANCED:\n
 * you may also supply your own serialize logic by registering
 * with #ZFCallbackSerializeCustomTypeRegister,
 * serializable data:
 * @code
 *   <ZFCallback>
 *       <zfstring category="type" ... />
 *       <something category="data" ... >
 *           // custom datas
 *       </something>
 *   </ZFCallback>
 * @endcode
 */
#define ZFPropertyTypeId_ZFCallback zfText("ZFCallback")
ZFPROPERTY_TYPE_DECLARE(ZFCallback, ZFCallback)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_owner zfText("owner")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_method zfText("method")

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_type zfText("type")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_data zfText("data")

// ============================================================
// custom serialize logic
/** @brief see #ZFCallbackSerializeCustomTypeRegister */
typedef zfbool (*ZFCallbackSerializeCustomSerializeCallback)(ZF_IN_OUT ZFCallback &result,
                                                             ZF_IN const ZFSerializableData &serializableData,
                                                             ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                             ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */);
/**
 * @brief see #ZFPropertyTypeId_ZFCallback
 *
 * typically you should use #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE to define your type for short:
 * @code
 *   // in header file
 *   #define ZFCallbackSerializeCustomType_yourType yourType
 *   #define ZFCallbackSerializeCustomTypeName_yourType ZFMACRO_TOSTRING(ZFCallbackSerializeCustomType_yourType)
 *
 *   // in cpp file
 *   ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(yourType)
 *   {
 *       // serialize callback from data
 *       return zftrue;
 *   }
 * @endcode
 * proto type:\n
 *   zfbool action(ZF_IN_OUT ZFCallback &result,
 *                 ZF_IN const ZFSerializableData &serializableData,
 *                 ZF_OUT_OPT zfstring *outErrorHintToAppend,
 *                 ZF_OUT_OPT const ZFSerializableData **outErrorPos);
 */
extern ZF_ENV_EXPORT void ZFCallbackSerializeCustomTypeRegister(ZF_IN const zfchar *customType,
                                                                ZFCallbackSerializeCustomSerializeCallback serializeCallback);
/** @brief see #ZFPropertyTypeId_ZFCallback */
extern ZF_ENV_EXPORT void ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType);
/** @brief see #ZFPropertyTypeId_ZFCallback */
extern ZF_ENV_EXPORT ZFCallbackSerializeCustomSerializeCallback ZFCallbackSerializeCustomTypeGet(ZF_IN const zfchar *customType);

#define _ZFP_ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(type) \
    static zfbool _ZFP_ZFCallbackSerializeCustomSerializeCallback_##type(ZF_IN_OUT ZFCallback &result, \
                                                                         ZF_IN const ZFSerializableData &serializableData, \
                                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */, \
                                                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCallbackSerializeCallbackRegister_##type, ZFLevelZFFrameworkNormal) \
    { \
        ZFCallbackSerializeCustomTypeRegister(ZFCallbackSerializeCustomTypeName_##type, _ZFP_ZFCallbackSerializeCustomSerializeCallback_##type); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFCallbackSerializeCallbackRegister_##type) \
    { \
        ZFCallbackSerializeCustomTypeUnregister(ZFCallbackSerializeCustomTypeName_##type); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFCallbackSerializeCallbackRegister_##type) \
    static zfbool _ZFP_ZFCallbackSerializeCustomSerializeCallback_##type(ZF_IN_OUT ZFCallback &result, \
                                                                         ZF_IN const ZFSerializableData &serializableData, \
                                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */, \
                                                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
/**
 * @brief see #ZFCallbackSerializeCustomTypeRegister
 */
#define ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(type) \
    _ZFP_ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(type)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallbackSerializableDef_h_


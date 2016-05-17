/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyTypeDef.h
 * @brief type define for ZFProperty
 */

#ifndef _ZFI_ZFPropertyTypeDef_h_
#define _ZFI_ZFPropertyTypeDef_h_

#include "ZFPropertyDef.h"
#include "ZFSerializableDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

template<typename T_Type>
zfclassNotPOD _ZFP_ZFPropertyDefaultValue
{
public:
    static inline T_Type value(void)
    {
        return T_Type();
    }
};

// ============================================================
/**
 * @brief register a property type
 *
 * usually internal use only,
 * ZFFramework would supply most of the types,
 * however you may use this to register your own types\n
 * \n
 * typical code to register a type:
 * @code
 *   // ============================================================
 *   // in h file
 *   // declare your type, name must be unique
 *   #define ZFPropertyTypeId_YourType zfText("YourType")
 *   ZFPROPERTY_TYPE_DECLARE(YourType, YourType)
 *
 *   // ============================================================
 *   // in cpp file
 *   // register your type
 *   ZFPROPERTY_TYPE_DEFINE(YourType, YourType)
 *   // supply converter from/to ZFSerializableData:
 *   ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(YourType, YourType)
 *   { ... }
 *   ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(YourType, YourType)
 *   { ... }
 *
 *   // or, you may use #ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE for short,
 *   // if your type support converter from/to string that suits these proto type:
 *   //   const zfchar *YourTypeFromString(ZF_OUT Type &,
 *                                        ZF_IN const zfchar *);
 *   //   zfstring YourTypeToString(ZF_IN Type const &);
 *   ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(YourType, YourType, ZFPropertyTypeId_YourType)
 * @endcode
 *
 * once registered, your type can be used as #ZFPROPERTY_ASSIGN
 * which benefits from the powerful automatic serialization logic
 */
#define ZFPROPERTY_TYPE_DECLARE(TypeName, Type) \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    extern ZF_ENV_EXPORT zfbool TypeName##FromSerializableData(ZF_OUT Type &result, \
                                                               ZF_IN const ZFSerializableData &serializableData, \
                                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull, \
                                                               ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull); \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    inline Type TypeName##FromSerializableData(ZF_IN const ZFSerializableData &serializableData, \
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull, \
                                               ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull) \
    { \
        Type ret; \
        if(TypeName##FromSerializableData(ret, serializableData, outErrorHintToAppend, outErrorPos)) \
        { \
            return ret; \
        } \
        else \
        { \
            return _ZFP_ZFPropertyDefaultValue<Type>::value() ;\
        } \
        return ret; \
    } \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    extern ZF_ENV_EXPORT zfbool TypeName##ToSerializableData(ZF_OUT ZFSerializableData &serializableData, \
                                                             ZF_IN Type const &v, \
                                                             ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull); \
    /** @brief see #ZFPROPERTY_TYPE_DECLARE */ \
    inline ZFSerializableData TypeName##ToSerializableData(ZF_IN Type const &v, \
                                                           ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull) \
    { \
        ZFSerializableData ret; \
        if(TypeName##ToSerializableData(ret, v, outErrorHintToAppend)) \
        { \
            return ret; \
        } \
        else \
        { \
            return ZFSerializableData(); \
        } \
    }
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 */
#define ZFPROPERTY_TYPE_DEFINE(TypeName, Type) \
    zfclassNotPOD _ZFP_ZFPropertyTypeRegisterHolder_##TypeName \
    { \
    public: \
        static zfbool serializeFrom(ZF_IN const ZFProperty *propertyInfo, \
                                    ZF_IN ZFObject *ownerObject, \
                                    ZF_IN const ZFSerializableData &serializableData, \
                                    ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull, \
                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull) \
        { \
            if(ZFSerializableUtil::requireSerializableClass(propertyInfo->propertyTypeIdName(), serializableData, outErrorHintToAppend, outErrorPos) == zfnull) \
            { \
                return zffalse; \
            } \
            Type v; \
            if(!TypeName##FromSerializableData(v, serializableData, outErrorHintToAppend, outErrorPos)) \
            { \
                return zffalse; \
            } \
            propertyInfo->setterMethod()->executeClassMember<void, Type const &>(ownerObject, v); \
            return zftrue; \
        } \
        static zfbool serializeTo(ZF_IN const ZFProperty *propertyInfo, \
                                  ZF_IN ZFObject *ownerObject, \
                                  ZF_OUT ZFSerializableData &serializableData, \
                                  ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull) \
        { \
            serializableData.itemClassSet(propertyInfo->propertyTypeIdName()); \
            serializableData.propertyNameSet(propertyInfo->propertyName()); \
            Type const &v = propertyInfo->getterMethod()->executeClassMember<Type const &>(ownerObject); \
            return TypeName##ToSerializableData(serializableData, v, outErrorHintToAppend); \
        } \
    }; \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyTypeIdHolder_##TypeName, ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFPropertyTypeRegister( \
            ZFPropertyTypeId_##TypeName, \
            _ZFP_ZFPropertyTypeRegisterHolder_##TypeName::serializeFrom, \
            _ZFP_ZFPropertyTypeRegisterHolder_##TypeName::serializeTo); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFPropertyTypeIdHolder_##TypeName) \
    { \
        _ZFP_ZFPropertyTypeUnregister(ZFPropertyTypeId_##TypeName); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFPropertyTypeIdHolder_##TypeName)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * proto type:
 * @code
 *   zfbool YourTypeFromSerializableData(ZF_OUT YourType &result,
 *                                       ZF_IN const ZFSerializableData &serializableData,
 *                                       ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
 *                                       ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
 * @endcode
 * \n
 * you should:
 * -  check whether the property's type match the serializableData's type
 * -  serialize from serializableData
 * -  set the property by the property's setter method
 */
#define ZFPROPERTY_TYPE_DECLARE_SERIALIZE_FROM_DEFINE(TypeName, Type) \
    zfbool TypeName##FromSerializableData(ZF_OUT Type &result, \
                                          ZF_IN const ZFSerializableData &serializableData, \
                                          ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */, \
                                          ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * proto type:
 * @code
 *   zfbool YourTypeToSerializableData(ZF_OUT ZFSerializableData &serializableData,
 *                                     ZF_IN YourType const &v,
 *                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
 * @endcode
 * \n
 * you should:
 * -  get the property's value by the property's getter method
 * -  save typeid to the serializableData
 * -  save property name to the serializableData
 * -  save property value to the serializableData
 *
 * note:
 * propertyInfo's typeid is ensured valid
 */
#define ZFPROPERTY_TYPE_DECLARE_SERIALIZE_TO_DEFINE(TypeName, Type) \
    zfbool TypeName##ToSerializableData(ZF_OUT ZFSerializableData &serializableData, \
                                        ZF_IN Type const &v, \
                                        ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)

/** @brief see #ZFPROPERTY_TYPE_DECLARE */
#define ZFPROPERTY_TYPE_DECLARE_SERIALIZE_CONVERTER_DEFINE(TypeName, Type, ZFPropertyTypeId_) \
    zfbool TypeName##FromSerializableData(ZF_OUT Type &result, \
                                          ZF_IN const ZFSerializableData &serializableData, \
                                          ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */, \
                                          ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */) \
    { \
        if(ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_, serializableData, outErrorHintToAppend, outErrorPos) == zfnull) \
        { \
            return zffalse; \
        } \
        const zfchar *propertyValue = ZFSerializableUtil::checkPropertyValue(serializableData); \
        if(propertyValue == zfnull) \
        { \
            result = _ZFP_ZFPropertyDefaultValue<Type>::value(); \
            return zftrue; \
        } \
        if(TypeName##FromString(result, propertyValue) != zfnull) \
        { \
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData, \
                zfText("invalid value: \"%s\""), propertyValue); \
            return zffalse; \
        } \
        serializableData.resolveMark(); \
        return zftrue; \
    } \
    zfbool TypeName##ToSerializableData(ZF_OUT ZFSerializableData &serializableData, \
                                        ZF_IN Type const &v, \
                                        ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */) \
    { \
        serializableData.itemClassSet(ZFPropertyTypeId_); \
        zfstring s; \
        TypeName##ToString(s, v); \
        serializableData.propertyValueSet(s.cString()); \
        return zftrue; \
    }

// ============================================================
/**
 * @brief see #ZFPropertyTypeGetSerializeFromCallback
 */
typedef zfbool (*ZFPropertyTypeSerializeFromCallback)(ZF_IN const ZFProperty *propertyInfo,
                                                      ZF_IN ZFObject *ownerObject,
                                                      ZF_IN const ZFSerializableData &serializableData,
                                                      ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                      ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */);
/**
 * @brief see #ZFPropertyTypeGetSerializeToCallback
 */
typedef zfbool (*ZFPropertyTypeSerializeToCallback)(ZF_IN const ZFProperty *propertyInfo,
                                                    ZF_IN ZFObject *ownerObject,
                                                    ZF_OUT ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyTypeRegister(ZF_IN const zfchar *typeIdName,
                                                      ZF_IN ZFPropertyTypeSerializeFromCallback serializeFromCallback,
                                                      ZF_IN ZFPropertyTypeSerializeToCallback serializeToCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyTypeUnregister(ZF_IN const zfchar *typeIdName);

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * directly access the serialize callback, for performance use only
 */
extern ZF_ENV_EXPORT ZFPropertyTypeSerializeFromCallback ZFPropertyTypeGetSerializeFromCallback(ZF_IN const zfchar *typeIdName);
/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * directly access the serialize callback, for performance use only
 */
extern ZF_ENV_EXPORT ZFPropertyTypeSerializeToCallback ZFPropertyTypeGetSerializeToCallback(ZF_IN const zfchar *typeIdName);

/**
 * @brief serialize property from serializable data, see #ZFPROPERTY_TYPE_DECLARE
 */
extern ZF_ENV_EXPORT zfbool ZFPropertySerializeFrom(ZF_IN ZFObject *ownerObject,
                                                    ZF_IN const ZFSerializableData &serializableData,
                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
/**
 * @brief serialize property to serializable data, see #ZFPROPERTY_TYPE_DECLARE
 */
extern ZF_ENV_EXPORT zfbool ZFPropertySerializeTo(ZF_IN const ZFProperty *propertyInfo,
                                                  ZF_IN ZFObject *ownerObject,
                                                  ZF_OUT ZFSerializableData &serializableData,
                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

// ============================================================
/**
 * @brief see #ZFPropertyTypeInfoGetterRegister
 */
typedef void (*ZFPropertyTypeInfoGetterCallback)(ZF_IN_OUT zfstring &ret,
                                                 ZF_IN const ZFProperty *propertyInfo,
                                                 ZF_IN ZFObject *ownerObject);

/**
 * @brief register an info getter for #ZFProperty
 *
 * this is an optional reflection type registration for ZFProperty,
 * used to get a short info of a ZFProperty within its owner object,
 * typically for debug use only
 */
extern ZF_ENV_EXPORT void ZFPropertyTypeInfoGetterRegister(ZF_IN const zfchar *typeIdName,
                                                           ZF_IN ZFPropertyTypeInfoGetterCallback propertyTypeInfoGetterCallback);
/**
 * @brief see #ZFPropertyTypeInfoGetterRegister
 */
extern ZF_ENV_EXPORT void ZFPropertyTypeInfoGetterUnregister(ZF_IN const zfchar *typeIdName);

/**
 * @brief see #ZFPropertyTypeInfoGetterRegister
 */
#define ZFPROPERTY_TYPE_INFO_GETTER_REGISTER(TypeName, Type) \
    zfclassNotPOD _ZFP_ZFPropertyTypeInfoGetter_##TypeName \
    { \
    public: \
        static void action(ZF_IN_OUT zfstring &ret, \
                           ZF_IN const ZFProperty *propertyInfo, \
                           ZF_IN ZFObject *ownerObject) \
        { \
            ZFOutputCallbackForString(ret) << propertyInfo->getterMethod()->executeClassMember<Type const &>(ownerObject); \
        } \
    }; \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyTypeInfoGetterHolder_##TypeName, ZFLevelZFFrameworkNormal) \
    { \
        ZFPropertyTypeInfoGetterRegister( \
            ZFPropertyTypeId_##TypeName, \
            _ZFP_ZFPropertyTypeInfoGetter_##TypeName::action); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFPropertyTypeInfoGetterHolder_##TypeName) \
    { \
        ZFPropertyTypeInfoGetterUnregister(ZFPropertyTypeId_##TypeName); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFPropertyTypeInfoGetterHolder_##TypeName)

/**
 * @brief see #ZFPropertyTypeInfoGetterRegister
 */
extern ZF_ENV_EXPORT ZFPropertyTypeInfoGetterCallback ZFPropertyTypeGetInfoGetterCallback(ZF_IN const zfchar *typeIdName);

/**
 * @brief get property instance info, see #ZFPropertyTypeInfoGetterRegister
 */
extern ZF_ENV_EXPORT void ZFPropertyGetInfo(ZF_IN_OUT zfstring &ret,
                                            ZF_IN const ZFProperty *propertyInfo,
                                            ZF_IN ZFObject *ownerObject);
/** @brief see #ZFPropertyGetInfo */
inline zfstring ZFPropertyGetInfo(ZF_IN const ZFProperty *propertyInfo,
                                  ZF_IN ZFObject *ownerObject)
{
    zfstring ret;
    ZFPropertyGetInfo(ret, propertyInfo, ownerObject);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyTypeDef_h_

#include "ZFPropertyTypeDef_CoreType.h"


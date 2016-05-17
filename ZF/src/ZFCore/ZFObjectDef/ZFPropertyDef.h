/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFPropertyDef.h
 * @brief property for ZFObject type
 */

#ifndef _ZFI_ZFPropertyDef_h_
#define _ZFI_ZFPropertyDef_h_

#include "ZFMethodDef.h"
#include "ZFObjectClassTypeFwd.h"
#include "ZFObjectCastDef.h"
#include "ZFAnyDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFProperty;
// ============================================================
/**
 * @brief used to check whether the property has been accessed
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsValueAccessed(propertyInfo, ownerObj);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyIsValueAccessed if necessary
 */
typedef zfbool (*ZFPropertyCallbackIsValueAccessed)(ZF_IN const ZFProperty *property,
                                                    ZF_IN ZFObject *ownerObj);
/**
 * @brief used to check whether the property is in init value state
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackIsInitValue(propertyInfo, ownerObj);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyIsInitValue if necessary
 */
typedef zfbool (*ZFPropertyCallbackIsInitValue)(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *ownerObj);
/**
 * @brief used to reset the property to its init value state
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackResetInitValue(propertyInfo, ownerObj);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyResetInitValue if necessary
 */
typedef void (*ZFPropertyCallbackResetInitValue)(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *ownerObj);
/**
 * @brief used to compare two object's property
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackCompare(propertyInfo, obj0, obj1);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyCompare if necessary
 */
typedef ZFCompareResult (*ZFPropertyCallbackCompare)(ZF_IN const ZFProperty *property,
                                                     ZF_IN ZFObject *obj0,
                                                     ZF_IN ZFObject *obj1);
/**
 * @brief used to copy property from different object
 *
 * usage:
 * @code
 *   const ZFProperty *propertyInfo = ...;
 *   propertyInfo->callbackCopy(propertyInfo, dstObj, srcObj);
 * @endcode
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyCopyAll if necessary
 */
typedef void (*ZFPropertyCallbackCopy)(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFObject *dstObj,
                                       ZF_IN ZFObject *srcObj);
/**
 * @brief used to set retain property without knowing the type
 *
 * do nothing if error
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyRetainSet if necessary
 */
typedef void (*ZFPropertyCallbackRetainSet)(ZF_IN const ZFProperty *property,
                                            ZF_IN ZFObject *dstObj,
                                            ZF_IN ZFObject *src);
/**
 * @brief used to get retain property without knowing the type
 *
 * return null if error
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyRetainGet if necessary
 */
typedef ZFObject *(*ZFPropertyCallbackRetainGet)(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *ownerObj);
/**
 * @brief used to set assign property without knowing the type
 *
 * you must ensure the src's type is correct
 * (or safe and valid to cast to desired type by reinterpret pointer cast),
 * otherwise, data may be invalid and app may crash
 * @note this method has no type or runtime safe check,
 *   use #ZFPropertyAssignSet if necessary
 */
typedef void (*ZFPropertyCallbackAssignSet)(ZF_IN const ZFProperty *property,
                                            ZF_IN ZFObject *dstObj,
                                            ZF_IN void *src);
/**
 * @brief used to get assign property without knowing the type
 *
 * return the property's address as (const void *) pointer,
 * or null if error
 */
typedef const void *(*ZFPropertyCallbackAssignGet)(ZF_IN const ZFProperty *property,
                                                   ZF_IN ZFObject *ownerObj);

// ============================================================
/**
 * @brief shows that the property has no type and can not be used in advanced property copy operation,
 *   ensured an empty string
 */
#define ZFPropertyTypeId_none zfText("")

#define _ZFP_ZFPropertyTypeId_ZFObject zfText("_ZFP_ZFPropertyTypeId_ZFObject")

// ============================================================
/**
 * @brief info for a property for ZFObject, see #ZFPROPERTY_RETAIN for more info
 */
zffinal zfclassNotPOD ZF_ENV_EXPORT ZFProperty
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFProperty)

public:
    /**
     * @brief whether two property's type id is matched,
     *   true if two type string is equal or both of them is #ZFPropertyTypeId_none
     */
    static zfbool propertyTypeIdIsMatch(ZF_IN const zfchar *type0, ZF_IN const zfchar *type1);

public:
    /**
     * @brief get info about this property
     */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief get the property's owner class
     */
    inline const ZFClass *ownerClass(void) const
    {
        return this->_ZFP_ZFProperty_ownerClass;
    }
    /**
     * @brief name for the property
     *
     * assume property's name is "myProperty",
     * then the setter would be "myPropertySet",
     * and the getter would be "myProperty",
     * and getter name would return "myProperty"
     */
    inline const zfchar *propertyName(void) const
    {
        return this->_ZFP_ZFProperty_name.cString();
    }
    /**
     * @brief type string for the property
     *
     * note the type string is the macro expand from the type,
     * it may or may not be same for same type,
     * usually for debug use only
     */
    inline const zfchar *propertyTypeName(void) const
    {
        return this->_ZFP_ZFProperty_typeName.cString();
    }
    /**
     * @brief typeid string declared in ZFPROPERTY_XXX
     *
     * this value should be ensured the type id for the type or #ZFPropertyTypeId_none if no known type,
     * this value is used for property's advanced copy function,
     * see #ZFPropertySerializeFrom
     * @note for retain property, this value is always #ZFPropertyTypeId_none
     */
    inline const zfchar *propertyTypeIdName(void) const
    {
        return this->_ZFP_ZFProperty_typeIdName.cString();
    }
    /**
     * @brief whether this property is declared as serializable
     *
     * by default, all property can be serialized,
     * you may explicitly declare a property not serializable by
     * -  for assign property, declare property with #ZFPropertyTypeId_none as type id
     * -  for retain property, using #ZFPROPERTY_RETAIN_NOT_SERIALIZABLE
     *
     * a non-serializable property would be ignored while serializing its owner object
     */
    inline zfbool propertyIsSerializable(void) const
    {
        return this->_ZFP_ZFProperty_serializable;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *setterMethod(void) const
    {
        return this->_ZFP_ZFProperty_setterMethod;
    }
    /**
     * @brief get the getter method
     */
    inline const ZFMethod *getterMethod(void) const
    {
        return this->_ZFP_ZFProperty_getterMethod;
    }

    /**
     * @brief true if the property is retain property
     *
     * note that bool property is also a assign property,
     * it also have a getter method named "propertyName"
     */
    inline zfbool propertyIsRetainProperty(void) const
    {
        return (this->callbackRetainSet != zfnull);
    }
    /**
     * @brief for retain property only, get the retain property's declared class
     *
     * note it's the declared class, property's value may be subclass of it
     */
    inline const ZFClass *propertyClassOfRetainProperty(void) const
    {
        return this->_ZFP_ZFProperty_propertyClassOfRetainProperty;
    }

public:
    /**
     * @brief used to check whether the property has been accessed
     */
    ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed;
    /**
     * @brief used to check whether the property is in init value state
     */
    ZFPropertyCallbackIsInitValue callbackIsInitValue;
    /**
     * @brief used to reset the property to its init value state
     */
    ZFPropertyCallbackResetInitValue callbackResetInitValue;
    /**
     * @brief used to compare two object's property value
     */
    ZFPropertyCallbackCompare callbackCompare;
    /**
     * @brief used to copy property from another object
     */
    ZFPropertyCallbackCopy callbackCopy;
    /**
     * @brief see #ZFPropertyRetainSet
     */
    ZFPropertyCallbackRetainSet callbackRetainSet;
    /**
     * @brief see #ZFPropertyRetainGet
     */
    ZFPropertyCallbackRetainGet callbackRetainGet;
    /**
     * @brief see #ZFPropertyAssignSet
     */
    ZFPropertyCallbackAssignSet callbackAssignSet;
    /**
     * @brief see #ZFPropertyAssignGet
     */
    ZFPropertyCallbackAssignGet callbackAssignGet;

public:
    /** @cond ZFPrivateDoc */
    ZFProperty(void);
    ~ZFProperty(void);
    zfbool _ZFP_ZFPropertyNeedInit;
    void _ZFP_ZFPropertyInit(ZF_IN const ZFClass *ownerClass,
                             ZF_IN const zfchar *name,
                             ZF_IN const zfchar *typeName,
                             ZF_IN const zfcharA *typeIdName,
                             ZF_IN const ZFMethod *setterMethod,
                             ZF_IN const ZFMethod *getterMethod,
                             ZF_IN const ZFClass *propertyClassOfRetainProperty,
                             ZF_IN ZFPropertyCallbackIsValueAccessed callbackIsValueAccessed,
                             ZF_IN ZFPropertyCallbackIsInitValue callbackIsInitValue,
                             ZF_IN ZFPropertyCallbackResetInitValue callbackResetInitValue,
                             ZF_IN ZFPropertyCallbackCompare callbackCompare,
                             ZF_IN ZFPropertyCallbackCopy callbackCopy);
    /** @endcond */
private:
    const ZFClass *_ZFP_ZFProperty_ownerClass;
    zfstring _ZFP_ZFProperty_name;
    zfstring _ZFP_ZFProperty_typeName;
    zfstring _ZFP_ZFProperty_typeIdName;
    zfbool _ZFP_ZFProperty_serializable;
    const ZFMethod *_ZFP_ZFProperty_setterMethod;
    const ZFMethod *_ZFP_ZFProperty_getterMethod;

    const ZFClass *_ZFP_ZFProperty_propertyClassOfRetainProperty;
};

// ============================================================
/**
 * @brief ensured be logged by ZFLeakTest
 * @see ZFPropertyChange
 */
#define ZFPropertyChangeWithLeakTest(property, newValue) \
    do \
    { \
        ZFAny _ZFP_ZFPropertyChangeTmpValue = property; \
        ZFCoreMutexLock(); \
        zflockfree_zfRetainWithLeakTest(property = newValue); \
        zflockfree_zfReleaseWithLeakTest(_ZFP_ZFPropertyChangeTmpValue.toObject()); \
        ZFCoreMutexUnlock(); \
    } while(zffalse)
/** @brief no lock version of #ZFPropertyChangeWithLeakTest, use with causion */
#define zflockfree_ZFPropertyChangeWithLeakTest(property, newValue) \
    do \
    { \
        ZFAny _ZFP_ZFPropertyChangeTmpValue = property; \
        zflockfree_zfRetainWithLeakTest(property = newValue); \
        zflockfree_zfReleaseWithLeakTest(_ZFP_ZFPropertyChangeTmpValue.toObject()); \
    } while(zffalse)
/**
 * @brief ensured not be logged by ZFLeakTest
 * @see ZFPropertyChange
 */
#define ZFPropertyChangeWithoutLeakTest(property, newValue) \
    do \
    { \
        ZFAny _ZFP_ZFPropertyChangeTmpValue = property; \
        ZFCoreMutexLock(); \
        zflockfree_zfRetainWithoutLeakTest(property = newValue); \
        zflockfree_zfReleaseWithoutLeakTest(_ZFP_ZFPropertyChangeTmpValue.toObject()); \
        ZFCoreMutexUnlock(); \
    } while(zffalse)
/** @brief no lock version of #ZFPropertyChangeWithoutLeakTest, use with causion */
#define zflockfree_ZFPropertyChangeWithoutLeakTest(property, newValue) \
    do \
    { \
        ZFAny _ZFP_ZFPropertyChangeTmpValue = property; \
        zflockfree_zfRetainWithoutLeakTest(property = newValue); \
        zflockfree_zfReleaseWithoutLeakTest(_ZFP_ZFPropertyChangeTmpValue.toObject()); \
    } while(zffalse)

/**
 * @brief for retain property only,
 *   release property's old value, retain newValue, then set to property
 *
 * this macro is similar to the retain property in Object-C\n
 * typical usage:
 * @code
 *   ZFObject *property = ...;
 *   ZFObject *newProperty = ...;
 *
 *   // OK, release property, retain newProperty, then set to property
 *   ZFPropertyChange(property, newProperty);
 *
 *   // OK, use return value of a function as new value
 *   // but keep it in mind, that the new value will be retained
 *   ZFPropertyChange(property, funcThatReturnZFObject());
 *
 *   // OK, same as release old property and set it to zfnull
 *   ZFPropertyChange(property, zfnull);
 *
 *   // error, newValue must be ZFObject
 *   // ZFPropertyChange(property, 123);
 *
 *   // error, property must be a variable contains a (ZFObject *)
 *   // ZFPropertyChange(zfnull, newProperty);
 *   // ZFPropertyChange(funcThatReturnZFObject(), newProperty);
 * @endcode
 * @see zfRetain, zfRelease, ZFPROPERTY_RETAIN
 */
#if ZF_LEAKTEST_ENABLE
    #define ZFPropertyChange(property, newValue) ZFPropertyChangeWithLeakTest(property, newValue)
#else
    #define ZFPropertyChange(property, newValue) ZFPropertyChangeWithoutLeakTest(property, newValue)
#endif

/**
 * @brief log leak test if ZF_LEAKTEST_ENABLE_INTERNAL is zftrue,
 *   internal use only
 */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define ZFPropertyChangeInternal(property, newValue) zflockfree_ZFPropertyChangeWithLeakTest(property, newValue)
#else
    #define ZFPropertyChangeInternal(property, newValue) zflockfree_ZFPropertyChangeWithoutLeakTest(property, newValue)
#endif
/** @brief no lock version of #ZFPropertyChangeInternal, use with causion */
#if ZF_LEAKTEST_ENABLE_INTERNAL
    #define zflocked_ZFPropertyChangeInternal(property, newValue) ZFPropertyChangeWithLeakTest(property, newValue)
#else
    #define zflocked_ZFPropertyChangeInternal(property, newValue) ZFPropertyChangeWithoutLeakTest(property, newValue)
#endif

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDef_h_

#include "ZFPropertyDeclareDef.h"
#include "ZFPropertyFilterDef.h"
#include "ZFPropertyUtilDef.h"


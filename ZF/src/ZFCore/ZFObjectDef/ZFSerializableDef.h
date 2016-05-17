/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDef.h
 * @brief serializable interface
 */

#ifndef _ZFI_ZFSerializableDef_h_
#define _ZFI_ZFSerializableDef_h_

#include "ZFSerializableDataDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief keyword for #ZFSerializable to hold styleable's type,
 *   see #ZFSerializable::serializableStyleableTypeSet
 */
#define ZFSerializableKeyword_styleableType zfText("styleableType")
/**
 * @brief keyword for #ZFSerializable to hold styleable's data,
 *   see #ZFSerializable::serializableStyleableTypeSet
 */
#define ZFSerializableKeyword_styleableData zfText("styleableData")
/**
 * @brief keyword for #ZFSerializable to hold owner tag,
 *   see #ZFSerializableOwnerTagSet
 */
#define ZFSerializableKeyword_ownerTag zfText("ownerTag")

/**
 * @brief used for #ZFSerializable to override default constructor
 *
 * by default, serializable would be created by #ZFClass::newInstance while serializing from data,
 * you may supply this method to override:
 * -  static zfautoObject serializableNewInstance(void);
 *
 * the method should be supplied as #ZFMethod, and is recommended to register it statically by #ZFMETHOD_REGISTER\n
 * the method should return a newly created object, or retain your existing singleton instance\n
 * typically this method is used to achieve some singleton logic
 */
#define ZFSerializableKeyword_serializableNewInstanceSig serializableNewInstance
/**
 * @brief see #ZFSerializableKeyword_serializableNewInstanceSig
 */
#define ZFSerializableKeyword_serializableNewInstance ZFMACRO_TOSTRING(ZFSerializableKeyword_serializableNewInstanceSig)

zfclassFwd _ZFP_ZFSerializable_PropertyTypeHolder;
// ============================================================
/**
 * @brief base class of call serializable object
 *
 * a serializable object can be encoded to and decoded from a string data,
 * use #ZFSerializableData to store necessary data\n
 * a ZFSerializableData can hold these datas:
 * -  serializable class:
 *   ZFObject's class name or other non-ZFObject's type name,
 *   such as "ZFString", "zfstring" and "zfint"
 * -  reference info:
 *   used to hold reference info,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE for more info
 * -  property name:
 *   used only when the serializable belongs to another serializable,
 *   it's the property name,
 *   and is ensured stored in attributes with "name" as the attribute name
 * -  property value:
 *   used only when the serializable can be converted directly to a type,
 *   and is ensured stored in attributes with "value" as the attribute name
 * -  category:
 *   used to mark the node should be resolved specially,
 *   and is ensured stored in attributes with "category" as the attribute name
 * -  attributes:
 *   used to hold extra type datas for the owner
 * -  elements:
 *   used to hold properties of the serializable,
 *   it's a ZFSerializableData too
 *
 * ZFSerializableData can be converted from and to xml elements,
 * to make it easier to understand,
 * here's a typical serializable data in xml format that shows the types:
 * @code
 *   // assume we have a object hold a ZFArray as retain property:
 *   zfclass TestClass : zfextends ZFSerializable
 *   {
 *       ZFOBJECT_DECLARE(TestClass, ZFSerializable)
 *       ZFPROPERTY_RETAIN(ZFArray *, testProperty)
 *       ...
 *   };
 *
 *   // we have a ZFSerializableData like:
 *   <TestClass test="test">
 *       <ZFArray name="testProperty">
 *           <ZFString>
 *               <zfstring name="src" value="string content" />
 *           </ZFString>
 *       </ZFArray>
 *       <SomeType category="CategoryName" />
 *   </TestClass>
 * @endcode
 * in this example:
 * -  the "TestClass" in "<TestClass>" is a serializable class
 * -  the "src" in "<zfstring name="src"..." is a property name
 * -  the "string content" in "<zfstring ... value="string content" />" is a property value
 * -  the "test="test"" in "<TestClass test="test">" is a attribute
 * -  the "category" in "<SomeType category="CategoryName" />" is a category
 *   that should be resolved by subclass during #serializableOnSerializeCategoryFromData
 *
 * we have these builtin keywords for serializable data,
 * you should not use them as attribute name:
 * -  "name":
 *   shows the serializable is it's parent's property,
 *   and the name is the property's name
 * -  "value":
 *   for basic type only, the value for the basic type
 * -  "refType", "refData":
 *   for advanced reference logic,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  "category":
 *   if exist this attribute,
 *   ZFSerializable would ignore this node and leave it to subclass to decode
 * -  "editMode":
 *   if exist this attribute,
 *   ZFSerializable would ignore this node and store it as raw data for future process,
 *   see #ZFSerializable::editModeData
 * -  "styleableType", "styleableData"
 *   for advanced styleable logic, see #ZFSTYLE_DEFAULT_DECLARE for more info
 * -  "ownerTag"
 *   for serializing elements that reference to external object,
 *   see #ZFSerializableOwnerTagSet
 *
 * \n
 * a simplest way to implement ZFSerializable is:
 * -  declare your object, and make it implement ZFSerializable,
 *   by default, all #ZFProperty would be auto serialized
 *
 * \n
 * if your object has extra serialize step to do, you may want to:
 * -  override #serializableOnCheck to check serializable depending on current object state
 * -  override one or more of these methods to achieve custom serialize step:
 *   -  #serializableOnSerializeCategoryFromData / #serializableOnSerializeCategoryToData
 *   -  #serializableOnSerializePropertyFromData / #serializableOnSerializePropertyToData
 *   -  #serializableOnSerializeEmbededPropertyFromData / #serializableOnSerializeEmbededPropertyToData
 *   -  #serializableOnSerializeFromData / #serializableOnSerializeToData
 * -  take care of referencedOwnerOrNull while serialize to data,
 *   where you should implement reference logic for your custom serialize step\n
 *   by default, serializable property and embeded property's reference logic
 *   would be done by ZFSerializable automatically,
 *   but you should take care of category's reference logic manually
 *
 *
 * \n
 * typically you should override
 * #serializableOnSerializeCategoryFromData and #serializableOnSerializeCategoryToData
 * to supply custom serialize step\n
 * \n
 * ADVANCED:\n
 * serializable would be created by #ZFClass::newInstance while serializing from data,
 * you may supply your custom constructor,
 * see #ZFSerializableKeyword_serializableNewInstance\n
 * \n
 * ADVANCED:\n
 * serializable also supply styleable logic:
 * @code
 *   <YourSerializable styleableType="type" styleableData="data" />
 * @endcode
 * the reserved keyword "styleableType" and "styleableData" shows the styleable's source,
 * which would be created by #ZFObjectCreate,
 * created object must be #ZFSerializable and #ZFStyleable,
 * and would be copied to the object being serialized by #ZFStyleable::styleableCopyFrom\n
 * for typical styleable usage, see #ZFSTYLE_DEFAULT_DECLARE
 * @note styleable logic and reference logic can not be used together
 * @note comparing with reference logic,
 *   styleable logic would create a styleable object first then copy style from it,
 *   reference logic would load reference to serializable data then serialize from it,
 *   usually reference logic would have better performance,
 *   and styleable logic would have better flexibility
 */
zfinterface ZF_ENV_EXPORT ZFSerializable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFSerializable, ZFInterface)

    // ============================================================
    // edit mode
public:
    /** @brief see #ZFSerializable::editModeData */
    zfclassLikePOD ZF_ENV_EXPORT EditModeData
    {
    public:
        /** @brief see #ZFSerializable::editModeData */
        const ZFClass *wrappedClass;
    };
    /**
     * @brief internal use only
     *
     * map of <className, #ZFSerializable::EditModeData>\n
     * used to store class data that currently not registered,
     * so that it can be serialized to data without data loss\n
     * \n
     * for normal serialize logic, we will reflect class type by #ZFClass::classForName,
     * so if the class is not registered currently,
     * we are unable to find it,
     * such as some plugin designed module,
     * can't be found until plugin has been loaded\n
     * to resolve the problem, we introduced this editMode,
     * which can map unknown type to existing class,
     * so that unknown type's serialize step can be done normally
     * with the logic of existing class\n
     * \n
     * edit mode data stores unresolved class name and serializable data to
     * #editModeWrappedClassName and #editModeWrappedElementDatas,
     * which should be resolved later
     */
    static ZFCoreMap &editModeData(void);
    /** @brief see #ZFSerializable::editModeData */
    static zfbool &editMode(void);
private:
    zfchar *_ZFP_ZFSerializable_editModeWrappedClassName;
    ZFCoreArray<ZFSerializableData> *_ZFP_ZFSerializable_editModeWrappedElementDatas;
public:
    /** @brief see #ZFSerializable::editModeData */
    virtual const zfchar *editModeWrappedClassName(void);
    /** @brief see #ZFSerializable::editModeData */
    virtual void editModeWrappedClassNameSet(ZF_IN const zfchar *value);
    /** @brief see #ZFSerializable::editModeData */
    virtual ZFCoreArray<ZFSerializableData> &editModeWrappedElementDatas(void);

    // ============================================================
protected:
    /** @cond ZFPrivateDoc */
    ZFSerializable(void)
    : _ZFP_ZFSerializable_editModeWrappedClassName(zfnull)
    , _ZFP_ZFSerializable_editModeWrappedElementDatas(zfnull)
    , _ZFP_ZFSerializable_propertyTypeHolder(zfnull)
    , _ZFP_ZFSerializable_referenceInfoMap(zfnull)
    , _ZFP_ZFSerializable_selfRefType(zfnull)
    , _ZFP_ZFSerializable_selfRefData(zfnull)
    , _ZFP_ZFSerializable_styleableType(zfnull)
    , _ZFP_ZFSerializable_styleableData(zfnull)
    , _ZFP_ZFSerializable_ownerTag(zfnull)
    {
    }
    /** @endcond */
    virtual ~ZFSerializable(void);

public:
    /**
     * @brief true if object is currently serializable, see #ZFSerializable
     *
     * subclass should override #serializableOnCheck to check whether serializable\n
     * some object may be serializable or not depends on content
     * @note you must check super's state first if override
     * @see ZFSerializable
     */
    zffinal zfbool serializable(void);
    /**
     * @brief serialize from data, see #ZFSerializable
     *
     * note that for performance, this method won't check whether serializable before execute
     */
    zffinal zfbool serializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                     ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
    /**
     * @brief serialize to data, see #ZFSerializable
     *
     * note that for performance, this method won't check whether serializable before execute\n
     * \n
     * ADVANCED:\n
     * the param named "referenced" is used to override ref/style logic,
     * all result serializable data that same as the referenced's one
     * would be ignored,
     * this is usually for internal use only
     */
    zffinal zfbool serializeToData(ZF_OUT ZFSerializableData &serializableData,
                                   ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                   ZF_IN_OPT ZFSerializable *referencedObject = zfnull);

private:
    _ZFP_ZFSerializable_PropertyTypeHolder *_ZFP_ZFSerializable_propertyTypeHolder;
    zffinal _ZFP_ZFSerializable_PropertyTypeHolder *_ZFP_ZFSerializable_getPropertyTypeHolder(void);
public:
    /**
     * @brief get all serializable property, usually for debug only, see #serializableOnCheckPropertyType
     */
    zffinal const ZFCoreArrayPOD<const ZFProperty *> serializableGetAllSerializableProperty(void);
    /**
     * @brief get all serializable embeded property, usually for debug only, see #serializableOnCheckPropertyType
     */
    zffinal const ZFCoreArrayPOD<const ZFProperty *> serializableGetAllSerializableEmbededProperty(void);

public:
    /**
     * @brief serializable property type, see #ZFSerializable::serializableOnCheckPropertyType
     */
    typedef enum {
        /**
         * @brief see #ZFSerializable::serializableOnCheckPropertyType
         */
        PropertyTypeNotSerializable,
        /**
         * @brief see #ZFSerializable::serializableOnCheckPropertyType
         */
        PropertyTypeSerializableProperty,
        /**
         * @brief see #ZFSerializable::serializableOnCheckPropertyType
         */
        PropertyTypeEmbededProperty,
    } PropertyType;
protected:
    /**
     * @brief check the property type that serializable should do what while serializing
     *
     * properties declared in ZFSerializalbe have these types:
     * -  not serializable:
     *   -  the property is not serializable and should be manually serialized if necessary
     * -  normal serializable property:
     *   -  the property would be serialized automatically
     *     during #serializableOnSerializePropertyFromData and #serializableOnSerializePropertyToData
     *   -  while serializing from data,
     *     ZFSerializable will serialize a property's new instance and then set to the property
     *   -  by default, a property is treated as normal serializable property if:
     *     -  the property's setter and getter is not private
     *     -  the property is retain property and its type is ZFSerializable
     *     -  the property is assign property and its type is registered by #ZFPROPERTY_TYPE_DECLARE
     * -  embeded serializable property:\n
     *   -  the property would be serialized automatically
     *     during #serializableOnSerializeEmbededPropertyFromData and #serializableOnSerializeEmbededPropertyToData
     *   -  while serializing from data,
     *     ZFSerializable will directly serialize the data to property instance
     *     (do nothing if property is null)
     *   -  by default, a property is treated as normal serializable property if:
     *     -  the property is retain property and its type is ZFSerializable
     *     -  the property's setter is private and getter is not private
     *   -  if a property is an embeded property,
     *     you must ensure it's not null while serializing,
     *     otherwise, serializing would fail
     *
     * \n
     * subclass may override this method to make ZFSerializable ignore or force serialize some property,
     * but you must make sure it's logical valid\n
     * ignored property (i.e. PropertyTypeNotSerializable) can be manually serialized
     * during #serializableOnSerializeFromData and #serializableOnSerializeToData
     */
    virtual ZFSerializable::PropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);

protected:
    /**
     * @brief see #serializable
     */
    virtual zfbool serializableOnCheck(void)
    {
        return zftrue;
    }

    /**
     * @brief for serializable data that has "category" attribute,
     *   ZFSerializable would ignore it and leave it to subclass to resolve,
     *   see #ZFSerializable
     *
     * while overriding this method, you should call super first,
     * and then check whether super has resolved the data\n
     * if subclass should resolve the category,
     * you should mark data as resolved and return whether resolve success\n
     * if not, subclass should leave the data unresoved and return true
     */
    virtual inline zfbool serializableOnSerializeCategoryFromData(ZF_IN const ZFSerializableData &ownerData,
                                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                  ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull)
    {
        return zftrue;
    }
    /**
     * @brief corresponding to #serializableOnSerializeCategoryFromData,
     *   return whether the task is success,
     *   see #ZFSerializable
     *
     * while overriding this method,
     * you should manage the reference logic manually,
     * see #serializableRefInfoSet
     */
    virtual inline zfbool serializableOnSerializeCategoryToData(ZF_IN_OUT ZFSerializableData &ownerData,
                                                                ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull)
    {
        return zftrue;
    }
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * if subclass override this method, you should check whether it's resolved by parent,
     * and then mark data as resolved and return whether resolve success
     * @code
     *   zfbool YourType::serializableOnSerializePropertyFromData(...)
     *   {
     *       if(!SuperSerializable::serializableOnSerializePropertyFromData(...))
     *       {
     *           return zffalse;
     *       }
     *       if(categoryData.resolved())
     *       {
     *           return zftrue;
     *       }
     *
     *       // mark resolve if you have resolved
     *       // or don't mark to leave it to subclass
     *       propertyData.resolveMark();
     *
     *       return zftrue;
     *   }
     * @endcode
     */
    virtual zfbool serializableOnSerializePropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                           ZF_IN const ZFProperty *property,
                                                           ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                           ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * set serializable class to null to show the property is in init value state
     * and have no need to be serialized
     */
    virtual zfbool serializableOnSerializePropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                         ZF_IN const ZFProperty *property,
                                                         ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     */
    virtual zfbool serializableOnSerializeEmbededPropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                                  ZF_IN const ZFProperty *property,
                                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                  ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
    /**
     * @brief see #serializableOnCheckPropertyType, usually you have no need to override this method,
     *   see #ZFSerializable
     *
     * set serializable class to null to show the property is in init value state
     * and have no need to be serialized
     */
    virtual zfbool serializableOnSerializeEmbededPropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                                ZF_IN const ZFProperty *property,
                                                                ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
    /**
     * @brief see #serializeFromData
     */
    virtual inline zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                          ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                          ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull)
    {
        return zftrue;
    }
    /**
     * @brief see #serializeToData
     */
    virtual inline zfbool serializableOnSerializeToData(ZF_OUT ZFSerializableData &serializableData,
                                                        ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull)
    {
        return zftrue;
    }

public:
    /**
     * @brief get info as a serializable
     */
    virtual void serializableGetInfoT(ZF_IN_OUT zfstring &ret);
    /** @brief see #serializableGetInfoT */
    virtual inline zfstring serializableGetInfo(void)
    {
        zfstring ret;
        this->serializableGetInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief internal use only
     *
     * used to copy serializable info from another serializable,
     * so that this object can serialize to data with the same behavior
     * of the source serializable object\n
     * the anotherSerializable must be same as this object
     */
    virtual void serializableCopyInfoFrom(ZF_IN ZFSerializable *anotherSerializable);

private:
    ZFCoreMap *_ZFP_ZFSerializable_referenceInfoMap; // ZFSerializableData *
public:
    /**
     * @brief used to store reference info for subclass,
     *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE for more info
     *
     * reference info would be stored as #ZFSerializableData,
     * you should only store refType and refData by
     * #ZFSerializableData::referenceRefTypeSet and #ZFSerializableData::referenceRefDataSet,
     * or store child reference info by add child data\n
     * reference info would be cleared automatically before next time you serializing from data
     */
    zffinal void serializableRefInfoSet(ZF_IN const zfchar *key,
                                        ZF_IN const ZFSerializableData *referenceInfo);
    /**
     * @brief see #serializableRefInfoSet
     */
    zffinal void serializableRefInfoSet(ZF_IN const zfchar *key,
                                        ZF_IN const ZFSerializableData &referenceInfo)
    {
        this->serializableRefInfoSet(key, &referenceInfo);
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    zffinal const ZFSerializableData *serializableRefInfoGet(ZF_IN const zfchar *key);

    /**
     * @brief see #serializableRefInfoSet
     */
    inline void serializableRefInfoSetForProperty(ZF_IN const ZFProperty *property,
                                                  ZF_IN const ZFSerializableData *referenceInfo)
    {
        this->serializableRefInfoSet(zfstringWithFormat(zfText("p:%s"), property->propertyName()).cString(), referenceInfo);
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline void serializableRefInfoSetForProperty(ZF_IN const ZFProperty *property,
                                                  ZF_IN const ZFSerializableData &referenceInfo)
    {
        this->serializableRefInfoSetForProperty(property, &referenceInfo);
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline const ZFSerializableData *serializableRefInfoGetForProperty(ZF_IN const ZFProperty *property)
    {
        return this->serializableRefInfoGet(zfstringWithFormat(zfText("p:%s"), property->propertyName()).cString());
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline void serializableRefInfoSetForCategory(ZF_IN const zfchar *category,
                                                  ZF_IN const ZFSerializableData *referenceInfo)
    {
        this->serializableRefInfoSet(zfstringWithFormat(zfText("c:%s"), category).cString(), referenceInfo);
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline void serializableRefInfoSetForCategory(ZF_IN const zfchar *category,
                                                  ZF_IN const ZFSerializableData &referenceInfo)
    {
        this->serializableRefInfoSetForCategory(category, &referenceInfo);
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline const ZFSerializableData *serializableRefInfoGetForCategory(ZF_IN const zfchar *category)
    {
        return this->serializableRefInfoGet(zfstringWithFormat(zfText("c:%s"), category).cString());
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline void serializableRefInfoSetForClass(ZF_IN const ZFClass *cls,
                                               ZF_IN const zfchar *name,
                                               ZF_IN const ZFSerializableData *referenceInfo)
    {
        this->serializableRefInfoSet(zfstringWithFormat(zfText("%s:%s"), cls->className(), name).cString(), referenceInfo);
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline void serializableRefInfoSetForClass(ZF_IN const ZFClass *cls,
                                               ZF_IN const zfchar *name,
                                               ZF_IN const ZFSerializableData &referenceInfo)
    {
        this->serializableRefInfoSetForClass(cls, name, &referenceInfo);
    }
    /**
     * @brief see #serializableRefInfoSet
     */
    inline const ZFSerializableData *serializableRefInfoGetForClass(ZF_IN const ZFClass *cls,
                                                                    ZF_IN const zfchar *name)
    {
        return this->serializableRefInfoGet(zfstringWithFormat(zfText("%s:%s"), cls->className(), name).cString());
    }

private:
    zfchar *_ZFP_ZFSerializable_selfRefType;
    zfchar *_ZFP_ZFSerializable_selfRefData;
public:
    /**
     * @brief usually internal use only, store the refType of this serializable,
     *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE for more info
     */
    inline void serializableSelfRefTypeSet(ZF_IN const zfchar *refType)
    {
        zfsChange(this->_ZFP_ZFSerializable_selfRefType, refType);
    }
    /**
     * @brief see #serializableSelfRefTypeSet
     */
    inline const zfchar *serializableSelfRefTypeGet(void)
    {
        return this->_ZFP_ZFSerializable_selfRefType;
    }
    /**
     * @brief usually internal use only, store the refData of this serializable,
     *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE for more info
     */
    inline void serializableSelfRefDataSet(ZF_IN const zfchar *refData)
    {
        zfsChange(this->_ZFP_ZFSerializable_selfRefData, refData);
    }
    /**
     * @brief see #serializableSelfRefDataSet
     */
    inline const zfchar *serializableSelfRefDataGet(void)
    {
        return this->_ZFP_ZFSerializable_selfRefData;
    }
private:
    zfchar *_ZFP_ZFSerializable_styleableType;
    zfchar *_ZFP_ZFSerializable_styleableData;
public:
    /**
     * @brief internal use only, store the styleable data of this serializable,
     *   see #ZFSerializable for more info
     */
    inline void serializableStyleableTypeSet(ZF_IN const zfchar *styleableType)
    {
        zfsChange(this->_ZFP_ZFSerializable_styleableType, styleableType);
    }
    /**
     * @brief see #serializableStyleableTypeSet
     */
    inline const zfchar *serializableStyleableTypeGet(void)
    {
        return this->_ZFP_ZFSerializable_styleableType;
    }
    /**
     * @brief see #serializableStyleableTypeSet
     */
    inline void serializableStyleableDataSet(ZF_IN const zfchar *styleableData)
    {
        zfsChange(this->_ZFP_ZFSerializable_styleableData, styleableData);
    }
    /**
     * @brief see #serializableStyleableTypeSet
     */
    inline const zfchar *serializableStyleableDataGet(void)
    {
        return this->_ZFP_ZFSerializable_styleableData;
    }
private:
    zfchar *_ZFP_ZFSerializable_ownerTag;
public:
    /**
     * @brief see #ZFPropertyTypeId_ZFCallback
     */
    zffinal void serializableOwnerTagSet(ZF_IN const zfchar *ownerTag);
    /**
     * @brief see #serializableOwnerTagSet
     */
    zffinal const zfchar *serializableOwnerTagGet(void);
};

// ============================================================
/**
 * @brief true if object is serializable
 *
 * note that null is treated as serializable
 */
extern ZF_ENV_EXPORT zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj);

// ============================================================
/**
 * @brief convenient method to serialize from encoded data
 *
 * you should release the result object if not null
 */
extern ZF_ENV_EXPORT zfbool ZFObjectFromString(ZF_OUT zfautoObject &result,
                                               ZF_IN const zfchar *encodedData,
                                               ZF_IN_OPT zfindex encodedDataLen = zfindexMax,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief convenient method to serialize from encoded data
 *
 * you should release the result object if not null
 * @note return null doesn't necessarily mean fail,
 *   if the input is ZFSerializableKeyword_null,
 *   which describe a null object,
 *   the result would be null
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectFromString(ZF_IN const zfchar *encodedData,
                                                     ZF_IN_OPT zfindex encodedDataLen = zfindexMax,
                                                     ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief convenient method to serialize to encoded data
 */
extern ZF_ENV_EXPORT zfbool ZFObjectToString(ZF_OUT zfstring &encodedData,
                                             ZF_IN ZFObject *obj,
                                             ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);
/**
 * @brief see #ZFObjectToString
 */
extern ZF_ENV_EXPORT zfstring ZFObjectToString(ZF_IN ZFObject *obj,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull);

// ============================================================
/**
 * @brief convenient method to serialize from serializable data
 *
 * you should release the result object if not null
 * @note return null doesn't necessarily mean fail,
 *   if the input is ZFSerializableKeyword_null,
 *   which describe a null object,
 *   the result would be null
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                                               ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                               ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
/**
 * @brief convenient method to serialize from serializable data
 *
 * you should release the result object if not null
 */
extern ZF_ENV_EXPORT zfbool ZFObjectFromSerializableData(ZF_OUT zfautoObject &result,
                                                         ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);
/**
 * @brief convenient method to serialize to serializable data
 */
extern ZF_ENV_EXPORT zfbool ZFObjectToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                                       ZF_IN ZFObject *obj,
                                                       ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                       ZF_IN_OPT ZFSerializable *referencedObject = zfnull);
/**
 * @brief convenient method to serialize to serializable data
 */
extern ZF_ENV_EXPORT ZFSerializableData ZFObjectToSerializableData(ZF_IN ZFObject *obj,
                                                                   ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                                                   ZF_OUT_OPT zfbool *outSuccess = zfnull,
                                                                   ZF_IN_OPT ZFSerializable *referencedObject = zfnull);

// ============================================================
/**
 * @brief register an object with specified name, or set null object to remove
 *
 * some serializable element depends on external object,
 * such as ZFCallback may depends on callback's owner object\n
 * to solve this problem, we introduced owner tag,
 * it's a <name, object> map to register object with an unique name
 * -  for owner object:
 *   -  if it's a normal ZFObject,
 *     you must register it manually by this method
 *   -  if it's a ZFSerializable,
 *     owner tag info would be automatically stored by #ZFSerializable::serializableOwnerTagSet,
 *     and would be automatically registered by this method
 * -  for any serializable element that need to reference the owner,
 *   it depends on the actual serializable element to achieve the reference logic
 *
 * \n
 * @note this method won't retain the object,
 *   and registered data is ensured to be removed when object deallocated
 */
extern ZF_ENV_EXPORT void ZFSerializableOwnerTagSet(ZF_IN const zfchar *name,
                                                    ZF_IN ZFObject *obj);
/**
 * @brief see #ZFSerializableOwnerTagSet
 */
extern ZF_ENV_EXPORT ZFObject *ZFSerializableOwnerTagGet(ZF_IN const zfchar *name);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDef_h_

#include "ZFSerializableUtilDef.h"


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFSerializableDataDef.h
 * @brief serializable data for #ZFSerializable
 */

#ifndef _ZFI_ZFSerializableDataDef_h_
#define _ZFI_ZFSerializableDataDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief keyword for #ZFSerializable to hold a no name node,
 *   which means the data doesn't care about its class name
 *
 * @code
 *   <node key="value" />
 * @endcode
 */
#define ZFSerializableKeyword_node zfText("node")
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's name,
 *   see #ZFSerializableData::propertyName
 */
#define ZFSerializableKeyword_name zfText("name")
/**
 * @brief keyword for #ZFSerializable to hold attribute or property's value,
 *   see #ZFSerializableData::propertyValue
 */
#define ZFSerializableKeyword_value zfText("value")
/**
 * @brief keyword for #ZFSerializable to hold the reference's type,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 */
#define ZFSerializableKeyword_refType zfText("refType")
/**
 * @brief keyword for #ZFSerializable to hold the reference's data,
 *   see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 */
#define ZFSerializableKeyword_refData zfText("refData")
/**
 * @brief keyword for #ZFSerializable's to hold category,
 *   see #ZFSerializableData::category
 */
#define ZFSerializableKeyword_category zfText("category")
/**
 * @brief keyword for #ZFSerializable's to hold whether it's under editMode,
 *   see #ZFSerializable::editModeData
 */
#define ZFSerializableKeyword_editMode zfText("editMode")
/**
 * @brief used to declare a null object
 *
 * @code
 *   <null>
 * @endcode
 * serializing this would result a null with success state\n
 * the actual type for the null is determined by the owner
 */
#define ZFSerializableKeyword_null zfText("null")

// ============================================================
zfclassFwd ZFSerializable;
zfclassFwd _ZFP_ZFSerializableDataPrivate;
/**
 * @brief ZFSerializable's data container, see #ZFSerializable
 *
 * this is a data structure similar to xml,
 * designed to reduce xml dependency
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFSerializableData
{
public:
    /** @cond ZFPrivateDoc */
    ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d);
    /** @endcond */
    /**
     * @brief construct an empty data
     */
    ZFSerializableData(void);
    /**
     * @brief retain from another ref
     */
    ZFSerializableData(ZF_IN const ZFSerializableData &ref);
    /**
     * @brief retain from another ref
     */
    ZFSerializableData &operator =(ZF_IN const ZFSerializableData &ref);
    ~ZFSerializableData(void);

public:
    /**
     * @brief copy all the contents
     */
    zffinal void copyFrom(ZF_IN const ZFSerializableData &ref);
    /**
     * @brief return a copy of this data
     */
    inline ZFSerializableData copy(void) const
    {
        ZFSerializableData ret;
        ret.copyFrom(*this);
        return ret;
    }

    /**
     * @brief get current retain count
     */
    zffinal zfindex objectRetainCount(void) const;

    // ============================================================
    // reference logic
public:
    /**
     * @brief internal use only, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     */
    zffinal void referenceRefTypeSet(ZF_IN const zfchar *refType);
    /**
     * @brief internal use only, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     */
    zffinal const zfchar *referenceRefType(void) const;
    /**
     * @brief internal use only, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     */
    zffinal void referenceRefDataSet(ZF_IN const zfchar *refData);
    /**
     * @brief internal use only, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     */
    zffinal const zfchar *referenceRefData(void) const;
    /**
     * @brief recursively load data from reference info, see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
     *
     * this method usually called after you decode ZFSerializableData from file or other source,
     * and before you serialize it to actual type,
     * and should only be called once on the root data
     */
    zffinal zfbool referenceCheckLoad(ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull);

    /**
     * @brief true if this data's reference info exists
     */
    zffinal zfbool referenceInfoExist(void) const;
    /**
     * @brief true if this data's or children's reference info exists
     */
    zffinal zfbool referenceInfoExistRecursively(void) const;
    /**
     * @brief copy this node's reference info or return an empty data if this data has no reference info
     */
    zffinal ZFSerializableData referenceInfoCopy(void) const;
    /**
     * @brief copy this node's and all children's reference info or return an empty data if this data has no reference info
     */
    zffinal ZFSerializableData referenceInfoCopyRecursively(void) const;
    /**
     * @brief load this data's reference info to another data, return an empty data if this data has no reference info
     */
    zffinal zfbool referenceLoadTo(ZF_OUT ZFSerializableData &outData,
                                   ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull,
                                   ZF_OUT_OPT const ZFSerializableData **outErrorPos = zfnull) const
    {
        outData.referenceRefTypeSet(this->referenceRefType());
        outData.referenceRefDataSet(this->referenceRefData());
        return outData.referenceCheckLoad(outErrorHintToAppend, outErrorPos);
    }

    // ============================================================
    // parent
public:
    /**
     * @brief access parent of this node, return true if parent exists
     */
    zffinal zfbool serializableDataParent(ZF_OUT ZFSerializableData &ret) const;

    // ============================================================
    // class
public:
    /** @brief see #ZFSerializable */
    zffinal void itemClassSet(ZF_IN const zfchar *clsName);
    /** @brief see #ZFSerializable */
    zffinal const zfchar *itemClass(void) const;

    // ============================================================
    // other types
public:
    /** @brief see #ZFSerializable, same as set value of attribute "name" */
    inline void propertyNameSet(ZF_IN const zfchar *name)
    {
        this->attributeSet(ZFSerializableKeyword_name, name);
    }
    /** @brief see #propertyNameSet */
    inline const zfchar *propertyName(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_name);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "value" */
    inline void propertyValueSet(ZF_IN const zfchar *value)
    {
        this->attributeSet(ZFSerializableKeyword_value, value);
    }
    /** @brief see #propertyValueSet */
    inline const zfchar *propertyValue(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_value);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "category" */
    inline void categorySet(ZF_IN const zfchar *category)
    {
        this->attributeSet(ZFSerializableKeyword_category, category);
    }
    /** @brief see #categorySet */
    inline const zfchar *category(void) const
    {
        return this->attributeForName(ZFSerializableKeyword_category);
    }
    /** @brief see #ZFSerializable, same as set value of attribute "editMode" */
    inline void editModeSet(ZF_IN zfbool editMode)
    {
        this->attributeSet(ZFSerializableKeyword_editMode, editMode ? ZFTOKEN_zfbool_zftrue : zfnull);
    }
    /** @brief see #categorySet */
    inline zfbool editMode(void) const
    {
        return (this->attributeForName(ZFSerializableKeyword_editMode) != zfnull);
    }

    // ============================================================
public:
    /**
     * @brief clear all the contents
     */
    zffinal void removeAll(void);

    // ============================================================
public:
    /**
     * @brief used to store extra state for this serializable data
     *
     * typically used by impl to achieve additional features
     */
    zffinal void serializableDataTagSet(ZF_IN const zfchar *key,
                                        ZF_IN ZFObject *tag,
                                        ZF_IN_OPT zfbool autoMarkCached = zffalse);
    /**
     * @brief #serializableDataTagSet and automatically mark tag as #ZFObject::objectCached
     */
    zffinal inline void serializableDataTagSetMarkCached(ZF_IN const zfchar *key,
                                                         ZF_IN ZFObject *tag)
    {
        this->serializableDataTagSet(key, tag, zftrue);
    }
    /**
     * @brief see #serializableDataTagSet
     */
    zffinal ZFObject *serializableDataTagGet(ZF_IN const zfchar *key) const;
    /** @brief see #serializableDataTagSet */
    template<typename T_ZFObject>
    T_ZFObject serializableDataTagGet(ZF_IN const zfchar *key) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->serializableDataTagGet(key));
    }
    /** @brief see #serializableDataTagSet */
    zffinal void serializableDataTagGetAllKeyValue(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &allKey,
                                                   ZF_IN_OUT ZFCoreArrayPOD<ZFObject *> &allValue) const;
    /**
     * @brief remove tag, same as set tag to null
     */
    zffinal void serializableDataTagRemove(ZF_IN const zfchar *key);
    /**
     * @brief remove tag, return removed tag or #zfautoObjectNull if not exist
     */
    zffinal zfautoObject serializableDataTagRemoveAndGet(ZF_IN const zfchar *key);
    /**
     * @brief see #serializableDataTagSet
     *
     * @note it's unsafe to remove all tag manually,
     *   which may break unrelated modules' additional logic,
     *   remove only if necessary\n
     *   typically, you should remove exactly the one you have added
     */
    zffinal void serializableDataTagRemoveAll(void);

    // ============================================================
    // attribute
public:
    /**
     * @brief see #ZFSerializable, remove if value is null (same as #attributeRemove)
     *
     * note that order of attributes is not ensured
     * after you add, remove or modify attributes
     */
    zffinal void attributeSet(ZF_IN const zfchar *name,
                             ZF_IN const zfchar *value);
    /** @brief see #attributeSet */
    zffinal const zfchar *attributeForName(ZF_IN const zfchar *name) const;

    /** @brief see #attributeSet */
    zffinal zfindex attributeCount(void) const;

    /**
     * @brief remove value with name, see #attributeSet
     */
    zffinal void attributeRemove(ZF_IN const zfchar *name);

    /**
     * @brief remove all attribute
     */
    zffinal void attributeRemoveAll(void);

    /** @brief see #zfiterator */
    zffinal zfiterator attributeIteratorForName(ZF_IN const zfchar *name) const;
    /** @brief see #zfiterator */
    zffinal zfiterator attributeIterator(void) const;
    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorIsValid(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorIsEqual(ZF_IN const zfiterator &it0,
                                            ZF_IN const zfiterator &it1) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorSet(ZF_IN_OUT zfiterator &it,
                                      ZF_IN const zfchar *value);
    /** @brief see #zfiterator */
    zffinal void attributeIteratorRemove(ZF_IN_OUT zfiterator &it);
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorGetKey(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorGet(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorNextKey(ZF_IN_OUT zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal const zfchar *attributeIteratorNext(ZF_IN_OUT zfiterator &it) const;

    /** @brief see #zfiterator */
    zffinal zfbool attributeIteratorResolved(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorResolveMark(ZF_IN const zfiterator &it) const;
    /** @brief see #zfiterator */
    zffinal void attributeIteratorResolveUnmark(ZF_IN const zfiterator &it) const;

    // ============================================================
    // element
public:
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void elementAdd(ZF_IN const ZFSerializableData &element);
    /**
     * @brief add child element
     *
     * note this method won't check duplicate data with same name
     */
    zffinal void elementAdd(ZF_IN const ZFSerializableData &element,
                            ZF_IN zfindex atIndex);

    /**
     * @brief replace existing element at index or assert fail if index out of range
     */
    zffinal void elementSetAtIndex(ZF_IN zfindex index,
                                   ZF_IN const ZFSerializableData &element);

    /**
     * @brief find element with name or zfindexMax if not found
     */
    zffinal zfindex elementFindByName(ZF_IN const zfchar *name,
                                      ZF_IN_OPT zfbool skipResolved = zffalse) const;
    /**
     * @brief see #elementFindByName
     */
    zffinal zfindex elementFindByCategory(ZF_IN const zfchar *name,
                                          ZF_IN_OPT zfbool skipResolved = zffalse) const;

    /** @brief see #ZFSerializable */
    zffinal zfindex elementCount(void) const;
    /** @brief see #ZFSerializable */
    zffinal const ZFSerializableData &elementAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFSerializable */
    zffinal ZFSerializableData &elementAtIndex(ZF_IN zfindex index);

    /**
     * @brief remove element at index
     */
    zffinal void elementRemoveAtIndex(ZF_IN zfindex index);
    /**
     * @brief remove all element
     */
    zffinal void elementRemoveAll(void);

    // ============================================================
    // resolved state
public:
    /**
     * @brief used to mark whether the data is resolved
     *
     * useful while serializing some type from serializable,
     * after operation's done,
     * you may check whether all data resolved
     * to see whether there's extra unrecognized data
     */
    zffinal zfbool resolved(void) const;
    /** @brief see #resolved */
    zffinal void resolveMark(void) const;
    /** @brief see #resolved */
    zffinal void resolveUnmark(void) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedAttribute(ZF_IN const zfchar *name) const;
    /** @brief see #resolved */
    zffinal void resolveAttributeMark(ZF_IN const zfchar *name) const;
    /** @brief see #resolved */
    zffinal void resolveAttributeUnmark(ZF_IN const zfchar *name) const;

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyName(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_name);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_name);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyNameUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_name);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedPropertyValue(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_value);
    }
    /** @brief see #resolved */
    zffinal void resolvePropertyValueUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_value);
    }

    /** @brief see #resolved */
    zffinal zfbool resolvedCategory(void) const
    {
        return this->resolvedAttribute(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryMark(void) const
    {
        this->resolveAttributeMark(ZFSerializableKeyword_category);
    }
    /** @brief see #resolved */
    zffinal void resolveCategoryUnmark(void) const
    {
        this->resolveAttributeUnmark(ZFSerializableKeyword_category);
    }

    /**
     * @brief see #resolved, check whether self and all attribute and child element resolved
     */
    zffinal zfbool resolvedAll(ZF_OUT_OPT const ZFSerializableData **firstNotResolvedElement = zfnull,
                             ZF_OUT_OPT zfstring *firstNotResolvedAttribute = zfnull) const;
    /**
     * @brief see #resolved, mark self and all attribute and child element
     */
    zffinal void resolveMarkAll(void) const;
    /**
     * @brief see #resolved, unmark self and all attribute and child element
     */
    zffinal void resolveUnmarkAll(void) const;
    /**
     * @brief see #resolved, mark all attribute
     */
    zffinal void resolveAttributeMarkAll(void) const;
    /**
     * @brief see #resolved, unmark all attribute
     */
    zffinal void resolveAttributeUnmarkAll(void) const;

    // ============================================================
    // other functions
public:
    /**
     * @brief get a short info
     */
    zffinal void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zffinal zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
    /**
     * @brief compare another serializable data
     *
     * if two data considered as same,
     * attribute's order have no need to be same,
     * but child element's order must be same
     */
    zffinal ZFCompareResult objectCompare(ZF_IN const ZFSerializableData &another) const;

public:
    /**
     * @brief true if the data contains nothing
     */
    zffinal zfbool isEmpty(void) const;

private:
    _ZFP_ZFSerializableDataPrivate *d;
    friend zfclassFwd _ZFP_ZFSerializableDataPrivate;
};

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFSerializableData, e0, ZFSerializableData, e1)
{
    return e0.objectCompare(e1);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFSerializableData, e0, ZFSerializableData, e1)

/**
 * @brief usually for debug use only
 *
 * macro names are recommended to be ZFSerializableDataRefType_XXX
 */
extern ZF_ENV_EXPORT ZFCoreArrayPOD<const zfchar *> ZFSerializableDataRefTypeGetAll(void);

typedef zfbool (*_ZFP_ZFSerializableDataReferenceCallback)(ZF_OUT ZFSerializableData &serializableData,
                                                           ZF_IN const zfchar *data,
                                                           ZF_OUT_OPT zfstring *outErrorHintToAppend);
extern ZF_ENV_EXPORT void _ZFP_ZFSerializableDataReferenceTypeRegister(ZF_IN const zfchar *referenceType,
                                                                       ZF_IN _ZFP_ZFSerializableDataReferenceCallback callback);
extern ZF_ENV_EXPORT void _ZFP_ZFSerializableDataReferenceTypeUnregister(ZF_IN const zfchar *referenceType);

#define _ZFP_ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(referenceTypeName, outSerializableDataName, inDataName, outErrorHintToAppend) \
    static zfbool _ZFP_ZFSerializableDataReferenceCallback_##referenceTypeName( \
        ZF_OUT ZFSerializableData &outSerializableDataName, \
        ZF_IN const zfchar *inDataName, \
        ZF_OUT_OPT zfstring *outErrorHintToAppend = zfnull); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFSerializableDataReferenceTypeRegister_##referenceTypeName, \
                                          ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFSerializableDataReferenceTypeRegister( \
            ZFMACRO_TOSTRING(referenceTypeName), \
            _ZFP_ZFSerializableDataReferenceCallback_##referenceTypeName); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(_ZFP_ZFSerializableDataReferenceTypeRegister_##referenceTypeName) \
    { \
        _ZFP_ZFSerializableDataReferenceTypeUnregister(ZFMACRO_TOSTRING(referenceTypeName)); \
    } \
    ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFSerializableDataReferenceTypeRegister_##referenceTypeName) \
    static zfbool _ZFP_ZFSerializableDataReferenceCallback_##referenceTypeName( \
        ZF_OUT ZFSerializableData &outSerializableDataName, \
        ZF_IN const zfchar *inDataName, \
        ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
/**
 * @brief advanced data reference logic for serializable
 *
 * used to reference another existing serializable data, typical example:
 * @code
 *   <OwnerType>
 *       <PropertyType refType="YourType" refData="YourType's reference data"
 *           yourExtraAttributeKey="yourExtraAttributeValue" >
 *           <YourExtraChildren />
 *       </PropertyType>
 *   </OwnerType>
 * @endcode
 * in this example, we declare a referenced node:
 * -  attribute "refType" shows the reference type,
 *   which was registered by #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  attribute "refData" shows the data for the reference,
 *   may be file path or content data or others,
 *   which would be decoded by the method declared by #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  you may add extra attributes or children to ref node,
 *   which would be appended to result decoded data
 *   (existing attributes would be replaced, and existing children would be appended normally)
 *
 * for example, assume we have two xml files:
 * @code
 *   // 1.xml
 *   <Holder>
 *       <MyProperty refType="xml"
 *           refData="2.xml"
 *           myAttr1="myValue1" >
 *           <MyChild1 />
 *       </MyProperty>
 *   </Holder>
 *   // 2.xml
 *   <AnyType myAttr1="myValue1 replaced"
 *       myAttr2="myValue2" >
 *       <MyChild2 />
 *   </AnyType>
 * @endcode
 * then the decoded data would be like:
 * @code
 *   <Holder>
 *       <MyProperty myAttr1="myValue1 replaced"
 *           myAttr2="myValue2" >
 *           <MyChild1 />
 *           <MyChild2 />
 *       </MyProperty>
 *   </Holder>
 * @endcode
 * \n
 * to declare your own type:
 * @code
 *   // in cpp file only
 *   ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(YourType, serializableData, data)
 *   {
 *       // you decode operation here
 *       // decode (const zfchar *)data to (ZFSerializableData)serializableData
 *
 *       // note, you must manually decode "refType" and "refData" attribute
 *       // and save the reference info to ZFSerializableData by
 *       // #ZFSerializableData::referenceRefTypeSet and #ZFSerializableData::referenceRefDataSet,
 *       // and you must call #ZFSerializableData::referenceCheckLoad
 *       // after decode the reference
 *   }
 * @endcode
 * \n
 * once declared, while reading ZFSerializableData for certain source (such as file),
 * you can use #ZFSerializableData::referenceCheckLoad
 * to automatically load data from the reference\n
 * \n
 * REMARKS:\n
 * -  typical "serialize from data" logic:
 *   -  load ZFSerializableData from source (files, strings, etc)
 *   -  load reference if necessary (by #ZFSerializableData::referenceCheckLoad)
 *   -  do the serialize step, owner have no need to worry about whether the data is loaded from reference
 * -  typical "serialize to data" logic for ZFSerializable:
 *   -  serialize a new instance of owner from the reference info
 *   -  compare owner and the newly serialized instance's property,
 *     and only serialize the different property to result serializable data
 *   -  save reference info to result serializable data
 * -  typical "serialize to data" logic for non-ZFSerializable:
 *   -  load serializable data from reference info
 *   -  serialize to a temp serializable data
 *   -  compare referenced data and temp data, and serialize different to result data
 *
 * in short, owner have no need to worry about reference logic,
 * it's done by #ZFSerializable automatically,
 * what you should do, is calling #ZFSerializableData::referenceCheckLoad
 * right after your decode serializable data tree from source
 * \n
 * ADVANCED:\n
 * reference logic can used in anywhere that need a ZFSerializableData,
 * what it does is only to load content from reference info\n
 * \n
 * however, the reference info (such as the file path of a xml file)
 * can only be automatically stored by #ZFSerializable,
 * which means only #ZFSerializable can restore reference info while serializing back to #ZFSerializableData\n
 * \n
 * for example, assume we have a serializable that have such a serializable info:
 * @code
 *   zfclass YourSerializable : zfextends ZFSerializable
 *   {
 *       ZFOBJECT_DECLARE(YourSerializable, ZFSerializable)
 *       ZFPROPERTY_ASSIGN(zfint, width)
 *       ZFPROPERTY_ASSIGN(zfint, height)
 *   };
 *
 *   <YourSerializable>
 *       <YourProperty name="width" (ref datas...) />
 *       <zfint name="height" value="1" />
 *   </YourSerializable>
 * @endcode
 * while serializing from data, we found the node that contains reference info,
 * #ZFSerializable would automatically save the reference info,
 * so that while serializing back to data,
 * the node would be restored instead of simply insert a "<zfint (datas...) />" node\n
 * \n
 * for an opposite example, if a non-ZFSerializable class supply a serializable logic:
 * @code
 *   zfclassNotPOD YourType {...};
 *   zfbool YourTypeFromSerializable(...) {...};
 *   zfbool YourTypeToSerializable(...) {...};
 *
 *   <YourType>
 *       <YourProperty (ref datas...) />
 *   </YourType>
 * @endcode
 * you must manually save reference info in YourType and manually restore it,
 * otherwise, the node's reference info would be missed\n
 * \n
 * the automatic reference info saving and restoring only applys to #ZFSerializable
 * and all serializable #ZFProperty declared in #ZFSerializable,
 * if a subclass of #ZFSerializable supply a custom serializing step,
 * you may want to use #ZFSerializable::serializableRefInfoSet and #ZFSerializable::serializableRefInfoGet
 * to manage the reference info manually
 */
#define ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(referenceTypeName, serializableData, data, outErrorHintToAppend) \
    _ZFP_ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(referenceTypeName, serializableData, data, outErrorHintToAppend)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataDef_h_

#include "ZFSerializableDataStringConverterDef.h"


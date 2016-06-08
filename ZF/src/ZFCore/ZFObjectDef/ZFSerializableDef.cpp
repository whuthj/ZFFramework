/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializableDef.h"
#include "ZFStyleableDef.h"
#include "ZFObjectCreatorDef.h"
#include "ZFObjectSmartPointerDef.h"
#include "ZFPropertyTypeDef.h"
#include "ZFListenerDeclareDef.h"
#include "ZFGlobalEventCenter.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cache in class's meta data for performance
zfclassNotPOD _ZFP_ZFSerializable_PropertyTypeData
{
public:
    const ZFProperty *property;
    ZFSerializable::PropertyType propertyType;
};
zfclass _ZFP_ZFSerializable_PropertyTypeHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFSerializable_PropertyTypeHolder, ZFObject)
public:
    ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> serializableProperty;
    ZFCoreMap serializablePropertyMap; // _ZFP_ZFSerializable_PropertyTypeData *

public:
    void addData(ZF_IN const ZFProperty *property,
                 ZF_IN ZFSerializable::PropertyType propertyType)
    {
        _ZFP_ZFSerializable_PropertyTypeData *typeData = zfnew(_ZFP_ZFSerializable_PropertyTypeData);
        typeData->property = property;
        typeData->propertyType = propertyType;

        this->serializableProperty.add(typeData);
        this->serializablePropertyMap.set(property->propertyName(),
            ZFCorePointerForObject<_ZFP_ZFSerializable_PropertyTypeData *>(typeData));
    }
};
ZFLEAKTEST_EXCLUDENAME_DEFINE(_ZFP_ZFSerializable_PropertyTypeHolder)

// ============================================================
// ZFSerializable
ZFSerializable::~ZFSerializable(void)
{
    zfsChange(this->_ZFP_ZFSerializable_editModeWrappedClassName, (const zfchar *)zfnull);
    if(this->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
    {
        zfdelete(this->_ZFP_ZFSerializable_editModeWrappedElementDatas);
        this->_ZFP_ZFSerializable_editModeWrappedElementDatas = zfnull;
    }
    if(this->_ZFP_ZFSerializable_referenceInfoMap != zfnull)
    {
        zfdelete(this->_ZFP_ZFSerializable_referenceInfoMap);
        this->_ZFP_ZFSerializable_referenceInfoMap = zfnull;
    }
    zfsChange(this->_ZFP_ZFSerializable_selfRefType, (const zfchar *)zfnull);
    zfsChange(this->_ZFP_ZFSerializable_selfRefData, (const zfchar *)zfnull);
    zfsChange(this->_ZFP_ZFSerializable_styleableType, (const zfchar *)zfnull);
    zfsChange(this->_ZFP_ZFSerializable_styleableData, (const zfchar *)zfnull);
    zfsChange(this->_ZFP_ZFSerializable_ownerTag, (const zfchar *)zfnull);
}

ZFCoreMap &ZFSerializable::editModeData(void)
{
    static ZFCoreMap m;
    return m;
}
zfbool &ZFSerializable::editMode(void)
{
    static zfbool v = zffalse;
    return v;
}
const zfchar *ZFSerializable::editModeWrappedClassName(void)
{
    return this->_ZFP_ZFSerializable_editModeWrappedClassName;
}
void ZFSerializable::editModeWrappedClassNameSet(ZF_IN const zfchar *value)
{
    zfsChange(this->_ZFP_ZFSerializable_editModeWrappedClassName, value);
}
ZFCoreArray<ZFSerializableData> &ZFSerializable::editModeWrappedElementDatas(void)
{
    if(this->_ZFP_ZFSerializable_editModeWrappedElementDatas == zfnull)
    {
        this->_ZFP_ZFSerializable_editModeWrappedElementDatas = zfnew(ZFCoreArray<ZFSerializableData>);
    }
    return *(this->_ZFP_ZFSerializable_editModeWrappedElementDatas);
}

zfbool ZFSerializable::serializable(void)
{
    return this->serializableOnCheck();
}
zfbool ZFSerializable::serializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                         ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                         ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    this->serializableOnSerializeFromDataPrepare(serializableData);

    // reference logic
    {
        const zfchar *refType = serializableData.referenceRefType();
        const zfchar *refData = serializableData.referenceRefData();
        if((refType == zfnull) != (refData == zfnull))
        {
            const zfchar *missing = ((refType == zfnull) ? ZFSerializableKeyword_refType : ZFSerializableKeyword_refData);
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("missing %s"), missing);
            return zffalse;
        }
        this->serializableSelfRefTypeSet(refType);
        this->serializableSelfRefDataSet(refData);
    }

    // styleable logic
    {
        const zfchar *styleableType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_styleableType);
        const zfchar *styleableData = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_styleableData);
        if((styleableType == zfnull) != (styleableData == zfnull))
        {
            const zfchar *missing = ((styleableType == zfnull) ? ZFSerializableKeyword_styleableType : ZFSerializableKeyword_styleableData);
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("missing %s"), missing);
            return zffalse;
        }
        if(styleableType != zfnull)
        {
            if(this->serializableSelfRefTypeGet() != zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                    zfText("reference logic and styleable logic can not be used together"));
                return zffalse;
            }

            if(!this->classData()->classIsTypeOf(ZFStyleable::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                    zfText("serializing object %s is not type of %s, while created from \"%s\" \"%s\""),
                    this->toObject()->objectInfoOfInstance().cString(),
                    ZFStyleable::ClassData()->className(),
                    styleableType, styleableData);
                return zffalse;
            }

            zfautoObject styleableObj = ZFObjectCreate(styleableType, styleableData);
            if(styleableObj == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                    zfText("failed to create object from \"%s\" \"%s\""), styleableType, styleableData);
                return zffalse;
            }
            if(!styleableObj.toObject()->classData()->classIsTypeOf(ZFSerializable::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                    zfText("object %s is not type of %s, while created from \"%s\" \"%s\""),
                    styleableObj.toObject()->objectInfoOfInstance().cString(),
                    ZFSerializable::ClassData()->className(),
                    styleableType, styleableData);
                return zffalse;
            }
            if(!styleableObj.toObject()->classData()->classIsTypeOf(ZFStyleable::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                    zfText("object %s is not type of %s, while created from \"%s\" \"%s\""),
                    styleableObj.toObject()->objectInfoOfInstance().cString(),
                    ZFStyleable::ClassData()->className(),
                    styleableType, styleableData);
                return zffalse;
            }

            ZFCastZFObjectUnchecked(ZFStyleable *, this)->styleableCopyFrom(ZFCastZFObjectUnchecked(ZFStyleable *, styleableObj));
        }
        this->serializableStyleableTypeSet(styleableType);
        this->serializableStyleableDataSet(styleableData);
    }

    // ownerTag
    const zfchar *ownerTag = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ownerTag);
    this->serializableOwnerTagSet(ownerTag);

    // editMode
    if(ZFSerializable::editMode())
    {
        ZFSerializable::EditModeData *editModeData = ZFSerializable::editModeData().get<ZFSerializable::EditModeData *>(this->editModeWrappedClassName());
        if(editModeData != zfnull)
        {
            for(zfindex i = 0; i < serializableData.elementCount(); ++i)
            {
                const ZFSerializableData &element = serializableData.elementAtIndex(i);
                if(element.editMode())
                {
                    this->editModeWrappedElementDatas().add(element.copy());
                    element.resolveMarkAll();
                }
            }
        }
    }

    // property
    {
        const ZFCoreMap &propertyMap = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializablePropertyMap;
        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            const ZFSerializableData &element = serializableData.elementAtIndex(i);
            if(element.resolved() || element.category() != zfnull)
            {
                continue;
            }
            const zfchar *propertyName = ZFSerializableUtil::checkPropertyName(element);
            if(propertyName == zfnull)
            {
                continue;
            }
            _ZFP_ZFSerializable_PropertyTypeData *data = propertyMap.get<_ZFP_ZFSerializable_PropertyTypeData *>(propertyName);
            if(data == zfnull)
            {
                continue;
            }

            // serialize the property
            switch(data->propertyType)
            {
                case ZFSerializable::PropertyTypeSerializableProperty:
                    if(!this->serializableOnSerializePropertyFromData(
                        element,
                        data->property,
                        outErrorHintToAppend,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializable::PropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyFromData(
                        element,
                        data->property,
                        outErrorHintToAppend,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializable::PropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
    }

    // category
    if(!this->serializableOnSerializeCategoryFromData(serializableData, outErrorHintToAppend, outErrorPos))
    {
        return zffalse;
    }

    // subclass
    if(!this->serializableOnSerializeFromData(serializableData, outErrorHintToAppend, outErrorPos))
    {
        return zffalse;
    }

    // mark self as resolved
    serializableData.resolveMark();

    // check whether all resoved
    if(!ZFSerializableUtilErrorOutputCallbacks.isEmpty())
    {
        zfstring tmp;
        if(ZFSerializableUtil::printResolveStatus(serializableData, ZFOutputCallbackForString(tmp)))
        {
            ZFCoreArray<ZFOutputCallback> &callbacks = ZFSerializableUtilErrorOutputCallbacks;
            for(zfindex i = 0; i < callbacks.count(); ++i)
            {
                callbacks[i].execute(tmp.cString());
            }
        }
    }

    return zftrue;
}
zfbool ZFSerializable::serializeToData(ZF_OUT ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                       ZF_IN_OPT ZFSerializable *referencedObject /* = zfnull */)
{
    this->serializableOnSerializeToDataPrepare(serializableData);

    if((this->serializableSelfRefTypeGet() != zfnull || this->serializableSelfRefDataGet() != zfnull)
        && (this->serializableStyleableTypeGet() != zfnull || this->serializableStyleableDataGet() != zfnull))
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("ref and style can not be used together"));
        return zffalse;
    }

    ZFSerializable *referencedOwnerOrNull = referencedObject;
    if(referencedOwnerOrNull == zfnull)
    {
        // reference logic
        if(this->serializableSelfRefTypeGet() != zfnull || this->serializableSelfRefDataGet() != zfnull)
        {
            ZFSerializableData referencedData;
            referencedData.itemClassSet(this->classData()->className());
            referencedData.referenceRefTypeSet(this->serializableSelfRefTypeGet());
            referencedData.referenceRefDataSet(this->serializableSelfRefDataGet());
            if(!referencedData.referenceCheckLoad(outErrorHintToAppend))
            {
                return zffalse;
            }
            zfautoObject referencedOwnerObject;
            if(!ZFObjectFromSerializableData(referencedOwnerObject, referencedData, outErrorHintToAppend))
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                    zfText(" while serializing %s to data"), this->toObject()->objectInfoOfInstance().cString());
                return zffalse;
            }
            referencedOwnerOrNull = zfRetain(ZFCastZFObjectUnchecked(zfself *, referencedOwnerObject.toObject()));

            serializableData.referenceRefTypeSet(this->serializableSelfRefTypeGet());
            serializableData.referenceRefDataSet(this->serializableSelfRefDataGet());
        }
        // styleable logic
        if(this->serializableStyleableTypeGet() != zfnull || this->serializableStyleableDataGet() != zfnull)
        {
            zfautoObject styleableObj = ZFObjectCreate(this->serializableStyleableTypeGet(), this->serializableStyleableDataGet());
            if(styleableObj == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                    zfText("failed to create object from \"%s\" \"%s\""), this->serializableStyleableTypeGet(), this->serializableStyleableDataGet());
                return zffalse;
            }
            referencedOwnerOrNull = zfRetain(ZFCastZFObject(zfself *, styleableObj.toObject()));
        }
    }
    else
    {
        zfRetain(referencedOwnerOrNull);
    }
    zfblockedRelease(referencedOwnerOrNull ? referencedOwnerOrNull->toObject() : zfnull);

    // ownerTag
    if(this->serializableOwnerTagGet() != zfnull)
    {
        if(referencedOwnerOrNull == zfnull
            || !zfscmpTheSame(referencedOwnerOrNull->serializableOwnerTagGet(), this->serializableOwnerTagGet()))
        {
            serializableData.attributeSet(ZFSerializableKeyword_ownerTag, this->serializableOwnerTagGet());
        }
    }

    // property
    {
        const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &allProperty = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            _ZFP_ZFSerializable_PropertyTypeData *data = allProperty[i];
            ZFSerializableData propertyData;

            switch(data->propertyType)
            {
                case ZFSerializable::PropertyTypeSerializableProperty:
                    if(!this->serializableOnSerializePropertyToData(propertyData,
                                                                    data->property,
                                                                    referencedOwnerOrNull,
                                                                    outErrorHintToAppend))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializable::PropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyToData(propertyData,
                                                                           data->property,
                                                                           referencedOwnerOrNull,
                                                                           outErrorHintToAppend))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializable::PropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }

            if(propertyData.itemClass() != zfnull)
            {
                propertyData.propertyNameSet(data->property->propertyName());
                serializableData.elementAdd(propertyData);
            }
        }
    }

    // category
    {
        if(!this->serializableOnSerializeCategoryToData(serializableData,
                                                        referencedOwnerOrNull,
                                                        outErrorHintToAppend))
        {
            return zffalse;
        }
    }

    // subclass
    if(!this->serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHintToAppend))
    {
        return zffalse;
    }

    if(ZFSerializable::editMode() && this->editModeWrappedClassName() != zfnull)
    {
        serializableData.itemClassSet(this->editModeWrappedClassName());
    }
    else
    {
        serializableData.itemClassSet(this->classData()->className());
    }

    if(ZFSerializable::editMode() && this->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
    {
        for(zfindex i = 0; i < this->_ZFP_ZFSerializable_editModeWrappedElementDatas->count(); ++i)
        {
            serializableData.elementAdd(this->_ZFP_ZFSerializable_editModeWrappedElementDatas->get(i));
        }
    }

    return zftrue;
}

_ZFP_ZFSerializable_PropertyTypeHolder *ZFSerializable::_ZFP_ZFSerializable_getPropertyTypeHolder(void)
{
    ZFCoreMutexLocker();
    if(this->_ZFP_ZFSerializable_propertyTypeHolder == zfnull)
    {
        this->_ZFP_ZFSerializable_propertyTypeHolder = this->classData()->classTagGet<_ZFP_ZFSerializable_PropertyTypeHolder *>(_ZFP_ZFSerializable_PropertyTypeHolder::ClassData()->className());
        if(this->_ZFP_ZFSerializable_propertyTypeHolder == zfnull)
        {
            zflockfree_zfblockedAllocWithoutLeakTest(_ZFP_ZFSerializable_PropertyTypeHolder, holderTmp);
            ZFCoreArrayPOD<const ZFProperty *> allProperty;
            {
                // the order affects the serialization step's order,
                // sort parent's property first for human-readable
                ZFCoreArrayPOD<const ZFClass *> allClass;
                const ZFClass *tmpCls = this->classData();
                allClass.add(tmpCls);
                do
                {
                    for(zfindex i = tmpCls->implementedInterfaceCount() - 1; i != zfindexMax; --i)
                    {
                        allClass.add(tmpCls->implementedInterfaceAtIndex(i));
                    }
                    if(tmpCls->parentClass() != zfnull)
                    {
                        allClass.add(tmpCls->parentClass());
                    }
                    tmpCls = tmpCls->parentClass();
                } while(tmpCls != zfnull);
                for(zfindex i = allClass.count() - 1; i != zfindexMax; --i)
                {
                    tmpCls = allClass[i];
                    for(zfindex iProperty = 0; iProperty < tmpCls->propertyCount(); ++iProperty)
                    {
                        allProperty.add(tmpCls->propertyAtIndex(iProperty));
                    }
                }
            }

            for(zfindex i = 0; i < allProperty.count(); ++i)
            {
                const ZFProperty *property = allProperty[i];
                switch(this->serializableOnCheckPropertyType(property))
                {
                    case ZFSerializable::PropertyTypeNotSerializable:
                        break;
                    case ZFSerializable::PropertyTypeSerializableProperty:
                        holderTmp->addData(property, ZFSerializable::PropertyTypeSerializableProperty);
                        break;
                    case ZFSerializable::PropertyTypeEmbededProperty:
                        holderTmp->addData(property, ZFSerializable::PropertyTypeEmbededProperty);
                        break;
                    default:
                        zfCoreCriticalShouldNotGoHere();
                        return zfnull;
                }
            }

            _ZFP_ZFSerializable_propertyTypeHolder = holderTmp;
            this->classData()->classTagSet(
                _ZFP_ZFSerializable_PropertyTypeHolder::ClassData()->className(),
                holderTmp);
        }
    }
    return this->_ZFP_ZFSerializable_propertyTypeHolder;
}
const ZFCoreArrayPOD<const ZFProperty *> ZFSerializable::serializableGetAllSerializableProperty(void)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    ZFCoreArrayPOD<const ZFProperty *> ret;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializable::PropertyTypeSerializableProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
    return ret;
}
const ZFCoreArrayPOD<const ZFProperty *> ZFSerializable::serializableGetAllSerializableEmbededProperty(void)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    ZFCoreArrayPOD<const ZFProperty *> ret;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializable::PropertyTypeEmbededProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
    return ret;
}

ZFSerializable::PropertyType ZFSerializable::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(!property->propertyReflectable)
    {
        return ZFSerializable::PropertyTypeNotSerializable;
    }

    if(property->propertyIsRetainProperty())
    {
        if(property->propertyClassOfRetainProperty()->classIsTypeOf(ZFSerializable::ClassData()))
        {
            if(property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
            {
                if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
                {
                    return ZFSerializable::PropertyTypeNotSerializable;
                }
                else
                {
                    return ZFSerializable::PropertyTypeEmbededProperty;
                }
            }
            else
            {
                if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
                {
                    return ZFSerializable::PropertyTypeNotSerializable;
                }
                else
                {
                    return ZFSerializable::PropertyTypeSerializableProperty;
                }
            }
        }
        else
        {
            return ZFSerializable::PropertyTypeNotSerializable;
        }
    }
    else
    {
        if(property->propertyIsSerializable())
        {
            return ZFSerializable::PropertyTypeSerializableProperty;
        }
        else
        {
            return ZFSerializable::PropertyTypeNotSerializable;
        }
    }
}

zfbool ZFSerializable::serializableOnSerializePropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                               ZF_IN const ZFProperty *property,
                                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                               ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    if(property->propertyIsRetainProperty())
    {
        zfautoObject obj;
        if(!ZFObjectFromSerializableData(obj, propertyData, outErrorHintToAppend, outErrorPos))
        {
            return zffalse;
        }
        if(obj != zfautoObjectNull && !obj.toObject()->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, propertyData,
                zfText("object %s not type of %s"),
                obj.toObject()->objectInfoOfInstance().cString(), property->propertyClassOfRetainProperty()->className());
            return zffalse;
        }
        property->callbackRetainSet(property, this->toObject(), obj.toObject());
        return zftrue;
    }

    ZFPropertyTypeSerializeFromCallback serializeFromCallback = ZFPropertyTypeGetSerializeFromCallback(propertyData.itemClass());
    if(serializeFromCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, propertyData,
            zfText("type \"%s\" not registered while serializing \"%s\""),
            propertyData.itemClass(),
            this->classData()->className());
        return zffalse;
    }
    if(!serializeFromCallback(property, this->toObject(), propertyData, outErrorHintToAppend, outErrorPos))
    {
        return zffalse;
    }

    // save assign property's reference info
    ZFSerializableData propertyReferenceInfo;
    if(ZFSerializableUtil::copyRefInfo(propertyReferenceInfo, propertyData))
    {
        this->serializableRefInfoSetForProperty(property, &propertyReferenceInfo);
    }

    return zftrue;
}
zfbool ZFSerializable::serializableOnSerializePropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                             ZF_IN const ZFProperty *property,
                                                             ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                             ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && property->callbackCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    if(!property->propertyIsSerializable()
        || property->callbackIsInitValue(property, this->toObject()))
    {
        return zftrue;
    }

    if(property->propertyIsRetainProperty())
    {
        ZFObject *obj = property->callbackRetainGet(property, this->toObject());
        if(!ZFObjectToSerializableData(propertyData, obj, outErrorHintToAppend))
        {
            return zffalse;
        }
    }
    else
    {
        ZFPropertyTypeSerializeToCallback serializeToCallback = ZFPropertyTypeGetSerializeToCallback(property->propertyTypeIdName());
        if(serializeToCallback == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                zfText("type \"%s\" not registered while serializing \"%s\""),
                property->propertyTypeIdName(),
                this->classData()->className());
            return zffalse;
        }
        if(!serializeToCallback(property, this->toObject(), propertyData, outErrorHintToAppend))
        {
            return zffalse;
        }
    }

    // resolve reference info
    const ZFSerializableData *propertyReferencedData = this->serializableRefInfoGetForProperty(property);
    if(propertyReferencedData != zfnull)
    {
        ZFSerializableData tmpData;
        if(!propertyReferencedData->referenceLoadTo(tmpData, outErrorHintToAppend))
        {
            return zffalse;
        }

        // merge duplicated referenced attributes and children
        ZFSerializableUtil::resolveReferencedData(propertyData, tmpData);
    }
    else
    {
        if(!property->propertyIsRetainProperty()
            && propertyData.elementCount() == 0
            && propertyData.attributeCount() == 0
            && propertyData.referenceRefType() == zfnull
            && propertyData.referenceRefData() == zfnull)
        {
            // no property ref info, and all value equals to ref owner's property,
            // so no need to serialize
            propertyData.itemClassSet(zfnull);
        }
    }

    return zftrue;
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                                      ZF_IN const ZFProperty *property,
                                                                      ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                                      ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    ZFObject *obj = property->callbackRetainGet(property, this->toObject());
    if(obj == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("embeded property %s is null while serializing \"%s\""),
            property->propertyName(),
            this->classData()->className());
        return zffalse;
    }
    else if(!ZFObjectIsSerializable(obj))
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("not serializable object %s while serializing \"%s\""),
            obj->objectInfoOfInstance().cString(),
            this->classData()->className());
        return zffalse;
    }
    else
    {
        const ZFClass *cls = ZFClass::classForName(propertyData.itemClass());
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                zfText("no class named %s while serializing \"%s\"'s property %s"),
                propertyData.itemClass(),
                this->classData()->className(),
                property->propertyName());
            return zffalse;
        }
        else if(!cls->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                zfText("node %s is not type of %s while serializing \"%s\"'s property %s"),
                propertyData.itemClass(),
                property->propertyClassOfRetainProperty()->className(),
                this->classData()->className(),
                property->propertyName());
            return zffalse;
        }
    }
    return ZFCastZFObjectUnchecked(zfself *, obj)->serializeFromData(propertyData, outErrorHintToAppend, outErrorPos);
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                                    ZF_IN const ZFProperty *property,
                                                                    ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && property->callbackCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    if(property->callbackIsInitValue(property, this->toObject()))
    {
        return zftrue;
    }

    ZFObject *obj = property->callbackRetainGet(property, this->toObject());
    if(obj == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("embeded property %s is null while serializing \"%s\""),
            property->propertyName(),
            this->classData()->className());
        return zffalse;
    }
    else if(!ZFObjectIsSerializable(obj))
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("not serializable object %s while serializing \"%s\""),
            obj->objectInfoOfInstance().cString(),
            this->classData()->className());
        return zffalse;
    }
    if(!ZFCastZFObjectUnchecked(zfself *, obj)->serializeToData(propertyData, outErrorHintToAppend))
    {
        return zffalse;
    }

    if(propertyData.elementCount() == 0
        && propertyData.attributeCount() == 0
        && propertyData.referenceRefType() == zfnull
        && propertyData.referenceRefData() == zfnull)
    {
        propertyData.itemClassSet(zfnull);
    }
    return zftrue;
}

void ZFSerializable::serializableGetInfoT(ZF_IN_OUT zfstring &ret)
{
    ZFSerializableData serializableData;
    if(!this->serializable()
       || !this->serializeToData(serializableData))
    {
        ret += ZFTOKEN_ZFObjectInfoLeft;
        this->toObject()->objectInfoOfInstanceT(ret);
        ret += zfText(": not serializable");
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
    else
    {
        serializableData.objectInfoT(ret);
    }
}

void ZFSerializable::serializableCopyInfoFrom(ZF_IN ZFSerializable *anotherSerializable)
{
    if(anotherSerializable == zfnull)
    {
        return ;
    }

    this->editModeWrappedClassNameSet(anotherSerializable->editModeWrappedClassName());
    if(anotherSerializable->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
    {
        this->editModeWrappedElementDatas().copyFrom(anotherSerializable->editModeWrappedElementDatas());
    }
    else
    {
        if(this->_ZFP_ZFSerializable_editModeWrappedElementDatas != zfnull)
        {
            this->_ZFP_ZFSerializable_editModeWrappedElementDatas->removeAll();
        }
    }

    if(anotherSerializable->_ZFP_ZFSerializable_referenceInfoMap != zfnull)
    {
        if(this->_ZFP_ZFSerializable_referenceInfoMap == zfnull)
        {
            this->_ZFP_ZFSerializable_referenceInfoMap = zfnew(ZFCoreMap);
        }
        this->_ZFP_ZFSerializable_referenceInfoMap->removeAll();
        this->_ZFP_ZFSerializable_referenceInfoMap->copyFrom(*(anotherSerializable->_ZFP_ZFSerializable_referenceInfoMap));
    }
    else
    {
        if(this->_ZFP_ZFSerializable_referenceInfoMap != zfnull)
        {
            zfdelete(this->_ZFP_ZFSerializable_referenceInfoMap);
            this->_ZFP_ZFSerializable_referenceInfoMap = zfnull;
        }
    }

    zfsChange(this->_ZFP_ZFSerializable_selfRefType, anotherSerializable->_ZFP_ZFSerializable_selfRefType);
    zfsChange(this->_ZFP_ZFSerializable_selfRefData, anotherSerializable->_ZFP_ZFSerializable_selfRefData);
    zfsChange(this->_ZFP_ZFSerializable_styleableType, anotherSerializable->_ZFP_ZFSerializable_styleableType);
    zfsChange(this->_ZFP_ZFSerializable_styleableData, anotherSerializable->_ZFP_ZFSerializable_styleableData);
    zfsChange(this->_ZFP_ZFSerializable_ownerTag, anotherSerializable->_ZFP_ZFSerializable_ownerTag);
}

void ZFSerializable::serializableRefInfoSet(ZF_IN const zfchar *key,
                                            ZF_IN const ZFSerializableData *referenceInfo)
{
    if(this->_ZFP_ZFSerializable_referenceInfoMap == zfnull)
    {
        this->_ZFP_ZFSerializable_referenceInfoMap = zfnew(ZFCoreMap);
    }
    zfiterator it = this->_ZFP_ZFSerializable_referenceInfoMap->iteratorForKey(key);
    if(referenceInfo != zfnull)
    {
        ZFSerializableData *p = zfnull;
        if(this->_ZFP_ZFSerializable_referenceInfoMap->iteratorIsValid(it))
        {
            p = this->_ZFP_ZFSerializable_referenceInfoMap->iteratorGetValue<ZFSerializableData *>(it);
        }
        else
        {
            p = zfnew(ZFSerializableData);
            this->_ZFP_ZFSerializable_referenceInfoMap->set(key, ZFCorePointerForObject<ZFSerializableData *>(p));
        }
        *p = *referenceInfo;
    }
    else if(this->_ZFP_ZFSerializable_referenceInfoMap->iteratorIsValid(it))
    {
        this->_ZFP_ZFSerializable_referenceInfoMap->iteratorRemove(it);
    }
}
const ZFSerializableData *ZFSerializable::serializableRefInfoGet(ZF_IN const zfchar *key)
{
    return ((this->_ZFP_ZFSerializable_referenceInfoMap != zfnull)
        ? this->_ZFP_ZFSerializable_referenceInfoMap->get<ZFSerializableData *>(key)
        : zfnull);
}
void ZFSerializable::serializableOwnerTagSet(ZF_IN const zfchar *ownerTag)
{
    ZFSerializableOwnerTagSet(this->_ZFP_ZFSerializable_ownerTag, zfnull);
    zfsChange(this->_ZFP_ZFSerializable_ownerTag, ownerTag);
    ZFSerializableOwnerTagSet(this->_ZFP_ZFSerializable_ownerTag, this->toObject());
}
const zfchar *ZFSerializable::serializableOwnerTagGet(void)
{
    return this->_ZFP_ZFSerializable_ownerTag;
}

// ============================================================
zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zftrue;
    }
    else
    {
        ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
        return (tmp != zfnull && tmp->serializable());
    }
}

// ============================================================
zfbool ZFObjectFromString(ZF_OUT zfautoObject &result,
                          ZF_IN const zfchar *encodedData,
                          ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax */,
                          ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFSerializableDataFromString(serializableData, encodedData, encodedDataLen, outErrorHintToAppend))
    {
        return zffalse;
    }
    return ZFObjectFromSerializableData(result, serializableData, outErrorHintToAppend);
}
zfautoObject ZFObjectFromString(ZF_IN const zfchar *encodedData,
                                ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax */,
                                ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromString(result, encodedData, encodedDataLen, outErrorHintToAppend);
    return result;
}
zfbool ZFObjectToString(ZF_OUT zfstring &encodedData,
                        ZF_IN ZFObject *obj,
                        ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFObjectToSerializableData(serializableData, obj, outErrorHintToAppend)) {return zffalse;}
    return ZFSerializableDataToString(encodedData, serializableData, outErrorHintToAppend);
}
zfstring ZFObjectToString(ZF_IN ZFObject *obj,
                        ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    zfstring ret;
    ZFObjectToString(ret, obj, outErrorHintToAppend);
    return ret;
}

// ============================================================
zfbool ZFObjectFromSerializableData(ZF_OUT zfautoObject &result,
                                    ZF_IN const ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                    ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    result = zfautoObjectNull;

    const zfchar *serializableClass = ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_none, serializableData, outErrorHintToAppend, outErrorPos);
    if(serializableClass == zfnull)
    {
        return zffalse;
    }
    if(zfscmpTheSame(serializableClass, ZFSerializableKeyword_null))
    {
        serializableData.resolveMark();
        return zftrue;
    }

    const ZFClass *cls = ZFClass::classForName(serializableClass);
    zfbool editModeWrapped = zffalse;
    if(cls == zfnull)
    {
        if(ZFSerializable::editMode())
        {
            ZFSerializable::EditModeData *editModeData = ZFSerializable::editModeData().get<ZFSerializable::EditModeData *>(serializableClass);
            if(editModeData != zfnull)
            {
                editModeWrapped = zftrue;
                cls = editModeData->wrappedClass;
            }
        }
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
                zfText("no class named \"%s\""),
                serializableData.itemClass());
            return zffalse;
        }
    }
    zfautoObject obj;
    {
        const ZFMethod *overridedCreateMethod = cls->methodForNameIgnoreParent(ZFSerializableKeyword_serializableNewInstance);
        if(overridedCreateMethod != zfnull)
        {
            obj = overridedCreateMethod->executeClassStatic<zfautoObject>();
        }
        else
        {
            obj = cls->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE);
        }
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj.toObject());
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, serializableData,
            zfText("object %s not serializable"),
            ZFObjectInfoOfInstance(obj.toObject()).cString());
        return zffalse;
    }
    if(editModeWrapped)
    {
        tmp->editModeWrappedClassNameSet(serializableClass);
    }
    if(!tmp->serializeFromData(serializableData, outErrorHintToAppend, outErrorPos))
    {
        return zffalse;
    }
    result = obj;
    return zftrue;
}
zfautoObject ZFObjectFromSerializableData(ZF_IN const ZFSerializableData &serializableData,
                                          ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                          ZF_OUT_OPT const ZFSerializableData **outErrorPos /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromSerializableData(result, serializableData, outErrorHintToAppend, outErrorPos);
    return result;
}
zfbool ZFObjectToSerializableData(ZF_OUT ZFSerializableData &serializableData,
                                  ZF_IN ZFObject *obj,
                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                  ZF_IN_OPT ZFSerializable *referencedObject /* = zfnull */)
{
    if(obj == zfnull)
    {
        serializableData.itemClassSet(ZFSerializableKeyword_null);
        return zftrue;
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("object %s not serializable"),
            obj->objectInfoOfInstance().cString());
        return zffalse;
    }
    return tmp->serializeToData(serializableData, outErrorHintToAppend, referencedObject);
}
ZFSerializableData ZFObjectToSerializableData(ZF_IN ZFObject *obj,
                                              ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                              ZF_OUT_OPT zfbool *outSuccess /* = zfnull */,
                                              ZF_IN_OPT ZFSerializable *referencedObject /* = zfnull */)
{
    ZFSerializableData serializableData;
    zfbool success = ZFObjectToSerializableData(serializableData, obj, outErrorHintToAppend, referencedObject);
    if(outSuccess != zfnull)
    {
        *outSuccess = success;
    }
    if(success)
    {
        return serializableData;
    }
    return ZFSerializableData();
}

// ============================================================
zfclass _ZFP_ZFSerializableOwnerTagData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFSerializableOwnerTagData, ZFObject)
public:
    zfstring name;
};
typedef zfstlmap<zfstring, ZFObject *> _ZFP_ZFSerializableOwnerTagMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSerializableOwnerTagDataHolder, ZFLevelZFFrameworkNormal)
{
    ZFLISTENER_LOCAL(ownerTagOnDealloc_, {
        _ZFP_ZFSerializableOwnerTagData *ownerTagData = userData->to<_ZFP_ZFSerializableOwnerTagData *>();
        ZFSerializableOwnerTagSet(ownerTagData->name, zfnull);
    })
    this->ownerTagOnDealloc = ownerTagOnDealloc_;
}
_ZFP_ZFSerializableOwnerTagMapType ownerTagMap;
ZFListener ownerTagOnDealloc;
ZF_GLOBAL_INITIALIZER_END(ZFSerializableOwnerTagDataHolder)
void ZFSerializableOwnerTagSet(ZF_IN const zfchar *name,
                               ZF_IN ZFObject *obj)
{
    if(name == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();
    _ZFP_ZFSerializableOwnerTagMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableOwnerTagDataHolder)->ownerTagMap;
    _ZFP_ZFSerializableOwnerTagMapType::iterator it = m.find(name);
    ZFObject *objRemoved = zfnull;
    ZFObject *objAdded = zfnull;
    if(it != m.end())
    {
        objRemoved = it->second;
        if(obj == zfnull)
        {
            m.erase(it);
        }
        else
        {
            objAdded = obj;
            it->second = obj;
        }
    }
    else
    {
        if(obj != zfnull)
        {
            objAdded = obj;
            m[name] = obj;
        }
    }
    if(objRemoved != objAdded)
    {
        if(objRemoved != zfnull)
        {
            objRemoved->observerRemove(
                ZFObject::EventObjectBeforeDealloc(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableOwnerTagDataHolder)->ownerTagOnDealloc);
        }
        if(objAdded != zfnull)
        {
            zfblockedAlloc(_ZFP_ZFSerializableOwnerTagData, ownerTagData);
            ownerTagData->name = name;
            objAdded->observerAddWithLevel(ZFLevelAppLow,
                ZFObject::EventObjectBeforeDealloc(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableOwnerTagDataHolder)->ownerTagOnDealloc,
                ownerTagData);
        }
    }
}
ZFObject *ZFSerializableOwnerTagGet(ZF_IN const zfchar *name)
{
    if(name == zfnull)
    {
        return zfnull;
    }

    ZFCoreMutexLocker();
    _ZFP_ZFSerializableOwnerTagMapType &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSerializableOwnerTagDataHolder)->ownerTagMap;
    _ZFP_ZFSerializableOwnerTagMapType::iterator it = m.find(name);
    if(it != m.end())
    {
        return it->second;
    }
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END


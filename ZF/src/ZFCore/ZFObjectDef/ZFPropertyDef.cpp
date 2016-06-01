/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfbool ZFProperty::propertyTypeIdIsMatch(ZF_IN const zfchar *type0, ZF_IN const zfchar *type1)
{
    if(type0 == zfnull || type1 == zfnull)
    {
        return zffalse;
    }
    return (zfscmpTheSame(type0, type1)
        || (zfscmpTheSame(type0, ZFPropertyTypeId_none)
        && zfscmpTheSame(type1, ZFPropertyTypeId_none)));
}

void ZFProperty::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '(';
    if(zfslen(this->propertyTypeIdName()) > 0)
    {
        ret += this->propertyTypeIdName();
    }
    else
    {
        ret += this->propertyTypeName();
    }
    ret += ')';
    ret += this->ownerClass()->className();
    ret += zfText("::");
    ret += this->propertyName();
    if(!this->propertyReflectable)
    {
        ret += zfText("(NoReflect)");
    }
}

/** @cond ZFPrivateDoc */
ZFProperty::ZFProperty(void)
: callbackIsValueAccessed(zfnull)
, callbackIsInitValue(zfnull)
, callbackResetInitValue(zfnull)
, callbackCompare(zfnull)
, callbackCopy(zfnull)
, callbackRetainSet(zfnull)
, callbackRetainGet(zfnull)
, callbackAssignSet(zfnull)
, callbackAssignGet(zfnull)
, callbackGetInfo(zfnull)
, propertyReflectable(zftrue)
, _ZFP_ZFPropertyNeedInit(zftrue)
, _ZFP_ZFProperty_ownerClass(zfnull)
, _ZFP_ZFProperty_name()
, _ZFP_ZFProperty_typeName()
, _ZFP_ZFProperty_typeIdName()
, _ZFP_ZFProperty_setterMethod(zfnull)
, _ZFP_ZFProperty_getterMethod(zfnull)
, _ZFP_ZFProperty_propertyClassOfRetainProperty(zfnull)
{
}
ZFProperty::~ZFProperty(void)
{
}
void ZFProperty::_ZFP_ZFPropertyInit(ZF_IN const ZFClass *ownerClass,
                                     ZF_IN const zfchar *name,
                                     ZF_IN const zfchar *typeName,
                                     ZF_IN const zfcharA *typeIdName,
                                     ZF_IN const ZFMethod *setterMethod,
                                     ZF_IN const ZFMethod *getterMethod,
                                     ZF_IN const ZFClass *propertyClassOfRetainProperty)
{
    this->_ZFP_ZFProperty_ownerClass = ownerClass;
    this->_ZFP_ZFProperty_name = name;
    this->_ZFP_ZFProperty_typeName = typeName;
    this->_ZFP_ZFProperty_typeIdName = zfsCoreA2Z(typeIdName);
    if(this->_ZFP_ZFProperty_typeIdName.compare(_ZFP_ZFPropertyTypeId_ZFObject) == 0)
    {
        this->_ZFP_ZFProperty_typeIdName = ZFPropertyTypeId_none;
        this->_ZFP_ZFProperty_serializable = zftrue;
    }
    else
    {
        this->_ZFP_ZFProperty_serializable = (this->_ZFP_ZFProperty_typeIdName.compare(ZFPropertyTypeId_none) != 0);
    }
    this->_ZFP_ZFProperty_setterMethod = setterMethod;
    this->_ZFP_ZFProperty_getterMethod = getterMethod;
    this->_ZFP_ZFProperty_propertyClassOfRetainProperty = propertyClassOfRetainProperty;
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END


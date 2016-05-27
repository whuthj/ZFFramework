/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMethodDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFMethodTypeToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFMethodType const &value)
{
    switch(value)
    {
        case ZFMethodTypeClassStatic:
            ret += ZFTOKEN_ZFMethodTypeClassStatic;
            return ;
        case ZFMethodTypeClassMember:
            ret += ZFTOKEN_ZFMethodTypeClassMember;
            return ;
        case ZFMethodTypeRawFunction:
            ret += ZFTOKEN_ZFMethodTypeRawFunction;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFMethodTypeFromString(ZF_OUT ZFMethodType &ret,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFMethodTypeClassStatic,
        ZFTOKEN_ZFMethodTypeClassMember,
        ZFTOKEN_ZFMethodTypeRawFunction,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFMACRO_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFMethodTypeRawFunction;
    switch(matched)
    {
        case 0:
            ret = ZFMethodTypeClassStatic;
            return zfnull;
        case 1:
            ret = ZFMethodTypeClassMember;
            return zfnull;
        case 2:
            ret = ZFMethodTypeRawFunction;
            return zfnull;
        default:
            return src;
    }
}

// ============================================================
void ZFMethodPrivilegeTypeToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFMethodPrivilegeType const &value)
{
    switch(value)
    {
        case ZFMethodPrivilegeTypePublic:
            ret += ZFTOKEN_ZFMethodPrivilegeTypePublic;
            return ;
        case ZFMethodPrivilegeTypeProtected:
            ret += ZFTOKEN_ZFMethodPrivilegeTypeProtected;
            return ;
        case ZFMethodPrivilegeTypePrivate:
            ret += ZFTOKEN_ZFMethodPrivilegeTypePrivate;
            return ;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}
const zfchar *ZFMethodPrivilegeTypeFromString(ZF_OUT ZFMethodPrivilegeType &ret,
                                              ZF_IN const zfchar *src,
                                              ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    const zfchar *tokens[] = {
        ZFTOKEN_ZFMethodPrivilegeTypePublic,
        ZFTOKEN_ZFMethodPrivilegeTypeProtected,
        ZFTOKEN_ZFMethodPrivilegeTypePrivate,
    };
    zfindex matched = ZFCoreStringCheckMatch(tokens, ZFMACRO_ARRAY_SIZE(tokens), src, srcLen);
    ret = ZFMethodPrivilegeTypePublic;
    switch(matched)
    {
        case 0:
            ret = ZFMethodPrivilegeTypePublic;
            return zfnull;
        case 1:
            ret = ZFMethodPrivilegeTypeProtected;
            return zfnull;
        case 2:
            ret = ZFMethodPrivilegeTypePrivate;
            return zfnull;
        default:
            return src;
    }
}

// ============================================================
ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(ZFMethodDataHolder, ZFLevelZFFrameworkHigh)
{
}
ZFCoreMap methodMap; // ZFMethod *
ZF_STATIC_INITIALIZER_END(ZFMethodDataHolder)
#define _ZFP_ZFMethodMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodDataHolder)->methodMap)

ZFMethod *ZFMethod::_ZFP_ZFMethodAccess(ZF_IN const zfchar *internalMethodId)
{
    ZFCoreMutexLocker();
    ZFMethod *v = _ZFP_ZFMethodMap.get<ZFMethod *>(internalMethodId);
    if(v == zfnull)
    {
        v = zfnew(ZFMethod);
        _ZFP_ZFMethodMap.set(internalMethodId, ZFCorePointerForObject<ZFMethod *>(v));
    }
    return v;
}
void ZFMethod::_ZFP_ZFMethod_init(ZF_IN const zfchar *methodName,
                                  ZF_IN const zfchar *methodId,
                                  ZF_IN const zfchar *returnTypeName,
                                  ...)
{
    this->_ZFP_ZFMethod_methodName = methodName;
    this->_ZFP_ZFMethod_methodId = methodId + 1;
    this->_ZFP_ZFMethod_returnTypeName = returnTypeName;
    this->_ZFP_ZFMethod_paramNum = 0;

    va_list vaList;
    va_start(vaList, returnTypeName);
    const zfchar *paramType = va_arg(vaList, const zfchar *);
    while(paramType != zfnull && zfslen(paramType) > 0)
    {
        this->_ZFP_ZFMethod_paramTypeNameList[this->_ZFP_ZFMethod_paramNum++] = paramType;
        paramType = va_arg(vaList, const zfchar *);
    }
    va_end(vaList);
}
void ZFMethod::_ZFP_ZFMethod_initClassMemberType(ZF_IN const ZFClass *ownerClass,
                                                 ZF_IN ZFFuncAddrType invoker,
                                                 ZF_IN ZFMethodPrivilegeType privilegeType,
                                                 ZF_IN zfbool isStatic,
                                                 ZF_IN zfbool isVirtual,
                                                 ZF_IN zfbool isConst)
{
    if(isStatic)
    {
        this->_ZFP_ZFMethod_methodType = ZFMethodTypeClassStatic;
    }
    else
    {
        this->_ZFP_ZFMethod_methodType = ZFMethodTypeClassMember;
    }

    this->_ZFP_ZFMethod_ownerClass = ownerClass;
    this->_ZFP_ZFMethod_invoker = invoker;

    this->_ZFP_ZFMethod_privilegeType = privilegeType;
    this->_ZFP_ZFMethod_methodIsStatic = isStatic;
    this->_ZFP_ZFMethod_methodIsVirtual = isVirtual;
    this->_ZFP_ZFMethod_methodIsConst = isConst;

    this->_ZFP_ZFMethod_functionAddr = zfnull;
}
void ZFMethod::_ZFP_ZFMethod_initFunctionType(ZF_IN const zfchar *methodNamespace,
                                              ZF_IN ZFFuncAddrType functionAddress)
{
    this->_ZFP_ZFMethod_methodType = ZFMethodTypeRawFunction;

    this->_ZFP_ZFMethod_methodNamespace = methodNamespace;
    this->_ZFP_ZFMethod_functionAddr = functionAddress;

    this->_ZFP_ZFMethod_ownerClass = zfnull;
    this->_ZFP_ZFMethod_privilegeType = ZFMethodPrivilegeTypePublic;
    this->_ZFP_ZFMethod_methodIsStatic = zftrue;
    this->_ZFP_ZFMethod_methodIsVirtual = zffalse;
    this->_ZFP_ZFMethod_methodIsConst = zffalse;
}

/** @cond ZFPrivateDoc */
ZFMethod::ZFMethod(void)
: _ZFP_ZFMethodNeedInit(zftrue)
, _ZFP_ZFMethod_methodType(ZFMethodTypeRawFunction)
, _ZFP_ZFMethod_methodName()
, _ZFP_ZFMethod_methodId()
, _ZFP_ZFMethod_returnTypeName()
, _ZFP_ZFMethod_paramNum(0)
, _ZFP_ZFMethod_paramTypeNameList()
, _ZFP_ZFMethod_ownerClass(zfnull)
, _ZFP_ZFMethod_privilegeType(ZFMethodPrivilegeTypePrivate)
, _ZFP_ZFMethod_methodIsStatic(zffalse)
, _ZFP_ZFMethod_methodIsVirtual(zffalse)
, _ZFP_ZFMethod_methodIsConst(zffalse)
, _ZFP_ZFMethod_invoker(zfnull)
, _ZFP_ZFMethod_methodNamespace()
, _ZFP_ZFMethod_functionAddr(zfnull)
{
}
/** @endcond */
ZFMethod::~ZFMethod(void)
{
}

void ZFMethod::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    if(this->methodType() == ZFMethodTypeRawFunction)
    {
        ret += this->methodNamespace();
        ret += zfText("::");
        ret += this->methodName();
    }
    else
    {
        ret += this->ownerClass()->className();
        ret += zfText("::");
        ret += this->methodName();
    }
}

const zfchar *ZFMethod::methodPrivilegeTypeString(void) const
{
    switch(this->_ZFP_ZFMethod_privilegeType)
    {
        case ZFMethodPrivilegeTypePublic:
            return zfText("public");
        case ZFMethodPrivilegeTypeProtected:
            return zfText("protected");
        case ZFMethodPrivilegeTypePrivate:
            return zfText("private");
    }
    return zfText("<UnknownPrivilegeType>");
}

ZF_NAMESPACE_GLOBAL_END


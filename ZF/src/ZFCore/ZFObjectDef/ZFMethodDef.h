/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodDef.h
 * @brief reflectable method definination for ZFFramework
 */

#ifndef _ZFI_ZFMethodDef_h_
#define _ZFI_ZFMethodDef_h_

#include "../ZFCoreDef.h"
#include "ZFObjectClassTypeFwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief method type
 */
typedef enum
{
    ZFMethodTypeClassStatic, /**< @brief class static member method */
    ZFMethodTypeClassMember, /**< @brief class memeber method that need a object instance to execute */
    ZFMethodTypeRawFunction, /**< @brief static function */
} ZFMethodType;
/** @brief string tokens */
#define ZFTOKEN_ZFMethodTypeClassStatic zfText("TypeClassStatic")
/** @brief string tokens */
#define ZFTOKEN_ZFMethodTypeClassMember zfText("TypeClassMember")
/** @brief string tokens */
#define ZFTOKEN_ZFMethodTypeRawFunction zfText("TypeRawFunction")

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFMethodType, e0, ZFMethodType, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFMethodType, e0, ZFMethodType, e1)
ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFMethodType, ZFMethodType)

// ============================================================
/**
 * @brief access type for ZFMethod
 */
typedef enum
{
    ZFMethodPrivilegeTypePublic, /**< @brief public */
    ZFMethodPrivilegeTypeProtected, /**< @brief protected */
    ZFMethodPrivilegeTypePrivate, /**< @brief private */
} ZFMethodPrivilegeType;
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypePublic zfText("public")
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypeProtected zfText("protected")
/** @brief string tokens */
#define ZFTOKEN_ZFMethodPrivilegeTypePrivate zfText("private")

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFMethodPrivilegeType, e0, ZFMethodPrivilegeType, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFMethodPrivilegeType, e0, ZFMethodPrivilegeType, e1)

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFMethodPrivilegeType, ZFMethodPrivilegeType)

// ============================================================
zfclassFwd ZFObject;
zfclassFwd ZFClass;
#define _ZFP_ZFMETHOD_INVOKER(N) \
    /** @brief see #ZFMethod */ \
    template<typename T_ReturnType ZFMACRO_REPEAT(N, ZFMARCO_REPEAT_TEMPLATE, ZFMACRO_COMMA)> \
    inline T_ReturnType execute(ZFObject *obj ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_COMMA)) const \
    { \
        switch(this->_ZFP_ZFMethod_methodType) \
        { \
            case ZFMethodTypeClassStatic: \
            case ZFMethodTypeClassMember: \
                return ZFCastReinterpret( \
                        T_ReturnType (*)(ZFObject * ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_COMMA)), \
                        this->_ZFP_ZFMethod_invoker) \
                    (obj ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_NAME, ZFMACRO_COMMA)); \
            case ZFMethodTypeRawFunction: \
            default: \
                return ZFCastReinterpret( \
                        T_ReturnType (*)(ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_EMPTY)), \
                        this->_ZFP_ZFMethod_functionAddr) \
                    (ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_NAME, ZFMACRO_EMPTY)); \
        } \
    } \
    /** @brief see #ZFMethod */ \
    template<typename T_ReturnType ZFMACRO_REPEAT(N, ZFMARCO_REPEAT_TEMPLATE, ZFMACRO_COMMA)> \
    inline T_ReturnType executeClassMember(ZFObject *obj ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_COMMA)) const \
    { \
        return ZFCastReinterpret( \
                T_ReturnType (*)(ZFObject * ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_COMMA)), \
                this->_ZFP_ZFMethod_invoker) \
            (obj ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_NAME, ZFMACRO_COMMA)); \
    } \
    /** @brief see #ZFMethod */ \
    template<typename T_ReturnType ZFMACRO_REPEAT(N, ZFMARCO_REPEAT_TEMPLATE, ZFMACRO_COMMA)> \
    inline T_ReturnType executeClassStatic(ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_EMPTY)) const \
    { \
        return ZFCastReinterpret( \
                T_ReturnType (*)(ZFObject * ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_COMMA)), \
                this->_ZFP_ZFMethod_invoker) \
            (zfnull ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_NAME, ZFMACRO_COMMA)); \
    } \
    /** @brief see #ZFMethod */ \
    template<typename T_ReturnType ZFMACRO_REPEAT(N, ZFMARCO_REPEAT_TEMPLATE, ZFMACRO_COMMA)> \
    inline T_ReturnType executeFunction(ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_EMPTY)) const \
    { \
        return ZFCastReinterpret( \
                T_ReturnType (*)(ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_PARAM, ZFMACRO_EMPTY)), \
                this->_ZFP_ZFMethod_functionAddr) \
            (ZFMACRO_REPEAT(N, ZFMACRO_REPEAT_NAME, ZFMACRO_EMPTY)); \
    }

/**
 * @brief max param supported by ZFMethod
 */
#define ZFMETHOD_MAX_PARAM 4

// ============================================================
/**
 * @brief reflectable method for ZFObject
 *
 * to use ZFMethod, you must first declare it in your class:
 * @code
 *   zfclass YourClass : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(YourClass, ZFObject)
 *      / **
 *        * you can add doxygen docs here
 *        * /
 *       ZFMETHOD_MEMBER_DECLARE_0(void, yourMethod)
 *       {
 *           // method code here
 *       }
 *   };
 * @endcode
 * or you may want to declare in header and define in cpp file:
 * @code
 *   // YourClass.h
 *   zfclass YourClass : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(YourClass, ZFObject)
 *       ZFMETHOD_MEMBER_DECLARE_0(void, yourMethod); // declare only
 *   };
 *
 *   // YourClass.cpp
 *   ZFMETHOD_MEMBER_DEFINE_0(YourClass, void, yourMethod)
 *   {
 *       // method code here
 *   }
 * @endcode
 * or define as function type method:
 * @code
 *   // in cpp file only
 *   ZFMETHOD_FUNCTION_DECLARE_0(void, yourMethod)
 *   {
 *       // method code here
 *   }
 * @endcode
 * you can use any types for return value and param, including reference type,
 * while the param's num is limited to #ZFMETHOD_MAX_PARAM\n
 * you can declare overloaded method,
 * but you must specify the methodId for each of them (see bellow)\n
 * \n
 * the ZFMETHOD_MEMBER_DECLARE_XXX macro takes many params,
 * to walk through it, let's take a example:
 * @code
 *   ZFMETHOD_MEMBER_DECLARE_DETAIL_2(
 *       PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot,
 *       ReturnType, MethodName, ZFMethodIdOrNoId
 *       , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0
 *       , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1
 *       )
 * @endcode
 * -  PublicOrProtectedOrPrivate is the access type for the method,
 *   could be: public, protected, private
 * -  ZFMethodStaticOrNot indicates whether the method is static,
 *   could be: ZFMethodIsStatic, ZFMethodNotStatic
 * -  ZFMethodVirtualOrNot indicates whether the method is virtual,
 *   could be: ZFMethodIsVirtual, ZFMethodNotVirtual
 * -  ZFMethodConstOrNot indicates whether the method is const,
 *   could be: ZFMethodIsConst, ZFMethodNotConst
 * -  ReturnType is the method's return value's type,
 *   could be any types
 * -  MethodName is the method's name
 * -  ZFMethodIdOrNoId is an extra string appended to method
 *   to take care of overloaded method (which have the same method name),
 *   could be any zfcharA sequences which is valid for a function name,
 *   but it's recommended to use the param type string,
 *   such as "int_double" if the method is "void func(int, double)"\n
 *   it's only necessary if you have overloaded methods and
 *   want to reflect them from ZFClass
 * -  ParamTypeN is the type of param at index N,
 *   could be any types
 * -  paramN is the name of param at index N,
 *   could be any string valid for a variable name
 * -  ZFMethodNoDefaultParamOrDefaultParamN is the default value for the param,
 *   could be ZFMethodNoDefaultParam or ZFMethodDefaultParam(YourDefaultValue)
 *
 * to override a method declared in parent,
 * subclass should use ZFMETHOD_MEMBER_OVERRIDE_DECLARE_XXX/ZFMETHOD_MEMBER_OVERRIDE_DEFINE_XXX,
 * which is similar to ZFMETHOD_MEMBER_DECLARE_XXX/ZFMETHOD_MEMBER_DEFINE_XXX\n
 * overrided method isn't belong to subclass,
 * they still can only be found by parent's ZFClass\n
 * \n
 * once declared, the method would be automatically registered to
 * it's owner class when at least one instance created\n
 * also, similar to #ZFOBJECT_REGISTER,
 * you can add ZFMETHOD_REGISTER to your header file or cpp file to
 * statically register ZFMethod, so that you may access the method
 * even if none of instance ever created\n
 * \n
 * to access the method, use ZFMethodAccessClassMember/ZFMethodAccessClassMemberDetail is recommended:
 * @code
 *   const ZFMethod *method = ZFMethodAccessClassMember(YourClassType, yourMethodName);
 * @endcode
 * also, you may use ZFClass's member function to reflect from string,
 * such as methodForName (see #ZFClass for more info)\n
 * \n
 * once you have successfully get the ZFMethod data,
 * you can execute it without knowing the owner's class type:
 * @code
 *   const ZFClass *cls = ...; // we may not know the actual class type
 *   const ZFMethod *method = cls->methodForName(zfText("someMethod"));
 *   zfautoObject objTmp = cls->newInstance();
 *   ZFObject *obj = objTmp.toObject();
 *
 *   if(!method->methodIsStatic())
 *   { // execute
 *       YourReturnType result = method->executeClassMember<YourReturnType, ParamType...>(obj, someParam...);
 *   }
 *   else
 *   { // or executeClassStatic if the method is static
 *       YourReturnType result = method->executeClassStatic<YourReturnType, ParamType...>(someParam...);
 *   }
 *
 *   // or, you may use generic version:
 *   method->execute<YourReturnType, ParamType...>(obj, someParam...);
 * @endcode
 * @warning you take the full responsibility to make sure
 *   the ReturnType and ParamType exactly match the method,
 *   if not, no compile error would occurred,
 *   but app may (or may not) crash at runtime
 *
 * \n
 * if you want to reflect overloaded method, use both methodName and methodId,
 * the methodId is defined in ZFMETHOD_MEMBER_DECLARE_XXX (ZFMethodIdOrNoId):
 * @code
 *   cls->methodForName(zfText("methodName"), zfText("methodId"));
 * @endcode
 *
 * @warning in subclass, you may declare a method with the same name of parent's one,
 *   but the behavior may be unexpected in some situations,
 *   so it's highly deprecated to do so
 * @note it's thread-safe to execute ZFMethod,
 *   but whether the actual run code (the code in the method body)
 *   is thread-safe, is depends on you
 */
zffinal zfclassNotPOD ZF_ENV_EXPORT ZFMethod
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFMethod)

public:
    static ZFMethod *_ZFP_ZFMethodAccess(ZF_IN const zfchar *internalMethodId);
    zfbool _ZFP_ZFMethodNeedInit;
    void _ZFP_ZFMethod_init(ZF_IN const zfchar *methodName,
                            ZF_IN const zfchar *methodId,
                            ZF_IN const zfchar *returnTypeName,
                            ...);
    void _ZFP_ZFMethod_initClassMemberType(ZF_IN const ZFClass *ownerClass,
                                           ZF_IN ZFFuncAddrType invoker,
                                           ZF_IN ZFMethodPrivilegeType privilegeType,
                                           ZF_IN zfbool isStatic,
                                           ZF_IN zfbool isVirtual,
                                           ZF_IN zfbool isConst);
    void _ZFP_ZFMethod_initFunctionType(ZF_IN const zfchar *methodNamespace,
                                        ZF_IN ZFFuncAddrType functionAddress);
    /** @cond ZFPrivateDoc */
    ZFMethod(void);
    /** @endcond */
    ~ZFMethod(void);

public:
    _ZFP_ZFMETHOD_INVOKER(0)
    _ZFP_ZFMETHOD_INVOKER(1)
    _ZFP_ZFMETHOD_INVOKER(2)
    _ZFP_ZFMETHOD_INVOKER(3)
    _ZFP_ZFMETHOD_INVOKER(4)

public:
    /**
     * @brief get a short info about the method
     *
     * may look like this: \n
     *   OwnerClass::MethodName
     */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

    // ============================================================
    // general
public:
    /**
     * @brief get the method's type
     */
    inline ZFMethodType methodType(void) const
    {
        return this->_ZFP_ZFMethod_methodType;
    }
    /**
     * @brief get the method's name
     * @see methodId
     */
    inline const zfchar *methodName(void) const
    {
        return this->_ZFP_ZFMethod_methodName.cString();
    }
    /**
     * @brief get the method id
     *
     * the method id is user-defined id to specify the overloaded method,
     * which was defined in ZFMETHOD_MEMBER_DECLARE_XXX seris,
     * it's ensured to be an empty string if not defined, instead of null
     * @see methodName
     */
    inline const zfchar *methodId(void) const
    {
        return this->_ZFP_ZFMethod_methodId.cString();
    }

    /**
     * @brief get the method's return value's type name
     */
    inline const zfchar *methodReturnTypeName(void) const
    {
        return this->_ZFP_ZFMethod_returnTypeName.cString();
    }

    /**
     * @brief get the method's param num
     */
    inline zfindex methodParamCount(void) const
    {
        return this->_ZFP_ZFMethod_paramNum;
    }
    /**
     * @brief get the method's param type name at index
     */
    inline const zfchar *methodParamTypeNameAtIndex(ZF_IN zfindex index) const
    {
        return this->_ZFP_ZFMethod_paramTypeNameList[index];
    }

    // ============================================================
    // class member type
public:
    /**
     * @brief get the owner's class info
     */
    inline const ZFClass *ownerClass(void) const
    {
        return this->_ZFP_ZFMethod_ownerClass;
    }

    /**
     * @brief get the method's access type
     */
    inline ZFMethodPrivilegeType methodPrivilegeType(void) const
    {
        return this->_ZFP_ZFMethod_privilegeType;
    }
    /**
     * @brief get the string of access type
     */
    const zfchar *methodPrivilegeTypeString(void) const;

    /**
     * @brief true if method is static
     */
    inline zfbool methodIsStatic(void) const
    {
        return this->_ZFP_ZFMethod_methodIsStatic;
    }

    /**
     * @brief true if method is virtual
     */
    inline zfbool methodIsVirtual(void) const
    {
        return this->_ZFP_ZFMethod_methodIsVirtual;
    }

    /**
     * @brief true if method is const
     */
    inline zfbool methodIsConst(void) const
    {
        return this->_ZFP_ZFMethod_methodIsConst;
    }

    /**
     * @brief invoker function address for class member type
     */
    inline ZFFuncAddrType methodInvokerFunctionAddress(void) const
    {
        return this->_ZFP_ZFMethod_invoker;
    }

    // ============================================================
    // function type
public:
    /**
     * @brief for function type only,
     *   get the funtion's namespace or null if not function type
     */
    inline const zfchar *methodNamespace(void) const
    {
        return this->_ZFP_ZFMethod_methodNamespace.cString();
    }
    /**
     * @brief for function type only,
     *   get the funtion's address or null if not function type
     */
    inline ZFFuncAddrType methodFunctionAddress(void) const
    {
        return this->_ZFP_ZFMethod_functionAddr;
    }

private:
    // general
    ZFMethodType _ZFP_ZFMethod_methodType;
    zfstring _ZFP_ZFMethod_methodName;
    zfstring _ZFP_ZFMethod_methodId;
    zfstring _ZFP_ZFMethod_returnTypeName;
    zfindex _ZFP_ZFMethod_paramNum;
    const zfchar *_ZFP_ZFMethod_paramTypeNameList[ZFMETHOD_MAX_PARAM];

    // for class member type
    const ZFClass *_ZFP_ZFMethod_ownerClass;
    ZFMethodPrivilegeType _ZFP_ZFMethod_privilegeType;
    zfbool _ZFP_ZFMethod_methodIsStatic;
    zfbool _ZFP_ZFMethod_methodIsVirtual;
    zfbool _ZFP_ZFMethod_methodIsConst;
    ZFFuncAddrType _ZFP_ZFMethod_invoker;

    // for function
    zfstring _ZFP_ZFMethod_methodNamespace;
    ZFFuncAddrType _ZFP_ZFMethod_functionAddr;
};

// ============================================================
/** @brief see #ZFMethod */
#define ZFMethodNoId _
/** @brief see #ZFMethod */
// use a "##" to fix this problem:
//   if id is a macro such as "#define zfstringA std::string"
#define ZFMethodId(id) _##id
/** @brief see #ZFMethod */
#define ZFMethodNoDefaultParam
/** @brief see #ZFMethod */
#define ZFMethodDefaultParam(DefaultValue) = (DefaultValue)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDef_h_

#include "ZFMethodDeclareClassMemberDef.h"
#include "ZFMethodDeclareFunctionDef.h"
#include "ZFMethodFilterDef.h"


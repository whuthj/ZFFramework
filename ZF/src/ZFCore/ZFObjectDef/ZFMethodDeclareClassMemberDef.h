/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodDeclareClassMemberDef.h
 * @brief ZFMethod declaration
 */

#ifndef _ZFI_ZFMethodDeclareClassMemberDef_h_
#define _ZFI_ZFMethodDeclareClassMemberDef_h_

#include "ZFMethodDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd ZFClass;
/**
 * @brief util method to find ZFMethod, return null if method not registered
 */
extern ZF_ENV_EXPORT const ZFMethod *ZFMethodGetClassMember(ZF_IN const zfchar *className,
                                                            ZF_IN const zfchar *methodName,
                                                            ZF_IN_OPT const zfchar *methodId = zfnull);
/**
 * @brief util method to find ZFMethod, return null if method not registered
 */
extern ZF_ENV_EXPORT const ZFMethod *ZFMethodGetClassMember(ZF_IN const ZFClass *cls,
                                                            ZF_IN const zfchar *methodName,
                                                            ZF_IN_OPT const zfchar *methodId = zfnull);

// ============================================================
#define _ZFP_ZFMethodAccessClassMemberDetail(OwnerClass, MethodName, ZFMethodIdOrNoId) \
    ((const ZFMethod *)OwnerClass::_ZFP_ZFMethod_##MethodName##_##ZFMethodIdOrNoId())
/** @brief see #ZFMethod */
#define ZFMethodAccessClassMember(OwnerClass, MethodName) \
    ZFMethodAccessClassMemberDetail(OwnerClass, MethodName, ZFMethodNoId)
/** @brief see #ZFMethod */
#define ZFMethodAccessClassMemberDetail(OwnerClass, MethodName, ZFMethodIdOrNoId) \
    _ZFP_ZFMethodAccessClassMemberDetail(OwnerClass, MethodName, ZFMethodIdOrNoId)

// ============================================================
/** @brief see #ZFMethod */
#define ZFMethodIsStatic isStatic
/** @brief see #ZFMethod */
#define ZFMethodNotStatic notStatic
/** @brief see #ZFMethod */
#define ZFMethodIsVirtual isVirtual
/** @brief see #ZFMethod */
#define ZFMethodNotVirtual notVirtual
/** @brief see #ZFMethod */
#define ZFMethodIsConst isConst
/** @brief see #ZFMethod */
#define ZFMethodNotConst notConst

// ============================================================
#define _ZFP_ZFMethod_AutoRegister_isAutoRegister(MethodName, ZFMethodIdOrNoId) \
    private: \
        zfclassNotPOD ZF_ENV_EXPORT _ZFP_ZFMethodAutoRegister_##MethodName##_##ZFMethodIdOrNoId \
        { \
        public: \
            _ZFP_ZFMethodAutoRegister_##MethodName##_##ZFMethodIdOrNoId(void) \
            { \
                (void)zfself::_ZFP_ZFMethod_##MethodName##_##ZFMethodIdOrNoId(); \
            } \
        } _ZFP_ZFMethodAutoRegister_##MethodName##_##ZFMethodIdOrNoId##_; \
    public:
#define _ZFP_ZFMethod_AutoRegister_notAutoRegister(MethodName, ZFMethodIdOrNoId)
#define _ZFP_ZFMethod_AutoRegister(autoRegisterOrNot, MethodName, ZFMethodIdOrNoId) \
    _ZFP_ZFMethod_AutoRegister_##autoRegisterOrNot(MethodName, ZFMethodIdOrNoId)

#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_static_isStatic static
#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_static_notStatic
#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_static(staticOrNot) _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_static_##staticOrNot

#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_virtual_isVirtual virtual
#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_virtual_notVirtual
#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_virtual(virtualOrNot) _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_virtual_##virtualOrNot

#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const_isConst const
#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const_notConst
#define _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const(constOrNot) _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const_##constOrNot

#define _ZFP_ZFMethod_initClassMemberType_privilege_public ZFMethodPrivilegeTypePublic
#define _ZFP_ZFMethod_initClassMemberType_privilege_protected ZFMethodPrivilegeTypeProtected
#define _ZFP_ZFMethod_initClassMemberType_privilege_private ZFMethodPrivilegeTypePrivate
#define _ZFP_ZFMethod_initClassMemberType_privilege(privilege) _ZFP_ZFMethod_initClassMemberType_privilege_##privilege

#define _ZFP_ZFMethod_initClassMemberType_static_isStatic zftrue
#define _ZFP_ZFMethod_initClassMemberType_static_notStatic zffalse
#define _ZFP_ZFMethod_initClassMemberType_static(staticOrNot) _ZFP_ZFMethod_initClassMemberType_static_##staticOrNot

#define _ZFP_ZFMethod_initClassMemberType_virtual_isVirtual zftrue
#define _ZFP_ZFMethod_initClassMemberType_virtual_notVirtual zffalse
#define _ZFP_ZFMethod_initClassMemberType_virtual(virtualOrNot) _ZFP_ZFMethod_initClassMemberType_virtual_##virtualOrNot

#define _ZFP_ZFMethod_initClassMemberType_const_isConst zftrue
#define _ZFP_ZFMethod_initClassMemberType_const_notConst zffalse
#define _ZFP_ZFMethod_initClassMemberType_const(constOrNot) _ZFP_ZFMethod_initClassMemberType_const_##constOrNot

#define _ZFP_ZFMETHOD_MEMBER_DECLARE( \
    autoRegisterOrNot, \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE_( \
        autoRegisterOrNot, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
#define _ZFP_ZFMETHOD_MEMBER_DECLARE_( \
    autoRegisterOrNot, \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    /** @cond ZFPrivateDoc */ \
    _ZFP_ZFMethod_AutoRegister(autoRegisterOrNot, MethodName, ZFMethodIdOrNoId) \
    public: \
        static ZFMethod *_ZFP_ZFMethodAccess_##MethodName##_##ZFMethodIdOrNoId(void) \
        { \
            ZFMethod *_method = ZFMethod::_ZFP_ZFMethodAccess( \
                zfstringWithFormat(zfText("%s::%s_%s"), \
                        zfself::ClassData()->className(), \
                        ZFMACRO_TOSTRING(MethodName), \
                        ZFMACRO_TOSTRING(ZFMethodIdOrNoId) \
                    ).cString()); \
            if(_method->_ZFP_ZFMethodNeedInit) \
            { \
                _method->_ZFP_ZFMethodNeedInit = zffalse; \
                _method->_ZFP_ZFMethod_init( \
                    zfText(#MethodName), \
                    zfText(#ZFMethodIdOrNoId), \
                    zfText(#ReturnType), \
                    zfText(#ParamType0), \
                    zfText(#ParamType1), \
                    zfText(#ParamType2), \
                    zfText(#ParamType3), \
                    zfnull \
                    ); \
                _method->_ZFP_ZFMethod_initClassMemberType( \
                    zfself::ClassData(), \
                    ZFCastReinterpret(ZFFuncAddrType, \
                        &zfself::_ZFP_ZFMethodInvoker_##MethodName##_##ZFMethodIdOrNoId), \
                    _ZFP_ZFMethod_initClassMemberType_privilege(PublicOrProtectedOrPrivate), \
                    _ZFP_ZFMethod_initClassMemberType_static(ZFMethodStaticOrNot), \
                    _ZFP_ZFMethod_initClassMemberType_virtual(ZFMethodVirtualOrNot), \
                    _ZFP_ZFMethod_initClassMemberType_const(ZFMethodConstOrNot) \
                    ); \
                zfself::_ZFP_ZFObjectGetClass()->_ZFP_ZFClass_methodRegister(_method); \
            } \
            return _method; \
        } \
        static ZFMethod *_ZFP_ZFMethod_##MethodName##_##ZFMethodIdOrNoId(void) \
        { \
            static ZFMethod *_method = zfself::_ZFP_ZFMethodAccess_##MethodName##_##ZFMethodIdOrNoId(); \
            return _method; \
        } \
        static inline ReturnType _ZFP_ZFMethodInvoker_##MethodName##_##ZFMethodIdOrNoId(ZFObject *obj \
            ParamCommaOrEmpty0() ParamType0 param0 \
            ParamCommaOrEmpty1() ParamType1 param1 \
            ParamCommaOrEmpty2() ParamType2 param2 \
            ParamCommaOrEmpty3() ParamType3 param3 \
            ) \
        { \
            return ZFCastZFObjectUnchecked(zfself *, obj)->MethodName( \
                param0 \
                ParamCommaOrEmpty1() param1 \
                ParamCommaOrEmpty2() param2 \
                ParamCommaOrEmpty3() param3 \
                ); \
        } \
    /** @endcond */ \
    PublicOrProtectedOrPrivate: \
        _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_static(ZFMethodStaticOrNot) \
        _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_virtual(ZFMethodVirtualOrNot) \
        ReturnType MethodName( \
            ParamType0 param0 ZFMethodNoDefaultParamOrDefaultParam0 \
            ParamCommaOrEmpty1() ParamType1 param1 ZFMethodNoDefaultParamOrDefaultParam1 \
            ParamCommaOrEmpty2() ParamType2 param2 ZFMethodNoDefaultParamOrDefaultParam2 \
            ParamCommaOrEmpty3() ParamType3 param3 ZFMethodNoDefaultParamOrDefaultParam3 \
            ) _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const(ZFMethodConstOrNot)

#define _ZFP_ZFMETHOD_MEMBER_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamCommaOrEmpty0, ParamType0, param0 \
    , ParamCommaOrEmpty1, ParamType1, param1 \
    , ParamCommaOrEmpty2, ParamType2, param2 \
    , ParamCommaOrEmpty3, ParamType3, param3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DEFINE_(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ParamCommaOrEmpty0, ParamType0, param0 \
        , ParamCommaOrEmpty1, ParamType1, param1 \
        , ParamCommaOrEmpty2, ParamType2, param2 \
        , ParamCommaOrEmpty3, ParamType3, param3 \
        )
#define _ZFP_ZFMETHOD_MEMBER_DEFINE_(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamCommaOrEmpty0, ParamType0, param0 \
    , ParamCommaOrEmpty1, ParamType1, param1 \
    , ParamCommaOrEmpty2, ParamType2, param2 \
    , ParamCommaOrEmpty3, ParamType3, param3 \
    ) \
    ReturnType OwnerClass::MethodName( \
        ParamType0 param0 \
        ParamCommaOrEmpty1() ParamType1 param1 \
        ParamCommaOrEmpty2() ParamType2 param2 \
        ParamCommaOrEmpty3() ParamType3 param3 \
        ) _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const(ZFMethodConstOrNot)

#define _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE_( \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
#define _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE_( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    PublicOrProtectedOrPrivate: \
        _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_static(ZFMethodStaticOrNot) \
        _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_virtual(ZFMethodVirtualOrNot) \
        ReturnType MethodName( \
            ParamType0 param0 ZFMethodNoDefaultParamOrDefaultParam0 \
            ParamCommaOrEmpty1() ParamType1 param1 ZFMethodNoDefaultParamOrDefaultParam1 \
            ParamCommaOrEmpty2() ParamType2 param2 ZFMethodNoDefaultParamOrDefaultParam2 \
            ParamCommaOrEmpty3() ParamType3 param3 ZFMethodNoDefaultParamOrDefaultParam3 \
            ) _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const(ZFMethodConstOrNot)

#define _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamCommaOrEmpty0, ParamType0, param0 \
    , ParamCommaOrEmpty1, ParamType1, param1 \
    , ParamCommaOrEmpty2, ParamType2, param2 \
    , ParamCommaOrEmpty3, ParamType3, param3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE_(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ParamCommaOrEmpty0, ParamType0, param0 \
        , ParamCommaOrEmpty1, ParamType1, param1 \
        , ParamCommaOrEmpty2, ParamType2, param2 \
        , ParamCommaOrEmpty3, ParamType3, param3 \
        )
#define _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE_(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamCommaOrEmpty0, ParamType0, param0 \
    , ParamCommaOrEmpty1, ParamType1, param1 \
    , ParamCommaOrEmpty2, ParamType2, param2 \
    , ParamCommaOrEmpty3, ParamType3, param3 \
    ) \
    ReturnType OwnerClass::MethodName( \
        ParamType0 param0 \
        ParamCommaOrEmpty1() ParamType1 param1 \
        ParamCommaOrEmpty2() ParamType2 param2 \
        ParamCommaOrEmpty3() ParamType3 param3 \
        ) _ZFP_ZFMETHOD_MEMBER_DECLARE_TYPE_const(ZFMethodConstOrNot)

#define _ZFP_ZFMETHOD_REGISTER(OwnerClass, MethodName, ZFMethodIdOrNoId) \
    _ZFP_ZFMETHOD_REGISTER_(OwnerClass, MethodName, ZFMethodIdOrNoId)
#define _ZFP_ZFMETHOD_REGISTER_(OwnerClass, MethodName, ZFMethodIdOrNoId) \
    ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(ZFMethodRegister_##OwnerClass##_##MethodName##_##ZFMethodIdOrNoId, \
        ZFLevelZFFrameworkNormal) \
    { \
        (void)OwnerClass::_ZFP_ZFMethod_##MethodName##_##ZFMethodIdOrNoId(); \
    } \
    ZF_STATIC_INITIALIZER_END(ZFMethodRegister_##OwnerClass##_##MethodName##_##ZFMethodIdOrNoId)

/**
 * @brief see #ZFMethod
 *
 * statically register a method\n
 * you can find the method in its owner class only if it's registered,
 * which can be achieved by any of these:
 * -  any instance of owner class has ever been created,
 *   by default, #ZFClass would do the registration work for you
 *   if the owner class is not an abstract class
 * -  ZFMETHOD_REGISTER is declared
 */
#define ZFMETHOD_REGISTER(OwnerClass, MethodName) \
    _ZFP_ZFMETHOD_REGISTER(OwnerClass, MethodName, ZFMethodNoId)

/** @brief see #ZFMethod, #ZFMETHOD_REGISTER */
#define ZFMETHOD_REGISTER_DETAIL(OwnerClass, MethodName, ZFMethodIdOrNoId) \
    _ZFP_ZFMETHOD_REGISTER(OwnerClass, MethodName, ZFMethodIdOrNoId)

// ============================================================
// 0 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_0(ReturnType, MethodName \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_0( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_STATIC_0(ReturnType, MethodName \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_0( \
        public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_DETAIL_0( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_NO_AUTOREGISTER_0( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        notAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_0(OwnerClass, ReturnType, MethodName \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_0(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_STATIC_0(OwnerClass, ReturnType, MethodName \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_0(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_DETAIL_0(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )
// ============================================================
// 0 param method override declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_0(ReturnType, MethodName \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_0( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_0( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_0(OwnerClass, ReturnType, MethodName \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_0(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_0(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )

// ============================================================
// 1 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_1(ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_1( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_STATIC_1(ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_1( \
        public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_DETAIL_1( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_NO_AUTOREGISTER_1( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        notAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_1(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_1(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        )

/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_STATIC_1(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_1(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_DETAIL_1(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )
// ============================================================
// 1 param method override declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_1(ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_1( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_1( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_1(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_1(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_1(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )

// ============================================================
// 2 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_2(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_2( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_STATIC_2(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_2( \
        public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_DETAIL_2( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_NO_AUTOREGISTER_2( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        notAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_2(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_2(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_STATIC_2(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_2(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_DETAIL_2(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_COMMA, ParamType1, param1 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )
// ============================================================
// 2 param method override declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_2(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_2( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_2( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_2(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_2(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_2(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_COMMA, ParamType1, param1 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )

// ============================================================
// 3 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_3(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_3( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_STATIC_3(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_3( \
        public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_DETAIL_3( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_NO_AUTOREGISTER_3( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        notAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_3(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_3(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        , ParamType2, param2 \
        )

/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_STATIC_3(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_3(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        , ParamType2, param2 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_DETAIL_3(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_COMMA, ParamType1, param1 \
        , ZFMACRO_COMMA, ParamType2, param2 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )
// ============================================================
// 3 param method override declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_3(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_3( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_3( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_3(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_3(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        , ParamType2, param2 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_3(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_COMMA, ParamType1, param1 \
        , ZFMACRO_COMMA, ParamType2, param2 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY() \
        )

// ============================================================
// 4 param method declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_4(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_4( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        , ParamType3, param3, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_STATIC_4(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_MEMBER_DECLARE_DETAIL_4( \
        public, ZFMethodIsStatic, ZFMethodNotVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        , ParamType3, param3, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_DETAIL_4( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        isAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_COMMA, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DECLARE_NO_AUTOREGISTER_4( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DECLARE( \
        notAutoRegister, \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_COMMA, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_4(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_4(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        , ParamType2, param2 \
        , ParamType3, param3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_STATIC_4(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_MEMBER_DEFINE_DETAIL_4(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        , ParamType2, param2 \
        , ParamType3, param3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_DEFINE_DETAIL_4(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_COMMA, ParamType1, param1 \
        , ZFMACRO_COMMA, ParamType2, param2 \
        , ZFMACRO_COMMA, ParamType3, param3 \
        )
// ============================================================
// 4 param method override declare
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_4(ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_4( \
        public, ZFMethodNotStatic, ZFMethodIsVirtual, ZFMethodNotConst, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        , ParamType3, param3, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DECLARE_DETAIL_4( \
    PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DECLARE( \
        PublicOrProtectedOrPrivate, ZFMethodStaticOrNot, ZFMethodVirtualOrNot, ZFMethodConstOrNot, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_COMMA, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_4(OwnerClass, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_4(OwnerClass, ZFMethodNotConst, ReturnType, MethodName \
        , ParamType0, param0 \
        , ParamType1, param1 \
        , ParamType2, param2 \
        , ParamType3, param3 \
        )
/** @brief see #ZFMethod */
#define ZFMETHOD_MEMBER_OVERRIDE_DEFINE_DETAIL_4(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    _ZFP_ZFMETHOD_MEMBER_OVERRIDE_DEFINE(OwnerClass, ZFMethodConstOrNot, ReturnType, MethodName \
        , ZFMACRO_COMMA, ParamType0, param0 \
        , ZFMACRO_COMMA, ParamType1, param1 \
        , ZFMACRO_COMMA, ParamType2, param2 \
        , ZFMACRO_COMMA, ParamType3, param3 \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDeclareClassMemberDef_h_


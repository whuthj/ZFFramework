/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFMethodDeclareFunctionDef.h
 * @brief ZFMethod declaration
 */

#ifndef _ZFI_ZFMethodDeclareFunctionDef_h_
#define _ZFI_ZFMethodDeclareFunctionDef_h_

#include "ZFMethodDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFMethodAccessFunctionWithDetail(MethodNamespace, MethodName, ZFMethodIdOrNoId) \
    ((const ZFMethod *)_ZFP_ZFMethodForFunction_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId())
/** @brief see #ZFMethod */
#define ZFMethodAccessFunction(MethodName) \
    ZFMethodAccessFunctionWithDetail(ZFMethodFunctionGlobalNamespace, MethodName, ZFMethodNoId)
/** @brief see #ZFMethod */
#define ZFMethodAccessFunctionWithDetail(MethodNamespace, MethodName, ZFMethodIdOrNoId) \
    _ZFP_ZFMethodAccessFunctionWithDetail(MethodNamespace, MethodName, ZFMethodIdOrNoId)

// ============================================================
/**
 * @brief get function type ZFMethod that declared by #ZFMETHOD_FUNCTION_DECLARE_0
 */
extern ZF_ENV_EXPORT const ZFMethod *ZFMethodGetFunction(ZF_IN const zfchar *methodName,
                                                         ZF_IN_OPT const zfchar *methodId = zfnull,
                                                         ZF_IN_OPT const zfchar *methodNamespace = zfnull);

/**
 * @brief usually for debug use only
 */
extern ZF_ENV_EXPORT ZFCoreArrayPOD<const ZFMethod *> ZFMethodFunctionGetAll(void);

// ============================================================
/**
 * @brief see #ZFMETHOD_FUNCTION_DECLARE_0
 */
#define ZFMethodFunctionGlobalNamespace

extern ZF_ENV_EXPORT void _ZFP_ZFMethodFunctionRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN ZFMethod *method);
extern ZF_ENV_EXPORT void _ZFP_ZFMethodFunctionUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN ZFMethod *method);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFMethodFunctionRegisterHolder
{
public:
    _ZFP_ZFMethodFunctionRegisterHolder(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN ZFMethod *method)
    : ZFCoreLibDestroyFlag(ZFCoreLibDestroyFlag)
    , method(method)
    {
        _ZFP_ZFMethodFunctionRegister(ZFCoreLibDestroyFlag, method);
    }
    ~_ZFP_ZFMethodFunctionRegisterHolder(void)
    {
        if(!*(this->ZFCoreLibDestroyFlag))
        {
            _ZFP_ZFMethodFunctionUnregister(this->ZFCoreLibDestroyFlag, this->method);
        }
    }
private:
    zfbool *ZFCoreLibDestroyFlag;
    ZFMethod *method;
};
#define _ZFP_ZFMETHOD_FUNCTION_DECLARE( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DECLARE_( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
#define _ZFP_ZFMETHOD_FUNCTION_DECLARE_( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    static ReturnType _ZFP_ZFMethodFunction_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId( \
        ParamType0 param0 ZFMethodNoDefaultParamOrDefaultParam0 \
        ParamCommaOrEmpty1() ParamType1 param1 ZFMethodNoDefaultParamOrDefaultParam1 \
        ParamCommaOrEmpty2() ParamType2 param2 ZFMethodNoDefaultParamOrDefaultParam2 \
        ParamCommaOrEmpty3() ParamType3 param3 ZFMethodNoDefaultParamOrDefaultParam3 \
        ); \
    static ZFMethod *_ZFP_ZFMethodForFunctionAccess_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId(void) \
    { \
        ZFMethod *_method = ZFMethod::_ZFP_ZFMethodAccess( \
            zfstringWithFormat(zfText("(function)%s::%s_%s"), \
                zfText(#MethodNamespace), \
                zfText(#MethodName), \
                zfText(#ZFMethodIdOrNoId) \
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
            _method->_ZFP_ZFMethod_initFunctionType( \
                zfText(#MethodNamespace), \
                ZFCastReinterpret(ZFFuncAddrType, \
                    _ZFP_ZFMethodFunction_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId) \
                ); \
        } \
        static zfbool ZFCoreLibDestroyFlag = zffalse; \
        static _ZFP_ZFMethodFunctionRegisterHolder _holder(&ZFCoreLibDestroyFlag, _method); \
        return _method; \
    } \
    static ZFMethod *_ZFP_ZFMethodForFunction_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId(void) \
    { \
        static ZFMethod *_method = _ZFP_ZFMethodForFunctionAccess_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId(); \
        return _method; \
    } \
    ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFMethodRegister_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId, ZFLevelZFFrameworkNormal) \
    { \
        (void)_ZFP_ZFMethodForFunction_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId(); \
    } \
    ZF_STATIC_INITIALIZER_END(_ZFP_ZFMethodRegister_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId) \
    static ReturnType _ZFP_ZFMethodFunction_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId( \
        ParamType0 param0 ZFMethodNoDefaultParamOrDefaultParam0 \
        ParamCommaOrEmpty1() ParamType1 param1 ZFMethodNoDefaultParamOrDefaultParam1 \
        ParamCommaOrEmpty2() ParamType2 param2 ZFMethodNoDefaultParamOrDefaultParam2 \
        ParamCommaOrEmpty3() ParamType3 param3 ZFMethodNoDefaultParamOrDefaultParam3 \
        )

#define _ZFP_ZFMETHOD_FUNCTION_DEFINE( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DEFINE_( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
#define _ZFP_ZFMETHOD_FUNCTION_DEFINE_( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamCommaOrEmpty0, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamCommaOrEmpty1, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamCommaOrEmpty2, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamCommaOrEmpty3, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    ReturnType _ZFP_ZFMethodFunction_##MethodNamespace##_##MethodName##_##ZFMethodIdOrNoId( \
        ParamType0 param0 ZFMethodNoDefaultParamOrDefaultParam0 \
        ParamCommaOrEmpty1() ParamType1 param1 ZFMethodNoDefaultParamOrDefaultParam1 \
        ParamCommaOrEmpty2() ParamType2 param2 ZFMethodNoDefaultParamOrDefaultParam2 \
        ParamCommaOrEmpty3() ParamType3 param3 ZFMethodNoDefaultParamOrDefaultParam3 \
        )

// ============================================================
// 0 param method override declare
/**
 * @brief declare a static function method
 *
 * usage:
 * @code
 *   // in cpp files only
 *   ZFMETHOD_FUNCTION_DECLARE_XXX(...)
 *   {
 *       // function body
 *   }
 * @endcode
 * \n
 * once declared, you can find the ZFMethod by #ZFMethodGetFunction
 */
#define ZFMETHOD_FUNCTION_DECLARE_0( \
    ReturnType, MethodName \
    ) \
    ZFMETHOD_FUNCTION_DECLARE_DETAIL_0( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_DETAIL_0( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DECLARE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_0( \
    ReturnType, MethodName \
    ) \
    ZFMETHOD_FUNCTION_DEFINE_DETAIL_0( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_DETAIL_0( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DEFINE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )

// ============================================================
// 1 param method override declare
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_1( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_FUNCTION_DECLARE_DETAIL_1( \
    ZFMethodFunctionGlobalNamespace, \
    ReturnType, MethodName, ZFMethodNoId \
    , ParamType0, param0, ZFMethodNoDefaultParam \
    )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_DETAIL_1( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DECLARE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_1( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    ) \
    ZFMETHOD_FUNCTION_DEFINE_DETAIL_1( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_DETAIL_1( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DEFINE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )

// ============================================================
// 2 param method override declare
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_2( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_FUNCTION_DECLARE_DETAIL_2( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_DETAIL_2( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DECLARE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_2( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    ) \
    ZFMETHOD_FUNCTION_DEFINE_DETAIL_2( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_DETAIL_2( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DEFINE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )

// ============================================================
// 3 param method override declare
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_3( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_FUNCTION_DECLARE_DETAIL_3( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_DETAIL_3( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DECLARE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_3( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    ) \
    ZFMETHOD_FUNCTION_DEFINE_DETAIL_3( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_DETAIL_3( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DEFINE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_EMPTY, ZFMACRO_EMPTY(), ZFMACRO_EMPTY(), ZFMethodNoDefaultParam \
        )

// ============================================================
// 4 param method override declare
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_4( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_FUNCTION_DECLARE_DETAIL_4( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        , ParamType3, param3, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DECLARE_DETAIL_4( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DECLARE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_COMMA, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_4( \
    ReturnType, MethodName \
    , ParamType0, param0 \
    , ParamType1, param1 \
    , ParamType2, param2 \
    , ParamType3, param3 \
    ) \
    ZFMETHOD_FUNCTION_DEFINE_DETAIL_4( \
        ZFMethodFunctionGlobalNamespace, \
        ReturnType, MethodName, ZFMethodNoId \
        , ParamType0, param0, ZFMethodNoDefaultParam \
        , ParamType1, param1, ZFMethodNoDefaultParam \
        , ParamType2, param2, ZFMethodNoDefaultParam \
        , ParamType3, param3, ZFMethodNoDefaultParam \
        )
/** @brief see #ZFMETHOD_FUNCTION_DECLARE_0 */
#define ZFMETHOD_FUNCTION_DEFINE_DETAIL_4( \
    MethodNamespace, \
    ReturnType, MethodName, ZFMethodIdOrNoId \
    , ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
    , ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
    , ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
    , ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
    ) \
    _ZFP_ZFMETHOD_FUNCTION_DEFINE( \
        MethodNamespace, \
        ReturnType, MethodName, ZFMethodIdOrNoId \
        , ZFMACRO_COMMA, ParamType0, param0, ZFMethodNoDefaultParamOrDefaultParam0 \
        , ZFMACRO_COMMA, ParamType1, param1, ZFMethodNoDefaultParamOrDefaultParam1 \
        , ZFMACRO_COMMA, ParamType2, param2, ZFMethodNoDefaultParamOrDefaultParam2 \
        , ZFMACRO_COMMA, ParamType3, param3, ZFMethodNoDefaultParamOrDefaultParam3 \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMethodDeclareFunctionDef_h_


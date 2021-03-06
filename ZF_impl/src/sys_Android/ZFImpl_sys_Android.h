/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFImpl_sys_Android_h_
#define _ZFI_ZFImpl_sys_Android_h_

#include "../ZFImpl.h"

#if ZF_ENV_sys_Android

#define _ZFP_ZFImpl_sys_Android_JNI_DEBUG 0
#if _ZFP_ZFImpl_sys_Android_JNI_DEBUG
    template<typename T_JNIType>
    T_JNIType _ZFP_ZFImpl_sys_Android_jniWrap_Created(ZF_IN T_JNIType obj,
                                                      ZF_IN const zfcharA *callerFile,
                                                      ZF_IN const zfcharA *callerFunction,
                                                      ZF_IN zfindex callerLine,
                                                      ZF_IN const zfcharA *actionName);
    template<typename T_JNIType>
    T_JNIType _ZFP_ZFImpl_sys_Android_jniWrap_Released(ZF_IN T_JNIType obj,
                                                       ZF_IN const zfcharA *callerFile,
                                                       ZF_IN const zfcharA *callerFunction,
                                                       ZF_IN zfindex callerLine,
                                                       ZF_IN const zfcharA *actionName);

    template<typename T_Type>
    void _ZFP_ZFImpl_sys_Android_jniWrap_FuncCalled(ZF_IN const T_Type *callerFile,
                                                    ZF_IN const zfcharA *callerFunction,
                                                    ZF_IN zfindex callerLine,
                                                    ZF_IN const zfcharA *actionName);

    #define JNIUtilWrap_Enable 1
    #if 1
        #define JNIUtilWrap_Created(obj, callerFile, callerFunction, callerLine, actionName) \
            _ZFP_ZFImpl_sys_Android_jniWrap_Created(obj, callerFile, callerFunction, callerLine, actionName)
        #define JNIUtilWrap_Released(obj, callerFile, callerFunction, callerLine, actionName) \
            _ZFP_ZFImpl_sys_Android_jniWrap_Released(obj, callerFile, callerFunction, callerLine, actionName)
    #endif
    #if 1
        #define JNIUtilWrap_FuncCalled(callerFile, callerFunction, callerLine, actionName) \
            _ZFP_ZFImpl_sys_Android_jniWrap_FuncCalled(callerFile, callerFunction, callerLine, actionName)
    #endif
#endif

#include "../thirdparty/JNIUtil/AndroidJNIUtil.h"
using namespace JNIUtil;
using namespace AndroidJNIUtil;

#if _ZFP_ZFImpl_sys_Android_JNI_DEBUG
    extern void _ZFP_ZFImpl_sys_Android_jniWrap_Created_action(ZF_IN const void *obj,
                                                               ZF_IN const zfcharA *callerFile,
                                                               ZF_IN const zfcharA *callerFunction,
                                                               ZF_IN zfindex callerLine,
                                                               ZF_IN const zfcharA *actionName);
    extern void _ZFP_ZFImpl_sys_Android_jniWrap_Released_action(ZF_IN const void *obj,
                                                                ZF_IN const zfcharA *callerFile,
                                                                ZF_IN const zfcharA *callerFunction,
                                                                ZF_IN zfindex callerLine,
                                                                ZF_IN const zfcharA *actionName);
    extern void _ZFP_ZFImpl_sys_Android_jniWrap_FuncCalled_action(ZF_IN const zfcharA *callerFile,
                                                                  ZF_IN const zfcharA *callerFunction,
                                                                  ZF_IN zfindex callerLine,
                                                                  ZF_IN const zfcharA *actionName);
    template<typename T_JNIType>
    T_JNIType _ZFP_ZFImpl_sys_Android_jniWrap_Created(ZF_IN T_JNIType obj,
                                                      ZF_IN const zfcharA *callerFile,
                                                      ZF_IN const zfcharA *callerFunction,
                                                      ZF_IN zfindex callerLine,
                                                      ZF_IN const zfcharA *actionName)
    {
        _ZFP_ZFImpl_sys_Android_jniWrap_Created_action(ZFCastReinterpret(const void *, obj), callerFile, callerFunction, callerLine, actionName);
        return obj;
    }
    template<typename T_JNIType>
    T_JNIType _ZFP_ZFImpl_sys_Android_jniWrap_Released(ZF_IN T_JNIType obj,
                                                       ZF_IN const zfcharA *callerFile,
                                                       ZF_IN const zfcharA *callerFunction,
                                                       ZF_IN zfindex callerLine,
                                                       ZF_IN const zfcharA *actionName)
    {
        _ZFP_ZFImpl_sys_Android_jniWrap_Released_action(ZFCastReinterpret(const void *, obj), callerFile, callerFunction, callerLine, actionName);
        return obj;
    }
    template<typename T_Type>
    void _ZFP_ZFImpl_sys_Android_jniWrap_FuncCalled(ZF_IN const T_Type *callerFile,
                                                    ZF_IN const zfcharA *callerFunction,
                                                    ZF_IN zfindex callerLine,
                                                    ZF_IN const zfcharA *actionName)
    {
        _ZFP_ZFImpl_sys_Android_jniWrap_FuncCalled_action(callerFile, callerFunction, callerLine, actionName);
    }
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_ROOT_JNI_ID com_ZFFramework_Android
#define ZFImpl_sys_Android_ROOT_JNI_NAME "com.ZFFramework.Android"

// usually used when declare JNI method
#define ZFImpl_sys_Android_JNI_ID(clsId) ZFMACRO_CAT(ZFMACRO_CAT(ZFImpl_sys_Android_ROOT_JNI_ID, _), clsId)
// usually used when find class
#define ZFImpl_sys_Android_JNI_NAME(clsName) ZFImpl_sys_Android_ROOT_JNI_NAME "." ZFMACRO_TOSTRING_A(clsName)

// ============================================================
// Object
#define ZFImpl_sys_Android_JNI_ID_Object java_lang_Object
#define ZFImpl_sys_Android_JNI_NAME_Object "java.lang.Object"
extern jclass ZFImpl_sys_Android_jclassObject(void);

// ============================================================
// String
#define ZFImpl_sys_Android_JNI_ID_String java_lang_String
#define ZFImpl_sys_Android_JNI_NAME_String "java.lang.String"
extern jclass ZFImpl_sys_Android_jclassString(void);

// ============================================================
// ZFAndroidLog
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidLog ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidLog)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidLog)

// ============================================================
// ZFAndroidUI
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidUI ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidUI)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidUI ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidUI)

// ============================================================
// utils
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertZFObjectToJNIType
#define JNIConvertZFObjectToJNIType(jniEnv, p) JNIConvertPointerToJNIType(jniEnv, ZFObjectToObject(p))
#endif
/**
 * @brief see #JNIConvertPointerToJNIType
 */
#ifndef JNIConvertZFObjectFromJNIType
#define JNIConvertZFObjectFromJNIType(jniEnv, d) ZFCastStatic(ZFObject *, JNIConvertPointerFromJNIType(jniEnv, d))
#endif

// ============================================================
extern void ZFImpl_sys_Android_zfstringFromString(ZF_IN_OUT zfstring &s, ZF_IN jobject jstr);
inline zfstring ZFImpl_sys_Android_zfstringFromString(ZF_IN jobject jstr)
{
    zfstring ret;
    ZFImpl_sys_Android_zfstringFromString(ret, jstr);
    return ret;
}
// note returned object must be deleted by DeleteLocalRef
extern jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfstring &s);
// note returned object must be deleted by DeleteLocalRef
extern jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfchar *s);

// ============================================================
extern zfstring ZFImpl_sys_Android_stackTrace(void);
extern void ZFImpl_sys_Android_stackTracePrint(void);
extern void ZFImpl_sys_Android_objectInfo(ZF_OUT zfstring &ret, ZF_IN jobject nativeObject);
inline zfstring ZFImpl_sys_Android_objectInfo(ZF_IN jobject nativeObject)
{
    zfstring ret;
    ZFImpl_sys_Android_objectInfo(ret, nativeObject);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_h_


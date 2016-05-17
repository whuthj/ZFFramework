/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android.h"
#include "ZFCore/ZFObject.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android

#if _ZFP_ZFImpl_sys_Android_JNI_DEBUG
void _ZFP_ZFImpl_sys_Android_jniWrap_Created_action(ZF_IN const void *obj,
                                                   ZF_IN const zfcharA *callerFile,
                                                   ZF_IN const zfcharA *callerFunction,
                                                   ZF_IN zfindex callerLine,
                                                   ZF_IN const zfcharA *actionName)
{
#if 0
    AndroidLog(zfTextA("JNICreated  %08X (%s) %s"),
        ZFCastReinterpret(zfuint, obj),
        actionName,
        ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine));
#endif
}
void _ZFP_ZFImpl_sys_Android_jniWrap_Released_action(ZF_IN const void *obj,
                                                    ZF_IN const zfcharA *callerFile,
                                                    ZF_IN const zfcharA *callerFunction,
                                                    ZF_IN zfindex callerLine,
                                                    ZF_IN const zfcharA *actionName)
{
#if 0
    AndroidLog(zfTextA("JNIRelease  %08X (%s) %s"),
        ZFCastReinterpret(zfuint, obj),
        actionName,
        ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine));
#endif
}
void _ZFP_ZFImpl_sys_Android_jniWrap_FuncCalled_action(ZF_IN const zfcharA *callerFile,
                                                      ZF_IN const zfcharA *callerFunction,
                                                      ZF_IN zfindex callerLine,
                                                      ZF_IN const zfcharA *actionName)
{
#if 0
    AndroidLog(zfTextA("JNIFuncCall (%s) %s"),
        actionName,
        ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine));
#endif
}
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_jclsHolder, ZFLevelZFFrameworkNormal)
{
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_Object).c_str());
    this->jclsObject = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_String).c_str());
    this->jclsString = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_jclsHolder)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsObject);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsString);
}
jclass jclsObject;
jclass jclsString;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_jclsHolder)

jclass ZFImpl_sys_Android_jclassObject(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_jclsHolder)->jclsObject;
}

void ZFImpl_sys_Android_zfstringFromString(ZF_IN_OUT zfstring &s, ZF_IN jobject jstr)
{
    if(jstr == NULL)
    {
        return ;
    }
    ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jstring nativeString = ZFCastStatic(jstring, jstr);
    const zfcharA *utf8 = JNIUtilGetStringUTFChars(jniEnv, nativeString, zfnull);
    if(utf8 != zfnull)
    {
        s += utf8;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, nativeString, utf8);
}
jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfstring &s)
{
    ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
    JNIEnv *jniEnv = JNIGetJNIEnv();
    return JNIUtilNewStringUTF(jniEnv, s.cString());
}
jobject ZFImpl_sys_Android_zfstringToString(ZF_IN const zfchar *s)
{
    ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
    JNIEnv *jniEnv = JNIGetJNIEnv();
    if(s == zfnull)
    {
        s = zfText("");
    }
    return JNIUtilNewStringUTF(jniEnv, s);
}

zfstring ZFImpl_sys_Android_stackTrace(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, zfTextA("stackTrace"),
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
        ).c_str());
    jstring jobjString = ZFCastStatic(jstring, JNIUtilCallStaticObjectMethod(jniEnv, jcls, jmId));
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, jobjString, zfnull);
    zfstring ret;
    if(utf != zfnull)
    {
        ret = utf;
    }
    JNIUtilReleaseStringUTFChars(jniEnv, jobjString, utf);
    return ret;
}
void ZFImpl_sys_Android_stackTracePrint(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, zfTextA("stackTracePrint"),
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    JNIUtilCallStaticVoidMethod(jniEnv, jcls, jmId);
}

void ZFImpl_sys_Android_objectInfo(ZF_OUT zfstring &ret, ZF_IN jobject nativeObject)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jcls = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidLog).c_str());
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jcls, zfTextA("objectInfo"),
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_String), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    jstring tmp = ZFCastStatic(jstring, JNIUtilCallStaticObjectMethod(jniEnv, jcls, jmId, nativeObject));
    const char *utf = JNIUtilGetStringUTFChars(jniEnv, tmp, zfnull);
    ret += utf;
    JNIUtilReleaseStringUTFChars(jniEnv, tmp, utf);
}

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE(jstring, ZFImpl_sys_Android_JNI_ID_ZFAndroidLog, native_1objectInfo,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjnipointerZFObject)
{
    ZFObject *obj = ZFCastZFObject(ZFObject *, JNIConvertZFObjectFromJNIType(jniEnv, zfjnipointerZFObject));
    return jniEnv->NewStringUTF(ZFObjectInfo(obj).cString());
}

#endif


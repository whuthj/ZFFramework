/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFCore_jclsHolder, ZFLevelZFFrameworkHigh)
{
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidBuffer).c_str());
    this->jclsBuffer = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFCore_jclsHolder)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsBuffer);
}
jclass jclsBuffer;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFCore_jclsHolder)

// ============================================================
// Buffer
const ZFImpl_sys_Android_Buffer ZFImpl_sys_Android_BufferZero = {0};
jclass ZFImpl_sys_Android_jclassZFAndroidBuffer(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFCore_jclsHolder)->jclsBuffer;
}
void ZFImpl_sys_Android_ZFAndroidBufferFromJava(ZF_OUT ZFImpl_sys_Android_Buffer &ret, ZF_IN jobject jobjBuffer)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsBuffer = ZFImpl_sys_Android_jclassZFAndroidBuffer();
    static jmethodID jmId_buffer = JNIUtilGetStaticMethodID(jniEnv, jclsBuffer, zfTextA("native_buffer"),
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    static jmethodID jmId_bufferSize = JNIUtilGetStaticMethodID(jniEnv, jclsBuffer, zfTextA("native_bufferSize"),
        JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    jobject jobjByteBuffer = JNIUtilCallStaticObjectMethod(jniEnv, jclsBuffer, jmId_buffer, jobjBuffer);
    jint jByteBufferSize = JNIUtilCallStaticIntMethod(jniEnv, jclsBuffer, jmId_bufferSize, jobjBuffer);
    ret.buffer = (zfbyte *)JNIUtilGetDirectBufferAddress(jniEnv, jobjByteBuffer);
    ret.bufferSize = (zfindex)jByteBufferSize;
    JNIUtilDeleteLocalRef(jniEnv, jobjByteBuffer);
}
jobject ZFImpl_sys_Android_ZFAndroidBufferToJava(ZF_IN void *buffer, ZF_IN zfindex bufferSize)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsBuffer = ZFImpl_sys_Android_jclassZFAndroidBuffer();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, jclsBuffer, zfTextA("native_bufferForByteBuffer"),
        JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
        ).c_str());
    jobject jobjByteBuffer = JNIUtilNewDirectByteBuffer(jniEnv, buffer, bufferSize);
    if(jobjByteBuffer == NULL)
    {
        return NULL;
    }
    jobject ret = JNIUtilCallStaticObjectMethod(jniEnv, jclsBuffer, jmId, jobjByteBuffer);
    JNIUtilDeleteLocalRef(jniEnv, jobjByteBuffer);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#endif


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFImpl_sys_Android_ZFCore_h_
#define _ZFI_ZFImpl_sys_Android_ZFCore_h_

#include "../ZFImpl_sys_Android.h"
#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFAndroidBuffer
// buffer object used to access raw byte[] data from/to Java side
// see ZFAndroidBuffer.java for more info
zfclassPOD ZFImpl_sys_Android_Buffer
{
public:
    zfbyte *buffer;
    zfindex bufferSize;
};
extern const ZFImpl_sys_Android_Buffer ZFImpl_sys_Android_BufferZero;
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidBuffer ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidBuffer)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidBuffer ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidBuffer)
extern jclass ZFImpl_sys_Android_jclassZFAndroidBuffer(void);

extern void ZFImpl_sys_Android_ZFAndroidBufferFromJava(ZF_OUT ZFImpl_sys_Android_Buffer &ret, ZF_IN jobject jobjBuffer);
inline ZFImpl_sys_Android_Buffer ZFImpl_sys_Android_ZFAndroidBufferFromJava(ZF_IN jobject jobjBuffer)
{
    ZFImpl_sys_Android_Buffer ret = ZFImpl_sys_Android_BufferZero;
    ZFImpl_sys_Android_ZFAndroidBufferFromJava(ret, jobjBuffer);
    return ret;
}
// note returned value must be released by DeleteLocalRef
extern jobject ZFImpl_sys_Android_ZFAndroidBufferToJava(ZF_IN void *buffer, ZF_IN zfindex bufferSize);

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFCore_h_


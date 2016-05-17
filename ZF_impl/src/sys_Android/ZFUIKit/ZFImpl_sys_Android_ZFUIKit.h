/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFImpl_sys_Android_ZFUIKit_h_
#define _ZFI_ZFImpl_sys_Android_ZFUIKit_h_

#include "../ZFImpl_sys_Android.h"
#include "../ZFCore/ZFImpl_sys_Android_ZFCore.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// Point
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidPoint ZFImpl_sys_Android_JNI_ID(ZFUIKit_NativeType_ZFAndroidPoint)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidPoint ZFImpl_sys_Android_JNI_NAME(ZFUIKit.NativeType.ZFAndroidPoint)
extern jclass ZFImpl_sys_Android_jclassZFAndroidPoint(void);
extern jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point, ZF_IN_OUT jobject jobjPoint);
// note returned value must be released by DeleteLocalRef
extern jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point);
extern void ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ZF_OUT ZFUIPoint &ret, ZF_IN jobject jobjPoint);
inline ZFUIPoint ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ZF_IN jobject jobjPoint)
{
    ZFUIPoint ret = ZFUIPointZero;
    ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ret, jobjPoint);
    return ret;
}

// ============================================================
// Size
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidSize ZFImpl_sys_Android_JNI_ID(ZFUIKit_NativeType_ZFAndroidSize)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidSize ZFImpl_sys_Android_JNI_NAME(ZFUIKit.NativeType.ZFAndroidSize)
extern jclass ZFImpl_sys_Android_jclassZFAndroidSize(void);
extern jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size, ZF_IN_OUT jobject jobjSize);
// note returned value must be released by DeleteLocalRef
extern jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size);
extern void ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ZF_OUT ZFUISize &ret, ZF_IN jobject jobjSize);
inline ZFUISize ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ZF_IN jobject jobjSize)
{
    ZFUISize ret = ZFUISizeZero;
    ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ret, jobjSize);
    return ret;
}

// ============================================================
// Margin
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidMargin ZFImpl_sys_Android_JNI_ID(ZFUIKit_NativeType_ZFAndroidMargin)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidMargin ZFImpl_sys_Android_JNI_NAME(ZFUIKit.NativeType.ZFAndroidMargin)
extern jclass ZFImpl_sys_Android_jclassZFAndroidMargin(void);
extern jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin, ZF_IN_OUT jobject jobjMargin);
// note returned value must be released by DeleteLocalRef
extern jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin);
extern void ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ZF_OUT ZFUIMargin &ret, ZF_IN jobject jobjMargin);
inline ZFUIMargin ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ZF_IN jobject jobjMargin)
{
    ZFUIMargin ret = ZFUIMarginZero;
    ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ret, jobjMargin);
    return ret;
}

// ============================================================
// Rect
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidRect ZFImpl_sys_Android_JNI_ID(ZFUIKit_NativeType_ZFAndroidRect)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidRect ZFImpl_sys_Android_JNI_NAME(ZFUIKit.NativeType.ZFAndroidRect)
extern jclass ZFImpl_sys_Android_jclassZFAndroidRect(void);
extern jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect, ZF_IN_OUT jobject jobjRect);
// note returned value must be released by DeleteLocalRef
extern jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect);
extern void ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ZF_OUT ZFUIRect &ret, ZF_IN jobject jobjRect);
inline ZFUIRect ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ZF_IN jobject jobjRect)
{
    ZFUIRect ret = ZFUIRectZero;
    ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ret, jobjRect);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFUIKit_h_


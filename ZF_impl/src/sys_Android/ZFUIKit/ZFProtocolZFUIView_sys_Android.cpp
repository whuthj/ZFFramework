/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIView ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIView ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIView)

#define ZFImpl_sys_Android_JNI_ID_ZFUIScrollView ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIScrollView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIScrollView ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIScrollView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Android, ZFUIView, ZFProtocolLevelSystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:View"))

public:
    ZFPROTOCOL_IMPLEMENTATION_CLASS(ZFUIViewImpl_sys_Android)(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIView).c_str());
        this->jclsZFUIView = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIScrollView).c_str());
        this->jclsZFUIScrollView = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);

        ZFUIKeyboardStateBuiltinImplRegister();
    }
    virtual ~ZFPROTOCOL_IMPLEMENTATION_CLASS(ZFUIViewImpl_sys_Android)(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();

        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFUIView);
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFUIScrollView);
    }

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        if(!view->classData()->classIsSubclassOf(ZFUIScrollView::ClassData()))
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_nativeViewCreate"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                    .add(JNIPointerJNIType)
                ).c_str());
            jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIView, jmId,
                JNIConvertZFObjectToJNIType(jniEnv, view));
            jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
            JNIUtilDeleteLocalRef(jniEnv, tmp);
            return ret;
        }
        else
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_nativeViewCreate"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                    .add(JNIPointerJNIType)
                ).c_str());
            jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIScrollView, jmId,
                JNIConvertZFObjectToJNIType(jniEnv, view));
            jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
            JNIUtilDeleteLocalRef(jniEnv, tmp);
            return ret;
        }
    }
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView)
    {
        // for performance, we won't have JNI call to destroy, simply delete the global ref

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeViewTmp = ZFCastStatic(jobject, nativeView);
        JNIUtilDeleteGlobalRef(jniEnv, nativeViewTmp);
    }

    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_nativeImplViewSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            ZFCastStatic(jobject, nativeImplView),
            (jint)view->internalBackgroundViewArray().count());
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_nativeViewScaleForImpl"),
            JNIGetMethodSig(JNIType::S_float, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticFloatMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView));
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_nativeViewScaleForPhysicalPixel"),
            JNIGetMethodSig(JNIType::S_float, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticFloatMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView));
    }

    // ============================================================
    // properties
public:
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_viewVisibleSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewVisible);
    }
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_viewAlphaSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_float)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jfloat)viewAlpha);
    }
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_viewUIEnableSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewUIEnable);
    }
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_viewUIEnableTreeSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewUIEnableTree);
    }
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_viewBackgroundColorSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            // must ensure it is ARGB format in Color of Java code
            (jint)viewBackgroundColor.colorValue());
    }

public:
    virtual void scrollViewScrollEnableSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_scrollViewScrollEnableSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeView()),
            (jboolean)scrollEnable);
    }
    virtual void scrollViewScrollBounceSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollBounceHorizontal,
                                           ZF_IN zfbool scrollBounceVertical,
                                           ZF_IN zfbool scrollBounceHorizontalAlways,
                                           ZF_IN zfbool scrollBounceVerticalAlways)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_scrollViewScrollBounceSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeView()),
            (jboolean)scrollBounceHorizontal,
            (jboolean)scrollBounceVertical,
            (jboolean)scrollBounceHorizontalAlways,
            (jboolean)scrollBounceVerticalAlways);
    }
    virtual void scrollViewScrollContentFrameSet(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN const ZFUIRect &frame)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_scrollViewScrollContentFrameSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeView()),
            (jint)frame.point.x,
            (jint)frame.point.y,
            (jint)frame.size.width,
            (jint)frame.size.height);
    }
    virtual zftimet scrollViewScrollAnimationStart(ZF_IN ZFUIScrollView *scrollView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_scrollViewScrollAnimationStart"),
            JNIGetMethodSig(JNIType::S_long, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return (zftimet)JNIUtilCallStaticLongMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeView()));
    }
    virtual void scrollViewScrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_scrollViewScrollAnimationStop"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeView()));
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex atIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        if(!parent->classData()->classIsSubclassOf(ZFUIScrollView::ClassData()))
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_childAdd"),
                JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
                ZFCastStatic(jobject, parent->nativeView()),
                ZFCastStatic(jobject, child->nativeView()),
                (jint)atIndex,
                (jint)childLayer,
                (jint)childLayerIndex);
        }
        else
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_childAdd"),
                JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
                ZFCastStatic(jobject, parent->nativeView()),
                ZFCastStatic(jobject, child->nativeView()),
                (jint)atIndex,
                (jint)childLayer,
                (jint)childLayerIndex);
        }
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN zfindex atIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        if(!parent->classData()->classIsSubclassOf(ZFUIScrollView::ClassData()))
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_childRemove"),
                JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
                ZFCastStatic(jobject, parent->nativeView()),
                (jint)atIndex,
                (jint)childLayer,
                (jint)childLayerIndex);
        }
        else
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_ZFUIScrollView_childRemove"),
                JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                    .add(JNIType::S_int)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
                ZFCastStatic(jobject, parent->nativeView()),
                (jint)atIndex,
                (jint)childLayer,
                (jint)childLayerIndex);
        }
    }

public:
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_viewFrameSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            rect.point.x,
            rect.point.y,
            rect.size.width,
            rect.size.height);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_layoutRequest"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()));
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_ZFUIView_measureNativeView"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        jobject jobjSize = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView),
            (jint)sizeHint.width,
            (jint)sizeHint.height);
        JNIBlockedDeleteLocalRef(jobjSize);
        ret = ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(jobjSize);
    }

private:
    jclass jclsZFUIView;
    jclass jclsZFUIScrollView;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// native methods for ZFUIView
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1ZFUIView_1notifyNeedLayout,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyNeedLayout(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1ZFUIView_1notifyLayoutRootView,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jobject jobjRect)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutRootView(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(jobjRect));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1ZFUIView_1notifyLayoutNativeImplView,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jobject jobjRect)
{
    ZFUIRect nativeImplViewRect;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutNativeImplView(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        nativeImplViewRect);
    ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(nativeImplViewRect, jobjRect);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1ZFUIView_1notifyUIEvent_1mouse,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jint mouseId,
                   jint mouseAction,
                   jint mousePointX,
                   jint mousePointY)
{
    ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, event);
    event->mouseId = mouseId;
    event->mouseAction = (ZFUIMouseActionEnum)mouseAction;
    event->mousePoint = ZFUIPointMake(mousePointX, mousePointY);
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        event);
}

JNI_METHOD_DECLARE(jboolean, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1ZFUIView_1notifyUIEvent_1key,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jint keyId,
                   jint keyAction,
                   jint keyCode,
                   jint keyCodeRaw)
{
    ZFCACHEABLE_ACCESS(ZFUIKeyEvent, ZFUIKeyEvent, event);
    event->keyId = keyId;
    event->keyAction = (ZFUIKeyActionEnum)keyAction;
    event->keyCode = (ZFUIKeyCodeEnum)keyCode;
    event->keyCodeRaw = keyCodeRaw;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        event);
    return event->eventResolved();
}

// ============================================================
// native methods for ZFUIScrollView
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1ZFUIScrollView_1notifyScrollViewDragBegin,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jint mousePosX,
                   jint mousePosY,
                   jlong mouseTime)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDragBegin(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        ZFUIPointMake(mousePosX, mousePosY), (zftimet)mouseTime);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1ZFUIScrollView_1notifyScrollViewDrag,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jint mousePosX,
                   jint mousePosY,
                   jlong mouseTime)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDrag(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        ZFUIPointMake(mousePosX, mousePosY), (zftimet)mouseTime);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1ZFUIScrollView_1notifyScrollViewDragEnd,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jlong mouseTime,
                   jboolean needScrollAni)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewDragEnd(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        (zftimet)mouseTime,
        (zfbool)needScrollAni);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1ZFUIScrollView_1notifyScrollViewScrollAnimation,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jlong relativeTimeInMiliseconds)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyScrollViewScrollAnimation(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        (zftimet)relativeTimeInMiliseconds);
}

#endif // #if ZF_ENV_sys_Android


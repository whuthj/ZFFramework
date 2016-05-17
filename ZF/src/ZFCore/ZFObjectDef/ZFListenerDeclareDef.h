/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFListenerDeclareDef.h
 * @brief util macros to declare #ZFListener as #ZFCallback
 */

#ifndef _ZFI_ZFListenerDeclareDef_h_
#define _ZFI_ZFListenerDeclareDef_h_

#include "ZFObjectDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief wrapper for #ZFMETHOD_MEMBER_DECLARE_2, see #ZFListener
 *
 * proto type:\n
 * virtual void listenerName(ZF_IN const ZFListenerData &listenerData, ZFObject *userData);
 */
#define ZFLISTENER_MEMBER_DECLARE(listenerName) \
    ZFMETHOD_MEMBER_DECLARE_2(void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_MEMBER_DEFINE(OwnerClass, listenerName) \
    ZFMETHOD_MEMBER_DEFINE_2(OwnerClass, void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_MEMBER_OVERRIDE_DECLARE(listenerName) \
    ZFMETHOD_MEMBER_OVERRIDE_DECLARE_2(void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_MEMBER_OVERRIDE_DEFINE(OwnerClass, listenerName) \
    ZFMETHOD_MEMBER_OVERRIDE_DEFINE_2(OwnerClass, void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)

/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_FUNCTION_DECLARE(listenerName) \
    ZFMETHOD_FUNCTION_DECLARE_2(void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_FUNCTION_DEFINE(listenerName) \
    ZFMETHOD_FUNCTION_DEFINE_2(void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)

/**
 * @brief util macro to expand as\n
 *   void methodPlaceholder(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *, userData)
 */
#define ZFLISTENER_PROTOTYPE_EXPAND(methodPlaceholder) \
    void methodPlaceholder(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject * userData)

/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_LOCAL_BEGIN(listenerName) \
    ZFCALLBACK_LOCAL_BEGIN_2(void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_LOCAL_END(listenerName) \
    ZFCALLBACK_LOCAL_END_WITH_TYPE(ZFListener, listenerName)
/** @brief see #ZFLISTENER_MEMBER_DECLARE */
#define ZFLISTENER_LOCAL(listenerName, listenerContent) \
    ZFLISTENER_LOCAL_BEGIN(listenerName) \
    listenerContent \
    ZFLISTENER_LOCAL_END(listenerName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFListenerDeclareDef_h_


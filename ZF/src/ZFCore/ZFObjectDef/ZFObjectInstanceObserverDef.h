/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectInstanceObserverDef.h
 * @brief global object alloc observer for #ZFObject
 */

#ifndef _ZFI_ZFObjectInstanceObserverDef_h_
#define _ZFI_ZFObjectInstanceObserverDef_h_

#include "ZFObjectObserverDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFObjectInstanceObserverStart */
zfclassLikePOD ZF_ENV_EXPORT ZFObjectInstanceObserverParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFObjectInstanceObserverParam)

public:
    /** @brief see #ZFObjectInstanceObserverStart */
    ZFCORE_PARAM_WITH_INIT(zfidentity, eventId, zfidentityInvalid)

    /** @brief see #ZFObjectInstanceObserverStart */
    ZFCORE_PARAM(ZFListener, observer)

    /** @brief see #ZFObjectInstanceObserverStart */
    _ZFP_ZFCORE_PARAM_RETAIN_DECLARE(ZFObject *, userData)
};
/**
 * @brief observer all instance of specified class, using #ZFClass::instanceObserverAdd
 *
 * automatically attach specified observer to newly allocated object of specified class
 * (and all subclass of it)\n
 * for debug use only, and may cause huge performance problems
 */
extern ZF_ENV_EXPORT zfidentity ZFObjectInstanceObserverStart(ZF_IN const ZFClass *cls,
                                                              ZF_IN const ZFCoreArray<ZFObjectInstanceObserverParam> &params);
/** @brief see #ZFObjectInstanceObserverStart */
extern ZF_ENV_EXPORT zfidentity ZFObjectInstanceObserverStart(ZF_IN const ZFClass *cls,
                                                              ZF_IN const ZFObjectInstanceObserverParam &param);
/** @brief see #ZFObjectInstanceObserverStart */
extern ZF_ENV_EXPORT zfidentity ZFObjectInstanceObserverStart(ZF_IN const ZFClass *cls,
                                                              ZF_IN zfidentity eventId,
                                                              ZF_IN const ZFListener &observer,
                                                              ZF_IN_OPT ZFObject *userData = zfnull);
/** @brief see #ZFObjectInstanceObserverStart */
extern ZF_ENV_EXPORT void ZFObjectInstanceObserverStop(ZF_IN zfidentity taskId);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectInstanceObserverDef_h_


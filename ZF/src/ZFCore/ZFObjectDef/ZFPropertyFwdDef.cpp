/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPropertyFwdDef.h"
#include "ZFObjectDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFPropertyCallbackIsValueAccessedChange(ZF_IN const ZFProperty *property,
                                             ZFPropertyCallbackIsValueAccessed callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackIsValueAccessed = callback;
}
void ZFPropertyCallbackIsInitValueChange(ZF_IN const ZFProperty *property,
                                         ZFPropertyCallbackIsInitValue callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackIsInitValue = callback;
}
void ZFPropertyCallbackResetInitValueChange(ZF_IN const ZFProperty *property,
                                            ZFPropertyCallbackResetInitValue callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackResetInitValue = callback;
}
void ZFPropertyCallbackCompareChange(ZF_IN const ZFProperty *property,
                                     ZFPropertyCallbackCompare callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackCompare = callback;
}
void ZFPropertyCallbackCopyChange(ZF_IN const ZFProperty *property,
                                  ZFPropertyCallbackCopy callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackCopy = callback;
}
void ZFPropertyCallbackRetainSetChange(ZF_IN const ZFProperty *property,
                                       ZFPropertyCallbackRetainSet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackRetainSet = callback;
}
void ZFPropertyCallbackRetainGetChange(ZF_IN const ZFProperty *property,
                                       ZFPropertyCallbackRetainGet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackRetainGet = callback;
}
void ZFPropertyCallbackAssignSetChange(ZF_IN const ZFProperty *property,
                                       ZFPropertyCallbackAssignSet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackAssignSet = callback;
}
void ZFPropertyCallbackAssignGetChange(ZF_IN const ZFProperty *property,
                                       ZFPropertyCallbackAssignGet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackAssignGet = callback;
}
void ZFPropertyCallbackGetInfoChange(ZF_IN const ZFProperty *property,
                                     ZFPropertyCallbackGetInfo callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    const_cast<ZFProperty *>(property)->callbackGetInfo = callback;
}

ZF_NAMESPACE_GLOBAL_END


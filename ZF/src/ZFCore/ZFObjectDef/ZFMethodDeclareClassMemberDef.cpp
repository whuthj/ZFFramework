/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMethodDeclareClassMemberDef.h"
#include "ZFClassDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFMethod *ZFMethodGetClassMember(ZF_IN const zfchar *className,
                                       ZF_IN const zfchar *methodName,
                                       ZF_IN_OPT const zfchar *methodId /* = zfnull */)
{
    const ZFClass *cls = ZFClass::classForName(className);
    if(cls != zfnull)
    {
        return cls->methodForName(methodName, methodId);
    }
    return zfnull;
}
const ZFMethod *ZFMethodGetClassMember(ZF_IN const ZFClass *cls,
                                       ZF_IN const zfchar *methodName,
                                       ZF_IN_OPT const zfchar *methodId /* = zfnull */)
{
    if(cls != zfnull)
    {
        return cls->methodForName(methodName, methodId);
    }
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END


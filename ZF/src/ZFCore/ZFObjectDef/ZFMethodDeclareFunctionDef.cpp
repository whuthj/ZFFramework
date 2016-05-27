/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFMethodDeclareFunctionDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFMethodFunctionData
{
public:
    zfindex refCount;
    ZFMethod *method;
    ZFCoreArrayPOD<zfbool *> ZFCoreLibDestroyFlag;
public:
    _ZFP_ZFMethodFunctionData(void)
    : refCount(1)
    , method(zfnull)
    , ZFCoreLibDestroyFlag()
    {
    }
    ~_ZFP_ZFMethodFunctionData(void)
    {
        for(zfindex i = 0; i < this->ZFCoreLibDestroyFlag.count(); ++i)
        {
            *(this->ZFCoreLibDestroyFlag[i]) = zftrue;
        }
    }
};

ZF_STATIC_INITIALIZER_INIT_WITH_LEVEL(ZFMethodFunctionDataHolder, ZFLevelZFFrameworkNormal)
{
}
ZFCoreArrayPOD<_ZFP_ZFMethodFunctionData *> methods;
ZFCoreMap methodMap; // sig to _ZFP_ZFMethodFunctionData *
ZF_STATIC_INITIALIZER_END(ZFMethodFunctionDataHolder)

static zfstring _ZFP_ZFMethodFunctionGetSig(ZF_IN const zfchar *methodNamespace,
                                            ZF_IN const zfchar *methodName,
                                            ZF_IN const zfchar *methodId)
{
    zfstring ret;
    if(methodNamespace != zfnull && *methodNamespace != '\0')
    {
        ret += methodNamespace;
        ret += zfText("::");
    }
    if(methodName != zfnull)
    {
        ret += methodName;
    }
    if(methodId != zfnull && *methodId != '\0')
    {
        ret += zfText(":");
        ret += methodId;
    }

    return ret;
}

const ZFMethod *ZFMethodGetFunction(ZF_IN const zfchar *methodName,
                                    ZF_IN_OPT const zfchar *methodId /* = zfnull */,
                                    ZF_IN_OPT const zfchar *methodNamespace /* = zfnull */)
{
    zfstring sig = _ZFP_ZFMethodFunctionGetSig(methodNamespace, methodName, methodId);
    _ZFP_ZFMethodFunctionData *data = ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFunctionDataHolder)->methodMap.get<_ZFP_ZFMethodFunctionData *>(sig.cString());
    return ((data != zfnull) ? data->method : zfnull);
}

ZFCoreArrayPOD<const ZFMethod *> ZFMethodFunctionGetAll(void)
{
    ZFCoreArrayPOD<const ZFMethod *> ret;
    const ZFCoreArrayPOD<_ZFP_ZFMethodFunctionData *> &t = ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFunctionDataHolder)->methods;
    for(zfindex i = 0; i < t.count(); ++i)
    {
        ret.add(t[i]->method);
    }
    return ret;
}

void _ZFP_ZFMethodFunctionRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN ZFMethod *method)
{
    zfstring sig = _ZFP_ZFMethodFunctionGetSig(method->methodNamespace(), method->methodName(), method->methodId());
    _ZFP_ZFMethodFunctionData *data = ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFunctionDataHolder)->methodMap.get<_ZFP_ZFMethodFunctionData *>(sig.cString());

    if(data == zfnull)
    {
        data = zfnew(_ZFP_ZFMethodFunctionData);
        data->method = method;

        ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFunctionDataHolder)->methods.add(data);
        ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFunctionDataHolder)->methodMap.set(
            sig.cString(),
            ZFCorePointerForObject<_ZFP_ZFMethodFunctionData *>(data));
    }
    else
    {
        ++(data->refCount);
    }
    data->ZFCoreLibDestroyFlag.add(ZFCoreLibDestroyFlag);
}
void _ZFP_ZFMethodFunctionUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN ZFMethod *method)
{
    zfstring sig = _ZFP_ZFMethodFunctionGetSig(method->methodNamespace(), method->methodName(), method->methodId());
    ZFCoreMap &methodMap = ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFunctionDataHolder)->methodMap;
    zfiterator it = methodMap.iteratorForKey(sig.cString());

    if(!methodMap.iteratorIsValid(it))
    {
        zfCoreCriticalShouldNotGoHere();
        return ;
    }

    _ZFP_ZFMethodFunctionData *data = methodMap.iteratorGetValue<_ZFP_ZFMethodFunctionData *>(it);
    data->ZFCoreLibDestroyFlag.removeElement(ZFCoreLibDestroyFlag);
    --(data->refCount);
    if(data->refCount == 0)
    {
        {
            ZFCoreArrayPOD<_ZFP_ZFMethodFunctionData *> &methods = ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFunctionDataHolder)->methods;
            for(zfiterator itMethod = methods.iterator(); methods.iteratorIsValid(itMethod); methods.iteratorNext(itMethod))
            {
                if(methods.iteratorGet(itMethod) == data)
                {
                    methods.iteratorRemove(itMethod);
                    break;
                }
            }
        }
        methodMap.iteratorRemove(it);
    }
}

ZF_NAMESPACE_GLOBAL_END


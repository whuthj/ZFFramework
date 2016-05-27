/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFObjectCreator_common.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_CREATOR_DEFINE(ZFObjectCreatorType_memberMethod, data)
{
    if(data == zfnull)
    {
        return zfautoObjectNull;
    }

    ZFCoreArrayPOD<zfindexRange> pos;
    if(zfnull != zfCoreDataPairSplitString(
            pos, zfHint("desiredCountOrIndexMax")3,
            data, zfHint("srcLen")zfindexMax,
            zfHint("leftToken")'\0', zfHint("rightToken")'\0',
            zfHint("separatorTokens")zfText(":"),
            zfHint("allowEmptyItem")zftrue)
       || pos[0].count == 0 || pos[1].count != 0 || pos[2].count == 0)
    {
        return zfautoObjectNull;
    }

    const ZFClass *cls = ZFClass::classForName(zfstring(data + pos[0].start, pos[0].count));
    if(cls == zfnull) {return zfautoObjectNull;}

    const ZFMethod *method = cls->methodForName(zfstring(data + pos[2].start, pos[2].count));
    if(method == zfnull || !method->methodIsStatic()) {return zfautoObjectNull;}

    return method->executeClassStatic<zfautoObject>();
}

ZFOBJECT_CREATOR_DEFINE(ZFObjectCreatorType_localMethod, data)
{
    if(data == zfnull)
    {
        return zfautoObjectNull;
    }

    ZFCoreArrayPOD<zfindexRange> pos;
    if(zfnull != zfCoreDataPairSplitString(
            pos, zfHint("desiredCountOrIndexMax")zfindexMax,
            data, zfHint("srcLen")zfindexMax,
            zfHint("leftToken")'\0', zfHint("rightToken")'\0',
            zfHint("separatorTokens")zfText(":"),
            zfHint("allowEmptyItem")zftrue)
        || (pos.count() != 3 && pos.count() != 4)
        || pos[1].count != 0 || pos[2].count == 0)
    {
        return zfautoObjectNull;
    }

    zfstring methodId;
    if(pos.count() == 4)
    {
        methodId.assign(data + pos[3].start, pos[3].count);
    }
    zfstring methodNamespace(data + pos[0].start, pos[0].count);
    const ZFMethod *method = ZFMethodGetFunction(
        zfHint("methodName")zfstring(data + pos[2].start, pos[2].count),
        zfHint("methodId")methodId.isEmpty() ? zfnull : methodId.cString(),
        zfHint("methodNamespace")methodNamespace.isEmpty() ? zfnull : methodNamespace.cString());
    if(method == zfnull || !method->methodIsStatic()) {return zfautoObjectNull;}

    return method->executeFunction<zfautoObject>();
}

ZF_NAMESPACE_GLOBAL_END


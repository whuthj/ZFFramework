/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFResCache_ZFJson.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfautoObject ZFResJson(ZF_IN const zfchar *resPath)
{
    if(resPath == zfnull)
    {
        return zfautoObjectNull;
    }

    zfstring key = zfstringWithFormat(zfText("ZFRJ:%s"), resPath);
    zfautoObject cached = ZFResCache::instance()->cacheAccess(key);
    if(cached != zfautoObjectNull)
    {
        return cached;
    }

    ZFSerializableData data;
    if(!ZFSerializableDataParseJson(data, ZFInputCallbackForResFile(resPath)))
    {
        return zfautoObjectNull;
    }
    cached = ZFObjectFromSerializableData(data);
    if(cached == zfautoObjectNull)
    {
        return zfautoObjectNull;
    }
    ZFResCache::instance()->cacheSave(key, cached.toObject());
    return cached;
}

ZF_NAMESPACE_GLOBAL_END


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFJsonHandle.h
 * @brief JSON handle util
 */

#ifndef _ZFI_ZFJsonHandle_h_
#define _ZFI_ZFJsonHandle_h_

#include "ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief JSON handle util to access json item easily
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFJsonHandle
{
public:
    /** @cond ZFPrivateDoc */
    ZFJsonHandle(void) : d(zfnull) {}
    ZFJsonHandle(ZF_IN ZFJsonItem *jsonItem) : d(jsonItem) {}
    ZFJsonHandle(ZF_IN const ZFJsonHandle &ref) : d(ref.d) {}
    ZFJsonHandle &operator = (ZF_IN ZFJsonItem *jsonItem) {d = jsonItem; return *this;}
    ZFJsonHandle &operator = (ZF_IN const ZFJsonHandle &ref) {d = ref.d; return *this;}
    operator ZFJsonItem *(void) const {return d;}
    /** @endcond */

    // ============================================================
    // conversion
public:
    /** @brief safely convert, return null if not desired type */
    ZFJsonItem *toJsonItem(void) const
    {
        return d;
    }

public:
    /** @brief safely convert, return null if not desired type */
    ZFJsonItem *toJsonValue(void) const
    {
        return ((d && d->jsonType() == ZFJsonType::e_JsonValue) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFJsonItem *toJsonObject(void) const
    {
        return ((d && d->jsonType() == ZFJsonType::e_JsonObject) ? d : zfnull);
    }
    /** @brief safely convert, return null if not desired type */
    ZFJsonItem *toJsonArray(void) const
    {
        return ((d && d->jsonType() == ZFJsonType::e_JsonArray) ? d : zfnull);
    }

public:
    /** @brief see #ZFJsonHandle */
    ZFJsonHandle operator[](ZF_IN const zfchar *const &key) const
    {
        ZFJsonItem *jsonObject = this->toJsonObject();
        if(jsonObject != zfnull)
        {
            return ZFJsonHandle(jsonObject->jsonItem(key));
        }
        return ZFJsonHandle();
    }
    /** @brief see #ZFJsonHandle */
    ZFJsonHandle operator[](ZF_IN zfindex const &jsonObjectIndex) const
    {
        ZFJsonItem *jsonArray = this->toJsonArray();
        if(jsonArray != zfnull)
        {
            return ZFJsonHandle(jsonArray->jsonObjectAtIndex(jsonObjectIndex));
        }
        return ZFJsonHandle();
    }

private:
    ZFJsonItem *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFJsonHandle_h_


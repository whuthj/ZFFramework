/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIStyle.h
 * @brief base styleable for ui elements
 */

#ifndef _ZFI_ZFUIStyle_h_
#define _ZFI_ZFUIStyle_h_

#include "ZFUITypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief base styleable for ui elements
 */
zfinterface ZF_ENV_EXPORT ZFUIStyle : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIStyle, ZFStyleable)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIStyle, ZFStyleable)
};
ZFSTYLE_DEFAULT_HOLDER_DECLARE(ZFUIStyle, ZFStyleable)

/**
 * @brief base styleable object for ui elements
 */
zfclass ZF_ENV_EXPORT ZFUIStyleObject : zfextends ZFStyleableObject, zfimplements ZFUIStyle
{
    ZFOBJECT_DECLARE(ZFUIStyleObject, ZFStyleableObject)
    ZFIMPLEMENTS_DECLARE(ZFUIStyle)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIStyle_h_


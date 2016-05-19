/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFJson.h
 * @brief protocol for ZFJson
 */

#ifndef _ZFI_ZFProtocolZFJson_h_
#define _ZFI_ZFProtocolZFJson_h_

#include "../ZFJson.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFJson
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFJson)
public:
    /**
     * @brief parse json
     */
    virtual zfbool jsonParse(ZF_IN_OUT ZFJsonItem *jsonObject,
                             ZF_IN const zfchar *src,
                             ZF_IN_OPT zfindex size = zfindexMax) = 0;
    /**
     * @brief parse json
     */
    virtual zfbool jsonParse(ZF_IN_OUT ZFJsonItem *jsonObject,
                             ZF_IN const ZFInputCallback &inputCallback) = 0;
ZFPROTOCOL_INTERFACE_END(ZFJson)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFJson_h_


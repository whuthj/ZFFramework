/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFXml.h
 * @brief protocol for ZFXml
 */

#ifndef _ZFI_ZFProtocolZFXml_h_
#define _ZFI_ZFProtocolZFXml_h_

#include "../ZFProtocol.h"
#include "../ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFXml
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFXml)
public:
    /**
     * @brief parse xml document
     *
     * you must parse the DOM tree and save to document,
     * which including:
     * -  name for each xml item
     * -  value for each xml item
     * -  CDATA for text node
     */
    virtual ZFXmlItem xmlParse(ZF_IN const zfchar *src,
                               ZF_IN_OPT zfindex size = zfindexMax,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault) = 0;
    /**
     * @brief parse xml document
     */
    virtual ZFXmlItem xmlParse(ZF_IN const ZFInputCallback &inputCallback,
                               ZF_IN_OPT const ZFXmlParseFlags &flags = ZFXmlParseFlagsDefault) = 0;
ZFPROTOCOL_INTERFACE_END(ZFXml)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFXml_h_


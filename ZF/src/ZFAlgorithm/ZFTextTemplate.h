/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTextTemplate.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplate_h_
#define _ZFI_ZFTextTemplate_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFTextTemplateApply */
zfclassLikePOD ZF_ENV_EXPORT ZFTextTemplateParam
{
public:
    /**
     * @brief see #ZFTextTemplateApply
     *
     * map to zfstring that holds the replaced text
     */
    ZFCoreMap replaceDatas;
    /**
     * @brief see #ZFTextTemplateApply
     *
     * map to zfbool that shows whether the key should be enabled
     */
    ZFCoreMap enableIfDatas;
    /**
     * @brief see #ZFTextTemplateApply
     *
     * whether the key should be enabled if not exists, false by default
     */
    zfbool enableIfByDefault;

public:
    /** @cond ZFPrivateDoc */
    ZFTextTemplateParam(void)
    : replaceDatas()
    , enableIfDatas()
    , enableIfByDefault(zffalse)
    {
    }
    /** @endcond */
};

/**
 * @brief util method to apply template to plain text
 *
 * data would be applied accorrding to #ZFTextTemplateParam,
 * and result would be written to output,
 * return newly size written if success,
 * or zfindexMax if failed\n
 * \n
 * params of #ZFTextTemplateParam:
 * -  replaceDatas: a <string, string> map to show which to replace\n
 *   assume we have a <"myKey", "myValue"> map,
 *   then the text would be applied like:
 *   @code
 *     // original:
 *     this is {ZFTT_R_myKey}, and this is {ZFTT_R_not_set}
 *
 *     // applied:
 *     this is myValue, and this is {ZFTT_R_not_set}
 *   @endcode
 * -  enableIfDatas: a <string, bool> map to show whether we should keep or remove some text blocks\n
 *   assume we have a <"myCond0", true> map and a <"myCond1", false> map,
 *   then the text would be applied like:
 *   @code
 *     // original:
 *     aaa {ZFTT_C_myCond0}text in myCond0{ZFTT_CE} bbb
 *     ccc {ZFTT_C_myCond1}text in myCond1{ZFTT_CE} ddd
 *
 *     // result:
 *     aaa text in myCond0 bbb
 *     ccc  ddd
 *   @endcode
 * -  enableIfByDefault: for entries not exist in enableIfDatas,
 *   whether we should enable it, false by default
 */
extern ZF_ENV_EXPORT zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN const ZFOutputCallback &output,
                                                 ZF_IN const zfchar *data,
                                                 ZF_IN_OPT zfindex dataSize = zfindexMax);
/** @brief see #ZFTextTemplateApply */
extern ZF_ENV_EXPORT zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN const ZFOutputCallback &output,
                                                 ZF_IN const ZFInputCallback &input);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplate_h_


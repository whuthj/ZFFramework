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

#include "ZFTextTemplateTypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util method to apply template to plain text
 *
 * data would be applied accorrding to #ZFTextTemplateParam,
 * and result would be written to output,
 * return newly size written if success,
 * or zfindexMax if failed\n
 * \n
 * params of #ZFTextTemplateParam:
 * -  replaceDatas: a <string, string> map\n
 *   replace the content with specified string\n
 *   assume we have a <"myKey", "myValue"> map,
 *   then the text would be applied like:
 *   @code
 *     // original:
 *     this is {ZFTT_R_myKey}, and this is {ZFTT_R_not_set}
 *
 *     // applied:
 *     this is myValue, and this is {ZFTT_R_not_set}
 *   @endcode
 * -  enableIfDatas: a <string, bool> map\n
 *   whether to enable text block\n
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
 * -  indexDatas: a <string, ZFTextTemplateIndexData> map\n
 *   replace the content with specified index value\n
 *   the text would be applied like:
 *   @code
 *     // original:
 *     this is {ZFTT_I_myKey} item
 *     this is {ZFTT_I_myKey} item
 *
 *     // applied:
 *     this is 0 item
 *     this is 1 item
 *   @endcode
 * -  indexDataDefault: default setting if not set in indexDatas
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


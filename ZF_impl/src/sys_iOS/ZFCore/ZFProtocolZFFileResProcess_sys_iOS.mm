/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFileResProcess.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_iOS
/*
 * res files must be placed at "YourApp.app/res/"
 *
 * recommended to use "Run Script" under "Build Phases" of XCode:
 * @code
 *   cp -rf "${SRCROOT}/relative_path_to_res/." "${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/res/" >/dev/null 2>&1
 * @endcode
 */
#import <Foundation/Foundation.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileResProcessImpl_sys_iOS, ZFLevelZFFrameworkHigh)
{
    NSString *resRootDirTmp = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"res"];
    ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
    ZFImpl_ZFFileResRootPath = resRootDirTmp.UTF8String;
    ZFImpl_ZFFileResRootPath += ZFFile::fileSeparator;
}
ZF_GLOBAL_INITIALIZER_END(ZFFileResProcessImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif


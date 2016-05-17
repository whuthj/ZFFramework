/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFFileResProcess.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// res files must be placed at "res/" in the same folder of your executable

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFileResProcessImpl_default, ZFLevelZFFrameworkHigh)
{
    ZFImpl_ZFFileResRootPath = ZFFile::modulePath();
    ZFImpl_ZFFileResRootPath += ZFFile::fileSeparator;
    #if ZF_ENV_sys_MacOS
        ZFImpl_ZFFileResRootPath += zfText("..");
        ZFImpl_ZFFileResRootPath += ZFFile::fileSeparator;
        ZFImpl_ZFFileResRootPath += zfText("Resources");
        ZFImpl_ZFFileResRootPath += ZFFile::fileSeparator;
    #endif
    ZFImpl_ZFFileResRootPath += zfText("res");
    ZFImpl_ZFFileResRootPath += ZFFile::fileSeparator;
}
ZF_GLOBAL_INITIALIZER_END(ZFFileResProcessImpl_default)

ZF_NAMESPACE_GLOBAL_END


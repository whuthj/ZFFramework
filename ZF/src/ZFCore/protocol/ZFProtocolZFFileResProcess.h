/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFProtocolZFFileResProcess.h
 * @brief protocol for ZFFile
 */

#ifndef _ZFI_ZFProtocolZFFileResProcess_h_
#define _ZFI_ZFProtocolZFFileResProcess_h_

#include "ZFProtocolZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief util variable to store root path for file res impl
 *
 * for most file res impl, res processing is the same as normal file processing,
 * except the root path differs,
 * so we supply an default impl that process like normal files,
 * and let different impl to specify different root path\n
 * the root path must be set properly before use the file res module,
 * set during #ZFFrameworkInit as level #ZFLevelZFFrameworkHigh is recommended,
 * and typically you should not change it at runtime\n
 * by default, this variable would have "./res/" as init value
 */
extern ZF_ENV_EXPORT zfstring ZFImpl_ZFFileResRootPath;

/**
 * @brief protocol for ZFFile
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFFileResProcess)
ZFPROTOCOL_INTERFACE_OPTIONAL

public:
    /**
     * @brief see ZFFile::resCopy
     */
    virtual zfbool resCopy(ZF_IN const zfchar *resPath,
                           ZF_IN const zfchar *dstPath,
                           ZF_IN_OPT zfbool isRecursive = zftrue,
                           ZF_IN_OPT zfbool isForce = zffalse,
                           ZF_IN_OPT zfstring *errPos = zfnull);
    /**
     * @brief see ZFFile::resOpen
     */
    virtual ZFFileToken resOpen(ZF_IN const zfchar *resPath);
    /**
     * @brief see ZFFile::resClose
     */
    virtual zfbool resClose(ZF_IN ZFFileToken token);

    /**
     * @brief see ZFFile::resTell
     */
    virtual zfindex resTell(ZF_IN ZFFileToken token);
    /**
     * @brief see ZFFile::resSeek
     */
    virtual zfbool resSeek(ZF_IN ZFFileToken token,
                           ZF_IN zfindex byteSize,
                           ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin);

    /**
     * @brief see ZFFile::resRead
     */
    virtual zfindex resRead(ZF_IN ZFFileToken token,
                            ZF_IN void *buf,
                            ZF_IN zfindex maxByteSize);

    /**
     * @brief see ZFFile::resEof
     */
    virtual zfbool resEof(ZF_IN ZFFileToken token);
    /**
     * @brief see ZFFile::resError
     */
    virtual zfbool resError(ZF_IN ZFFileToken token);

    /**
     * @brief see #ZFFile::resFindFirst, #ZFFile::fileFindFirst
     */
    virtual zfbool resFindFirst(ZF_IN const zfchar *resPath,
                                ZF_IN_OUT ZFFileFindDataContainer &fd);
    /**
     * @brief see #ZFFile::resFindNext, #ZFFile::fileFindNext
     */
    virtual zfbool resFindNext(ZF_IN_OUT ZFFileFindDataContainer &fd);
    /**
     * @brief see #ZFFile::resFindClose, #ZFFile::fileFindClose
     */
    virtual void resFindClose(ZF_IN_OUT ZFFileFindDataContainer &fd);
ZFPROTOCOL_INTERFACE_END(ZFFileResProcess)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFFileResProcess_h_


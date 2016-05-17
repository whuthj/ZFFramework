/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFUtility.hh"
#include "ZFTestCase.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFTestCase)

void ZFTestCase::objectOnDeallocPrepare(void)
{
    zfCoreAssert(!this->testCaseIsRunning());
    zfsuper::objectOnDeallocPrepare();
}

zfbool ZFTestCase::testCaseIsRunning(void)
{
    return this->_testCaseIsRunning;
}
void ZFTestCase::testCaseOutput(ZF_IN const zfchar *info, ...)
{
    va_list vaList;
    va_start(vaList, info);
    zfstring data = zfstringWithFormatV(info, vaList);
    va_end(vaList);

    this->testCaseOnOutput(data.cString());
}
void ZFTestCase::testCaseStart(void)
{
    if(this->_testCaseIsRunning)
    {
        return ;
    }
    this->_testCaseIsRunning = zftrue;
    zfRetainWithLeakTest(this);
    this->testCaseOnStart();
}

void ZFTestCase::testCaseProgress(ZF_IN_OPT ZFObject *progress /* = zfnull */)
{
    if(!this->_testCaseIsRunning)
    {
        return ;
    }
    this->testCaseOnProgress(progress);

    zfReleaseWithLeakTest(progress);
}

void ZFTestCase::testCaseStop(ZF_IN_OPT ZFResultTypeEnum testCaseResult /* = ZFResultType::e_Success */)
{
    if(!this->_testCaseIsRunning)
    {
        return ;
    }
    this->_testCaseIsRunning = zffalse;
    this->testCaseOnStop(testCaseResult);
}

ZF_NAMESPACE_GLOBAL_END


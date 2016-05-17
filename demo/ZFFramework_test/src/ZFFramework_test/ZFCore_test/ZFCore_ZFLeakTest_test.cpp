/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFLeakTest_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFLeakTest_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        {

        ZFObject *obj1 = zfnull, *obj2 = zfnull, *obj3 = zfnull, *obj4 = zfnull;

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFLeakTest, should have no leak:"));
        ZFLeakTestBegin();
        {
            obj1 = zfAllocWithLeakTest(ZFString, zfText("obj1")); // obj1 : 1
            zfReleaseWithLeakTest(obj1); // obj1 : 0
            obj1 = zfnull;
        }
        ZFLeakTestPrintStatus();
        ZFLeakTestEnd();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFLeakTest, should have two object leaked:"));
        ZFLeakTestBegin();
        {
            obj1 = zfAllocWithLeakTest(ZFString, zfText("obj1")); // obj1 : 1
            zfRetainWithLeakTest(obj1); // obj1 : 2

            obj2 = zfAllocWithLeakTest(ZFString, zfText("obj2")); // obj2 : 1

            obj3 = zfAllocWithLeakTest(ZFString, zfText("obj3")); // obj3 : 1

            ZFLeakTestBegin();
            {
                zfRetainWithLeakTest(obj1); // obj1 : 3
                zfReleaseWithLeakTest(obj1); // obj1 : 2

                zfReleaseWithLeakTest(obj2); // obj2 : 0, over-released by disabled by default
                obj2 = zfnull;

                zfRetainWithLeakTest(obj3); // obj3 : 2
            }
            ZFLeakTestPrintStatus(); // should have obj3 leaked from 1 to 2
            ZFLeakTestEnd();

            zfReleaseWithLeakTest(obj1); // obj1 : 1

            zfReleaseWithLeakTest(obj3); // obj3 : 1
            zfReleaseWithLeakTest(obj3); // obj2 : 0
            obj3 = zfnull;
        }
        ZFLeakTestPrintStatus(); // should have obj1 leaked from 0 to 1
        zfReleaseWithoutLeakTest(obj1); // obj1 : 0
        obj1 = zfnull;
        ZFLeakTestEnd();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFLeakTest, leak test with blocked alloc object:"));
        ZFLeakTestBegin();
        {
            zfblockedAllocWithLeakTest(ZFString, obj1_tmp, zfText("obj1"));
            obj1 = obj1_tmp;
            zfRetainWithLeakTest(obj1);
        }
        ZFLeakTestPrintStatus(); // should have obj1 leaked from 0 to 1
        zfReleaseWithoutLeakTest(obj1); obj1 = zfnull;
        ZFLeakTestEnd();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFLeakTest, four leaked objects but one is filtered by instance, one by class name and one by global instance filter:"));
        ZFLeakTestBegin();
        {
            obj1 = zfAllocWithLeakTest(ZFString, zfText("obj1"));
            obj2 = zfAllocWithLeakTest(ZFString, zfText("obj2"));
            obj3 = zfAllocWithLeakTest(ZFStringEditable, zfText("obj3"));
            obj4 = zfAllocWithLeakTest(ZFObject);
        }
        ZFLeakTestExcludeInstanceAdd(obj4);
        ZFLeakTestObjectFilter filter;
        filter.filterAdd(obj2);
        filter.filterAddName(zfText("ZFStringEditable"));
        ZFLeakTestPrintStatus(filter); // should have obj1 leaked from 0 to 1
        ZFLeakTestExcludeInstanceRemove(obj4);
        zfReleaseWithoutLeakTest(obj1); obj1 = zfnull;
        zfReleaseWithoutLeakTest(obj2); obj2 = zfnull;
        zfReleaseWithoutLeakTest(obj3); obj3 = zfnull;
        zfReleaseWithoutLeakTest(obj4); obj4 = zfnull;
        ZFLeakTestEnd();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFLeakTest, over-released object:"));
        obj1 = zfAllocWithoutLeakTest(ZFString, zfText("obj1"));
        zfRetainWithoutLeakTest(obj1);
        obj2 = zfAllocWithoutLeakTest(ZFString, zfText("obj2"));
        ZFLeakTestBegin(ZFLeakTestBeginParam().checkOverReleaseSet(zftrue));
        {
            zfReleaseWithLeakTest(obj1); // 2 > 1, over-released
            zfReleaseWithLeakTest(obj2); // 1 > 0, over-released, should be printed immediately since obj2 would be deallocated
        }
        ZFLeakTestPrintStatus(); // should have obj1 over-released from 2 to 1
        zfReleaseWithoutLeakTest(obj1); obj1 = zfnull;
        obj2 = zfnull;
        ZFLeakTestEnd();
        }
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFLeakTest_test)

ZF_NAMESPACE_GLOBAL_END


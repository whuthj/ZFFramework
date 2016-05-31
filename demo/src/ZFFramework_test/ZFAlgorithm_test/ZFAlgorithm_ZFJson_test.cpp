/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFJson_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFJson_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFJson"));

        const zfchar *src = zfText(
            "{"
            "    \"k0\" : \"v0\","
            ""
            "    \"object0\" : {},"
            "    \"object1\" : {"
            "        \"k0\" : \"v0\","
            "        \"k1\" : \"v1\""
            "    },"
            "    \"object2\" : {"
            "        \"k0\" : {\"k0\" : \"v0\"}"
            "    },"
            ""
            "    \"array0\" : [],"
            "    \"array1\" : ["
            "        {\"k0\" : \"v0\"},"
            "        {\"k0\" : \"v0\"}"
            "    ],"
            ""
            "    \"k1\" : \"v1\""
            "}"
            );

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFJson parse from string:"));
        ZFJsonItem json = ZFJsonFromString(src);

        this->testCaseOutput(zfText("ZFJson parse from string, result:"));
        zfLogTrimT() << ZFJsonToString(json);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFJson add element"));
        json.jsonItemValueSet(zfText("testJsonKey"), zfText("testJsonValue"));
        this->testCaseOutput(zfText("ZFJson add element, result:"));
        zfLogTrimT() << ZFJsonToString(json);

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFJson_test)

ZF_NAMESPACE_GLOBAL_END


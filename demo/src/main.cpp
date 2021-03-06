/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFramework_test_global.h"
#include "ZFFramework_test/ZFUIKit_test/ZFUIKit_test.h"
#include "ZFCore.h"
#include "ZFUIKit.h"
#include "ZFUIWidget.h"
#include "ZFUIWebKit.h"

static zfbool _autoLeakTestOn = zftrue;
static void leakTestBegin(void)
{
    if(_autoLeakTestOn)
    {
#if 0
        zfLogTrimT() << zfText("leakTest begin");
#endif
        ZFLeakTestBegin();
    }
}
static void leakTestEnd(void)
{
    if(_autoLeakTestOn)
    {
        ZFLeakTestPrintStatus(ZFLeakTestObjectFilter().filterAddName(ZFThreadTaskRequestData::ClassData()->className()));
        ZFLeakTestEnd();
#if 0
        zfLogTrimT() << zfText("leakTest end");
#endif
    }
}

static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView);
ZFMAIN_ENTRY(params)
{
    {
        ZFCoreArray<ZFProtocolImplInfoData> implDatas = ZFProtocolImplInfoDataGetNotImplemented();
        if(!implDatas.isEmpty())
        {
            zfclassNotPOD _ZFP_main_ZFProtocolImplInfoData_sort
            {
            public:
                static ZFCompareResult action(ZF_IN ZFProtocolImplInfoData const &v0, ZF_IN ZFProtocolImplInfoData const &v1)
                {
                    return ZFComparerDefault(v0.protocolName, v1.protocolName);
                }
            };
            implDatas.sort(_ZFP_main_ZFProtocolImplInfoData_sort::action);

            zfLogTrimT() << zfText("note, these protocol has not been implemented:");
            for(zfindex i = 0; i < implDatas.count(); ++i)
            {
                zfLogTrimT().autoSpaceSet(zffalse).autoEndlSet(zffalse) << zfText("    ");
                ZFProtocolImplInfoDataPrint(implDatas[i],
                    zfLogTrimT().autoSpaceSet(zffalse).autoEndlSet(zffalse));
                zfLogTrimT();
            }
        }
    }
    if(!ZFProtocolIsAvailable(zfText("ZFUIView"))
       || !ZFProtocolIsAvailable(zfText("ZFUISysWindow"))
       || !ZFProtocolIsAvailable(zfText("ZFThreadTaskRequest")))
    {
        ZFTestCaseRunAllStart();
        return 0;
    }

    zfblockedAlloc(ZFUIWindow, window);
    window->windowShow();

    zfblockedAlloc(ZFUIKit_test_ListView, containerView);
    window->childAdd(containerView);
    containerView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

    {
        zfblockedAlloc(ZFUIKit_test_Button, button);
        containerView->childAdd(button);

        ZFLISTENER_LOCAL(onClickButton, {
            _autoLeakTestOn = !_autoLeakTestOn;
            listenerData.sender->to<ZFUIButtonBasic *>()->buttonLabelTextStringSet(zfstringWithFormat(zfText("autoLeakTest %s"), _autoLeakTestOn ? zfText("on") : zfText("off")));
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton);
        button->buttonLabelTextStringSet(zfstringWithFormat(zfText("autoLeakTest %s"), _autoLeakTestOn ? zfText("on") : zfText("off")));
        button->buttonBackgroundStyle()->to<ZFUIViewStyle *>()->viewBackgroundColorSet(ZFUIColorRed);

        zfblockedAlloc(ZFUIView, separator);
        containerView->childAdd(separator);
        separator->viewSizeMinSet(ZFUISizeMake(0, 5));
        separator->viewSizeMaxSet(ZFUISizeMake(-1, 5));
        separator->viewBackgroundColorSet(ZFUIColorGray);
    }

    _ZFP_ZFFramework_test_prepareTestCase(containerView);

    return 0;
}

static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(ZF_IN ZFUIView *containerView,
                                                           ZF_IN const zfchar *subModuleName,
                                                           ZF_IN ZFCoreArrayPOD<const ZFClass *> const &testCases);
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(ZF_IN ZFUIView *containerView,
                                                               ZF_IN const zfchar *subModuleName,
                                                               ZF_IN const ZFClass *testCase);
static void _ZFP_ZFFramework_test_prepareTestCase(ZF_IN ZFUIView *containerView)
{
    ZFCoreArrayPOD<const ZFClass *> allTestCase = ZFTestCaseGetAll();
    zfclassNotPOD _ZFP_main_class_sort
    {
    public:
        static ZFCompareResult action(ZF_IN const ZFClass * const &cls0, ZF_IN const ZFClass * const &cls1)
        {
            return ZFComparerDefault(cls0->className(), cls1->className());
        }
    };
    allTestCase.sort(_ZFP_main_class_sort::action);

    while(!allTestCase.isEmpty())
    {
        ZFCoreArrayPOD<const ZFClass *> subModule;
        zfstring subModuleName = allTestCase[0]->className();
        {
            zfindex t = zfstringFind(subModuleName, '_');
            if(t == zfindexMax)
            {
                subModuleName.removeAll();
            }
            else
            {
                subModuleName = zfstring(subModuleName.cString(), t);
            }
        }
        for(zfindex i = 0; i < allTestCase.count(); ++i)
        {
            if(zfsncmp(allTestCase[i]->className(), subModuleName.cString(), subModuleName.length()) == 0)
            {
                subModule.add(allTestCase[i]);
                allTestCase.remove(i);
                --i;
            }
        }
        _ZFP_ZFFramework_test_prepareTestCaseSubModule(containerView, subModuleName, subModule);
    }
}
zfclass _ZFP_ZFFramework_test_TestCaseSubModuleData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFFramework_test_TestCaseSubModuleData, ZFObject)

    zfstring subModuleName;
    ZFCoreArrayPOD<const ZFClass *> testCases;
};
static void _ZFP_ZFFramework_test_prepareTestCaseSubModule(ZF_IN ZFUIView *containerView,
                                                           ZF_IN const zfchar *subModuleName,
                                                           ZF_IN ZFCoreArrayPOD<const ZFClass *> const &testCases)
{
    zfblockedAlloc(ZFUIKit_test_Button, button);
    containerView->childAdd(button);

    ZFLISTENER_LOCAL(onClickButton, {
        _ZFP_ZFFramework_test_TestCaseSubModuleData *subModuleData = userData->toAny();

        zfblockedAlloc(ZFUIWindow, subModuleWindow);
        subModuleWindow->viewBackgroundColorSet(ZFUIColorWhite);
        subModuleWindow->windowShow();
        zfblockedAlloc(ZFUIKit_test_ListView, containerView);
        subModuleWindow->childAdd(containerView);
        containerView->layoutParam()->sizeParamSet(ZFUISizeParamFillWidthFillHeight);

        {
            zfblockedAlloc(ZFUIKit_test_Button, closeButton);
            containerView->childAdd(closeButton);
            closeButton->buttonLabelTextStringSet(zfText("back"));
            ZFLISTENER_LOCAL(closeButtonOnClick, {
                userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIWindow *>()->windowHide();
            })
            closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), closeButtonOnClick, subModuleWindow->objectHolder());
            closeButton->buttonBackgroundStyle()->to<ZFUIViewStyle *>()->viewBackgroundColorSet(ZFUIColorRed);

            zfblockedAlloc(ZFUIView, separator);
            containerView->childAdd(separator);
            separator->viewSizeMinSet(ZFUISizeMake(0, 5));
            separator->viewSizeMaxSet(ZFUISizeMake(-1, 5));
            separator->viewBackgroundColorSet(ZFUIColorGray);
        }

        for(zfindex i = 0; i < subModuleData->testCases.count(); ++i)
        {
            _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(containerView, subModuleData->subModuleName, subModuleData->testCases[i]);
        }
    })
    zfblockedAlloc(_ZFP_ZFFramework_test_TestCaseSubModuleData, subModuleData);
    subModuleData->subModuleName = subModuleName;
    subModuleData->testCases = testCases;
    button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton, subModuleData);
    button->buttonLabelTextStringSet(subModuleName);
}
static void _ZFP_ZFFramework_test_prepareTestCaseSubModuleTest(ZF_IN ZFUIView *containerView,
                                                               ZF_IN const zfchar *subModuleName,
                                                               ZF_IN const ZFClass *testCase)
{
    zfblockedAlloc(ZFUIKit_test_Button, button);
    containerView->childAdd(button);

    ZFLISTENER_LOCAL(onClickButton, {
        leakTestBegin();

        const ZFClass *testCase = userData->tagGet<ZFPointerHolder *>(zfText("testCase"))->holdedDataPointer<const ZFClass *>();
        ZFUIView *containerView = userData->tagGet<ZFObjectHolder *>(zfText("containerView"))->holdedObj;
        ZFTestCase *running = zfnull;
        containerView->viewUIEnableTreeSet(zffalse);
        ZFTestCaseRun(testCase, &running);
        if(running != zfnull)
        {
            ZFLISTENER_LOCAL(testCaseOnStop, {
                ZFUIView *containerView = userData->tagGet<ZFObjectHolder *>(zfText("containerView"))->holdedObj;
                containerView->viewUIEnableTreeSet(zftrue);
                if(_autoLeakTestOn)
                {
                    ZFLISTENER_LOCAL(leakTestEndDelay, {
                        leakTestEnd();
                    })
                    ZFThreadTaskRequest(leakTestEndDelay);
                }
            })
            running->observerAdd(ZFTestCase::EventTestCaseOnStop(), testCaseOnStop, userData);
        }
        else
        {
            leakTestEnd();
            containerView->viewUIEnableTreeSet(zftrue);
        }
    })
    zfblockedAlloc(ZFObject, userData);
    userData->tagSet(zfText("testCase"), zflineAlloc(ZFPointerHolder, testCase));
    userData->tagSet(zfText("containerView"), containerView->objectHolder());
    button->observerAdd(ZFUIButton::EventButtonOnClick(), onClickButton, userData);
    button->buttonLabelTextStringSet(zfstring(testCase->className() + zfslen(subModuleName) + 1));
}

ZF_GLOBAL_INITIALIZER_INIT(iOS7AutoTitleSpace)
{
    zfstring frameworkName = ZFEnvInfo::frameworkName();
    if(zfstringFindCaseInsensitive(frameworkName, zfText("iOS")) != zfindexMax
       || zfstringFindCaseInsensitive(frameworkName, zfText("iPhone")) != zfindexMax
       || zfstringFindCaseInsensitive(frameworkName, zfText("iPad")) != zfindexMax)
    {
        if(ZFVersionGetInt(ZFEnvInfo::frameworkVersion(), ZFVersion_main) >= 7)
        {
            ZFLISTENER_LOCAL(windowLayoutParamOnUpdate, {
                ZFUIViewLayoutParam *lp = listenerData.sender->toAny();
                ZFUIMargin margin = lp->layoutMargin();
                margin.top = 20;
                lp->layoutMarginSet(margin);
            })
            ZFGlobalEventCenter::instance()->observerAdd(ZFUIWindow::EventWindowLayoutParamOnInit(), windowLayoutParamOnUpdate);
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(iOS7AutoTitleSpace)


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS.h"
#include "ZFMainEntry_sys_iOS.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFArray.h"

#if ZF_ENV_DEBUG
#include "ZFUIKit/ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/ZFUIViewPrintViewTree.h"
#include "ZFUIKit/ZFUISysWindow.h"
#endif

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

static UIApplication *_ZFP_ZFImpl_sys_iOS_application = nil;
static UIWindow *_ZFP_ZFImpl_sys_iOS_rootWindow = nil;

void ZFImpl_sys_iOS_applicationDidFinishLaunchingWithOptions(UIApplication *application,
                                                            NSDictionary *launchOptions,
                                                            UIWindow *rootWindow)
{
    _ZFP_ZFImpl_sys_iOS_application = application;
    _ZFP_ZFImpl_sys_iOS_rootWindow = rootWindow;

    ZFFrameworkInit();
    ZFMainExecute();

#if ZF_ENV_DEBUG && 0 // framework's init and dealloc
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        NSLog(@"test cleanup");
        ZFFrameworkCleanup();
        NSLog(@"test cleanup end");
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            NSLog(@"test re-init");
            ZFFrameworkInit();
            NSLog(@"test re-init end");
            ZFMainExecute();
            NSLog(@"test main re-enter end");
        });
    });
#endif
}
void ZFImpl_sys_iOS_applicationWillTerminate(UIApplication *application)
{
    ZFFrameworkCleanup();

    _ZFP_ZFImpl_sys_iOS_rootWindow.rootViewController = nil;

    _ZFP_ZFImpl_sys_iOS_application = nil;
    _ZFP_ZFImpl_sys_iOS_rootWindow = nil;
}
void ZFImpl_sys_iOS_applicationDidReceiveMemoryWarning(UIApplication *application)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnReceiveMemoryWarning());
}

void ZFImpl_sys_iOS_applicationWillResignActive(UIApplication *application)
{
    [_ZFP_ZFImpl_sys_iOS_rootWindow.rootViewController viewWillDisappear:NO];
#if ZF_ENV_DEBUG
    zfLogTrimT() << ZFImpl_sys_iOS_ZFUIKit_UIViewGetViewTree(_ZFP_ZFImpl_sys_iOS_rootWindow);
    ZFUIViewPrintViewTree(ZFUISysWindow::mainWindow()->rootView());
#endif
}
void ZFImpl_sys_iOS_applicationWillEnterForeground(UIApplication *application)
{
    [_ZFP_ZFImpl_sys_iOS_rootWindow.rootViewController viewWillAppear:NO];
}

UIApplication *ZFImpl_sys_iOS_application(void)
{
    return _ZFP_ZFImpl_sys_iOS_application;
}
UIWindow *ZFImpl_sys_iOS_rootWindow(void)
{
    return _ZFP_ZFImpl_sys_iOS_rootWindow;
}

ZF_NAMESPACE_GLOBAL_END

#endif


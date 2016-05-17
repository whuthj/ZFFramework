/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFMainEntry_sys_iOS_h_
#define _ZFI_ZFMainEntry_sys_iOS_h_

#include "ZFCore.h"
#include "ZFImpl_sys_iOS.h"

#if ZF_ENV_sys_iOS

#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// project must call these methods manually, a root view controller would be attached to rootWindow automatically
extern void ZFImpl_sys_iOS_applicationDidFinishLaunchingWithOptions(UIApplication *application,
                                                                   NSDictionary *launchOptions,
                                                                   UIWindow *rootWindow);
extern void ZFImpl_sys_iOS_applicationWillTerminate(UIApplication *application);

extern void ZFImpl_sys_iOS_applicationWillResignActive(UIApplication *application);
extern void ZFImpl_sys_iOS_applicationWillEnterForeground(UIApplication *application);

extern void ZFImpl_sys_iOS_applicationDidReceiveMemoryWarning(UIApplication *application);

// ============================================================
// implementation may use this to access the application
extern UIApplication *ZFImpl_sys_iOS_application(void);
// implementation may use this to access the root window
extern UIWindow *ZFImpl_sys_iOS_rootWindow(void);

ZF_NAMESPACE_GLOBAL_END

#endif
#endif // #ifndef _ZFI_ZFMainEntry_sys_iOS_h_


/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#import "AppDelegate.h"
#import "RootViewController.h"
#include "sys_iOS/ZFMainEntry_sys_iOS.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    self.window.backgroundColor = [UIColor whiteColor];

    self.window.rootViewController = [[RootViewController new] autorelease];

    [self.window makeKeyAndVisible];

    ZFImpl_sys_iOS_applicationDidFinishLaunchingWithOptions(application, launchOptions, self.window);

    return YES;
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    ZFImpl_sys_iOS_applicationWillTerminate(application);
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    ZFImpl_sys_iOS_applicationWillResignActive(application);
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    ZFImpl_sys_iOS_applicationWillEnterForeground(application);
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    ZFImpl_sys_iOS_applicationDidReceiveMemoryWarning(application);
}

@end

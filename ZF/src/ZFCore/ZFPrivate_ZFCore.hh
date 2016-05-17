/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/** @cond ZFPrivateDoc */

#ifndef _ZFI_ZFPrivate_ZFCore_hh_
#define _ZFI_ZFPrivate_ZFCore_hh_

#include "ZFCoreDef/ZFCoreEnvDef.h"

ZF_ENV_SENSITIVE
#if ZF_ENV_DEBUG
    #if (defined(ANDROID) || defined(__ANDROID__))
        #include <jni.h>
        #include <android/log.h>
        #define zfzfzfLog(fmt, ...) \
            ((void)__android_log_print(ANDROID_LOG_ERROR, "test", fmt, ##__VA_ARGS__))
    #elif (defined(QT_VERSION) || defined(QT_CORE_LIB))
        #include <QDebug>
        #define zfzfzfLog(fmt, ...) \
            qDebug(fmt, ##__VA_ARGS__)
    #else
        #include <stdio.h>
        #define zfzfzfLog(fmt, ...) \
            do { \
                printf(fmt, ##__VA_ARGS__); \
                printf("\n"); \
            } while(false)
    #endif
#endif // #if ZF_ENV_DEBUG

#endif // #ifndef _ZFI_ZFPrivate_ZFCore_hh_

/** @endcond */


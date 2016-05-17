/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFLog.h
 * @brief thread-safe log utility
 */

#ifndef _ZFI_ZFLog_h_
#define _ZFI_ZFLog_h_

#include "ZFDebug.h"
#include "ZFMutex.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT ZFMutex *_ZFP_ZFLogMutex(void);
/**
 * @brief mutex used by ZFLog,
 *   you may use this lock to prevent your log actions from being interrupted
 */
#define ZFLogMutex _ZFP_ZFLogMutex()

zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfLogTAccess
{
public:
    ZFDebugCallback *debugCallback;

public:
    ZFDebugCallback &debugCallbackGet(void) const
    {
        return *debugCallback;
    }

public:
    _ZFP_zfLogTAccess(void);
    ~_ZFP_zfLogTAccess(void);
};
#define _ZFP_zfLogT() (_ZFP_zfLogTAccess().debugCallbackGet())

/**
 * @brief a null log callback that do nothing while appending data
 */
#define zfLogTNull zfDebugTNull

// ============================================================
/**
 * @brief a convenient debug header string
 *
 * typically looks like this (not ensured):\n
 *   "timestamp [file scope::function (line)] "
 */
#define ZFLOG_HEADER_STRING (zfstringWithFormat(zfText("%s %s "), \
    _ZFP_zfLogCurTimeString().cString(), \
    ZFDEBUG_HEADER_STRING) \
    .cString())

/**
 * @brief similar to zfDebug
 *
 * @code
 *   zfLog(format, ...);
 * @endcode
 */
#define zfLog(format, ...) (void)(_ZFP_zfLogT().addPrefix(ZFLOG_HEADER_STRING) \
    << zfstringWithFormat(format, ##__VA_ARGS__).cString())

/**
 * @brief similar to zfDebugT
 *
 * @code
 *   zfLogT() << something;
 * @endcode
 */
#define zfLogT() _ZFP_zfLogT().addPrefix(ZFLOG_HEADER_STRING)

/**
 * @brief log without ZFLog's header string
 * @see zfLog
 */
#define zfLogTrim(format, ...) (void)(_ZFP_zfLogT() \
    << zfstringWithFormat(format, ##__VA_ARGS__).cString())

/**
 * @brief log without ZFLog's header string
 * @see zfLogT
 */
#define zfLogTrimT() _ZFP_zfLogT()

// ============================================================
// other convenient method
extern ZF_ENV_EXPORT zfstring _ZFP_zfLogCurTimeString(void);
/**
 * @brief get a string contains current time with format HH:mm:ss.SSS
 */
#define zfLogCurTimeString() (_ZFP_zfLogCurTimeString().cString())

// ============================================================
/** @brief see #ZFDEBUG_LEVEL */
typedef enum
{
    ZFLogLevelVerbose = ZFDebugLevelVerbose,
    ZFLogLevelDebug = ZFDebugLevelDebug,
    ZFLogLevelInfo = ZFDebugLevelInfo,
    ZFLogLevelWarning = ZFDebugLevelWarning,
    ZFLogLevelError = ZFDebugLevelError,
    ZFLogLevelAssert = ZFDebugLevelAssert,
} ZFLogLevel;
/** @brief string tokens */
#define ZFTOKEN_ZFLogLevelVerbose ZFTOKEN_ZFDebugLevelVerbose
/** @brief string tokens */
#define ZFTOKEN_ZFLogLevelDebug ZFTOKEN_ZFDebugLevelDebug
/** @brief string tokens */
#define ZFTOKEN_ZFLogLevelInfo ZFTOKEN_ZFDebugLevelInfo
/** @brief string tokens */
#define ZFTOKEN_ZFLogLevelWarning ZFTOKEN_ZFDebugLevelWarning
/** @brief string tokens */
#define ZFTOKEN_ZFLogLevelError ZFTOKEN_ZFDebugLevelError
/** @brief string tokens */
#define ZFTOKEN_ZFLogLevelAssert ZFTOKEN_ZFDebugLevelAssert

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFLogLevel, e0, ZFLogLevel, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFLogLevel, e0, ZFLogLevel, e1)

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFLogLevel, ZFLogLevel)

ZFOUTPUT_TYPE_DECLARE(ZFLogLevel)
ZFOUTPUT_TYPE(const ZFLogLevel *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFLogLevel *, {output << (const ZFLogLevel *)v;})
ZFINPUT_TYPE_DECLARE(ZFLogLevel, ZFLogLevel)

/**
 * @brief project's log level
 *
 * see #ZFDEBUG_LEVEL for more infomation\n
 * default is ZFLogLevelWarning
 */
#ifndef ZFLOG_LEVEL
    #define ZFLOG_LEVEL ZFLogLevelWarning
#endif

/**
 * @brief true if level is higher than ZFLOG_LEVEL
 * @see ZFLOG_LEVEL
 */
#define ZFLOG_IS_ACTIVE(level) (ZFLOG_LEVEL <= level)

/**
 * @brief see #ZFLOG_LEVEL
 */
extern ZF_ENV_EXPORT void zfLogLevelSet(ZF_IN ZFLogLevel level);
/**
 * @brief see #ZFLOG_LEVEL
 */
extern ZF_ENV_EXPORT ZFLogLevel zfLogLevelGet(void);
/**
 * @brief see #ZFLOG_LEVEL
 */
extern ZF_ENV_EXPORT zfbool zfLogIsActive(ZF_IN ZFLogLevel level);

// ============================================================
/** @brief see #ZFDebugAutoSpaceOn */
#define ZFLogAutoSpaceOn ZFDebugAutoSpaceOn
/** @brief see #ZFDebugAutoSpaceOff */
#define ZFLogAutoSpaceOff ZFDebugAutoSpaceOff
/** @brief see #ZFDebugAutoEndlOn */
#define ZFLogAutoEndlOn ZFDebugAutoEndlOn
/** @brief see #ZFDebugAutoEndlOff */
#define ZFLogAutoEndlOff ZFDebugAutoEndlOff

// ============================================================
// Verbose
/**
 * @def ZFLOGV
 * @see ZFLOG_LEVEL
 * @def ZFLOGV_TRIM
 * @see ZFLOG_LEVEL
 */
#if ZFLOG_IS_ACTIVE(ZFLogLevelVerbose)
    #define ZFLOGV(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("V")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
    #define ZFLOGV_TRIM(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("V")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
#else
    #define ZFLOGV(format, ...) ((void)0)
    #define ZFLOGV_TRIM(format, ...) ((void)0)
#endif
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogV(format, ...) (zfLogIsActive(ZFLogLevelVerbose) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("V")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogVT() (zfLogIsActive(ZFLogLevelVerbose) \
    ? _ZFP_zfLogT().addPrefix(zfText("V")).addPrefix(ZFLOG_HEADER_STRING) \
    : zfLogTNull)
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogVTrim(format, ...) (zfLogIsActive(ZFLogLevelVerbose) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("V")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogVTrimT() (zfLogIsActive(ZFLogLevelVerbose) \
    ? _ZFP_zfLogT().addPrefix(zfText("V")).addPrefix(ZFLOG_HEADER_STRING) \
    : zfLogTNull)

// ============================================================
// Debug
/**
 * @def ZFLOGD
 * @see ZFLOG_LEVEL
 * @def ZFLOGD_TRIM
 * @see ZFLOG_LEVEL
 */
#if ZFLOG_IS_ACTIVE(ZFLogLevelVerbose)
    #define ZFLOGD(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("D")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
    #define ZFLOGD_TRIM(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("D")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
#else
    #define ZFLOGD(format, ...) ((void)0)
    #define ZFLOGD_TRIM(format, ...) ((void)0)
#endif
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogD(format, ...) (zfLogIsActive(ZFLogLevelDebug) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("D")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogDT() (zfLogIsActive(ZFLogLevelDebug) \
    ? _ZFP_zfLogT().addPrefix(zfText("D")).addPrefix(ZFLOG_HEADER_STRING) \
    : zfLogTNull)
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogDTrim(format, ...) (zfLogIsActive(ZFLogLevelDebug) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("D")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogDTrimT() (zfLogIsActive(ZFLogLevelDebug) \
    ? _ZFP_zfLogT().addPrefix(zfText("D")) \
    : zfLogTNull)

// ============================================================
// Info
/**
 * @def ZFLOGI
 * @see ZFLOG_LEVEL
 * @def ZFLOGI_TRIM
 * @see ZFLOG_LEVEL
 */
#if ZFLOG_IS_ACTIVE(ZFLogLevelVerbose)
    #define ZFLOGI(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("I")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
    #define ZFLOGI_TRIM(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("I")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
#else
    #define ZFLOGI(format, ...) ((void)0)
    #define ZFLOGI_TRIM(format, ...) ((void)0)
#endif
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogI(format, ...) (zfLogIsActive(ZFLogLevelInfo) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("I")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogIT() (zfLogIsActive(ZFLogLevelInfo) \
    ? _ZFP_zfLogT().addPrefix(zfText("I")).addPrefix(ZFLOG_HEADER_STRING) \
    : zfLogTNull)
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogITrim(format, ...) (zfLogIsActive(ZFLogLevelInfo) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("I")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogITrimT() (zfLogIsActive(ZFLogLevelInfo) \
    ? _ZFP_zfLogT().addPrefix(zfText("I")) \
    : zfLogTNull)

// ============================================================
// Warning
/**
 * @def ZFLOGW
 * @see ZFLOG_LEVEL
 * @def ZFLOGW_TRIM
 * @see ZFLOG_LEVEL
 */
#if ZFLOG_IS_ACTIVE(ZFLogLevelVerbose)
    #define ZFLOGW(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("W")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
    #define ZFLOGW_TRIM(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("W")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
#else
    #define ZFLOGW(format, ...) ((void)0)
    #define ZFLOGW_TRIM(format, ...) ((void)0)
#endif
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogW(format, ...) (zfLogIsActive(ZFLogLevelWarning) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("W")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogWT() (zfLogIsActive(ZFLogLevelWarning) \
    ? _ZFP_zfLogT().addPrefix(zfText("W")).addPrefix(ZFLOG_HEADER_STRING) \
    : zfLogTNull)
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogWTrim(format, ...) (zfLogIsActive(ZFLogLevelWarning) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("W")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogWTrimT() (zfLogIsActive(ZFLogLevelWarning) \
    ? _ZFP_zfLogT().addPrefix(zfText("W")) \
    : zfLogTNull)

// ============================================================
// Error
/**
 * @def ZFLOGE
 * @see ZFLOG_LEVEL
 * @def ZFLOGE_TRIM
 * @see ZFLOG_LEVEL
 */
#if ZFLOG_IS_ACTIVE(ZFLogLevelVerbose)
    #define ZFLOGE(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("E")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
    #define ZFLOGE_TRIM(format, ...) (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("E")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString())
#else
    #define ZFLOGE(format, ...) ((void)0)
    #define ZFLOGE_TRIM(format, ...) ((void)0)
#endif
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogE(format, ...) (zfLogIsActive(ZFLogLevelError) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("E")).addPrefix(ZFLOG_HEADER_STRING) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogET() (zfLogIsActive(ZFLogLevelError) \
    ? _ZFP_zfLogT().addPrefix(zfText("E")).addPrefix(ZFLOG_HEADER_STRING) \
    : zfLogTNull)
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogETrim(format, ...) (zfLogIsActive(ZFLogLevelError) \
    ? (void)(_ZFP_zfLogT() \
        .addPrefix(zfText("E")) \
        << zfstringWithFormat(format, ##__VA_ARGS__).cString()) \
    : ((void)0))
/**
 * @brief see #ZFLOG_LEVEL
 */
#define zfLogETrimT() (zfLogIsActive(ZFLogLevelError) \
    ? _ZFP_zfLogT().addPrefix(zfText("E")) \
    : zfLogTNull)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLog_h_

#include "ZFLog_StackTrace.h"


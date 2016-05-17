/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFDebug.h
 * @brief debug utility, not thread-safe
 */

#ifndef _ZFI_ZFDebug_h_
#define _ZFI_ZFDebug_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFDebugSessionPrivate;
/**
 * @brief session used by ZFDebug
 *
 * this class is designed to be used by ZFDebug to output to
 * different position at the same time, typical usage:
 * @code
 *   ZFDebugSession session1, session2;
 *   zfDebugSessionStart(session1, aOutputCallback);
 *   zfDebugSessionStart(session2, aOutputCallback);
 *   zfDebugSessionT(session1) << something1;
 *   zfDebugSessionT(session2) << something2;
 * @endcode
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFDebugSession
{
public:
    /**
     * @brief main constructor
     */
    ZFDebugSession(void);
    /**
     * @brief retain from another session
     */
    ZFDebugSession(ZF_IN const ZFDebugSession &ref);
    /**
     * @brief retain from another session
     */
    zffinal ZFDebugSession &operator =(ZF_IN const ZFDebugSession &ref);
    ~ZFDebugSession(void);

public:
    /**
     * @brief see #ZFDebugAutoSpaceOn
     */
    zffinal void autoSpaceSet(ZF_IN_OPT zfbool autoSpace = zftrue);
    /**
     * @brief see #ZFDebugAutoSpaceOn
     */
    zffinal zfbool autoSpace(void) const;
    /**
     * @brief see #ZFDebugAutoEndlOn
     */
    zffinal void autoEndlSet(ZF_IN_OPT zfbool autoEndl = zftrue);
    /**
     * @brief see #ZFDebugAutoEndlOn
     */
    zffinal zfbool autoEndl(void) const;

    /** @cond ZFPrivateDoc */
    _ZFP_ZFDebugSessionPrivate *_ZFP_ZFDebugSession_d;
    /** @endcond */
};

/**
 * @brief internal class used by ZFDebug
 *
 * use zfDebugT or zfDebugSessionT series to access this class
 * @see zfDebugT, zfDebugSessionT
 * @warning you should not use this class directly,
 *   unless you know what you're doing
 *
 * ADVANCED\n
 * to output your own type, please refer to #ZFOUTPUT_TYPE
 */
ZFCALLBACK_DECLARE_BEGIN(ZFDebugCallback, ZFOutputCallback)
public:
    /**
     * @brief whether to auto add space during each output
     */
    virtual const ZFDebugCallback &autoSpaceSet(ZF_IN_OPT zfbool autoSpace = zftrue) const;
    /**
     * @brief whether to auto add endl during each output end
     */
    virtual const ZFDebugCallback &autoEndlSet(ZF_IN_OPT zfbool autoEndl = zftrue) const;

public:
    /**
     * @brief prefix before output anything
     */
    virtual const ZFDebugCallback &addPrefix(ZF_IN const zfchar *prefix) const;
    /**
     * @brief suffix output while dealloc of ZFDebugCallback
     */
    virtual const ZFDebugCallback &addSuffix(ZF_IN const zfchar *suffix) const;
    /**
     * @brief append format
     */
    virtual const ZFDebugCallback &append(ZF_IN const zfchar *format,
                                          ...) const;
    /**
     * @brief append format
     */
    virtual const ZFDebugCallback &appendV(ZF_IN const zfchar *format,
                                           ZF_IN va_list vaList) const;
ZFCALLBACK_DECLARE_END(ZFDebugCallback, ZFOutputCallback)

extern ZF_ENV_EXPORT ZFDebugCallback _ZFP_ZFDebugCallbackCreate(ZF_IN_OUT ZFDebugSession &session);

extern ZF_ENV_EXPORT ZFDebugCallback _ZFP_zfDebugTNull(void);

/**
 * @brief a null debug callback that do nothing while appending data
 */
#define zfDebugTNull (_ZFP_zfDebugTNull())

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugCallbackConvert
{
};
/**
 * @brief used to convert ZFOutputCallback to ZFDebugCallback
 *
 * usually you should not care about this command, use with causion
 * @see ZFDebugAutoSpaceOn
 */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugCallbackConvert ZFDebugConvert;
/**
 * @brief convert ZFOutputCallback to ZFDebugCallback, see #ZFDebugConvert
 */
extern ZF_ENV_EXPORT const ZFDebugCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugCallbackConvert const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoSpaceOn
{
};
/**
 * @brief used to set ZFDebugCallback's auto space enabled
 *
 * usage:
 * @code
 *   // will append space automatically between something0 and something1
 *   zfDebug() << ZFDebugAutoSpaceOn << something0 << something1;
 * @endcode
 *
 * @note there are more control types which can be added to ZFDebugCallback:
 *   -  ZFDebugConvert: convert ZFOutputCallback to ZFDebugCallback
 *   -  ZFDebugAutoSpaceOn: set auto space enabled to ZFDebugCallback
 *   -  ZFDebugAutoSpaceOff: set auto space disabled to ZFDebugCallback
 *   -  ZFDebugAutoEndlOn: set auto endl enabled to ZFDebugCallback
 *   -  ZFDebugAutoEndlOff: set auto endl disabled to ZFDebugCallback
 *
 * @warning you must make sure the object you call operator<< to is ZFDebugCallback,
 *   otherwise app may crash or throw exception
 */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoSpaceOn ZFDebugAutoSpaceOn;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoSpaceOn const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoSpaceOff
{
};
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoSpaceOff ZFDebugAutoSpaceOff;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoSpaceOff const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoEndlOn
{
};
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoEndlOn ZFDebugAutoEndlOn;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoEndlOn const &v);

zfclassPOD ZF_ENV_EXPORT _ZFP_ZFDebugAutoEndlOff
{
};
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const _ZFP_ZFDebugAutoEndlOff ZFDebugAutoEndlOff;
/** @brief see #ZFDebugAutoSpaceOn */
extern ZF_ENV_EXPORT const ZFOutputCallback &operator << (const ZFOutputCallback &d, _ZFP_ZFDebugAutoEndlOff const &v);

// ============================================================
extern ZF_ENV_EXPORT zfstring _ZFP_ZFDebugMakeHeaderString(ZF_IN const zfcharA *callerFile,
                                                           ZF_IN const zfcharA *callerFunction,
                                                           ZF_IN zfindex callerLine);

/** @cond ZFPrivateDoc */
ZF_NAMESPACE_BEGIN(_ZFP_ZFDebugDummyNamespace)
    ZF_NAMESPACE_BEGIN(zfsuper)
        inline const ZFClass *ClassData(void)
        {
            return zfnull;
        }
    ZF_NAMESPACE_END(zfsuper)
    ZF_NAMESPACE_BEGIN(zfself)
        inline const ZFClass *ClassData(void)
        {
            return zfnull;
        }
    ZF_NAMESPACE_END(zfself)
ZF_NAMESPACE_END(_ZFP_ZFDebugDummyNamespace)
ZF_NAMESPACE_USE(_ZFP_ZFDebugDummyNamespace)
/** @endcond */
/**
 * @brief a convenient debug header string
 *
 * typically looks like this (not ensured):\n
 *   "[file function (line)]"
 */
#define ZFDEBUG_HEADER_STRING (_ZFP_ZFDebugMakeHeaderString(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).cString())

// ============================================================
/**
 * @brief see #zfDebugSessionStart
 */
extern ZF_ENV_EXPORT zfbool zfDebugStart(ZF_IN_OPT const ZFOutputCallback &callback = ZFOutputCallbackDefault,
                                         ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief see #zfDebugStart
 */
extern ZF_ENV_EXPORT const ZFOutputCallback &zfDebugOutputCallback(void);
/**
 * @brief see #zfDebugSessionEnableSet
 */
extern ZF_ENV_EXPORT zfbool zfDebugEnableSet(ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief see #zfDebugSessionEnabled
 */
extern ZF_ENV_EXPORT zfbool zfDebugEnabled(void);
/**
 * @brief see #zfDebugSessionStop
 */
extern ZF_ENV_EXPORT void zfDebugStop(void);
/**
 * @brief see #zfDebugSessionT
 */
extern ZF_ENV_EXPORT ZFDebugCallback zfDebugT(void);
/**
 * @brief see #zfDebugSession
 */
extern ZF_ENV_EXPORT void zfDebug(ZF_IN const zfchar *format,
                                  ...);

/**
 * @brief active only if ZF_ENV_DEBUG is set to 1
 * @see ZF_ENV_DEBUG, ZFDEBUG_LEVEL
 */
#if ZF_ENV_DEBUG
    #define ZFDEBUG(format, ...) zfDebug(format, ##__VA_ARGS__)
#else
    #define ZFDEBUG(format, ...) ((void)0)
#endif

/**
 * @brief init ZFDebug and set callback as output callback
 * @param session session used to specify the position to output
 * @param callback output callback
 * @param isEnabled zffalse to completely disable ZFDebug,
 *   ZFDebug won't active until next time you call init or enable
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionStart(ZF_IN_OUT ZFDebugSession &session,
                                                ZF_IN_OPT const ZFOutputCallback &callback = ZFOutputCallbackDefault,
                                                ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief get the internal output callback
 */
extern ZF_ENV_EXPORT const ZFOutputCallback &zfDebugSessionOutputCallback(ZF_IN const ZFDebugSession &session);
/**
 * @brief temporarily enable/disable ZFDebug
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionEnableSet(ZF_IN_OUT ZFDebugSession &session,
                                                    ZF_IN_OPT zfbool isEnabled = zftrue);
/**
 * @brief return true if ZFDebug currently enabled
 *
 * always return false if isEnabled is set to zffalse when init
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionEnabled(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief cleanup and stop ZFDebug described by session
 */
extern ZF_ENV_EXPORT void zfDebugSessionStop(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief begin to debug
 *
 * typical usage:
 * @code
 *   zfDebugSessionT(session) << something1 << something2;
 * @endcode
 * by default, space would be auto added between "something1" and "something2",
 * and '\\n' would be auto added after this line
 * @see ZFDebugAutoSpaceOn, ZFDebugAutoEndlOn
 */
extern ZF_ENV_EXPORT ZFDebugCallback zfDebugSessionT(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief begin to debug
 *
 * typical usage:
 * @code
 *   zfDebugSession(session, format, paramList...);
 * @endcode
 * this function would automatically add a "\n" to tail
 * @see zfDebugSessionT
 */
extern ZF_ENV_EXPORT void zfDebugSession(ZF_IN_OUT ZFDebugSession &session,
                                         ZF_IN const zfchar *format,
                                         ...);

/**
 * @brief active only if ZF_ENV_DEBUG is set to 1
 * @see ZF_ENV_DEBUG, ZFDEBUG_LEVEL
 */
#if ZF_ENV_DEBUG
    #define ZFDEBUGSESSION(session, format, ...) zfDebugSession(session, format, ##__VA_ARGS__)
#else
    #define ZFDEBUGSESSION(session, format, ...) ((void)0)
#endif

// ============================================================
/** @brief see #ZFDEBUG_LEVEL */
typedef enum
{
    ZFDebugLevelVerbose = 0,
    ZFDebugLevelDebug = 1,
    ZFDebugLevelInfo = 2,
    ZFDebugLevelWarning = 3,
    ZFDebugLevelError = 4,
    ZFDebugLevelAssert = 5,
} ZFDebugLevel;
/** @brief string tokens */
#define ZFTOKEN_ZFDebugLevelVerbose zfText("Verbose")
/** @brief string tokens */
#define ZFTOKEN_ZFDebugLevelDebug zfText("Debug")
/** @brief string tokens */
#define ZFTOKEN_ZFDebugLevelInfo zfText("Info")
/** @brief string tokens */
#define ZFTOKEN_ZFDebugLevelWarning zfText("Warning")
/** @brief string tokens */
#define ZFTOKEN_ZFDebugLevelError zfText("Error")
/** @brief string tokens */
#define ZFTOKEN_ZFDebugLevelAssert zfText("Assert")

ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFDebugLevel, e0, ZFDebugLevel, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFDebugLevel, e0, ZFDebugLevel, e1)

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFDebugLevel, ZFDebugLevel)

ZFOUTPUT_TYPE(ZFDebugLevel, {output << ZFDebugLevelToString(v);})
ZFOUTPUT_TYPE(const ZFDebugLevel *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFDebugLevel *, {output << (const ZFDebugLevel *)v;})
ZFINPUT_TYPE_DECLARE(ZFDebugLevel, ZFDebugLevel)

/**
 * @brief project's log level
 *
 * similar to Android's Log.v() to Log.e(),
 * we have zfDebug V ~ E series: Verbose, Debug, Info, Warning, Error\n
 * you can use them by two methods:
 * -  the precompiler version (ZFDEBUGV/ZFDEBUGSESSIONV)\n
 *   controlled by ZFDEBUG_LEVEL,
 *   to control the level, add desired level to precompiler,
 *   such as "ZFDEBUG_LEVEL=ZFDebugLevelVerbose
 * -  the dynamic version (zfDebugV/zfDebugVT/zfDebugSessionV/zfDebugSessionVT)\n
 *   controlled by zfDebugLevelSet/zfDebugSessionLevelSet,
 *   to control the level, call zfDebugLevelSet or zfDebugSessionLevelSet
 *
 * @see ZFDEBUG_IS_ACTIVE, zfDebugIsActive
 */
#ifndef ZFDEBUG_LEVEL
    #define ZFDEBUG_LEVEL ZFDebugLevelWarning
#endif

/**
 * @brief true if level is higher than ZFDEBUG_LEVEL
 *
 * @code
 *   #if ZFDEBUG_IS_ACTIVE(ZFDebugLevelVerbose)
 *     // output log that level is higher than I(Info)
 *   #endif
 * @endcode
 */
#define ZFDEBUG_IS_ACTIVE(level) (ZFDEBUG_LEVEL <= level)

/**
 * @brief see #ZFDEBUG_LEVEL
 */
extern ZF_ENV_EXPORT void zfDebugLevelSet(ZF_IN ZFDebugLevel level);
/**
 * @brief see #ZFDEBUG_LEVEL
 */
extern ZF_ENV_EXPORT ZFDebugLevel zfDebugLevelGet(void);
/**
 * @brief see #ZFDEBUG_LEVEL
 */
extern ZF_ENV_EXPORT zfbool zfDebugIsActive(ZF_IN ZFDebugLevel level);

/**
 * @brief see #ZFDEBUG_LEVEL
 */
extern ZF_ENV_EXPORT void zfDebugSessionLevelSet(ZF_IN_OUT ZFDebugSession &session,
                                                 ZF_IN ZFDebugLevel level);
/**
 * @brief see #ZFDEBUG_LEVEL
 */
extern ZF_ENV_EXPORT ZFDebugLevel zfDebugSessionLevelGet(ZF_IN_OUT ZFDebugSession &session);
/**
 * @brief see #ZFDEBUG_LEVEL
 */
extern ZF_ENV_EXPORT zfbool zfDebugSessionIsActive(ZF_IN_OUT ZFDebugSession &session,
                                                   ZF_IN ZFDebugLevel level);

// ============================================================
// Verbose
/**
 * @def ZFDEBUGV
 * @see ZFDEBUG_LEVEL
 * @def ZFDEBUGSESSIONV
 * @see ZFDEBUG_LEVEL
 */
#if ZFDEBUG_IS_ACTIVE(ZFDebugLevelVerbose)
    #define ZFDEBUGV(format, ...) (void)zfDebug(format, ##__VA_ARGS__)
    #define ZFDEBUGSESSIONV(session, format, ...) (void)zfDebugSession(session, format, ##__VA_ARGS__)
#else
    #define ZFDEBUGV(format, ...) ((void)0)
    #define ZFDEBUGSESSIONV(session, format, ...) ((void)0)
#endif
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugV(format, ...) (zfDebugIsActive(ZFDebugLevelVerbose) \
    ? zfDebug(format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugVT() (zfDebugIsActive(ZFDebugLevelVerbose) \
    ? zfDebugT() \
    : zfDebugTNull)
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionV(session, format, ...) (zfDebugSessionIsActive(session, ZFDebugLevelVerbose) \
    ? zfDebugSession(session, format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionVT(session) (zfDebugSessionIsActive(session, ZFDebugLevelVerbose) \
    ? zfDebugSessionT(session) \
    : zfDebugTNull)

// ============================================================
// Debug
/**
 * @def ZFDEBUGD
 * @see ZFDEBUG_LEVEL
 * @def ZFDEBUGSESSIOND
 * @see ZFDEBUG_LEVEL
 */
#if ZFDEBUG_IS_ACTIVE(ZFDebugLevelDebug)
    #define ZFDEBUGD(format, ...) (void)zfDebug(format, ##__VA_ARGS__)
    #define ZFDEBUGSESSIOND(session, format, ...) (void)zfDebugSession(session, format, ##__VA_ARGS__)
#else
    #define ZFDEBUGD(format, ...) ((void)0)
    #define ZFDEBUGSESSIOND(session, format, ...) ((void)0)
#endif
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugD(format, ...) (zfDebugIsActive(ZFDebugLevelDebug) \
    ? zfDebug(format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugDT() (zfDebugIsActive(ZFDebugLevelDebug) \
    ? zfDebugT() \
    : zfDebugTNull)
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionD(session, format, ...) (zfDebugSessionIsActive(session, ZFDebugLevelDebug) \
    ? zfDebugSession(session, format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionDT(session) (zfDebugSessionIsActive(session, ZFDebugLevelDebug) \
    ? zfDebugSessionT(session) \
    : zfDebugTNull)

// ============================================================
// Info
/**
 * @def ZFDEBUGI
 * @see ZFDEBUG_LEVEL
 * @def ZFDEBUGSESSIONI
 * @see ZFDEBUG_LEVEL
 */
#if ZFDEBUG_IS_ACTIVE(ZFDebugLevelInfo)
    #define ZFDEBUGI(format, ...) (void)zfDebug(format, ##__VA_ARGS__)
    #define ZFDEBUGSESSIONI(session, format, ...) (void)zfDebugSession(session, format, ##__VA_ARGS__)
#else
    #define ZFDEBUGI(format, ...) ((void)0)
    #define ZFDEBUGSESSIONI(session, format, ...) ((void)0)
#endif
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugI(format, ...) (zfDebugIsActive(ZFDebugLevelInfo) \
    ? zfDebug(format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugIT() (zfDebugIsActive(ZFDebugLevelInfo) \
    ? zfDebugT() \
    : zfDebugTNull)
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionI(session, format, ...) (zfDebugSessionIsActive(session, ZFDebugLevelInfo) \
    ? zfDebugSession(session, format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionIT(session) (zfDebugSessionIsActive(session, ZFDebugLevelInfo) \
    ? zfDebugSessionT(session) \
    : zfDebugTNull)

// ============================================================
// Warning
/**
 * @def ZFDEBUGW
 * @see ZFDEBUG_LEVEL
 * @def ZFDEBUGSESSIONW
 * @see ZFDEBUG_LEVEL
 */
#if ZFDEBUG_IS_ACTIVE(ZFDebugLevelWarning)
    #define ZFDEBUGW(format, ...) (void)zfDebug(format, ##__VA_ARGS__)
    #define ZFDEBUGSESSIONW(session, format, ...) (void)zfDebugSession(session, format, ##__VA_ARGS__)
#else
    #define ZFDEBUGW(format, ...) ((void)0)
    #define ZFDEBUGSESSIONW(session, format, ...) ((void)0)
#endif
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugW(format, ...) (zfDebugIsActive(ZFDebugLevelWarning) \
    ? zfDebug(format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugWT() (zfDebugIsActive(ZFDebugLevelWarning) \
    ? zfDebugT() \
    : zfDebugTNull)
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionW(session, format, ...) (zfDebugSessionIsActive(session, ZFDebugLevelWarning) \
    ? zfDebugSession(session, format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionWT(session) (zfDebugSessionIsActive(session, ZFDebugLevelWarning) \
    ? zfDebugSessionT(session) \
    : zfDebugTNull)

// ============================================================
// Error
/**
 * @def ZFDEBUGE
 * @see ZFDEBUG_LEVEL
 * @def ZFDEBUGSESSIONE
 * @see ZFDEBUG_LEVEL
 */
#if ZFDEBUG_IS_ACTIVE(ZFDebugLevelError)
    #define ZFDEBUGE(format, ...) (void)zfDebug(format, ##__VA_ARGS__)
    #define ZFDEBUGSESSIONE(session, format, ...) (void)zfDebugSession(session, format, ##__VA_ARGS__)
#else
    #define ZFDEBUGE(format, ...) ((void)0)
    #define ZFDEBUGSESSIONE(session, format, ...) ((void)0)
#endif
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugE(format, ...) (zfDebugIsActive(ZFDebugLevelError) \
    ? zfDebug(format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugET() (zfDebugIsActive(ZFDebugLevelError) \
    ? zfDebugT() \
    : zfDebugTNull)
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionE(session, format, ...) (zfDebugSessionIsActive(session, ZFDebugLevelError) \
    ? zfDebugSession(session, format, ##__VA_ARGS__) \
    : ((void)0))
/**
 * @brief see #ZFDEBUG_LEVEL
 */
#define zfDebugSessionET(session) (zfDebugSessionIsActive(session, ZFDebugLevelError) \
    ? zfDebugSessionT(session) \
    : zfDebugTNull)

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFDebug_h_


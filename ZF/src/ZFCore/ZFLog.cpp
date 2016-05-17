/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFPrivate_ZFCore.hh"
#include "ZFLog.h"
#include "ZFTime.h"
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_I_ZFLogMutex : zfextends ZFMutex
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFLogMutex, ZFMutex)
};
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogDataHolder, ZFLevelZFFrameworkHigh)
{
    this->logMutex = zfnull;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogDataHolder)
{
    if(this->logMutex != zfnull)
    {
        zfReleaseInternal(this->logMutex);
        this->logMutex = zfnull;
    }
}
ZFMutex *getLogMutex(void)
{
    if(this->logMutex == zfnull)
    {
        this->logMutex = zfAllocInternal(_ZFP_I_ZFLogMutex,
            zfHint("ignore if impl not available")zftrue);
        this->logMutex->objectDebugInfoSet(zfText("ZFLogMutex"));
    }
    return this->logMutex;
}
ZFDebugSession globalSession;
ZFMutex *logMutex;
ZF_GLOBAL_INITIALIZER_END(ZFLogDataHolder)
#define _ZFP_ZFLogGlobalSession (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLogDataHolder)->globalSession)
#define _ZFP_ZFLogGlobalMutex (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLogDataHolder)->getLogMutex())

ZFMutex *_ZFP_ZFLogMutex(void)
{
    return _ZFP_ZFLogGlobalMutex;
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLogInitDataHolder, ZFLevelZFFrameworkNormal)
{
    this->ZFOutputCallbackDefaultOnChangeListener = ZFCallbackForRawFunction(zfself::ZFOutputCallbackDefaultOnChange);
    ZFGlobalEventCenter::instance()->observerAdd(ZFGlobalEvent::EventZFOutputCallbackDefaultOnChange(), this->ZFOutputCallbackDefaultOnChangeListener);

    zfDebugSessionStart(_ZFP_ZFLogGlobalSession);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLogInitDataHolder)
{
    ZFGlobalEventCenter::instance()->observerRemove(ZFGlobalEvent::EventZFOutputCallbackDefaultOnChange(), this->ZFOutputCallbackDefaultOnChangeListener);
    zfDebugSessionStop(_ZFP_ZFLogGlobalSession);
}
ZFListener ZFOutputCallbackDefaultOnChangeListener;
static ZFLISTENER_PROTOTYPE_EXPAND(ZFOutputCallbackDefaultOnChange)
{
    zfDebugSessionStop(_ZFP_ZFLogGlobalSession);
    zfDebugSessionStart(_ZFP_ZFLogGlobalSession);
}
ZF_GLOBAL_INITIALIZER_END(ZFLogInitDataHolder)

// ============================================================
zfclassNotPOD _ZFP_ZFLogPrivate
{
public:
    zfindex refCount;
    ZFDebugCallback debugCallback;

public:
    _ZFP_ZFLogPrivate(void)
    : refCount(1)
    , debugCallback()
    {
    }
    _ZFP_ZFLogPrivate(ZF_IN const ZFDebugCallback &debugCallback)
    : refCount(1)
    , debugCallback(debugCallback)
    {
    }
};

// ============================================================
_ZFP_zfLogTAccess::_ZFP_zfLogTAccess(void)
: debugCallback(zfnull)
{
    _ZFP_ZFLogGlobalMutex->mutexLock();
    this->debugCallback = zfnew(ZFDebugCallback, zfDebugSessionT(_ZFP_ZFLogGlobalSession));
}
_ZFP_zfLogTAccess::~_ZFP_zfLogTAccess(void)
{
    // cleanup
    zfdelete(this->debugCallback);
    this->debugCallback = zfnull;

    // restore global settings
    // must after callback's dealloc
    _ZFP_ZFLogGlobalSession.autoSpaceSet(zftrue);
    _ZFP_ZFLogGlobalSession.autoEndlSet(zftrue);

    _ZFP_ZFLogGlobalMutex->mutexUnlock();
}

// ============================================================
// other convenient method
zfstring _ZFP_zfLogCurTimeString(void)
{
    zfstring s;
    ZFTimeInfo ti = ZFTime::currentTimeInfo();
    zfstringAppend(s,
        zfText("%02d:%02d:%02d.%03d"),
        ti.hour,
        ti.minute,
        ti.second,
        ti.miliSecond);
    return s;
}

// ============================================================
void ZFLogLevelToString(ZF_IN_OUT zfstring &ret, ZF_IN ZFLogLevel const &value)
{
    ZFDebugLevelToString(ret, (ZFDebugLevel)(zfint)value);
}
const zfchar *ZFLogLevelFromString(ZF_OUT ZFLogLevel &ret,
                                   ZF_IN const zfchar *src,
                                   ZF_IN_OPT zfindex srcLen /* = zfindexMax */)
{
    ZFDebugLevel tmpResult = ZFDebugLevelVerbose;
    const zfchar *tmpRet = ZFDebugLevelFromString(tmpResult, src, srcLen);
    return tmpRet;
}

ZFOUTPUT_TYPE_DEFINE(ZFLogLevel, {output << ZFLogLevelToString(v);})
ZFINPUT_TYPE_DEFINE(ZFLogLevel, ZFLogLevel, {
    ZFDebugLevel tmpValue = ZFDebugLevelVerbose;
    if(ZFDebugLevelFromInput(tmpValue, input))
    {
        v = (ZFLogLevel)tmpValue;
        return zftrue;
    }
    return zffalse;
})

// ============================================================
void zfLogLevelSet(ZF_IN ZFLogLevel level)
{
    zfDebugSessionLevelSet(_ZFP_ZFLogGlobalSession, (ZFDebugLevel)level);
}
ZFLogLevel zfLogLevelGet(void)
{
    return (ZFLogLevel)zfDebugSessionLevelGet(_ZFP_ZFLogGlobalSession);
}
zfbool zfLogIsActive(ZF_IN ZFLogLevel level)
{
    return zfDebugSessionIsActive(_ZFP_ZFLogGlobalSession, (ZFDebugLevel)level);
}

ZF_NAMESPACE_GLOBAL_END


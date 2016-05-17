/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFTimerImpl_sys_iOS_TimerOwner : NSObject
@property (nonatomic, readonly) id nativeImpl;
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFTimer) *impl;
@property (nonatomic, assign) ZFTimer *ownerZFTimer;

- (void)startTimer;
- (void)stopTimer;

// private
@property (nonatomic, retain) NSTimer *_timer;
@property (nonatomic, assign) BOOL _timerNotifiedFlag;
@property (nonatomic, assign) zfint _timerTaskId;
- (zfbool)_timerOwnerIsTaskIdValid:(NSNumber *)savedTimerTaskId;
- (void)_timerOwnerOnTimerDelay:(NSNumber *)savedTimerTaskId;
- (void)_timerOwnerOnTimerEvent:(NSTimer *)timer;
- (void)_timerOwnerOnTimerEventOnMainThread:(NSNumber *)taskId;
@end
@implementation _ZFP_ZFTimerImpl_sys_iOS_TimerOwner
@synthesize nativeImpl, impl, ownerZFTimer;
@synthesize _timer, _timerNotifiedFlag, _timerTaskId;

- (id)nativeImpl ZFImpl_sys_iOS_overrideProperty
{
    return self._timer;
}

- (void)startTimer
{
    [self stopTimer];

    self._timerNotifiedFlag = NO;
    [self retain];
    NSNumber *taskId = [NSNumber numberWithInt:self._timerTaskId];
    if(self.ownerZFTimer->timerDelay() > 0)
    {
        zftimet timerDelay = self.ownerZFTimer->timerDelay();
        dispatch_async(dispatch_get_main_queue(), ^{
            [self performSelector:@selector(_timerOwnerOnTimerDelay:) withObject:taskId afterDelay:timerDelay];
        });
    }
    else
    {
        [self _timerOwnerOnTimerDelay:taskId];
    }
}
- (void)stopTimer
{
    [NSObject cancelPreviousPerformRequestsWithTarget:self];
    self._timerTaskId = self._timerTaskId + 1;
    if(self._timer != nil)
    {
        [self._timer invalidate];
        self._timer = nil;
        self._timerNotifiedFlag = NO;
        self.impl->notifyTimerStop(self.ownerZFTimer);
    }
}
- (zfbool)_timerOwnerIsTaskIdValid:(NSNumber *)savedTimerTaskId
{
    return (savedTimerTaskId != nil && self._timerTaskId == [savedTimerTaskId intValue]);
}
- (void)_timerOwnerOnTimerDelay:(NSNumber *)taskId
{
    if(![self _timerOwnerIsTaskIdValid:taskId])
    {
        return ;
    }
    self._timer = [NSTimer scheduledTimerWithTimeInterval:((zffloat)self.ownerZFTimer->timerInterval() / 1000) target:self selector:@selector(_timerOwnerOnTimerEvent:) userInfo:taskId repeats:YES];
    [self release];
}
- (void)_timerOwnerOnTimerEvent:(NSTimer *)timer
{
    NSNumber *taskId = (NSNumber *)[timer userInfo];
    if(![self _timerOwnerIsTaskIdValid:taskId])
    {
        return ;
    }

    if(!self._timerNotifiedFlag)
    {
        self._timerNotifiedFlag = YES;
        self.impl->notifyTimerStart(self.ownerZFTimer);
    }
    if(self.ownerZFTimer->timerActivateInMainThread())
    {
        [self performSelectorOnMainThread:@selector(_timerOwnerOnTimerEventOnMainThread:) withObject:taskId waitUntilDone:YES];
    }
    else
    {
        self.impl->notifyTimerActivate(self.ownerZFTimer);
    }
}
- (void)_timerOwnerOnTimerEventOnMainThread:(NSNumber *)taskId
{
    if(![self _timerOwnerIsTaskIdValid:taskId])
    {
        return ;
    }
    self.impl->notifyTimerActivate(self.ownerZFTimer);
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_iOS, ZFTimer, ZFProtocolLevelSystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:NSObject_NSTimer"))
public:
    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *timerOwner = [_ZFP_ZFTimerImpl_sys_iOS_TimerOwner new];
        timerOwner.impl = this;
        timerOwner.ownerZFTimer = timer;
        return timerOwner;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer,
                                    ZF_IN void *nativeTimer)
    {
        [ZFCastStatic(_ZFP_ZFTimerImpl_sys_iOS_TimerOwner *, nativeTimer) release];
    }
    virtual void timerStart(ZF_IN ZFTimer *timer)
    {
        _ZFP_ZFTimerImpl_sys_iOS_TimerOwner *timerOwner = ZFCastStatic(_ZFP_ZFTimerImpl_sys_iOS_TimerOwner *, timer->nativeTimer());
        [timerOwner startTimer];
    }
    virtual void timerStop(ZF_IN ZFTimer *timer)
    {
        [ZFCastStatic(_ZFP_ZFTimerImpl_sys_iOS_TimerOwner *, timer->nativeTimer()) stopTimer];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTimerImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


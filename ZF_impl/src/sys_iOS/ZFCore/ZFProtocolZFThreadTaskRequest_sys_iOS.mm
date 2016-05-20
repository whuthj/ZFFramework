/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"

#if ZF_ENV_sys_iOS

// ============================================================
// global type
@interface _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder : NSObject
@property (nonatomic, assign) ZFListener task;
@property (nonatomic, assign) ZFObject *param0;
@property (nonatomic, assign) ZFObject *param1;
- (void)taskCallback;
@end
@implementation _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder
@synthesize task, param0, param1;
- (void)dealloc
{
    self.param0 = zfnull;
    self.param1 = zfnull;
    [super dealloc];
}
- (void)setParam0:(ZFObject *)newParam0 ZFImpl_sys_iOS_overrideProperty
{
    ZFPropertyChange(self->param0, newParam0);
}
- (void)setParam1:(ZFObject *)newParam1 ZFImpl_sys_iOS_overrideProperty
{
    ZFPropertyChange(self->param1, newParam1);
}
- (void)taskCallback
{
    if(self.task.callbackIsValid())
    {
        self.task.execute(ZFListenerData(zfidentityInvalid, zfnull, self.param0, self.param1));
    }

    self.param0 = zfnull;
    self.param1 = zfnull;
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

static NSMutableArray *_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders = nil;
// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_sys_iOS, ZFThreadTaskRequest, ZFProtocolLevelSystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:GrandCentralDispatch"))

public:
    ZFPROTOCOL_IMPLEMENTATION_CLASS(ZFThreadTaskRequestImpl_sys_iOS)(void)
    {
        _ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders = [NSMutableArray new];
    }
    virtual ~ZFPROTOCOL_IMPLEMENTATION_CLASS(ZFThreadTaskRequestImpl_sys_iOS)(void)
    {
        for(NSUInteger i = 0; i < [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders count]; ++i)
        {
            _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder *taskHolder = [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders objectAtIndex:i];
            taskHolder.task = ZFCallbackNull();
            taskHolder.param0 = zfnull;
            taskHolder.param1 = zfnull;
        }
        [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders release];
        _ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders = nil;
    }
public:
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        _ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder *taskHolder = [[_ZFP_ZFThreadTaskRequestImpl_sys_iOS_TaskHolder new] autorelease];
        taskHolder.task = task;
        taskHolder.param0 = param0;
        taskHolder.param1 = param1;
        [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders addObject:taskHolder];

        dispatch_async(dispatch_get_main_queue(), ^{
            if([_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders containsObject:taskHolder])
            {
                [[taskHolder retain] autorelease];
                [_ZFP_ZFThreadTaskRequestImpl_sys_iOS_taskHolders removeObject:taskHolder];
                [taskHolder taskCallback];
            }
        });
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


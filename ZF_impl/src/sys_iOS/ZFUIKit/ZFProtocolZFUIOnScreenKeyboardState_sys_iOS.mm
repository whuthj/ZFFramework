/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIOnScreenKeyboardState.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS : NSObject
@property (nonatomic, assign) zfbool keyboardShowing;
@property (nonatomic, assign) ZFUIRect keyboardFrame;
- (void)_keyboardOnShow:(NSNotification *)aNotification;
- (void)_keyboardOnHide:(NSNotification *)aNotification;
@end
static _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS *_ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance = nil;
@implementation _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS
- (instancetype)init
{
    self = [super init];
    if(self)
    {
        self.keyboardShowing = zffalse;
        self.keyboardFrame = ZFUIRectZero;

        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_keyboardOnShow:) name:UIKeyboardWillChangeFrameNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_keyboardOnShow:) name:UIKeyboardWillShowNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_keyboardOnHide:) name:UIKeyboardWillHideNotification object:nil];
    }
    return self;
}
- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];

    [super dealloc];
}
- (void)_keyboardOnShow:(NSNotification *)aNotification
{
    self.keyboardShowing = zftrue;
    CGRect rect = [[aNotification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    self.keyboardFrame = ZFImpl_sys_iOS_ZFUIKit_ZFUIRectFromCGRect(rect);

    ZFPROTOCOL_INTERFACE_CLASS(ZFUIOnScreenKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIOnScreenKeyboardState);
    if(impl != zfnull)
    {
        impl->notifyKeyboardStateOnChange(ZFUIOnScreenKeyboardState::instanceForSysWindow());
    }
}
- (void)_keyboardOnHide:(NSNotification *)aNotification
{
    self.keyboardShowing = zffalse;
    self.keyboardFrame = ZFUIRectZero;

    ZFPROTOCOL_INTERFACE_CLASS(ZFUIOnScreenKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIOnScreenKeyboardState);
    if(impl != zfnull)
    {
        impl->notifyKeyboardStateOnChange(ZFUIOnScreenKeyboardState::instanceForSysWindow());
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardStateImpl_sys_iOS_DataHolder, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance = [_ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS new];
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardStateImpl_sys_iOS_DataHolder)
{
    [_ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance release];
    _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance = nil;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardStateImpl_sys_iOS_DataHolder)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIOnScreenKeyboardStateImpl_sys_iOS, ZFUIOnScreenKeyboardState, ZFProtocolLevelSystemNormal)
public:
    virtual zfbool keyboardShowing(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        return _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance.keyboardShowing;
    }
    virtual ZFUIRect keyboardFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        return _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance.keyboardFrame;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIOnScreenKeyboardStateImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIOnScreenKeyboardStateImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


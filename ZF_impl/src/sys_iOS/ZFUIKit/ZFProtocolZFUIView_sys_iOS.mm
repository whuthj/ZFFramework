/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"

#define _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG ZF_ENV_DEBUG

#if ZF_ENV_sys_iOS

#import <objc/runtime.h>

static BOOL _ZFP_ZFUIViewImpl_sys_iOS_isImplView(id obj);

static zftimet _ZFP_ZFUIViewImpl_sys_iOS_timestamp(void)
{
    return (zftimet)([[NSDate date] timeIntervalSince1970] * 1000);
}

@interface _ZFP_ZFUIViewImpl_sys_iOS_View : UIView
#if _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG
@property (nonatomic, retain) NSString *text;
#endif
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *_ZFP_impl;
@property (nonatomic, assign) ZFUIView *_ZFP_ownerZFUIView;
@property (nonatomic, assign) CGRect _ZFP_frame;
@property (nonatomic, retain) UIView *_ZFP_nativeImplView;
@property (nonatomic, assign) BOOL _ZFP_uiEnable;
@property (nonatomic, assign) BOOL _ZFP_uiEnableTree;
@property (nonatomic, assign) BOOL _ZFP_layoutRequested;
@property (nonatomic, assign) ZFCoreArrayPOD<UITouch *> _ZFP_mouseRecords;
@property (nonatomic, assign) BOOL _ZFP_ZFUIViewFocus_viewFocusable;
@end
@implementation _ZFP_ZFUIViewImpl_sys_iOS_View
#if _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG
@synthesize text;
#endif
@synthesize
    _ZFP_impl,
    _ZFP_ownerZFUIView,
    _ZFP_frame,
    _ZFP_nativeImplView,
    _ZFP_uiEnable,
    _ZFP_uiEnableTree,
    _ZFP_layoutRequested,
    _ZFP_mouseRecords;
@synthesize _ZFP_ZFUIViewFocus_viewFocusable;
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];

    self.autoresizesSubviews = NO;
    self.multipleTouchEnabled = YES;
    self.exclusiveTouch = YES;
    self.clipsToBounds = YES;

    // status init
    self._ZFP_uiEnable = YES;
    self._ZFP_uiEnableTree = YES;
    self._ZFP_layoutRequested = NO;

    self._ZFP_ZFUIViewFocus_viewFocusable = NO;

    return self;
}
- (void)dealloc
{
    zfCoreAssert(self._ZFP_nativeImplView == nil);

#if _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG
    self.text = nil;
#endif

    [super dealloc];
}

#if _ZFP_ZFUIViewImpl_sys_iOS_VIEW_TREE_DEBUG
- (void)set_ZFP_ownerZFUIView:(ZFUIView *)newOwnerZFUIView ZFImpl_sys_iOS_overrideProperty
{
    ZFLISTENER_LOCAL(viewIdListener, {
        const ZFProperty *property = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFProperty *>();
        if(property == ZFPropertyAccess(ZFUIView, viewId))
        {
            ZFUIView *view = ZFCastZFObjectUnchecked(ZFUIView *, listenerData.sender);
            const zfstring &viewId = view->viewId();
            ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView()).text
                = (viewId.isEmpty() ? nil : [NSString stringWithUTF8String:view->viewId().cString()]);
        }
    })
    if(self->_ZFP_ownerZFUIView != zfnull)
    {
        self->_ZFP_ownerZFUIView->observerRemove(ZFObject::EventObjectPropertyValueAfterChange(), viewIdListener);
    }
    self->_ZFP_ownerZFUIView = newOwnerZFUIView;
    if(newOwnerZFUIView != zfnull)
    {
        newOwnerZFUIView->observerAdd(ZFObject::EventObjectPropertyValueAfterChange(), viewIdListener);
    }
}
#endif

// ============================================================
// ui and tree enable
- (void)set_ZFP_uiEnableTree:(BOOL)newUIEnableTree ZFImpl_sys_iOS_overrideProperty
{
    self->_ZFP_uiEnableTree = newUIEnableTree;
    self.userInteractionEnabled = newUIEnableTree;
}
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *tmp = [super hitTest:point withEvent:event];
    if(!self._ZFP_uiEnable && tmp == self)
    {
        return nil;
    }
    else
    {
        return tmp;
    }
}

// ============================================================
// internal view
- (void)set_ZFP_nativeImplView:(UIView *)newNativeImplView ZFImpl_sys_iOS_overrideProperty
{
    [newNativeImplView retain];
    [newNativeImplView removeFromSuperview];
    [self->_ZFP_nativeImplView removeFromSuperview];
    [self->_ZFP_nativeImplView release];
    self->_ZFP_nativeImplView = newNativeImplView;

    if(newNativeImplView != nil)
    {
        [self insertSubview:newNativeImplView atIndex:self._ZFP_ownerZFUIView->internalBackgroundViewArray().count()];
    }
}
// ============================================================
// child add and remove
- (void)_ZFP_childAdd:(UIView *)view atVirtualIndex:(zfindex)atVirtualIndex childLayer:(ZFUIViewChildLayerEnum)childLayer
{
    NSUInteger index = atVirtualIndex;
    if(childLayer != ZFUIViewChildLayer::e_Background && self._ZFP_nativeImplView != nil)
    {
        ++index;
    }
    [self insertSubview:view atIndex:index];
}
- (void)_ZFP_childRemoveAtVirtualIndex:(zfindex)atVirtualIndex childLayer:(ZFUIViewChildLayerEnum)childLayer
{
    NSUInteger index = atVirtualIndex;
    if(childLayer != ZFUIViewChildLayer::e_Background && self._ZFP_nativeImplView != nil)
    {
        ++index;
    }
    [(UIView *)[self.subviews objectAtIndex:index] removeFromSuperview];
}

// ============================================================
// frame and layout
- (void)set_ZFP_frame:(CGRect)newFrame ZFImpl_sys_iOS_overrideProperty
{
    self->_ZFP_frame = newFrame;
    self.frame = newFrame;
}
- (void)setNeedsLayout
{
    if(!self._ZFP_layoutRequested)
    {
        self._ZFP_layoutRequested = YES;
        if(self.superview != nil
            && !_ZFP_ZFUIViewImpl_sys_iOS_isImplView(self.superview))
        {
            self._ZFP_impl->notifyNeedLayout(self._ZFP_ownerZFUIView);
        }
    }
    [super setNeedsLayout];
}
- (CGSize)sizeThatFits:(CGSize)size
{
    return self._ZFP_frame.size;
}
- (void)layoutSubviews
{
    self._ZFP_layoutRequested = NO;
    [super layoutSubviews];
    if(self.superview != nil
        && !_ZFP_ZFUIViewImpl_sys_iOS_isImplView(self.superview))
    {
        self._ZFP_impl->notifyLayoutRootView(self._ZFP_ownerZFUIView, ZFImpl_sys_iOS_ZFUIKit_ZFUIRectFromCGRect(self.frame));
    }

    NSArray *children = self.subviews;
    for(NSUInteger i = 0; i < [children count]; ++i)
    {
        UIView *child = [children objectAtIndex:i];
        if([child isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
        {
            child.frame = ((_ZFP_ZFUIViewImpl_sys_iOS_View *)child)._ZFP_frame;
        }
        else if(child == self._ZFP_nativeImplView)
        {
            ZFUIRect rect;
            self._ZFP_impl->notifyLayoutNativeImplView(self._ZFP_ownerZFUIView, rect);
            child.frame = ZFImpl_sys_iOS_ZFUIKit_ZFUIRectToCGRect(rect);
        }
        else
        {
            child.frame = self.bounds;
        }
    }
}

// ============================================================
// touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesBegan:touches withEvent:event];

    for(UITouch *touch in touches)
    {
        self._ZFP_mouseRecords.add(touch);

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseDown;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesMoved:touches withEvent:event];

    for(UITouch *touch in touches)
    {
        if(self._ZFP_mouseRecords.find(touch, ZFComparerCheckEqualOnly) == zfindexMax)
        {
            self._ZFP_mouseRecords.add(touch);
        }
    }

    for(zfindex i = 0; i < self._ZFP_mouseRecords.count(); ++i)
    {
        UITouch *touch = self._ZFP_mouseRecords.get(i);

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseMove;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesEnded:touches withEvent:event];

    for(UITouch *touch in touches)
    {
        zfindex index = self._ZFP_mouseRecords.find(touch, ZFComparerCheckEqualOnly);
        if(index != zfindexMax)
        {
            self._ZFP_mouseRecords.remove(index);
        }

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseUp;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    // should not called otherwise touch would be passed to parent for a empty UIView
    // [super touchesCancelled:touches withEvent:event];

    for(UITouch *touch in touches)
    {
        zfindex index = self._ZFP_mouseRecords.find(touch, ZFComparerCheckEqualOnly);
        if(index != zfindexMax)
        {
            self._ZFP_mouseRecords.remove(index);
        }

        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)[touch hash];
        ev->mouseAction = ZFUIMouseAction::e_MouseCancel;
        ev->mousePoint = ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint([touch locationInView:self]);
        self._ZFP_impl->notifyUIEvent(self._ZFP_ownerZFUIView, ev);
    }
}

// ============================================================
// ZFUIViewFocus
- (BOOL)canBecomeFirstResponder
{
    return self._ZFP_ZFUIViewFocus_viewFocusable;
}
- (BOOL)becomeFirstResponder
{
    BOOL ret = NO;
    if(self._ZFP_nativeImplView != nil && !self._ZFP_nativeImplView.isFirstResponder)
    {
        ret = [self._ZFP_nativeImplView becomeFirstResponder];
    }
    else if(!self._ZFP_nativeImplView)
    {
        ret = [super becomeFirstResponder];
    }
    return ret;
}
- (BOOL)resignFirstResponder
{
    BOOL ret = NO;
    if(self._ZFP_nativeImplView != nil && self._ZFP_nativeImplView.isFirstResponder)
    {
        ret = [self._ZFP_nativeImplView resignFirstResponder];
    }
    else if(self.isFirstResponder)
    {
        ret = [super resignFirstResponder];
    }
    return ret;
}
@end

@interface _ZFP_ZFUIViewImpl_sys_iOS_ScrollViewInternal : UIScrollView
@property (nonatomic, assign) CGPoint _ZFP_mouseDrag;
@property (nonatomic, assign) CGPoint _ZFP_mouseDragPrevPos;
@property (nonatomic, assign) zfbool _ZFP_mouseDragOverride;
@property (nonatomic, assign) ZFUIRect _ZFP_scrollContentFrame;
@end
@implementation _ZFP_ZFUIViewImpl_sys_iOS_ScrollViewInternal
@synthesize
    _ZFP_mouseDrag,
    _ZFP_mouseDragPrevPos,
    _ZFP_mouseDragOverride,
    _ZFP_scrollContentFrame;
- (BOOL)touchesShouldCancelInContentView:(UIView *)view
{
    if([view isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        return !(((_ZFP_ZFUIViewImpl_sys_iOS_View *)view)._ZFP_uiEnable);
    }
    return [super touchesShouldCancelInContentView:view];
}
- (void)set_ZFP_scrollContentFrame:(ZFUIRect)newScrollContentFrame ZFImpl_sys_iOS_overrideProperty
{
    self->_ZFP_scrollContentFrame = newScrollContentFrame;

    self._ZFP_mouseDragOverride = zftrue;
    self.contentSize = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(newScrollContentFrame.size);
    [self setContentOffset:CGPointMake(-newScrollContentFrame.point.x, -newScrollContentFrame.point.y) animated:NO];
    self._ZFP_mouseDragPrevPos = self.contentOffset;
    self._ZFP_mouseDragOverride = zffalse;
}
@end

@interface _ZFP_ZFUIViewImpl_sys_iOS_ScrollView : _ZFP_ZFUIViewImpl_sys_iOS_View<UIScrollViewDelegate>
@property (nonatomic, assign) ZFUIScrollView *_ZFP_ownerZFUIScrollView;
@property (nonatomic, readonly) _ZFP_ZFUIViewImpl_sys_iOS_ScrollViewInternal *_ZFP_scrollView;
@property (nonatomic, assign) zfbool _ZFP_layoutFlag;

- (void)_ZFP_scrollViewInternalBgViewAdd:(UIView *)view atVirtualIndex:(zfindex)atVirtualIndex;
- (void)_ZFP_scrollViewInternalBgViewRemoveAtVirtualIndex:(zfindex)atVirtualIndex;

- (void)_ZFP_scrollViewAdd:(UIView *)view atVirtualIndex:(zfindex)atVirtualIndex;
- (void)_ZFP_scrollViewRemoveAtVirtualIndex:(zfindex)atVirtualIndex;

- (void)_ZFP_scrollViewInternalFgViewAdd:(UIView *)view atVirtualIndex:(zfindex)atVirtualIndex;
- (void)_ZFP_scrollViewInternalFgViewRemoveAtVirtualIndex:(zfindex)atVirtualIndex;

@property (nonatomic, assign) zfindex _ZFP_scrollViewBgViewCount;
@property (nonatomic, assign) zfindex _ZFP_scrollViewCenterViewCount;
@property (nonatomic, retain) NSMutableArray *_ZFP_scrollViewContentViews;
@property (nonatomic, retain) NSTimer *_ZFP_scrollImplScrollAnimationTimer;
- (void)_ZFP_scrollContentFrameFix;
- (void)_ZFP_scrollImplScrollAnimationTimerEvent:(id)dummy;
@end
@implementation _ZFP_ZFUIViewImpl_sys_iOS_ScrollView
@synthesize
    _ZFP_ownerZFUIScrollView,
    _ZFP_scrollView;
@synthesize
    _ZFP_scrollViewBgViewCount,
    _ZFP_scrollViewCenterViewCount,
    _ZFP_scrollViewContentViews,
    _ZFP_scrollImplScrollAnimationTimer;
- (id)init
{
    self = [super init];

    self._ZFP_scrollViewContentViews = [[NSMutableArray new] autorelease];

    self._ZFP_scrollViewCenterViewCount = 0;

    // add a dummy button for blocking touch event from background
    [self _ZFP_childAdd:[UIButton buttonWithType:UIButtonTypeCustom] atVirtualIndex:0 childLayer:ZFUIViewChildLayer::e_Background];
    self._ZFP_scrollViewCenterViewCount += 1;

    // add the scroll view
    self->_ZFP_scrollView = [_ZFP_ZFUIViewImpl_sys_iOS_ScrollViewInternal new];
    [self _ZFP_childAdd:self._ZFP_scrollView atVirtualIndex:1 childLayer:ZFUIViewChildLayer::e_Background];
    self._ZFP_scrollViewCenterViewCount += 1;
    self._ZFP_scrollView.delegate = self;

    self._ZFP_scrollView.bounces = YES;
    self._ZFP_scrollView.alwaysBounceHorizontal = NO;
    self._ZFP_scrollView.alwaysBounceVertical = NO;

    self._ZFP_scrollView.showsHorizontalScrollIndicator = NO;
    self._ZFP_scrollView.showsVerticalScrollIndicator = NO;

    self._ZFP_scrollView.exclusiveTouch = YES;

    return self;
}
- (void)dealloc
{
    [self->_ZFP_scrollView release];
    self->_ZFP_scrollView = nil;

    self._ZFP_scrollViewContentViews = nil;

    [super dealloc];
}
- (void)set_ZFP_ownerZFUIView:(ZFUIView *)newOwnerZFUIView ZFImpl_sys_iOS_overrideProperty
{
    [super set_ZFP_ownerZFUIView:newOwnerZFUIView];
    self._ZFP_ownerZFUIScrollView = ZFCastZFObject(ZFUIScrollView *, newOwnerZFUIView);
}

- (void)_ZFP_scrollViewInternalBgViewAdd:(UIView *)view atVirtualIndex:(zfindex)atVirtualIndex
{
    self._ZFP_scrollViewBgViewCount += 1;
    [self _ZFP_childAdd:view atVirtualIndex:(NSUInteger)atVirtualIndex childLayer:ZFUIViewChildLayer::e_Background];
}
- (void)_ZFP_scrollViewInternalBgViewRemoveAtVirtualIndex:(zfindex)atVirtualIndex
{
    self._ZFP_scrollViewBgViewCount -= 1;
    [self _ZFP_childRemoveAtVirtualIndex:(NSUInteger)atVirtualIndex childLayer:ZFUIViewChildLayer::e_Background];
}

- (void)_ZFP_scrollViewAdd:(UIView *)view atVirtualIndex:(zfindex)atVirtualIndex
{
    if(atVirtualIndex == (zfindex)[self._ZFP_scrollViewContentViews count])
    {
        [self._ZFP_scrollViewContentViews addObject:view];
        [self._ZFP_scrollView addSubview:view];
    }
    else
    {
        [self._ZFP_scrollViewContentViews insertObject:view atIndex:(NSUInteger)atVirtualIndex];
        [self._ZFP_scrollView insertSubview:view aboveSubview:[self._ZFP_scrollViewContentViews objectAtIndex:(NSUInteger)atVirtualIndex]];
    }
}
- (void)_ZFP_scrollViewRemoveAtVirtualIndex:(zfindex)atVirtualIndex
{
    UIView *child = [[[self._ZFP_scrollViewContentViews objectAtIndex:(NSUInteger)atVirtualIndex] retain] autorelease];
    [self._ZFP_scrollViewContentViews removeObjectAtIndex:(NSUInteger)atVirtualIndex];
    [child removeFromSuperview];
}

- (void)_ZFP_scrollViewInternalFgViewAdd:(UIView *)view atVirtualIndex:(zfindex)atVirtualIndex
{
    [self _ZFP_childAdd:view atVirtualIndex:(self._ZFP_scrollViewBgViewCount + self._ZFP_scrollViewCenterViewCount + atVirtualIndex) childLayer:ZFUIViewChildLayer::e_Foreground];
}
- (void)_ZFP_scrollViewInternalFgViewRemoveAtVirtualIndex:(zfindex)atVirtualIndex
{
    [self _ZFP_childRemoveAtVirtualIndex:(self._ZFP_scrollViewBgViewCount + self._ZFP_scrollViewCenterViewCount + atVirtualIndex) childLayer:ZFUIViewChildLayer::e_Foreground];
}

// scroll logic
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    self._ZFP_scrollView._ZFP_mouseDrag = point;
    UITouch *touch = [[event allTouches] anyObject];
    if(touch.phase == UITouchPhaseBegan)
    {
        self._ZFP_ownerZFUIScrollView->scrollContentFrameSet(self._ZFP_ownerZFUIScrollView->scrollContentFrame());
    }
    return [super hitTest:point withEvent:event];
}
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    [self _ZFP_scrollContentFrameFix];
    self._ZFP_scrollView._ZFP_mouseDragPrevPos = self._ZFP_scrollView.contentOffset;
    self._ZFP_scrollView._ZFP_mouseDragOverride = zffalse;
    self._ZFP_impl->notifyScrollViewDragBegin(
        self._ZFP_ownerZFUIScrollView,
        ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint(self._ZFP_scrollView._ZFP_mouseDrag),
        _ZFP_ZFUIViewImpl_sys_iOS_timestamp());
}
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if(self._ZFP_layoutFlag)
    {
        self._ZFP_scrollView._ZFP_scrollContentFrame = self._ZFP_scrollView._ZFP_scrollContentFrame;
        return ;
    }

    if(self._ZFP_scrollView.dragging)
    {
        if(!self._ZFP_scrollView._ZFP_mouseDragOverride)
        {
            self._ZFP_scrollView._ZFP_mouseDrag = CGPointMake(
                self._ZFP_scrollView._ZFP_mouseDrag.x - (self._ZFP_scrollView.contentOffset.x - self._ZFP_scrollView._ZFP_mouseDragPrevPos.x),
                self._ZFP_scrollView._ZFP_mouseDrag.y - (self._ZFP_scrollView.contentOffset.y - self._ZFP_scrollView._ZFP_mouseDragPrevPos.y));

            self._ZFP_impl->notifyScrollViewDrag(
                self._ZFP_ownerZFUIScrollView,
                ZFImpl_sys_iOS_ZFUIKit_ZFUIPointFromCGPoint(self._ZFP_scrollView._ZFP_mouseDrag),
                _ZFP_ZFUIViewImpl_sys_iOS_timestamp());
        }
    }
    self._ZFP_scrollView._ZFP_mouseDragPrevPos = self._ZFP_scrollView.contentOffset;
}
- (void)scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset
{
    *targetContentOffset = self._ZFP_scrollView.contentOffset;
    // iOS9 or later would have strange decerlating logic,
    // which would cause shake when bounce back,
    // force update contentOffset once, to prevent shake
    dispatch_async(dispatch_get_main_queue(), ^{
        [self._ZFP_scrollView setContentOffset:self._ZFP_scrollView.contentOffset animated:NO];
    });
}
- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    self._ZFP_impl->notifyScrollViewDragEnd(self._ZFP_ownerZFUIScrollView, _ZFP_ZFUIViewImpl_sys_iOS_timestamp(), zftrue);
}
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    [self _ZFP_scrollContentFrameFix];
}
- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView
{
    [self _ZFP_scrollContentFrameFix];
}
- (void)_ZFP_scrollContentFrameFix
{
    self._ZFP_impl->scrollViewScrollContentFrameSetForImpl(
        self._ZFP_ownerZFUIScrollView,
        ZFUIRectMake(
            zfmRound(-self._ZFP_scrollView.contentOffset.x),
            zfmRound(-self._ZFP_scrollView.contentOffset.y),
            zfmRound(self._ZFP_scrollView.contentSize.width),
            zfmRound(self._ZFP_scrollView.contentSize.height)));
}
- (void)_ZFP_scrollImplScrollAnimationTimerEvent:(id)dummy
{
    if(self._ZFP_scrollImplScrollAnimationTimer != nil)
    {
        self._ZFP_impl->notifyScrollViewScrollAnimation(self._ZFP_ownerZFUIScrollView, _ZFP_ZFUIViewImpl_sys_iOS_timestamp());
    }
}

- (void)layoutSubviews
{
    self._ZFP_layoutFlag = zftrue;
    [super layoutSubviews];
    for(NSUInteger i = 0; i < [self._ZFP_scrollViewContentViews count]; ++i)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *child = (_ZFP_ZFUIViewImpl_sys_iOS_View *)[self._ZFP_scrollViewContentViews objectAtIndex:i];
        child.frame = child._ZFP_frame;
    }
    self._ZFP_layoutFlag = zffalse;
}

- (BOOL)scrollViewShouldScrollToTop:(UIScrollView *)scrollView
{
    return NO;
}
@end

static BOOL _ZFP_ZFUIViewImpl_sys_iOS_isImplView(id obj)
{
    return [obj isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]] || [obj isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_ScrollViewInternal class]];
}

static void _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged(ZF_IN UIView *nativeImplView)
{
    if([nativeImplView isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        ZFProtocolZFUIViewFocus_notifyViewFocusChanged(((_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView)._ZFP_ownerZFUIView);
    }
    else if([nativeImplView.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        ZFProtocolZFUIViewFocus_notifyViewFocusChanged(((_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview)._ZFP_ownerZFUIView);
    }
    else if([nativeImplView.superview.superview isKindOfClass:[_ZFP_ZFUIViewImpl_sys_iOS_View class]])
    {
        ZFProtocolZFUIViewFocus_notifyViewFocusChanged(((_ZFP_ZFUIViewImpl_sys_iOS_View *)nativeImplView.superview.superview)._ZFP_ownerZFUIView);
    }
}

@implementation UIView (_ZFP_ZFUIViewImpl_sys_iOS_MethodSwizzling)
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder
{
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder];
    if(!old && self.isFirstResponder)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged(self);
    }
    return ret;
}
- (BOOL)_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder
{
    BOOL old = self.isFirstResponder;
    BOOL ret = [self _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder];
    if(old && !self.isFirstResponder)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_notifyViewFocusChanged(self);
    }
    return ret;
}
@end
static void _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare(void)
{
    {
        Method methodOrg =  class_getInstanceMethod([UIView class], @selector(becomeFirstResponder));
        Method methodNew = class_getInstanceMethod([UIView class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_becomeFirstResponder));
        method_exchangeImplementations(methodOrg, methodNew);
    }
    {
        Method methodOrg =  class_getInstanceMethod([UIView class], @selector(resignFirstResponder));
        Method methodNew = class_getInstanceMethod([UIView class], @selector(_ZFP_ZFUIViewImpl_sys_iOS_methodSwizzling_resignFirstResponder));
        method_exchangeImplementations(methodOrg, methodNew);
    }
}

ZF_NAMESPACE_GLOBAL_BEGIN
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_iOS, ZFUIView, ZFProtocolLevelSystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIView"))

public:
    ZFPROTOCOL_IMPLEMENTATION_CLASS(ZFUIViewImpl_sys_iOS)(void)
    {
        static zfbool _needMethodSwizzling = zftrue;
        if(_needMethodSwizzling)
        {
            _needMethodSwizzling = zffalse;
            _ZFP_ZFUIViewImpl_sys_iOS_methodSwizzlePrepare();
        }

        ZFUIKeyboardStateBuiltinImplRegister();
    }
    virtual ~ZFPROTOCOL_IMPLEMENTATION_CLASS(ZFUIViewImpl_sys_iOS)(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();
    }

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        if(!view->classData()->classIsSubclassOf(ZFUIScrollView::ClassData()))
        {
            _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = [_ZFP_ZFUIViewImpl_sys_iOS_View new];
            nativeView._ZFP_impl = this;
            nativeView._ZFP_ownerZFUIView = view;
            return nativeView;
        }
        else
        {
            _ZFP_ZFUIViewImpl_sys_iOS_ScrollView *nativeView = [_ZFP_ZFUIViewImpl_sys_iOS_ScrollView new];
            nativeView._ZFP_impl = this;
            nativeView._ZFP_ownerZFUIView = view;
            return nativeView;
        }
    }
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView)
    {
        [ZFCastStatic(UIView *, nativeView) release];
    }

    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView())._ZFP_nativeImplView = ZFCastStatic(UIView *, nativeImplView);
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        return 1;
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        UIScreen *ownerScreen = ZFCastStatic(UIView *, nativeView).window.screen;
        return (zffloat)((ownerScreen == nil) ? [UIScreen mainScreen].scale : ownerScreen.scale);
    }

    // ============================================================
    // properties
public:
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView()).hidden = !viewVisible;
    }
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView()).alpha = viewAlpha;
    }
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView())._ZFP_uiEnable = viewUIEnable;
    }
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView())._ZFP_uiEnableTree = viewUIEnableTree;
    }
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView()).backgroundColor = ZFImpl_sys_iOS_ZFUIKit_ZFUIColorToUIColor(viewBackgroundColor);
    }

public:
    virtual void scrollViewScrollEnableSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollEnable)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_ScrollView *, scrollView->nativeView())._ZFP_scrollView.scrollEnabled = scrollEnable;
    }
    virtual void scrollViewScrollBounceSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollBounceHorizontal,
                                           ZF_IN zfbool scrollBounceVertical,
                                           ZF_IN zfbool scrollBounceHorizontalAlways,
                                           ZF_IN zfbool scrollBounceVerticalAlways)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_ScrollView *, scrollView->nativeView());
        nativeScrollView._ZFP_scrollView.bounces = (scrollBounceHorizontal || scrollBounceVertical);
        nativeScrollView._ZFP_scrollView.alwaysBounceHorizontal = scrollBounceHorizontalAlways;
        nativeScrollView._ZFP_scrollView.alwaysBounceVertical = scrollBounceVerticalAlways;
    }
    virtual void scrollViewScrollContentFrameSet(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN const ZFUIRect &frame)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_ScrollView *, scrollView->nativeView());
        nativeScrollView._ZFP_scrollView._ZFP_scrollContentFrame = frame;
    }
    virtual zftimet scrollViewScrollAnimationStart(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_ScrollView *, scrollView->nativeView());
        nativeScrollView._ZFP_scrollImplScrollAnimationTimer = [NSTimer timerWithTimeInterval:0.03f target:nativeScrollView selector:@selector(_ZFP_scrollImplScrollAnimationTimerEvent:) userInfo:nil repeats:YES];
        [[NSRunLoop mainRunLoop] addTimer:nativeScrollView._ZFP_scrollImplScrollAnimationTimer forMode:NSRunLoopCommonModes];
        return _ZFP_ZFUIViewImpl_sys_iOS_timestamp();
    }
    virtual void scrollViewScrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_ScrollView *, scrollView->nativeView());
        [nativeScrollView._ZFP_scrollImplScrollAnimationTimer invalidate];
        nativeScrollView._ZFP_scrollImplScrollAnimationTimer = nil;
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex atIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        if(!parent->classData()->classIsSubclassOf(ZFUIScrollView::ClassData()))
        {
            [ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, parent->nativeView())
                _ZFP_childAdd:ZFCastStatic(UIView *, child->nativeView())
                atVirtualIndex:atIndex
                childLayer:childLayer];
        }
        else
        {
            _ZFP_ZFUIViewImpl_sys_iOS_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_ScrollView *, parent->nativeView());
            UIView *nativeChildView = ZFCastStatic(UIView *, child->nativeView());
            switch(childLayer)
            {
                case ZFUIViewChildLayer::e_Background:
                    [nativeScrollView _ZFP_scrollViewInternalBgViewAdd:nativeChildView atVirtualIndex:childLayerIndex];
                    break;
                case ZFUIViewChildLayer::e_Normal:
                    [nativeScrollView _ZFP_scrollViewAdd:nativeChildView atVirtualIndex:childLayerIndex];
                    break;
                case ZFUIViewChildLayer::e_Foreground:
                    [nativeScrollView _ZFP_scrollViewInternalFgViewAdd:nativeChildView atVirtualIndex:childLayerIndex];
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN zfindex atIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        if(!parent->classData()->classIsSubclassOf(ZFUIScrollView::ClassData()))
        {
            [ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, parent->nativeView()) _ZFP_childRemoveAtVirtualIndex:atIndex childLayer:childLayer];
        }
        else
        {
            _ZFP_ZFUIViewImpl_sys_iOS_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_ScrollView *, parent->nativeView());
            switch(childLayer)
            {
                case ZFUIViewChildLayer::e_Background:
                    [nativeScrollView _ZFP_scrollViewInternalBgViewRemoveAtVirtualIndex:childLayerIndex];
                    break;
                case ZFUIViewChildLayer::e_Normal:
                    [nativeScrollView _ZFP_scrollViewRemoveAtVirtualIndex:childLayerIndex];
                    break;
                case ZFUIViewChildLayer::e_Foreground:
                    [nativeScrollView _ZFP_scrollViewInternalFgViewRemoveAtVirtualIndex:childLayerIndex];
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
    }

public:
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView())._ZFP_frame =
            ZFImpl_sys_iOS_ZFUIKit_ZFUIRectToCGRect(rect);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        // iOS needs to setNeedsLayout recursively
        UIView *nativeView = ZFCastStatic(UIView *, view->nativeView());
        do
        {
            [nativeView setNeedsLayout];
            nativeView = nativeView.superview;
        } while(nativeView != nil);
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
    {
        ret = ZFUISizeMake(zfmMax(sizeHint.width, 0), zfmMax(sizeHint.height, 0));
        ret = ZFImpl_sys_iOS_ZFUIKit_ZFUISizeFromCGSize(
            [ZFCastStatic(UIView *, nativeView) sizeThatFits:ZFImpl_sys_iOS_ZFUIKit_ZFUISizeToCGSize(ret)]);
        ret = ZFUIViewLayoutParam::sizeHintApply(ret, sizeHint, sizeParam);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_iOS)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_iOS, ZFUIViewFocus, ZFProtocolLevelSystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("iOS:UIView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewFocusableSet(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocusable)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView());
        nativeView._ZFP_ZFUIViewFocus_viewFocusable = viewFocusable;
        if(!viewFocusable)
        {
            [nativeView resignFirstResponder];
        }
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_iOS_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_iOS_View *, view->nativeView());
        return (nativeView.isFirstResponder
            || (nativeView._ZFP_nativeImplView != nil && nativeView._ZFP_nativeImplView.isFirstResponder));
    }
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus)
    {
        UIView *nativeView = ZFCastStatic(UIView *, view->nativeView());
        if(viewFocus)
        {
            [nativeView becomeFirstResponder];
        }
        else
        {
            [nativeView resignFirstResponder];
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS


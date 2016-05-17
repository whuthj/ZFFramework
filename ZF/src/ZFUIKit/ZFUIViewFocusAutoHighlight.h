/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUIViewFocusAutoHighlight.h
 * @brief auto highlight focused view
 */

#ifndef _ZFI_ZFUIViewFocusAutoHighlight_h_
#define _ZFI_ZFUIViewFocusAutoHighlight_h_

#include "ZFUIView.h"
#include "ZFUIImageIO.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief filter to exclude certain view to be auto highlighted, empty by default
 */
extern ZF_ENV_EXPORT ZFFilterForZFObject ZFUIViewFocusAutoHighlightFilter;

// ============================================================
/**
 * @brief whether auto start #ZFUIViewFocusAutoHighlightStart, true by default
 *
 * this value would be initialized as true during #ZFFrameworkInit as level #ZFLevelZFFrameworkLow
 */
extern ZF_ENV_EXPORT zfbool ZFUIViewFocusAutoHighlightAutoStart;
/**
 * @brief default mask image for #ZFUIViewFocusAutoHighlightStart, an alpha white image by default
 *
 * this property would be initialized and destroyed automatically during #ZFFrameworkCleanup
 * as level #ZFLevelZFFrameworkLow
 */
extern ZF_ENV_EXPORT zfautoObject ZFUIViewFocusAutoHighlightMaskImage;

/**
 * @brief util method to automatically highlight focused view
 *
 * by default:
 * -  #ZFUIViewFocusAutoHighlightStart would be called automatically during #ZFFrameworkInit
 *   as level #ZFLevelAppLow if #ZFUIViewFocusAutoHighlightAutoStart
 * -  #ZFUIViewFocusAutoHighlightStop would be called automatically during #ZFFrameworkCleanup
 *   as level #ZFLevelZFFrameworkLow
 */
extern ZF_ENV_EXPORT void ZFUIViewFocusAutoHighlightStart(ZF_IN_OPT ZFUIImage *img = zfnull);
/**
 * @brief see #ZFUIViewFocusAutoHighlightStart
 */
extern ZF_ENV_EXPORT void ZFUIViewFocusAutoHighlightStop(void);
/**
 * @brief see #ZFUIViewFocusAutoHighlightStart
 */
extern ZF_ENV_EXPORT zfbool ZFUIViewFocusAutoHighlightStarted(void);

/**
 * @brief temporary pause the auto highlight
 *
 * can be called more than one time, but must be paired with #ZFUIViewFocusAutoHighlightResume
 */
extern ZF_ENV_EXPORT void ZFUIViewFocusAutoHighlightPause(void);
/** @brief see #ZFUIViewFocusAutoHighlightPause */
extern ZF_ENV_EXPORT void ZFUIViewFocusAutoHighlightResume(void);
/** @brief see #ZFUIViewFocusAutoHighlightPause */
extern ZF_ENV_EXPORT zfindex ZFUIViewFocusAutoHighlightPaused(void);

// ============================================================
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was highlighted\n
 * notified when #ZFUIViewFocusAutoHighlightStart
 */
ZFOBSERVER_EVENT_GLOBAL(ViewFocusAutoHighlightStart)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was highlighted\n
 * notified when #ZFUIViewFocusAutoHighlightStop
 */
ZFOBSERVER_EVENT_GLOBAL(ViewFocusAutoHighlightStop)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was highlighted\n
 * notified when #ZFUIViewFocusAutoHighlightPause
 */
ZFOBSERVER_EVENT_GLOBAL(ViewFocusAutoHighlightPause)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was highlighted\n
 * notified when #ZFUIViewFocusAutoHighlightResume
 */
ZFOBSERVER_EVENT_GLOBAL(ViewFocusAutoHighlightResume)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was highlighted\n
 * notified when a view is highlighted
 */
ZFOBSERVER_EVENT_GLOBAL(ViewFocusAutoHighlightViewOn)
/**
 * @brief see #ZFObject::observerNotify
 *
 * sender is the view that was highlighted\n
 * notified when a view is highlighted
 */
ZFOBSERVER_EVENT_GLOBAL(ViewFocusAutoHighlightViewOff)

// ============================================================
// ZFUIViewFocusAutoHighlightPauseForTime
/**
 * @brief pause #ZFUIViewFocusAutoHighlightPause for a specified time,
 *   and resume it automatically after time out
 *
 * the task would be scheduled and run in future even if time is 0
 */
extern ZF_ENV_EXPORT void ZFUIViewFocusAutoHighlightPauseForTime(ZF_IN zftimet time = 0);
/**
 * @brief cancel #ZFUIViewFocusAutoHighlightPauseForTime
 *
 * typically you should not call this method manually,
 * which would cancel all the task that started by #ZFUIViewFocusAutoHighlightPauseForTime
 */
extern ZF_ENV_EXPORT void ZFUIViewFocusAutoHighlightPauseForTimeCancel(void);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewFocusAutoHighlight_h_


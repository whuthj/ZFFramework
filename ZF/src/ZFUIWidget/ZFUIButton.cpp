/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIButton.h"
#include "../ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#if 0
    #define _ZFP_ZFUIButton_DEBUG_EVENT(actionName) \
        zfLogTrimT() << ZFMACRO_TOSTRING(actionName) << ZFDEBUG_HEADER_STRING;
#else
    #define _ZFP_ZFUIButton_DEBUG_EVENT(actionName)
#endif

// ============================================================
ZFENUM_DEFINE(ZFUIButtonState)

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIButtonStyle, ZFUIViewStyle)

// ============================================================
// _ZFP_ZFUIButtonPrivate
zfclassPOD _ZFP_ZFUIButtonMouseData
{
public:
    zfidentity mouseId;
    ZFUIMouseActionEnum mouseAction;
    ZFUIPoint mousePoint;
};
static inline _ZFP_ZFUIButtonMouseData _ZFP_ZFUIButtonMouseDataFromEvent(ZF_IN ZFUIMouseEvent *event)
{
    _ZFP_ZFUIButtonMouseData ret = {
        event->mouseId,
        event->mouseAction,
        event->mousePoint,
    };
    return ret;
}
#define _ZFP_ZFUIButtonMouseDataToEvent(event, mouseData) \
    ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, event); \
    event->mouseId = mouseData.mouseId; \
    event->mouseAction = mouseData.mouseAction; \
    event->mousePoint = mouseData.mousePoint

zfclassNotPOD _ZFP_ZFUIButtonPrivate
{
public:
    ZFUIButton *owner;
    ZFUIButtonStateEnum buttonState;
    zfidentity processingMouseId;
    ZFCoreArrayPOD<zfidentity> ignoredMouses;
    ZFCoreArrayPOD<_ZFP_ZFUIButtonMouseData> processingMouses;
    zfstlmap<zfidentity, ZFUIPoint> prevMousePointMap;
    zfstlmap<zfidentity, zfbool> fakeMouseId;
    zfbool buttonEnableCache;
    zfbool buttonHighlightedFlag;
    zftimet buttonLastClickTimestamp;

public:
    void buttonEnableSet(ZF_IN zfbool buttonEnable)
    {
        if(this->buttonEnableCache == buttonEnable)
        {
            return ;
        }
        this->buttonEnableCache = buttonEnable;

        if(buttonEnable)
        {
            if(this->owner->buttonChecked())
            {
                if(this->buttonHighlightedFlag)
                {
                    this->buttonState = ZFUIButtonState::e_CheckedHighlighted;
                }
                else
                {
                    this->buttonState = ZFUIButtonState::e_Checked;
                }
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnChecked)
                this->owner->buttonStateOnChange();
            }
            else
            {
                if(this->buttonHighlightedFlag)
                {
                    this->buttonState = ZFUIButtonState::e_Highlighted;
                }
                else
                {
                    this->buttonState = ZFUIButtonState::e_Normal;
                }
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
                this->owner->buttonStateOnChange();
            }
        }
        else
        {
            this->buttonState = ZFUIButtonState::e_Disabled;
            _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnDisabled)
            this->owner->buttonStateOnChange();

            if(!this->processingMouses.isEmpty())
            {
                for(zfindex i = 0; i < this->processingMouses.count(); ++i)
                {
                    this->ignoredMouses.add(this->processingMouses[i].mouseId);
                }
                ZFCoreArrayPOD<_ZFP_ZFUIButtonMouseData> tmp;
                tmp.copyFrom(this->processingMouses);
                this->processingMouses.removeAll();

                for(zfindex i = 0; i < tmp.count(); ++i)
                {
                    const _ZFP_ZFUIButtonMouseData &mouseData = tmp[i];
                    this->prevMousePointMap.erase(mouseData.mouseId);
                    _ZFP_ZFUIButtonMouseDataToEvent(event, mouseData);

                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                    this->owner->buttonMouseOnUp(event);
                }
            }
        }
    }
    void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
    {
        ZFUIMouseActionEnum mouseAction = mouseEvent->mouseAction;
        switch(mouseAction)
        {
            case ZFUIMouseAction::e_MouseDown:
            {
                if(!this->buttonEnableCache)
                {
                    this->ignoredMouses.add(mouseEvent->mouseId);
                    break;
                }

                if(this->processingMouseId != zfidentityInvalid)
                {
                    this->ignoredMouses.add(mouseEvent->mouseId);
                }
                else
                {
                    this->processingMouseId = mouseEvent->mouseId;
                    this->processingMouses.add(_ZFP_ZFUIButtonMouseDataFromEvent(mouseEvent));
                    this->processMouse(mouseEvent);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseMove:
            {
                if(this->ignoredMouses.find(mouseEvent->mouseId) == zfindexMax)
                {
                    this->processMouse(mouseEvent);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseUp:
            case ZFUIMouseAction::e_MouseCancel:
            {
                for(zfindex i = 0; i < this->processingMouses.count(); ++i)
                {
                    if(this->processingMouses[i].mouseId == mouseEvent->mouseId)
                    {
                        this->processingMouses.remove(i);
                        break;
                    }
                }
                zfindex ignoredIndex = this->ignoredMouses.find(mouseEvent->mouseId);
                if(ignoredIndex != zfindexMax)
                {
                    this->ignoredMouses.remove(ignoredIndex);
                }
                else
                {
                    this->processMouse(mouseEvent);
                    if(this->processingMouseId == mouseEvent->mouseId)
                    {
                        this->processingMouseId = zfidentityInvalid;
                    }
                }
            }
                break;
            case ZFUIMouseAction::e_MouseHoverEnter:
            case ZFUIMouseAction::e_MouseHover:
            case ZFUIMouseAction::e_MouseHoverExit:
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
        mouseEvent->eventResolvedSet(zftrue);
    }

private:
    void processMouse(ZF_IN ZFUIMouseEvent *mouseEvent)
    {
        switch(mouseEvent->mouseAction)
        {
            case ZFUIMouseAction::e_MouseDown:
            {
                this->prevMousePointMap[mouseEvent->mouseId] = mouseEvent->mousePoint;

                this->buttonHighlightedFlag = zftrue;
                if(this->owner->buttonChecked())
                {
                    this->buttonState = ZFUIButtonState::e_CheckedHighlighted;
                }
                else
                {
                    this->buttonState = ZFUIButtonState::e_Highlighted;
                }
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnHighlighted)
                this->owner->buttonStateOnChange();
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnDown)
                this->owner->buttonMouseOnDown(mouseEvent);
            }
                break;
            case ZFUIMouseAction::e_MouseMove:
            {
                zfstlmap<zfidentity, ZFUIPoint>::iterator prevMousePointMapIt = this->prevMousePointMap.find(mouseEvent->mouseId);
                if(prevMousePointMapIt == this->prevMousePointMap.end())
                {
                    break;
                }
                ZFUIPoint &prevMousePoint = prevMousePointMapIt->second;
                ZFUIRect bounds = ZFUIRectGetBounds(this->owner->layoutedFrame());
                zfbool mouseInside = this->buttonClickedInside(bounds, mouseEvent->mousePoint);
                zfbool mouseInsidePrev = this->buttonClickedInside(bounds, prevMousePoint);
                prevMousePoint = mouseEvent->mousePoint;
                this->buttonHighlightedFlag = mouseInside;

                if(mouseInside != mouseInsidePrev)
                {
                    if(mouseInside)
                    {
                        if(this->owner->buttonChecked())
                        {
                            this->buttonState = ZFUIButtonState::e_CheckedHighlighted;
                        }
                        else
                        {
                            this->buttonState = ZFUIButtonState::e_Highlighted;
                        }
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnHighlighted)
                        this->owner->buttonStateOnChange();
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveEnter)
                        this->owner->buttonMouseOnMoveEnter(mouseEvent);
                    }
                    else
                    {
                        if(this->owner->buttonChecked())
                        {
                            this->buttonState = ZFUIButtonState::e_Checked;
                        }
                        else
                        {
                            this->buttonState = ZFUIButtonState::e_Normal;
                        }
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
                        this->owner->buttonStateOnChange();
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveExit)
                        this->owner->buttonMouseOnMoveExit(mouseEvent);
                    }
                }
                if(mouseInside)
                {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveInside)
                    this->owner->buttonMouseOnMoveInside(mouseEvent);
                }
                else
                {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnMoveOutside)
                    this->owner->buttonMouseOnMoveOutside(mouseEvent);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseUp:
            {
                zfstlmap<zfidentity, ZFUIPoint>::iterator prevMousePointMapIt = this->prevMousePointMap.find(mouseEvent->mouseId);
                if(prevMousePointMapIt == this->prevMousePointMap.end())
                {
                    break;
                }
                this->prevMousePointMap.erase(prevMousePointMapIt);

                this->buttonHighlightedFlag = zffalse;
                zfbool mouseInside = this->buttonClickedInside(
                    ZFUIRectGetBounds(this->owner->layoutedFrame()),
                    mouseEvent->mousePoint);
                if(mouseInside)
                {
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUpInside)
                    this->owner->buttonMouseOnUpInside(mouseEvent);
                    if(this->owner->buttonCheckable())
                    {
                        this->owner->buttonCheckedSet(!this->owner->buttonChecked());
                    }
                    else
                    {
                        this->buttonState = ZFUIButtonState::e_Normal;
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
                        this->owner->buttonStateOnChange();
                    }
                    this->buttonClicked();
                }
                else
                {
                    if(this->owner->buttonChecked())
                    {
                        this->buttonState = ZFUIButtonState::e_Checked;
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnChecked)
                        this->owner->buttonStateOnChange();
                    }
                    else
                    {
                        this->buttonState = ZFUIButtonState::e_Normal;
                        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
                        this->owner->buttonStateOnChange();
                    }
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                    this->owner->buttonMouseOnUp(mouseEvent);
                }
            }
                break;
            case ZFUIMouseAction::e_MouseCancel:
            {
                zfstlmap<zfidentity, ZFUIPoint>::iterator prevMousePointMapIt = this->prevMousePointMap.find(mouseEvent->mouseId);
                if(prevMousePointMapIt == this->prevMousePointMap.end())
                {
                    break;
                }
                this->prevMousePointMap.erase(prevMousePointMapIt);

                this->buttonHighlightedFlag = zffalse;
                if(this->owner->buttonChecked())
                {
                    this->buttonState = ZFUIButtonState::e_Checked;
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnChecked)
                    this->owner->buttonStateOnChange();
                }
                else
                {
                    this->buttonState = ZFUIButtonState::e_Normal;
                    _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
                    this->owner->buttonStateOnChange();
                }
                _ZFP_ZFUIButton_DEBUG_EVENT(buttonMouseOnUp)
                this->owner->buttonMouseOnUp(mouseEvent);
            }
                break;
            case ZFUIMouseAction::e_MouseHoverEnter:
            case ZFUIMouseAction::e_MouseHover:
            case ZFUIMouseAction::e_MouseHoverExit:
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
    zfbool buttonClickedInside(ZF_IN const ZFUIRect &bounds,
                            ZF_IN const ZFUIPoint &mousePoint)
    {
        return ZFUIRectIsContainPoint(bounds, mousePoint, this->owner->buttonClickTolerance());
    }
    void buttonClicked(void)
    {
        zftimet curTime = ZFTime::timestamp();
        if(curTime - this->buttonLastClickTimestamp >= this->owner->buttonClickInterval())
        {
            this->buttonLastClickTimestamp = curTime;

            _ZFP_ZFUIButton_DEBUG_EVENT(buttonOnClick)
            this->owner->buttonOnClick();
        }
    }

public:
    _ZFP_ZFUIButtonPrivate(void)
    : owner(zfnull)
    , buttonState(ZFUIButtonState::e_Normal)
    , processingMouseId(zfidentityInvalid)
    , ignoredMouses()
    , processingMouses()
    , prevMousePointMap()
    , buttonEnableCache(zftrue)
    , buttonHighlightedFlag(zffalse)
    , buttonLastClickTimestamp(0)
    {
    }
};

// ============================================================
// ZFUIButton
ZFOBJECT_REGISTER(ZFUIButton)

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIButton, zfbool, buttonEnable)
{
    zfsuperI(ZFUIButtonStyle)::buttonEnableSet(newValue);
    d->buttonEnableSet(this->buttonEnable());
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIButton, zfbool, buttonCheckable)
{
    zfsuperI(ZFUIButtonStyle)::buttonCheckableSet(newValue);
    if(!this->buttonCheckable() && this->buttonChecked())
    {
        this->buttonCheckedSet(zffalse);
    }
}
ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIButton, zfbool, buttonChecked)
{
    zfsuperI(ZFUIButtonStyle)::buttonCheckedSet(this->buttonCheckable() && newValue);
    if(this->buttonEnable())
    {
        if(this->buttonChecked())
        {
            if(d->buttonHighlightedFlag)
            {
                d->buttonState = ZFUIButtonState::e_CheckedHighlighted;
            }
            else
            {
                d->buttonState = ZFUIButtonState::e_Checked;
            }
            _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnHighlighted)
            this->buttonStateOnChange();
        }
        else
        {
            if(d->buttonHighlightedFlag)
            {
                d->buttonState = ZFUIButtonState::e_Highlighted;
            }
            else
            {
                d->buttonState = ZFUIButtonState::e_Normal;
            }
            _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnNormal)
            this->buttonStateOnChange();
        }
    }
    else
    {
        d->buttonState = ZFUIButtonState::e_Disabled;
        _ZFP_ZFUIButton_DEBUG_EVENT(buttonStateOnDisabled)
        this->buttonStateOnChange();
    }
}

ZFObject *ZFUIButton::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIButtonPrivate);
    d->owner = this;
    return this;
}
void ZFUIButton::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIButton::buttonSimulateClick(ZF_IN_OPT zfbool preventFrequentlyClick /* = zffalse */)
{
    zfCoreAssert(ZFThread::currentThread() == ZFThread::mainThread());
    if(!preventFrequentlyClick)
    {
        d->buttonLastClickTimestamp = 0;
    }
    zfidentity mouseId = (zfidentity)zfmRand();
    {
        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, mouse);
        mouse->mouseId = mouseId;
        mouse->mouseAction = ZFUIMouseAction::e_MouseDown;
        mouse->mousePoint = ZFUIPointZero;
        this->viewEventSend(mouse);
    }
    {
        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, mouse);
        mouse->mouseId = mouseId;
        mouse->mouseAction = ZFUIMouseAction::e_MouseUp;
        mouse->mousePoint = ZFUIPointZero;
        this->viewEventSend(mouse);
    }
}

void ZFUIButton::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    zfsuper::viewEventOnMouseEvent(mouseEvent);

    d->viewEventOnMouseEvent(mouseEvent);
}
static zfidentity _ZFP_ZFUIButton_mouseIdFromKeyId(ZF_IN zfidentity keyId)
{
    return (keyId ^ 0x70000000);
}
void ZFUIButton::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    zfsuper::viewEventOnKeyEvent(keyEvent);
    if(keyEvent->eventResolved()) {
        return ;
    }

    switch(keyEvent->keyCode)
    {
        case ZFUIKeyCode::e_kEnter:
        case ZFUIKeyCode::e_kSpace:
            switch(keyEvent->keyAction)
            {
                case ZFUIKeyAction::e_KeyDown:
                {
                    ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, mouse);
                    mouse->mouseId = _ZFP_ZFUIButton_mouseIdFromKeyId(keyEvent->keyId);
                    mouse->mouseAction = ZFUIMouseAction::e_MouseDown;
                    mouse->mousePoint = ZFUIPointZero;
                    d->fakeMouseId[mouse->mouseId] = zftrue;
                    this->viewEventSend(mouse);
                }
                    break;
                case ZFUIKeyAction::e_KeyRepeat:
                    break;
                case ZFUIKeyAction::e_KeyUp:
                {
                    ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, mouse);
                    mouse->mouseId = _ZFP_ZFUIButton_mouseIdFromKeyId(keyEvent->keyId);
                    mouse->mouseAction = ZFUIMouseAction::e_MouseUp;
                    mouse->mousePoint = ZFUIPointZero;
                    d->fakeMouseId.erase(mouse->mouseId);
                    this->viewEventSend(mouse);
                }
                    break;
                case ZFUIKeyAction::e_KeyCancel:
                {
                    ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, mouse);
                    mouse->mouseId = _ZFP_ZFUIButton_mouseIdFromKeyId(keyEvent->keyId);
                    mouse->mouseAction = ZFUIMouseAction::e_MouseCancel;
                    mouse->mousePoint = ZFUIPointZero;
                    d->fakeMouseId.erase(mouse->mouseId);
                    this->viewEventSend(mouse);
                }
                    break;
            }
            keyEvent->eventResolvedSet(zftrue);
            break;
        default:
            break;
    }
}
void ZFUIButton::viewFocusOnChange(void)
{
    if(!d->fakeMouseId.empty())
    {
        zfstlmap<zfidentity, zfbool> t = d->fakeMouseId;
        d->fakeMouseId.clear();
        for(zfstlmap<zfidentity, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
        {
            ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, mouse);
            mouse->mouseId = it->first;
            mouse->mouseAction = ZFUIMouseAction::e_MouseCancel;
            mouse->mousePoint = ZFUIPointZero;
            this->viewEventSend(mouse);
        }
    }

    zfsuper::viewFocusOnChange();
}

void ZFUIButton::viewPropertyOnUpdate(void)
{
    zfsuper::viewPropertyOnUpdate();
    this->buttonStateUpdate();
}

ZFUIButtonStateEnum ZFUIButton::buttonState(void)
{
    return d->buttonState;
}
void ZFUIButton::buttonStateUpdate(void)
{
    this->buttonStateOnChange();
}

ZF_NAMESPACE_GLOBAL_END


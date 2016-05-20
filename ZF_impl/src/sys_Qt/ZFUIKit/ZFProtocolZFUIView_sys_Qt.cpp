/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewFocus.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_Qt

#include <QTime>
#include <QWidget>
#include <QLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QTimer>
#include <QCoreApplication>
#include <QGraphicsOpacityEffect>
#include <QSet>

// ============================================================
extern ZFUIKeyCodeEnum ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(int qKeyCode);
static zfbool _ZFP_ZFUIViewImpl_sys_Qt_isImplView(QWidget *obj);

static zftimet _ZFP_ZFUIViewImpl_sys_Qt_timestamp(void)
{
    return (zftimet)QDateTime::currentMSecsSinceEpoch();
}

class _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData
{
public:
    zfbool cancelFlag;
    QSet<QWidget *> forwardedFlag;
public:
    _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData(void)
    : cancelFlag(zffalse)
    , forwardedFlag()
    {
    }
};

static QMap<QEvent *, _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData> &_ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagMap(void)
{
    static QMap<QEvent *, _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData> m;
    return m;
}
static _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData *_ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagCheck(ZF_IN QEvent *event)
{
    QMap<QEvent *, _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData> &m = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagMap();
    QMap<QEvent *, _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData>::iterator it = m.find(event);
    if(it != m.end())
    {
        return &(it.value());
    }
    return zfnull;
}
static _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData *_ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagAccess(ZF_IN QEvent *event)
{
    QMap<QEvent *, _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData> &m = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagMap();
    QMap<QEvent *, _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData>::iterator it = m.find(event);
    if(it != m.end())
    {
        return &(it.value());
    }
    m.insert(event, _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData());
    return &(m.find(event).value());
}

static QMouseEvent *_ZFP_ZFUIViewImpl_sys_Qt_MouseEventClone(ZF_IN QMouseEvent *event,
                                                            ZF_IN QEvent::Type type,
                                                            ZF_IN QPointF const &localPos)
{
    QMouseEvent *ret = new QMouseEvent(
                type
                , localPos
                , event->windowPos()
                , event->screenPos()
                , event->button()
                , event->buttons()
                , event->modifiers()
                );
    ret->setTimestamp(event->timestamp());
    _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagCheck(event);
    if(tag != zfnull)
    {
        *(_ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagAccess(ret)) = *tag;
    }
    return ret;
}
static zfbool _ZFP_ZFUIViewImpl_sys_Qt_ChildOffset(ZF_OUT zfint &xOffset,
                                                  ZF_OUT zfint &yOffset,
                                                  ZF_IN QWidget *parent,
                                                  ZF_IN QWidget *child)
{
    while(child != zfnull && child != parent)
    {
        xOffset += child->geometry().x();
        yOffset += child->geometry().y();
        child = child->parentWidget();
    }
    return (child == parent);
}

// ============================================================
// layout
zfclassPOD _ZFP_ZFUIViewImpl_sys_Qt_ItemHolder
{
public:
    QWidget *widget;
    QLayoutItem *layoutItem;
};
class _ZFP_ZFUIViewImpl_sys_Qt_View;
class _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy : public QLayout
{
    Q_OBJECT

public:
    _ZFP_ZFUIViewImpl_sys_Qt_View *_ZFP_owner;
    ZFCoreArrayPOD<_ZFP_ZFUIViewImpl_sys_Qt_ItemHolder> _ZFP_children;

public:
    ~_ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy(void);

public:
    void childAdd(ZF_IN ZFUIView *ownerZFUIViewOrNull,
                  ZF_IN QWidget *widget,
                  ZF_IN zfindex atIndex,
                  ZF_IN ZFUIViewChildLayerEnum childLayer);
    void childRemove(ZF_IN zfindex atIndex,
                     ZF_IN ZFUIViewChildLayerEnum childLayer);

public:
    virtual void addItem(QLayoutItem *layoutItem);
    virtual QSize sizeHint(void) const;
    virtual QSize minimumSize(void) const;
    virtual void setGeometry(const QRect &rect);

    virtual QLayoutItem *itemAt(int index) const;
    virtual QLayoutItem *takeAt(int index);
    virtual int indexOf(QWidget *widget) const;
    virtual int count() const;
};

// ============================================================
// focus
extern void *_ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(ZF_IN ZFUIView *ownerZFUIView,
                                                      ZF_IN QWidget *nativeOwner,
                                                      ZF_IN QWidget *nativeImplViewOrNull);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(ZF_IN void *token);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(ZF_IN void *token, ZF_IN zfbool v);
extern void _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(ZF_IN void *token, ZF_IN zfbool v);
extern zfbool _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(ZF_IN void *token);

// ============================================================
// native view
class _ZFP_ZFUIViewImpl_sys_Qt_View : public QWidget
{
    Q_OBJECT

public:
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *_ZFP_impl;
    ZFUIView *_ZFP_ownerZFUIView;
    _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy _ZFP_layoutProxy;
    void *_ZFP_focusProxyToken;
    QRect _ZFP_frame;
    QWidget *_ZFP_nativeImplView;
    zfbool _ZFP_viewUIEnable;
    zfbool _ZFP_viewUIEnableTree;
    zfbool _ZFP_mousePressed;
    zfbool _ZFP_mouseEnterFlag;
    ZFUIPoint _ZFP_mouseMoveLastPoint;
    zfbool _ZFP_layoutRequested;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_View(void) : QWidget()
    , _ZFP_impl(zfnull)
    , _ZFP_ownerZFUIView(zfnull)
    , _ZFP_layoutProxy()
    , _ZFP_focusProxyToken(zfnull)
    , _ZFP_frame()
    , _ZFP_nativeImplView(zfnull)
    , _ZFP_viewUIEnable(zftrue)
    , _ZFP_viewUIEnableTree(zftrue)
    , _ZFP_mousePressed(zffalse)
    , _ZFP_mouseEnterFlag(zffalse)
    , _ZFP_mouseMoveLastPoint(ZFUIPointZero)
    , _ZFP_layoutRequested(zftrue)
    {
        this->setLayout(&_ZFP_layoutProxy);
        _ZFP_layoutProxy._ZFP_owner = this;

        QPalette palette = this->palette();
        palette.setColor(QPalette::Background, QColor(0, 0, 0, 0));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_View(void)
    {
        zfCoreAssert(this->_ZFP_nativeImplView == zfnull);
    }

public:
    void _ZFP_frameSet(ZF_IN const ZFUIRect &v)
    {
        this->_ZFP_frame = ZFImpl_sys_Qt_ZFUIKit_ZFUIRectToQRect(v);
        if(this->geometry() != _ZFP_frame)
        {
            this->setGeometry(_ZFP_frame);
        }
    }
    void _ZFP_nativeImplViewSet(QWidget *v)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(_ZFP_focusProxyToken);

        if(this->_ZFP_nativeImplView != zfnull)
        {
            _ZFP_layoutProxy.childRemove(this->_ZFP_ownerZFUIView->internalBackgroundViewArray().count(), ZFUIViewChildLayer::e_Background);
        }
        this->_ZFP_nativeImplView = v;
        if(this->_ZFP_nativeImplView != zfnull)
        {
            _ZFP_layoutProxy.childAdd(zfnull, this->_ZFP_nativeImplView, this->_ZFP_ownerZFUIView->internalBackgroundViewArray().count(), ZFUIViewChildLayer::e_Background);
        }

        _ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(_ZFP_ownerZFUIView, this, _ZFP_nativeImplView);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(_ZFP_focusProxyToken, _ZFP_ownerZFUIView->viewFocusable());
    }
    void _ZFP_viewUIEnableSet(ZF_IN zfbool v)
    {
        _ZFP_viewUIEnable = v;
    }
    void _ZFP_viewUIEnableTreeSet(ZF_IN zfbool v)
    {
        _ZFP_viewUIEnableTree = v;
        this->setEnabled(_ZFP_viewUIEnableTree);
    }

public:
    bool event(QEvent *event)
    {
        if(_ZFP_ownerZFUIView == zfnull)
        {
            return QWidget::event(event);
        }
        switch(event->type())
        {
            // mouse
            case QEvent::MouseButtonPress:
            case QEvent::MouseMove:
            case QEvent::MouseButtonRelease:
                if(!this->_ZFP_viewUIEnableTree)
                {
                    return false;
                }
                if(!this->_ZFP_viewUIEnable)
                {
                    QMouseEvent *mouseEvent = (QMouseEvent *)event;
                    if(this->childAt(mouseEvent->pos()) == zfnull)
                    {
                        return false;
                    }
                }
                return QWidget::event(event);

            // key
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
                if(!this->_ZFP_viewUIEnableTree)
                {
                    return false;
                }
                return QWidget::event(event);

            // wheel
            case QEvent::Wheel:
            {
                QWheelEvent *eventTmp = (QWheelEvent *)event;
                ZFCACHEABLE_ACCESS(ZFUIWheelEvent, ZFUIWheelEvent, wheelEvent);
                QPoint eventSteps = eventTmp->angleDelta() / 8 / 15;
                wheelEvent->wheelX = -eventSteps.x();
                wheelEvent->wheelY = -eventSteps.y();
                if(wheelEvent->wheelX != 0 || wheelEvent->wheelY != 0)
                {
                    this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, wheelEvent);
                }
            }
                return true;

            // layout
            case QEvent::LayoutRequest:
            case QEvent::Resize:
                if(_ZFP_ownerZFUIView != zfnull
                    && this->parentWidget() != zfnull && !_ZFP_ZFUIViewImpl_sys_Qt_isImplView(this->parentWidget()))
                {
                    if(!_ZFP_layoutRequested)
                    {
                        _ZFP_layoutRequested = zftrue;
                        _ZFP_impl->notifyNeedLayout(_ZFP_ownerZFUIView);
                    }
                    if(event->type() == QEvent::LayoutRequest)
                    {
                        QCoreApplication::processEvents();
                    }
                    if(_ZFP_layoutRequested)
                    {
                        _ZFP_impl->notifyLayoutRootView(_ZFP_ownerZFUIView, ZFImpl_sys_Qt_ZFUIKit_ZFUIRectFromQRect(this->geometry()));
                        _ZFP_layoutRequested = zffalse;
                    }
                    return true;
                }
                return QWidget::event(event);

            // default
            default:
                return QWidget::event(event);
        }
    }

public:
    virtual void mousePressEvent(QMouseEvent *event)
    {
        this->_ZFP_mousePressed = zftrue;
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::mousePressEvent(event);
            return ;
        }
        this->mouseEventResolve(event, ZFUIMouseAction::e_MouseDown);
    }
    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        this->_ZFP_mouseMoveLastPoint = ZFImpl_sys_Qt_ZFUIKit_ZFUIPointFromQPoint(event->pos());
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::mouseMoveEvent(event);
            return ;
        }
        if(this->_ZFP_mousePressed)
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseMove);
        }
        else if(this->_ZFP_ownerZFUIView->viewMouseHoverEventEnable())
        {
            if(this->_ZFP_mouseEnterFlag)
            {
                this->_ZFP_mouseEnterFlag = zffalse;
                this->mouseHoverEventResolve(this->_ZFP_mouseMoveLastPoint, ZFUIMouseAction::e_MouseHoverEnter);
            }
            else
            {
                this->mouseHoverEventResolve(this->_ZFP_mouseMoveLastPoint, ZFUIMouseAction::e_MouseHover);
            }
        }
    }
    virtual void mouseReleaseEvent(QMouseEvent *event)
    {
        this->_ZFP_mousePressed = zffalse;
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::mouseReleaseEvent(event);
            return ;
        }
        _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagCheck(event);
        if(tag != zfnull && tag->cancelFlag)
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseCancel);
        }
        else
        {
            this->mouseEventResolve(event, ZFUIMouseAction::e_MouseUp);
        }
    }
    virtual void enterEvent(QEvent *event)
    {
        this->_ZFP_mouseEnterFlag = zftrue;
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::enterEvent(event);
            return ;
        }
    }
    virtual void leaveEvent(QEvent *event)
    {
        this->_ZFP_mouseEnterFlag = zffalse;
        if(_ZFP_ownerZFUIView == zfnull
            || !this->_ZFP_viewUIEnableTree || !this->_ZFP_viewUIEnable)
        {
            QWidget::leaveEvent(event);
            return ;
        }
        if(this->_ZFP_ownerZFUIView->viewMouseHoverEventEnable())
        {
            this->mouseHoverEventResolve(this->_ZFP_mouseMoveLastPoint, ZFUIMouseAction::e_MouseHoverExit);
        }
    }

private:
    void mouseEventResolve(QMouseEvent *event, ZFUIMouseActionEnum mouseAction)
    {
        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = (zfidentity)event->button();
        ev->mouseAction = mouseAction;
        ev->mousePoint = ZFImpl_sys_Qt_ZFUIKit_ZFUIPointFromQPoint(event->pos());
        switch(event->button())
        {
            case Qt::RightButton:
                ev->mouseButton = ZFUIMouseButton::e_RightButton;
                break;
            case Qt::MiddleButton:
                ev->mouseButton = ZFUIMouseButton::e_CenterButton;
                break;
            case Qt::LeftButton:
            default:
                ev->mouseButton = ZFUIMouseButton::e_LeftButton;
                break;
        }

        this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, ev);
    }
    void mouseHoverEventResolve(const ZFUIPoint &pos, ZFUIMouseActionEnum mouseAction)
    {
        ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, ev);
        ev->mouseId = 0;
        ev->mouseAction = mouseAction;
        ev->mousePoint = pos;
        ev->mouseButton = ZFUIMouseButton::e_LeftButton;
        this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, ev);
    }

public:
    virtual void keyPressEvent(QKeyEvent *event)
    {
        if(_ZFP_ownerZFUIView == zfnull)
        {
            QWidget::keyPressEvent(event);
            return ;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? ZFUIKeyAction::e_KeyRepeat : ZFUIKeyAction::e_KeyDown);
    }
    virtual void keyReleaseEvent(QKeyEvent *event)
    {
        if(_ZFP_ownerZFUIView == zfnull)
        {
            QWidget::keyReleaseEvent(event);
            return ;
        }
        this->keyEventResolve(event, event->isAutoRepeat() ? ZFUIKeyAction::e_KeyRepeat : ZFUIKeyAction::e_KeyUp);
    }
private:
    void keyEventResolve(QKeyEvent *event, ZFUIKeyActionEnum keyAction)
    {
        if(this->_ZFP_viewUIEnableTree && this->_ZFP_viewUIEnable)
        {
            ZFCACHEABLE_ACCESS(ZFUIKeyEvent, ZFUIKeyEvent, ev);
            ev->keyId = (zfidentity)event->key();
            ev->keyAction = keyAction;
            ev->keyCode = ZFUIViewImpl_sys_Qt_ZFUIKeyCodeFromQKeyCode(event->key());
            ev->keyCodeRaw = (zfuint32)event->key();
            this->_ZFP_impl->notifyUIEvent(this->_ZFP_ownerZFUIView, ev);

            ZFUIKeyboardStateImplNotifyKeyEvent(ev);

            event->setAccepted(ev->eventResolved());
        }
    }

protected:
    virtual bool focusNextPrevChild(bool next)
    {
        return false;
    }
};

// ============================================================
// native scroll view
class _ZFP_ZFUIViewImpl_sys_Qt_ScrollView : public _ZFP_ZFUIViewImpl_sys_Qt_View, public ZFUIScrollViewImplHelperProtocol
{
    Q_OBJECT

public:
    ZFUIScrollView *_ZFP_ownerZFUIScrollView;
    zfbool _ZFP_scrollEnable;
    zfbool _ZFP_scrollBounceHorizontal;
    zfbool _ZFP_scrollBounceVertical;
    zfbool _ZFP_scrollBounceHorizontalAlways;
    zfbool _ZFP_scrollBounceVerticalAlways;
public:
    ZFUIScrollViewImplHelper _ZFP_scrollViewImplHelper;
    QTimer _ZFP_scrollAnimationTimer;
    zfindex _ZFP_scrollViewBgViewCount;
    _ZFP_ZFUIViewImpl_sys_Qt_View *_ZFP_scrollViewContentView;

public:
    _ZFP_ZFUIViewImpl_sys_Qt_ScrollView(void)
    : _ZFP_ZFUIViewImpl_sys_Qt_View()
    , _ZFP_ownerZFUIScrollView(zfnull)
    , _ZFP_scrollEnable(zftrue)
    , _ZFP_scrollBounceHorizontal(zftrue)
    , _ZFP_scrollBounceVertical(zftrue)
    , _ZFP_scrollBounceHorizontalAlways(zffalse)
    , _ZFP_scrollBounceVerticalAlways(zffalse)
    , _ZFP_scrollAnimationTimer()
    , _ZFP_scrollViewBgViewCount(0)
    , _ZFP_scrollViewContentView(zfnull)
    {
        _ZFP_scrollViewContentView = new _ZFP_ZFUIViewImpl_sys_Qt_View();
        _ZFP_layoutProxy.childAdd(zfnull, _ZFP_scrollViewContentView, 0, ZFUIViewChildLayer::e_Background);

        QCoreApplication::instance()->installEventFilter(this);
    }
    ~_ZFP_ZFUIViewImpl_sys_Qt_ScrollView(void)
    {
        QCoreApplication::instance()->removeEventFilter(this);

        zfCoreAssert(_ZFP_scrollViewBgViewCount == 0);
        _ZFP_layoutProxy.childRemove(0, ZFUIViewChildLayer::e_Background);
        delete _ZFP_scrollViewContentView;
    }

public:
    void _ZFP_scrollContentFrameSet(ZF_IN const ZFUIRect &value)
    {
        _ZFP_scrollViewContentView->_ZFP_frameSet(value);
    }
    void _ZFP_scrollBgAdd(ZF_IN ZFUIView *ownerZFUIView, ZF_IN QWidget *view, ZF_IN zfindex atVirtualIndex)
    {
        ++_ZFP_scrollViewBgViewCount;
        _ZFP_layoutProxy.childAdd(ownerZFUIView, view, atVirtualIndex, ZFUIViewChildLayer::e_Background);
    }
    void _ZFP_scrollBgRemove(ZF_IN zfindex atVirtualIndex)
    {
        --_ZFP_scrollViewBgViewCount;
        _ZFP_layoutProxy.childRemove(atVirtualIndex, ZFUIViewChildLayer::e_Background);
    }
    void _ZFP_scrollChildAdd(ZF_IN ZFUIView *ownerZFUIView, ZF_IN QWidget *view, ZF_IN zfindex atVirtualIndex)
    {
        _ZFP_scrollViewContentView->_ZFP_layoutProxy.childAdd(ownerZFUIView, view, atVirtualIndex, ZFUIViewChildLayer::e_Normal);
    }
    void _ZFP_scrollChildRemove(ZF_IN zfindex atVirtualIndex)
    {
        _ZFP_scrollViewContentView->_ZFP_layoutProxy.childRemove(atVirtualIndex, ZFUIViewChildLayer::e_Normal);
    }
    void _ZFP_scrollFgAdd(ZF_IN ZFUIView *ownerZFUIView, ZF_IN QWidget *view, ZF_IN zfindex atVirtualIndex)
    {
        _ZFP_layoutProxy.childAdd(ownerZFUIView, view, _ZFP_scrollViewBgViewCount + 1 + atVirtualIndex, ZFUIViewChildLayer::e_Foreground);
    }
    void _ZFP_scrollFgRemove(ZF_IN zfindex atVirtualIndex)
    {
        _ZFP_layoutProxy.childRemove(_ZFP_scrollViewBgViewCount + 1 + atVirtualIndex, ZFUIViewChildLayer::e_Foreground);
    }

public slots:
    void _ZFP_scrollAnimationTimerOnActivate(void)
    {
        this->_ZFP_impl->notifyScrollViewScrollAnimation(this->_ZFP_ownerZFUIScrollView, _ZFP_ZFUIViewImpl_sys_Qt_timestamp());
    }

    // ============================================================
    // scroll drag impl
protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        QWidget *child = qobject_cast<QWidget *>(obj);
        if(child == zfnull || child->window() != this->window())
        {
            return false;
        }
        ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel;
        switch(event->type())
        {
            case QEvent::MouseButtonPress:
                mouseAction = ZFUIMouseAction::e_MouseDown;
                break;
            case QEvent::MouseMove:
                mouseAction = ZFUIMouseAction::e_MouseMove;
                break;
            case QEvent::MouseButtonRelease:
                mouseAction = ZFUIMouseAction::e_MouseUp;
                break;
            default:
                return false;
        }

        QWidget *parentScrollView = this->parentWidget();
        while(parentScrollView != NULL)
        {
            if(qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *>(parentScrollView) != NULL)
            {
                break;
            }
            parentScrollView = parentScrollView->parentWidget();
        }

        _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagData *tag = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagCheck(event);
        if(parentScrollView != NULL && (tag == zfnull || !tag->forwardedFlag.contains(parentScrollView)))
        {
            return false;
        }
        if(tag != zfnull && tag->forwardedFlag.contains(this))
        {
            return false;
        }
        zfint xOffset = 0;
        zfint yOffset = 0;
        if(!_ZFP_ZFUIViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, child))
        {
            return false;
        }

        QMouseEvent *e = (QMouseEvent *)event;

        QWidget *touchedFgView = this->_ZFP_findFgView(e->localPos().x() + xOffset, e->localPos().y() + yOffset);
        if(touchedFgView != zfnull)
        {
            // cloned even if no extra processing
            // to ensure tag map would be cleaned to the event
            QMouseEvent *t = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(e->localPos().x() + xOffset, e->localPos().y() + yOffset));

            QEvent *eTmp = (QEvent *)this->translateFromParentToChild(touchedFgView, t, t->localPos().x() + xOffset, t->localPos().y() + yOffset);
            _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagAccess(eTmp)->forwardedFlag.insert(this);
            QCoreApplication::instance()->sendEvent(touchedFgView, eTmp);
            this->mouseEventCleanup(eTmp);
            return true;
        }

        zfRetainWithoutLeakTest(this->_ZFP_ownerZFUIScrollView);
        zfblockedReleaseWithoutLeakTest(this->_ZFP_ownerZFUIScrollView);

        // cloned even if no extra processing
        // to ensure tag map would be cleaned to the event
        QMouseEvent *t = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(e->localPos().x() + xOffset, e->localPos().y() + yOffset));
        _ZFP_scrollViewImplHelper.interceptMouse(t, mouseAction);
        this->mouseEventCleanup(t);
        return true;
    }
public:
    virtual zftimet nativeTime(void)
    {
        return _ZFP_ZFUIViewImpl_sys_Qt_timestamp();
    }
    virtual zfint mouseEventGetX(ZF_IN void *nativeMouseEvent)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        return (zfint)e->x();
    }
    virtual zfint mouseEventGetY(ZF_IN void *nativeMouseEvent)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        return (zfint)e->y();
    }
    virtual void *mouseEventClone(ZF_IN void *nativeMouseEvent,
                                  ZF_IN_OPT zfbool changeMouseAction = zffalse,
                                  ZF_IN_OPT ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        QEvent::Type type = e->type();
        if(changeMouseAction)
        {
            switch(mouseAction)
            {
                case ZFUIMouseAction::e_MouseDown:
                    type = QEvent::MouseButtonPress;
                    break;
                case ZFUIMouseAction::e_MouseMove:
                    type = QEvent::MouseMove;
                    break;
                case ZFUIMouseAction::e_MouseUp:
                    type = QEvent::MouseButtonRelease;
                    break;
                case ZFUIMouseAction::e_MouseCancel:
                    type = QEvent::MouseButtonRelease;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfnull;
            }
        }
        QMouseEvent *ret = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventClone(e, type, e->localPos());
        if(changeMouseAction && mouseAction == ZFUIMouseAction::e_MouseCancel)
        {
            _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagAccess(ret)->cancelFlag = zftrue;
        }
        return ret;
    }
    virtual void mouseEventCleanup(ZF_IN void *nativeMouseEvent)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagMap().remove(e);
        delete e;
    }
    virtual void mouseEventForward(ZF_IN void *nativeChild,
                                   ZF_IN void *nativeMouseEvent,
                                   ZF_IN zfbool forwardToChildScrollView)
    {
        QWidget *v = ZFCastStatic(QWidget *, nativeChild);
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);

        QWidget *parent = (forwardToChildScrollView ? v->parentWidget() : v);
        while(parent != NULL)
        {
            if(qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *>(parent) != NULL)
            {
                _ZFP_ZFUIViewImpl_sys_Qt_MouseEventTagAccess(e)->forwardedFlag.insert(parent);
            }
            parent = parent->parentWidget();
        }
        QCoreApplication::instance()->sendEvent(v, e);
    }

    QWidget *_ZFP_findFgView(ZF_IN zfint x, ZF_IN zfint y)
    {
        for(zfindex i = (zfindex)_ZFP_layoutProxy.count() - 1, iEnd = _ZFP_scrollViewBgViewCount; i != iEnd; --i)
        {
            QWidget *t = _ZFP_layoutProxy.itemAt(i)->widget();
            if(t->isEnabled() && t->geometry().contains(x, y))
            {
                return t;
            }
        }
        return zfnull;
    }
    QWidget *_ZFP_findChildRecursive(ZF_IN QWidget *parent, ZF_IN zfint x, ZF_IN zfint y, ZF_IN zfbool findScrollView)
    {
        if(findScrollView && qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *>(parent) != zfnull)
        {
            return parent;
        }
        const QObjectList &children = parent->children();
        for(int i = children.size() - 1; i != -1; --i)
        {
            QWidget *t = qobject_cast<QWidget *>(children.at(i));
            if(t != zfnull && t->isEnabled() && t->geometry().contains(x, y))
            {
                if(qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *>(t) != zfnull)
                {
                    return t;
                }
                QWidget *inner = _ZFP_findChildRecursive(t, x - t->geometry().x(), y - t->geometry().y(), findScrollView);
                if(inner != zfnull)
                {
                    return inner;
                }
                if(!findScrollView)
                {
                    return t;
                }
            }
        }
        return zfnull;
    }
    QWidget *_ZFP_findChild(ZF_IN_OUT zfindex &beforeIndex, ZF_IN zfint x, ZF_IN zfint y)
    {
        --beforeIndex;
        x -= _ZFP_scrollViewContentView->geometry().x();
        y -= _ZFP_scrollViewContentView->geometry().y();
        for( ; beforeIndex != zfindexMax; --beforeIndex)
        {
            QWidget *t = _ZFP_scrollViewContentView->_ZFP_layoutProxy.itemAt(beforeIndex)->widget();
            if(t->isEnabled() && t->geometry().contains(x, y))
            {
                return t;
            }
        }
        return zfnull;
    }
    virtual ZFUIScrollViewImplHelper *findTouchedChildScrollView(ZF_IN zfint x, ZF_IN zfint y)
    {
        zfindex beforeIndex = _ZFP_scrollViewContentView->_ZFP_layoutProxy.count();
        do
        {
            QWidget *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull)
            {
                QWidget *innerChild = _ZFP_findChildRecursive(t, x - t->geometry().x(), y - t->geometry().y(), zftrue);
                if(innerChild != zfnull)
                {
                    return &(qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *>(innerChild)->_ZFP_scrollViewImplHelper);
                }
            }
        } while(beforeIndex != zfindexMax);
        return zfnull;
    }
    virtual void findTouchedChildScrollViewCleanup(ZF_IN void *nativeChild)
    {
        // nothing to do
    }
    virtual void *findTouchedChild(ZF_IN zfint x, ZF_IN zfint y)
    {
        zfindex beforeIndex = _ZFP_scrollViewContentView->_ZFP_layoutProxy.count();
        do
        {
            QWidget *t = _ZFP_findChild(beforeIndex, x, y);
            if(t != zfnull)
            {
                if(qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *>(t) != zfnull)
                {
                    return t;
                }
                QWidget *innerChild = _ZFP_findChildRecursive(t, x - t->geometry().x(), y - t->geometry().y(), zffalse);
                if(innerChild != zfnull)
                {
                    return innerChild;
                }
                return t;
            }
        } while(beforeIndex != zfindexMax);
        return zfnull;
    }
    virtual void findTouchedChildCleanup(ZF_IN void *nativeChild)
    {
        // nothing to do
    }

    virtual void *translateFromParentToChild(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zfint const &xInParent, ZF_IN zfint const &yInParent)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        zfint xOffset = 0;
        zfint yOffset = 0;
        _ZFP_ZFUIViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, ZFCastStatic(QWidget *, nativeChild));
        QMouseEvent *ret = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(xInParent - xOffset, yInParent - yOffset));
        this->mouseEventCleanup(e);
        return ret;
    }
    virtual void *translateFromChildToParent(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zfint const &xInChild, ZF_IN zfint const &yInChild)
    {
        QMouseEvent *e = ZFCastStatic(QMouseEvent *, nativeMouseEvent);
        zfint xOffset = 0;
        zfint yOffset = 0;
        _ZFP_ZFUIViewImpl_sys_Qt_ChildOffset(xOffset, yOffset, this, ZFCastStatic(QWidget *, nativeChild));
        QMouseEvent *ret = _ZFP_ZFUIViewImpl_sys_Qt_MouseEventClone(e, e->type(), QPointF(xInChild + xOffset, yInChild + yOffset));
        this->mouseEventCleanup(e);
        return ret;
    }
};

// ============================================================
// impl
ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Qt, ZFUIView, ZFProtocolLevelSystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QWidget"))

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        if(!view->classData()->classIsSubclassOf(ZFUIScrollView::ClassData()))
        {
            _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = new _ZFP_ZFUIViewImpl_sys_Qt_View();
            nativeView->_ZFP_impl = this;
            nativeView->_ZFP_ownerZFUIView = view;
            nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(nativeView->_ZFP_ownerZFUIView, nativeView, zfnull);
            return nativeView;
        }
        else
        {
            _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeView = new _ZFP_ZFUIViewImpl_sys_Qt_ScrollView();
            nativeView->_ZFP_impl = this;
            nativeView->_ZFP_ownerZFUIView = view;
            nativeView->_ZFP_ownerZFUIScrollView = view->to<ZFUIScrollView *>();
            nativeView->_ZFP_scrollViewImplHelper.implProtocol = nativeView;
            nativeView->_ZFP_scrollViewImplHelper.scrollView = nativeView->_ZFP_ownerZFUIScrollView;
            nativeView->_ZFP_focusProxyToken = _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_setup(nativeView->_ZFP_ownerZFUIView, nativeView, zfnull);
            return nativeView;
        }
    }
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, nativeView);
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_cleanup(nativeViewTmp->_ZFP_focusProxyToken);
        delete nativeViewTmp;
    }

    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView)
    {
        ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView())->_ZFP_nativeImplViewSet(ZFCastStatic(QWidget*, nativeImplView));
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        return 1;
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        return 1;
    }

    // ============================================================
    // properties
public:
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View  *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        // set to visible when no parent would cause QWidget changed to a window
        // for this case, we would delay until added to parent
        if(nativeView->parentWidget() != zfnull)
        {
            nativeView->setVisible(viewVisible);
        }
    }
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        if(viewAlpha == 1)
        {
            nativeViewTmp->setGraphicsEffect(zfnull);
        }
        else
        {
            QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect *>(nativeViewTmp->graphicsEffect());
            if(effect == zfnull)
            {
                effect = new QGraphicsOpacityEffect(nativeViewTmp);
                nativeViewTmp->setGraphicsEffect(effect);
            }
            effect->setOpacity(viewAlpha);
        }
    }
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_viewUIEnableSet(viewUIEnable);
    }
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_viewUIEnableTreeSet(viewUIEnableTree);
    }
    virtual void viewMouseHoverEventEnableSet(ZF_IN ZFUIView *view,
                                              ZF_IN zfbool viewMouseHoverEventEnable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->setMouseTracking(viewMouseHoverEventEnable);
    }
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        QPalette palette = nativeViewTmp->palette();
        palette.setColor(QPalette::Background, ZFImpl_sys_Qt_ZFUIKit_ZFUIColorToQColor(viewBackgroundColor));
        nativeViewTmp->setPalette(palette);
    }

public:
    virtual void scrollViewScrollEnableSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollEnable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *, scrollView->nativeView());
        nativeViewTmp->_ZFP_scrollEnable = scrollEnable;
    }
    virtual void scrollViewScrollBounceSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollBounceHorizontal,
                                           ZF_IN zfbool scrollBounceVertical,
                                           ZF_IN zfbool scrollBounceHorizontalAlways,
                                           ZF_IN zfbool scrollBounceVerticalAlways)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *, scrollView->nativeView());
        nativeViewTmp->_ZFP_scrollBounceHorizontal = scrollBounceHorizontal;
        nativeViewTmp->_ZFP_scrollBounceVertical = scrollBounceVertical;
        nativeViewTmp->_ZFP_scrollBounceHorizontalAlways = scrollBounceHorizontalAlways;
        nativeViewTmp->_ZFP_scrollBounceVerticalAlways = scrollBounceVerticalAlways;
    }
    virtual void scrollViewScrollContentFrameSet(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN const ZFUIRect &frame)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *, scrollView->nativeView());
        nativeViewTmp->_ZFP_scrollContentFrameSet(frame);
    }
    virtual zftimet scrollViewScrollAnimationStart(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *, scrollView->nativeView());
        nativeViewTmp->_ZFP_scrollAnimationTimer.connect(
            &(nativeViewTmp->_ZFP_scrollAnimationTimer), SIGNAL(timeout()),
            nativeViewTmp, SLOT(_ZFP_scrollAnimationTimerOnActivate()));
        nativeViewTmp->_ZFP_scrollAnimationTimer.moveToThread(QCoreApplication::instance()->thread());
        nativeViewTmp->_ZFP_scrollAnimationTimer.start(30);
        return _ZFP_ZFUIViewImpl_sys_Qt_timestamp();
    }
    virtual void scrollViewScrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *, scrollView->nativeView());
        nativeViewTmp->_ZFP_scrollAnimationTimer.disconnect();
        nativeViewTmp->_ZFP_scrollAnimationTimer.stop();
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
            _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, parent->nativeView());
            QWidget *nativeChildView = ZFCastStatic(QWidget *, child->nativeView());
            nativeView->_ZFP_layoutProxy.childAdd(child, nativeChildView, atIndex, childLayer);
        }
        else
        {
            _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *, parent->nativeView());
            QWidget *nativeChildView = ZFCastStatic(QWidget *, child->nativeView());
            switch(childLayer)
            {
                case ZFUIViewChildLayer::e_Background:
                    nativeScrollView->_ZFP_scrollBgAdd(child, nativeChildView, childLayerIndex);
                    break;
                case ZFUIViewChildLayer::e_Normal:
                    nativeScrollView->_ZFP_scrollChildAdd(child, nativeChildView, childLayerIndex);
                    break;
                case ZFUIViewChildLayer::e_Foreground:
                    nativeScrollView->_ZFP_scrollFgAdd(child, nativeChildView, childLayerIndex);
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
            _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, parent->nativeView());
            nativeView->_ZFP_layoutProxy.childRemove(atIndex, childLayer);
        }
        else
        {
            _ZFP_ZFUIViewImpl_sys_Qt_ScrollView *nativeScrollView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_ScrollView *, parent->nativeView());
            switch(childLayer)
            {
                case ZFUIViewChildLayer::e_Background:
                    nativeScrollView->_ZFP_scrollBgRemove(childLayerIndex);
                    break;
                case ZFUIViewChildLayer::e_Normal:
                    nativeScrollView->_ZFP_scrollChildRemove(childLayerIndex);
                    break;
                case ZFUIViewChildLayer::e_Foreground:
                    nativeScrollView->_ZFP_scrollFgRemove(childLayerIndex);
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
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeViewTmp = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        nativeViewTmp->_ZFP_frameSet(rect);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        QWidget *nativeViewTmp = ZFCastStatic(QWidget *, view->nativeView());
        while(nativeViewTmp != zfnull)
        {
            if(nativeViewTmp->layout() != zfnull)
            {
                nativeViewTmp->layout()->invalidate();
            }
            nativeViewTmp = nativeViewTmp->parentWidget();
        }
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
    {
        QWidget *nativeViewTmp = ZFCastStatic(QWidget *, nativeView);
        QSize maxSizeSaved = nativeViewTmp->maximumSize();
        if(sizeHint.width >= 0)
        {
            nativeViewTmp->setMaximumWidth(sizeHint.width);
        }
        if(sizeHint.height >= 0)
        {
            nativeViewTmp->setMaximumWidth(sizeHint.height);
        }
        QSize t = nativeViewTmp->sizeHint();
        nativeViewTmp->setMaximumSize(maxSizeSaved);
        ret = ZFUIViewLayoutParam::sizeHintApply(ZFImpl_sys_Qt_ZFUIKit_ZFUISizeFromQSize(t), sizeHint, sizeParam);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_Qt)

// ============================================================
// ZFUIViewFocus
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewFocusImpl_sys_Qt, ZFUIViewFocus, ZFProtocolLevelSystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:QWidget"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("Qt:QWidget"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewFocusableSet(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocusable)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusableSet(nativeView->_ZFP_focusProxyToken, viewFocusable);
    }
    virtual zfbool viewFocused(ZF_IN ZFUIView *view)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        return _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocused(nativeView->_ZFP_focusProxyToken);
    }
    virtual void viewFocusRequest(ZF_IN ZFUIView *view,
                                  ZF_IN zfbool viewFocus)
    {
        _ZFP_ZFUIViewImpl_sys_Qt_View *nativeView = ZFCastStatic(_ZFP_ZFUIViewImpl_sys_Qt_View *, view->nativeView());
        _ZFP_ZFUIViewImpl_sys_Qt_FocusProxy_viewFocusRequest(nativeView->_ZFP_focusProxyToken, viewFocus);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewFocusImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewFocusImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
static zfbool _ZFP_ZFUIViewImpl_sys_Qt_isImplView(QWidget *obj)
{
    return (qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_View *>(obj) != zfnull);
}

// ============================================================
// layout
_ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::~_ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy(void)
{
    if(!_ZFP_children.isEmpty())
    {
        zfCoreCriticalErrorPrepare();
        zfLogTrimT() << zfText("[ZFUIView] these views has not been removed when parent dealloc:");
        for(zfindex i = 0; i < _ZFP_children.count(); ++i)
        {
            QWidget *v = _ZFP_children.get(i).widget;
            zfLogTrimT() << zfText(" ") << ZFImpl_sys_Qt_ZFUIKit_QWidgetGetViewInfo(v);
        }
        zfCoreCriticalError();
    }
}

void _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::childAdd(ZF_IN ZFUIView *ownerZFUIViewOrNull,
                                                    ZF_IN QWidget *widget,
                                                    ZF_IN zfindex atIndex,
                                                    ZF_IN ZFUIViewChildLayerEnum childLayer)
{
    if(atIndex != zfindexMax && childLayer != ZFUIViewChildLayer::e_Background && this->_ZFP_owner->_ZFP_nativeImplView != zfnull)
    {
        ++atIndex;
    }
    zfindex oldCount = _ZFP_children.count();
    this->addWidget(widget);
    if(ownerZFUIViewOrNull != zfnull && ownerZFUIViewOrNull->viewVisible() != widget->isVisible())
    {
        widget->setVisible(ownerZFUIViewOrNull->viewVisible());
    }
    widget->update();
    if(atIndex < oldCount)
    {
        _ZFP_children.move(oldCount, atIndex);
        for(zfindex i = atIndex + 1; i < _ZFP_children.count(); ++i)
        {
            _ZFP_children[i].widget->raise();
        }
    }
}
void _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::childRemove(ZF_IN zfindex atIndex,
                                                       ZF_IN ZFUIViewChildLayerEnum childLayer)
{
    if(childLayer != ZFUIViewChildLayer::e_Background && this->_ZFP_owner->_ZFP_nativeImplView != zfnull)
    {
        ++atIndex;
    }
    if(atIndex < _ZFP_children.count())
    {
        QWidget *t = _ZFP_children[atIndex].widget;
        this->removeWidget(t);
        t->setParent(NULL);
    }
}

void _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::addItem(QLayoutItem *layoutItem)
{
    _ZFP_ZFUIViewImpl_sys_Qt_ItemHolder item;
    item.widget = layoutItem->widget();
    item.layoutItem = layoutItem;
    _ZFP_children.add(item);
}
QSize _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::sizeHint(void) const
{
    return this->_ZFP_owner->_ZFP_frame.size();
}
QSize _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::minimumSize(void) const
{
    return QSize(0, 0);
}
void _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    for(zfindex i = 0; i < _ZFP_children.count(); ++i)
    {
        const _ZFP_ZFUIViewImpl_sys_Qt_ItemHolder &item = _ZFP_children[i];
        _ZFP_ZFUIViewImpl_sys_Qt_View *view = qobject_cast<_ZFP_ZFUIViewImpl_sys_Qt_View *>(item.widget);
        if(view != zfnull && view != this->_ZFP_owner->_ZFP_nativeImplView)
        {
            if(view->geometry() != view->_ZFP_frame)
            {
                view->setGeometry(view->_ZFP_frame);
            }
        }
        else if(item.widget == this->_ZFP_owner->_ZFP_nativeImplView)
        {
            ZFUIRect nativeImplViewRect;
            this->_ZFP_owner->_ZFP_impl->notifyLayoutNativeImplView(this->_ZFP_owner->_ZFP_ownerZFUIView, nativeImplViewRect);
            QRect t = ZFImpl_sys_Qt_ZFUIKit_ZFUIRectToQRect(nativeImplViewRect);
            if(item.layoutItem->geometry() != t)
            {
                item.layoutItem->setGeometry(t);
            }
        }
        else
        {
            QRect t;
            t.setSize(_ZFP_owner->_ZFP_frame.size());
            if(item.layoutItem->geometry() != t)
            {
                item.layoutItem->setGeometry(t);
            }
        }
    }
}

QLayoutItem *_ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::itemAt(int index) const
{
    if(index < (int)_ZFP_children.count())
    {
        return _ZFP_children[index].layoutItem;
    }
    return zfnull;
}
QLayoutItem *_ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::takeAt(int index)
{
    if(index < (int)_ZFP_children.count())
    {
        QLayoutItem *ret = _ZFP_children[index].layoutItem;
        _ZFP_children.remove(index);
        return ret;
    }
    return zfnull;
}
int _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::indexOf(QWidget *widget) const
{
    for(zfindex i = 0; i < _ZFP_children.count(); ++i)
    {
        if(_ZFP_children[i].widget == widget)
        {
            return (int)i;
        }
    }
    return -1;
}
int _ZFP_ZFUIViewImpl_sys_Qt_LayoutProxy::count() const
{
    return (int)_ZFP_children.count();
}

#include "ZFProtocolZFUIView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt


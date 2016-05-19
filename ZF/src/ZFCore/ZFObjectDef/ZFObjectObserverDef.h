/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFObjectObserverDef.h
 * @brief observer types for #ZFObject
 */

#ifndef _ZFI_ZFObjectObserverDef_h_
#define _ZFI_ZFObjectObserverDef_h_

#include "../ZFCoreDef.h"
#include "ZFCallbackDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief listener data used by #ZFListener
 *
 * @note this object only hold the necessary datas as pointer,
 *   without auto retain or release logic
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFListenerData
{
public:
    /**
     * @brief event id, may be #zfidentityInvalid
     */
    zfidentity eventId;
    /**
     * @brief who notified the listener event, may be null
     */
    ZFObject *sender;
    /**
     * @brief params, may be null
     */
    ZFObject *param0;
    /**
     * @brief params, may be null
     */
    ZFObject *param1;

public:
    /**
     * @brief main constructor
     */
    explicit ZFListenerData(void)
    : eventId(zfidentityInvalid)
    , sender(zfnull)
    , param0(zfnull)
    , param1(zfnull)
    {
    }
    /**
     * @brief construct with sender and params
     */
    explicit ZFListenerData(ZF_IN zfidentity eventId,
                            ZF_IN ZFObject *sender,
                            ZF_IN_OPT ZFObject *param0 = zfnull,
                            ZF_IN_OPT ZFObject *param1 = zfnull)
    : eventId(eventId)
    , sender(sender)
    , param0(param0)
    , param1(param1)
    {
    }
    /**
     * @brief construct with another data
     */
    explicit ZFListenerData(ZF_IN const ZFListenerData &ref)
    : eventId(ref.eventId)
    , sender(ref.sender)
    , param0(ref.param0)
    , param1(ref.param1)
    {
    }
public:
    /** @cond ZFPrivateDoc */
    ZFListenerData &operator = (ZF_IN const ZFListenerData &ref)
    {
        this->eventId = ref.eventId;
        this->sender = ref.sender;
        this->param0 = ref.param0;
        this->param1 = ref.param1;
        return *this;
    }
    /** @endcond */
public:
    /**
     * @brief get a short info of this object
     */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /**
     * @brief util method to set #eventId
     */
    ZFListenerData &eventIdSet(ZF_IN zfidentity eventId)
    {
        this->eventId = eventId;
        return *this;
    }
    /**
     * @brief util method to set #sender (no auto retain)
     */
    ZFListenerData &senderSet(ZF_IN ZFObject *sender)
    {
        this->sender = sender;
        return *this;
    }
    /**
     * @brief util method to set #param0 (no auto retain)
     */
    ZFListenerData &param0Set(ZF_IN ZFObject *param0)
    {
        this->param0 = param0;
        return *this;
    }
    /**
     * @brief util method to set #param1 (no auto retain)
     */
    ZFListenerData &param1Set(ZF_IN ZFObject *param1)
    {
        this->param1 = param1;
        return *this;
    }
};
/** @cond ZFPrivateDoc */
inline zfbool operator == (ZF_IN const ZFListenerData &v0,
                           ZF_IN const ZFListenerData &v1)
{
    return (v0.eventId == v1.eventId && v0.sender == v1.sender && v0.param0 == v1.param0 && v0.param1 == v1.param1);
}
inline zfbool operator != (ZF_IN const ZFListenerData &v0,
                           ZF_IN const ZFListenerData &v1)
{
    return !(v0 == v1);
}
/** @endcond */
ZFCOMPARER_DEFAULT_DECLARE_BEGIN(ZFListenerData, e0, ZFListenerData, e1)
{
    return ((e0 == e1) ? ZFCompareTheSame : ZFCompareUncomparable);
}
ZFCOMPARER_DEFAULT_DECLARE_END(ZFListenerData, e0, ZFListenerData, e1)
/**
 * @brief listener as ZFCallback, mostly used by #ZFObject::observerNotify
 *
 * listeners usually achieved by observer logic in ZFObject,
 * see #ZFObject::observerNotify for more info\n
 * \n
 * what params means, is determined by invoker of the listener,
 * see each listener's comments before use it
 */
ZFCALLBACK_DECLARE_BEGIN(ZFListener, ZFCallback)
public:
    /** @brief see #ZFListener */
    inline void execute(ZF_IN const ZFListenerData &listenerData,
                        ZF_IN_OPT ZFObject *userData = zfnull) const
    {
        ZFCallback::executeExact<void, const ZFListenerData &, ZFObject *>(listenerData, userData);
    }
    /** @brief see #ZFListener */
    inline void operator()(ZF_IN const ZFListenerData &listenerData,
                           ZF_IN_OPT ZFObject *userData = zfnull) const
    {
        ZFCallback::executeExact<void, const ZFListenerData &, ZFObject *>(listenerData, userData);
    }
ZFCALLBACK_DECLARE_END(ZFListener, ZFCallback)

// ============================================================
// observer
extern ZF_ENV_EXPORT const zfidentity *_ZFP_ZFObserverEventIdRegister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN const zfchar *name);
extern ZF_ENV_EXPORT void _ZFP_ZFObserverEventIdUnregister(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN zfidentity eventId);
/**
 * @brief see #ZFOBSERVER_EVENT
 *
 * get name from event id, or null if no such event id
 */
extern ZF_ENV_EXPORT const zfchar *ZFObserverEventGetName(ZF_IN const zfidentity &eventId);
/**
 * @brief see #ZFOBSERVER_EVENT
 *
 * get event id from name, or zfidentityInvalid if no such event name
 */
extern ZF_ENV_EXPORT zfidentity ZFObserverEventGetId(ZF_IN const zfchar *name);

zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFObserverEventUnregisterHolder
{
public:
    _ZFP_ZFObserverEventUnregisterHolder(ZF_IN zfbool *ZFCoreLibDestroyFlag, ZF_IN zfidentity identity)
    : ZFCoreLibDestroyFlag(ZFCoreLibDestroyFlag)
    , identity(identity)
    {
    }
    ~_ZFP_ZFObserverEventUnregisterHolder(void)
    {
        if(!*(this->ZFCoreLibDestroyFlag))
        {
            _ZFP_ZFObserverEventIdUnregister(this->ZFCoreLibDestroyFlag, this->identity);
        }
    }

private:
    zfbool *ZFCoreLibDestroyFlag;
    zfidentity identity;
};
/**
 * @brief see #ZFObject::observerNotify
 *
 * usage:
 * @code
 *   zfclass YourClass ...
 *   {
 *       ZFOBSERVER_EVENT(YourEvent)
 *   };
 * @endcode
 * declare a event for ZFObject's observer logic,
 * declared event name can be accessed by:
 * @code
 *   zfidentity eventId = YourClass::EventYourEvent();
 *   const zfchar *eventName = YourClass::EventYourEvent_name();
 * @endcode
 * note that subclass may declare a event same as parent,
 * while the final event name is different:\n
 *   ParentClass::EventYourEvent() => "ParentClassEventYourEvent"\n
 *   ChildClass::EventYourEvent() => "ChildClassEventYourEvent"\n
 * \n
 * @note we declare the event id as int types for performance,
 *   it's ensured each event has different event id,
 *   but it's only ensured while app is running,
 *   after relaunching the app,
 *   the event id is not ensured the same,
 *   you should use the name of the event to store or pass between apps,
 *   and you can use #ZFObserverEventGetId or #ZFObserverEventGetName
 *   to convert them easily
 */
#define ZFOBSERVER_EVENT(YourEvent) \
    public: \
        /** \n see #ZFOBSERVER_EVENT */ \
        static zfidentity Event##YourEvent(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static const zfidentity *eventId = _ZFP_ZFObserverEventIdRegister(&ZFCoreLibDestroyFlag, zfself::Event##YourEvent##_name()); \
            static _ZFP_ZFObserverEventUnregisterHolder unregisterHolder(&ZFCoreLibDestroyFlag, *eventId); \
            return *eventId; \
        } \
        /** @brief see #ZFOBSERVER_EVENT, @ref Event##YourEvent */ \
        static const zfchar *Event##YourEvent##_name(void) \
        { \
            static zfstring _s = zfsConnectLineFree(zfself::ClassData()->className(), zfText("::Event"), zfText(#YourEvent)); \
            return _s.cString(); \
        }
/**
 * @brief declare a observer event in global scope, see #ZFOBSERVER_EVENT
 *
 * usage:
 * @code
 *   // in header files
 *   ZFOBSERVER_EVENT_GLOBAL_WITH_NAMESPACE(YourNamespace, YourEvent)
 *
 *   // use the event
 *   zfidentity eventId = YourNamespace::EventYourEvent();
 * @endcode
 * unlike #ZFOBSERVER_EVENT, this macro would declare event outside of class scope,
 * typically you should use #ZFOBSERVER_EVENT_GLOBAL which have "ZFGlobalEvent" as namespace
 */
#define ZFOBSERVER_EVENT_GLOBAL_WITH_NAMESPACE(GlobalNamespace, YourEvent) \
    ZF_NAMESPACE_BEGIN(GlobalNamespace) \
    zfclass ZF_ENV_EXPORT _ZFP_ZFObserverEventHolder_##GlobalNamespace##_##YourEvent \
    { \
    public: \
        static zfidentity event(void) \
        { \
            static zfbool ZFCoreLibDestroyFlag = zffalse; \
            static const zfidentity *eventId = _ZFP_ZFObserverEventIdRegister( \
                &ZFCoreLibDestroyFlag, \
                _ZFP_ZFObserverEventHolder_##GlobalNamespace##_##YourEvent::name()); \
            static _ZFP_ZFObserverEventUnregisterHolder unregisterHolder(&ZFCoreLibDestroyFlag, *eventId); \
            return *eventId; \
        } \
        static const zfchar *name(void) \
        { \
            return zfText(#GlobalNamespace) zfText("::Event") zfText(#YourEvent); \
        } \
    }; \
    /** @brief see #ZFOBSERVER_EVENT */ \
    inline zfidentity Event##YourEvent(void) \
    { \
        return _ZFP_ZFObserverEventHolder_##GlobalNamespace##_##YourEvent::event(); \
    } \
    /** @brief see #ZFOBSERVER_EVENT */ \
    inline const zfchar *Event##YourEvent##_name(void) \
    { \
        return _ZFP_ZFObserverEventHolder_##GlobalNamespace##_##YourEvent::name(); \
    } \
    ZF_NAMESPACE_END(GlobalNamespace)
/**
 * @brief global event with namespace "ZFGlobalEvent", see #ZFOBSERVER_EVENT_GLOBAL_WITH_NAMESPACE
 */
#define ZFOBSERVER_EVENT_GLOBAL(YourEvent) \
    ZFOBSERVER_EVENT_GLOBAL_WITH_NAMESPACE(ZFGlobalEvent, YourEvent)

// ============================================================
// util
/**
 * @brief util macro to cache whether a event has added to #ZFObject::observerAdd
 *
 * usage:
 * @code
 *   zfclass MyObject : zfextends ZFObject
 *   {
 *       ZFOBJECT_DECLARE(MyObject, ZFObject)
 *
 *       ZFOBSERVER_EVENT(MyEvent)
 *
 *       ZFOBSERVER_HAS_ADD_BEGIN()
 *           ZFOBSERVER_HAS_ADD_VALUE(HasAdd_MyEvent)
 *       ZFOBSERVER_HAS_ADD_SEPARATOR()
 *           ZFOBSERVER_HAS_ADD_VALUE_REGISTER(HasAdd_MyEvent, MyObject::EventMyEvent())
 *       ZFOBSERVER_HAS_ADD_END()
 *
 *   public:
 *       void func(void)
 *       {
 *           if(ZFOBSERVER_HAS_ADD(HasAdd_MyEvent))
 *           {
 *               // do something
 *           }
 *       }
 *   };
 * @endcode
 *
 * this macro would cache the state that whether the event has attached observer\n
 * #ZFObject::observerHasAdd supply same function as this macro,
 * however, #ZFObject::observerHasAdd would search the entire observer map
 * each time when it's called,
 * which may cause performance issue if called frequently
 * or if many observers attached
 */
#define ZFOBSERVER_HAS_ADD_BEGIN() \
    private: \
        /** @cond ZFPrivateDoc */ \
        typedef enum { \
            _ZFP_ZFObserverHasAddDummy = 0,
/** @brief see #ZFOBSERVER_HAS_ADD_BEGIN */
#define ZFOBSERVER_HAS_ADD_VALUE(EventEnumName) \
            EventEnumName,
/** @brief see #ZFOBSERVER_HAS_ADD_BEGIN */
#define ZFOBSERVER_HAS_ADD_SEPARATOR() \
        } _ZFP_ZFObserverHasAddFlagType; \
        zfclassLikePOD _ZFP_ZFObserverHasAddFlagHolder \
        { \
        public: \
            _ZFP_ZFObserverHasAddFlagHolder(void) : _flag(0) {} \
        public: \
            static zfflags flagForEventId(ZF_IN zfidentity eventId) \
            { \
                if(zffalse) {}
/** @brief see #ZFOBSERVER_HAS_ADD_BEGIN */
#define ZFOBSERVER_HAS_ADD_VALUE_REGISTER(EventEnumName, EventId) \
                else if(eventId == EventId) \
                { \
                    return zfself::EventEnumName; \
                }
/** @brief see #ZFOBSERVER_HAS_ADD_BEGIN */
#define ZFOBSERVER_HAS_ADD_END() \
                return 0; \
            } \
        public: \
             zfflags _flag; \
        }; \
        _ZFP_ZFObserverHasAddFlagHolder _ZFP_ZFObserverHasAddFlag; \
    protected: \
        zfoverride \
        virtual void observerOnAddFirst(ZF_IN const zfidentity &eventId) \
        { \
            zfsuper::observerOnAddFirst(eventId); \
            zfflags flag = _ZFP_ZFObserverHasAddFlagHolder::flagForEventId(eventId); \
            ZFBitSet(_ZFP_ZFObserverHasAddFlag._flag, flag); \
        } \
        zfoverride \
        virtual void observerOnRemoveLast(ZF_IN const zfidentity &eventId) \
        { \
            zfflags flag = _ZFP_ZFObserverHasAddFlagHolder::flagForEventId(eventId); \
            ZFBitUnset(_ZFP_ZFObserverHasAddFlag._flag, flag); \
            zfsuper::observerOnRemoveLast(eventId); \
        } \
        /** @endcond */ \
    public:
/** @brief see #ZFOBSERVER_HAS_ADD_BEGIN */
#define ZFOBSERVER_HAS_ADD(EventEnumName) \
    ZFBitTest(this->_ZFP_ZFObserverHasAddFlag._flag, zfself::EventEnumName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectObserverDef_h_


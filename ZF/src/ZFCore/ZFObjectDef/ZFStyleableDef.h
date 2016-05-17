/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFStyleableDef.h
 * @brief styleable
 */

#ifndef _ZFI_ZFStyleableDef_h_
#define _ZFI_ZFStyleableDef_h_

#include "ZFObjectDef.h"
#include "ZFObjectGlobalInstanceDef.h"
#include "ZFSerializableDef.h"
#include "ZFCopyableDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFStyleable
zfclassFwd _ZFP_ZFStyleable_PropertyTypeHolder;
/**
 * @brief styleable element that can apply style from different source
 *
 * a styleable typically is a serializable,
 * while serializable process data from and to #ZFSerializableData,
 * a styleable process data from and to other styleable object\n
 * \n
 * to use styleable, declare it as #ZFInterface (see #ZFInterface for more info,
 * including the limitation of interface),
 * then implements it with different inherit tree,
 * typically a functional inherit tree that achieve main function,
 * and a holder inherit tree that simply used to hold the styleable itself\n
 * here's a simple example inherit tree to use styleable:
 * @code
 *   // styleable declare
 *   zfinterface YourBaseStyleable : zfextends ZFInterface {...};
 *   zfinterface YourChildStyleable : zfextends ZFInterface {...};
 *
 *   // functional inherit tree
 *   zfclass YourBaseType : zfextends ZFObject, zfimplements ZFStyleable, zfimplements YourBaseStyleable {...};
 *   zfclass YourChildType : zfextends YourBaseType, zfimplements YourChildStyleable {...};
 *
 *   // holder inherit tree
 *   zfclass YourBaseHolder : zfextends ZFObject, zfimplements ZFStyleable, zfimplements YourBaseStyleable {...};
 *   zfclass YourChildHolder : zfextends YourBaseHolder, zfimplements YourChildStyleable {...};
 * @endcode
 * once declared, you may copy styles freely from different styleable object
 */
zfinterface ZF_ENV_EXPORT ZFStyleable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(ZFStyleable, ZFSerializable, ZFCopyable)

protected:
    /** @cond ZFPrivateDoc */
    ZFStyleable(void)
    : _ZFP_ZFStyleable_defaultStyleCache(zfnull)
    , _ZFP_ZFStyleable_propertyTypeHolder(zfnull)
    {
    }
    /** @endcond */

private:
    ZFStyleable *_ZFP_ZFStyleable_defaultStyleCache;
public:
    /**
     * @brief return default style of this instance
     */
    virtual ZFStyleable *defaultStyle(void);

public:
    /**
     * @brief copy style from another styleable, see #ZFStyleable
     *
     * usually you should not override this method,
     * override #styleableOnCopyFrom instead
     */
    zffinal void styleableCopyFrom(ZF_IN ZFStyleable *anotherStyleable);

protected:
    /** @brief see #ZFStyleable::styleableOnCheckPropertyType */
    typedef enum {
        PropertyTypeNotCopyable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeNormal, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeStyleable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
    } PropertyType;
    /**
     * @brief check property type
     *
     * property type shows how styleable would act while copying from another styleable:
     * -  PropertyTypeNotCopyable:
     *   shows the property should not be copied
     * -  PropertyTypeNormal:
     *   shows the property should be copied normally,
     *   i.e. use getter and setter to copy value
     * -  PropertyTypeStyleable:
     *   shows the property itself is styleable,
     *   we would access its value and copy style by #styleableCopyFrom
     *
     * \n
     * by default, a property is treated as not copyable if matches any of conditions below:
     * -  getter or setter method is private
     *
     * a property is treated as styleable property if matches all of conditions below:
     * -  is retain property and the type is ZFStyleable
     * -  getter method is not private and setter method is private
     *
     * all other property are treated as normal property
     */
    virtual ZFStyleable::PropertyType styleableOnCheckPropertyType(ZF_IN const ZFProperty *property);
    /**
     * @brief copy property with styleable logic
     *
     * @note anotherStyleable not ensured to be same type as self,
     *   while the property is ensured to be same and safe to copy
     */
    virtual void styleableOnCopyPropertyFrom(ZF_IN ZFStyleable *anotherStyleable,
                                             ZF_IN const ZFProperty *property,
                                             ZF_IN ZFStyleable::PropertyType propertyType);
    /**
     * @brief for subclass to achieve custom style copy step,
     *   called by #styleableCopyFrom, see #ZFStyleable
     */
    virtual inline void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
    {
    }

private:
    _ZFP_ZFStyleable_PropertyTypeHolder *_ZFP_ZFStyleable_propertyTypeHolder;
private:
    zffinal _ZFP_ZFStyleable_PropertyTypeHolder *_ZFP_ZFStyleable_getPropertyTypeHolder(void);
};

// ============================================================
// ZFStyleableHolder
/**
 * @brief used to hold style as ZFObject
 */
zfclass ZF_ENV_EXPORT ZFStyleableHolder : zfextends ZFObject, zfimplements ZFStyleable, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFStyleableHolder, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFStyleable, ZFSerializable, ZFCopyable)

public:
    /**
     * @brief compare with another styleable holder object
     *
     * by default,
     * a styleable holder object would be compared by comparing all of its property,
     * which may consume much CPU time,
     * override if you want different impl
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->styleableCopyFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }
};

// ============================================================
// ZFStyleableObject
/**
 * @brief styleable object
 */
zfclass ZF_ENV_EXPORT ZFStyleableObject : zfextends ZFObject, zfimplements ZFStyleable, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFStyleableObject, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFStyleable, ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->styleableCopyFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }
};

// ============================================================
// ZFStyleableHolder
#define _ZFP_ZFStyleableDefault_level ZFLevelZFFrameworkNormal
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultPointerHolder
{
public:
    void *d;
    _ZFP_ZFStyleableDefaultPointerHolder(void) : d(zfnull) {}
};

extern ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultPointerHolder *_ZFP_ZFStyleableDefaultRefAccess(ZF_IN const zfchar *name);
typedef void (*_ZFP_ZFStyleableDefaultDeleteCallback)(ZF_IN void *instance);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultDeleteCallbackHolder
{
public:
    _ZFP_ZFStyleableDefaultDeleteCallbackHolder(ZF_IN _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback,
                                                ZF_IN void *instance)
    : deleteCallback(deleteCallback)
    , instance(instance)
    {
    }
    ~_ZFP_ZFStyleableDefaultDeleteCallbackHolder(void)
    {
        if(this->deleteCallback && this->instance)
        {
            this->deleteCallback(this->instance);
        }
    }
private:
    _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback;
    void *instance;
};

#define _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle, YourStyleHolder, ParentStyle) \
    public: \
        /** \n default style for @ref YourStyle */ \
        static YourStyle *DefaultStyle(void); \
        /** @brief default style for @ref YourStyle (reflectable) */ \
        ZFMETHOD_MEMBER_DECLARE_STATIC_0(ZFStyleable *, DefaultStyleReflect); \
    private: \
        static void _ZFP_ZFStyleable_DefaultStyleSet(ZF_IN YourStyle *newInstance); \
        static ZFCorePointerBase *&_ZFP_ZFStyleableDefaultCleaner(void); \
        static void _ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance); \
    public:
#define _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle, YourStyleHolder, ParentStyle) \
    YourStyle *YourStyle::DefaultStyle(void) \
    { \
        static _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFMACRO_TOSTRING(YourStyle)); \
        if(holder->d == zfnull) \
        { \
            if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) == ZFFrameworkStateAvailable) \
            { \
                zfautoObject obj = ZFClass::newInstanceForNameWithLeakTest(ZFMACRO_TOSTRING(YourStyleHolder), \
                                                                           ZF_CALLER_FILE, zfTextA("DefaultStyle"), ZF_CALLER_LINE); \
                if(obj != zfautoObjectNull && obj.toObject()->classData()->classIsTypeOf(YourStyle::ClassData())) \
                { \
                    zfself::_ZFP_ZFStyleable_DefaultStyleSet(ZFCastZFObjectUnchecked(YourStyle *, obj.toObject())); \
                } \
            } \
        } \
        return ZFCastStatic(YourStyle *, holder->d); \
    } \
    ZFMETHOD_MEMBER_DEFINE_STATIC_0(YourStyle, ZFStyleable *, DefaultStyleReflect) \
    { \
        return ZFCastZFObjectUnchecked(ZFStyleable *, zfself::DefaultStyle()); \
    } \
    void YourStyle::_ZFP_ZFStyleable_DefaultStyleSet(ZF_IN YourStyle *newInstance) \
    { \
        if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) != ZFFrameworkStateAvailable) \
        { \
            return ; \
        } \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFMACRO_TOSTRING(YourStyle)); \
        if(holder->d == newInstance) \
        { \
            return ; \
        } \
        ZFCorePointerBase *&cleanerRef = _ZFP_ZFStyleableDefaultCleaner(); \
        ZFCorePointerBase *cleanerOld = cleanerRef; \
        cleanerRef = zfnull; \
        if(newInstance != zfnull) \
        { \
            holder->d = zfRetainWithLeakTest(newInstance); \
            newInstance->toObject()->objectCachedSet(zftrue); \
            cleanerRef = ZFObjectGlobalInstanceAdd(ZFCorePointerForObject<_ZFP_ZFStyleableDefaultDeleteCallbackHolder *>( \
                zfnew(_ZFP_ZFStyleableDefaultDeleteCallbackHolder, YourStyle::_ZFP_ZFStyleableDefaultOnDelete, holder->d)), \
                _ZFP_ZFStyleableDefault_level); \
        } \
        if(cleanerOld != zfnull) \
        { \
            ZFObjectGlobalInstanceRemove(cleanerOld, _ZFP_ZFStyleableDefault_level); \
        } \
    } \
    ZFCorePointerBase *&YourStyle::_ZFP_ZFStyleableDefaultCleaner(void) \
    { \
        static ZFCorePointerBase *_cleaner = zfnull; \
        return _cleaner; \
    } \
    void YourStyle::_ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance) \
    { \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFMACRO_TOSTRING(YourStyle)); \
        if(holder->d == instance) \
        { \
            holder->d = zfnull; \
        } \
        YourStyle *tmp = ZFCastStatic(YourStyle *, instance); \
        tmp->toObject()->objectCachedSet(zffalse); \
        zfReleaseWithLeakTest(tmp); \
    }

/**
 * @brief used to declare a default style, see #ZFStyleable
 *
 * usage:
 * @code
 *   // in h file
 *   zfinterface YourObjectStyle : zfextends ZFInterface
 *   {
 *       ZFINTERFACE_DECLARE(YourObjectStyle, ParentStyle)
 *       ZFSTYLE_DEFAULT_DECLARE(YourObjectStyle, ParentStyle)
 *   };
 *   ZFSTYLE_DEFAULT_HOLDER_DECLARE(YourObjectStyle, ParentStyle)
 *
 *   zfclass YourObject : zfextends Parent, zfimplements YourObjectStyle
 *   {
 *       ZFOBJECT_DECLARE(YourObject, Parent)
 *       ZFIMPLEMENTS_DECLARE(YourObjectStyle)
 *   };
 * @endcode
 * the macros above declare these types for you:
 * -  an object named YourObjectStyleHolder
 * -  default style access method:
 *   static YourObjectStyle *DefaultStyle(void);
 *   virtual ZFStyleable *defaultStyle(void);
 * -  default style access method as ZFMethod (reflectable):
 *   static ZFStyleable *DefaultStyleReflect(void);
 *
 * \n
 * @note a style holder object would be created automatically when access the default style,
 *   and would be deleted automatically during #ZFFrameworkCleanup with level #ZFLevelZFFrameworkNormal,
 *   so it would result null if you access DefaultStyle before initialized or after it's deallocated
 * @note it is useful to combine styleable logic and serializable logic, typical usage:
 *   @code
 *     <YourObject styleableType="memberMethod" styleableData="YourStyle::DefaultStyleReflect" />
 *   @endcode
 *   serializing this code would result to create an instance of YourObject,
 *   then copy style from YourStyle::DefaultStyleReflect by #ZFStyleable::styleableCopyFrom\n
 *   see #ZFObjectCreatorType_memberMethod for more info
 * @note for performance, the default style won't be copied to its instance by default,
 *   you may use #ZFClass::instanceObserverAdd to observe styleable object's
 *   instance alloc event, and apply your default styles
 */
#define ZFSTYLE_DEFAULT_DECLARE(YourStyle, ParentStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle, YourStyle##Holder, ParentStyle)
/** @brief see #ZFSTYLE_DEFAULT_DECLARE */
#define ZFSTYLE_DEFAULT_DEFINE(YourStyle, ParentStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle, YourStyle##Holder, ParentStyle) \
    ZFOBJECT_REGISTER(YourStyle##Holder) \
    ZFMETHOD_REGISTER(YourStyle, DefaultStyleReflect)
/** @brief see #ZFSTYLE_DEFAULT_DECLARE */
#define ZFSTYLE_DEFAULT_HOLDER_DECLARE_BEGIN(YourStyle, ParentStyle) \
    /** @brief see @ref YourStyle */ \
    zfclass ZF_ENV_EXPORT YourStyle##Holder : zfextends ParentStyle##Holder, zfimplements YourStyle \
    { \
        ZFOBJECT_DECLARE(YourStyle##Holder, ParentStyle##Holder) \
        ZFIMPLEMENTS_DECLARE(YourStyle)
/** @brief see #ZFSTYLE_DEFAULT_DECLARE */
#define ZFSTYLE_DEFAULT_HOLDER_DECLARE_END(YourStyle, ParentStyle) \
    };
/** @brief see #ZFSTYLE_DEFAULT_DECLARE */
#define ZFSTYLE_DEFAULT_HOLDER_DECLARE(YourStyle, ParentStyle) \
    ZFSTYLE_DEFAULT_HOLDER_DECLARE_BEGIN(YourStyle, ParentStyle) \
    ZFSTYLE_DEFAULT_HOLDER_DECLARE_END(YourStyle, ParentStyle)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStyleableDef_h_


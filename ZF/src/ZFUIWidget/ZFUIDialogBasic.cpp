/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIDialogBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIDialogButtonType)

ZFOBJECT_REGISTER(ZFUIDialogContent)

static const ZFClass *_ZFP_ZFUIDialogContentClass = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogContentClassInit, ZFLevelZFFrameworkNormal)
{
    _ZFP_ZFUIDialogContentClass = ZFUIDialogContentBasic::ClassData();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogContentClassInit)
void ZFUIDialogContentClassSet(ZF_IN const ZFClass *cls)
{
    if(cls == zfnull)
    {
        _ZFP_ZFUIDialogContentClass = ZFUIDialogContentBasic::ClassData();
    }
    else if(!cls->classIsTypeOf(ZFUIDialogContent::ClassData()))
    {
        zfCoreCriticalMessage(zfTextA("class %s not type of %s"),
            zfsCoreZ2A(cls->className()), zfsCoreZ2A(ZFUIDialogContent::ClassData()->className()));
        return ;
    }
    else
    {
        _ZFP_ZFUIDialogContentClass = cls;
    }
}
const ZFClass *ZFUIDialogContentClass(void)
{
    return _ZFP_ZFUIDialogContentClass;
}

ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogBasicStyle, ZFUIDialogStyle)

ZFPROPERTY_OVERRIDE_SETTER_DEFINE(ZFUIDialogBasic, ZFUIDialogContent *, dialogContent)
{
    if(newValue == zfnull)
    {
        zfCoreCriticalMessage(zfTextA("change dialogContent to null is not allowed"));
        return ;
    }
    else if(!newValue->classData()->classIsTypeOf(ZFUIDialogContent::ClassData()))
    {
        zfCoreCriticalMessage(zfTextA("class %s not type of %s"),
            zfsCoreZ2A(newValue->classData()->className()), zfsCoreZ2A(ZFUIDialogContent::ClassData()->className()));
        return ;
    }
    this->dialogContentSetInternal(newValue);
    this->dialogViewSet(this->dialogContent()->to<ZFUIView *>());
}

void ZFUIDialogBasic::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    this->dialogViewSet(this->dialogContent()->to<ZFUIView *>());
}

ZF_NAMESPACE_GLOBAL_END


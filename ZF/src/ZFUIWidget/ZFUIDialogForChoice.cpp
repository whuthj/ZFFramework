/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFUIDialogForChoice.h"

#include "ZFUIListView.h"
#include "ZFUIListAdapterArray.h"
#include "ZFUIButtonGroup.h"
#include "ZFUIButtonCheckBox.h"
#include "ZFUIButtonRatio.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFENUM_DEFINE(ZFUIDialogForChoiceMode)
ZFSTYLE_DEFAULT_DEFINE(ZFUIDialogForChoiceStyle, ZFUIDialogStyle)

// ============================================================
zfclass _ZFP_ZFUIDialogForChoicePrivate : zfextends ZFUIListView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogForChoicePrivate, ZFUIListView)

public:
    ZFUIDialogForChoice *owner;
    ZFUIListAdapterArray *listAdapter;
    ZFListener updateTaskListener;
    zfidentity updateTaskId;
    ZFListener buttonEventListener;
    ZFUIButtonGroupArray *buttonGroup;

public:
    ZFLISTENER_MEMBER_DECLARE(updateTask)
    {
        this->buttonGroup->observerRemove(ZFUIButton::EventButtonOnClick(), this->buttonEventListener);
        this->updateTaskId = zfidentityInvalid;
        this->listAdapter->listCellRemoveAll();
        this->buttonGroup->buttonRemoveAll();
        switch(this->owner->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
                this->updateForChoiceSingle();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                this->updateForChoiceSingle();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
                this->updateForChoiceMultiple();
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        this->buttonGroup->observerAdd(ZFUIButton::EventButtonOnClick(), this->buttonEventListener);
        this->updateListSize();
        this->listReload();
    }

private:
    void updateListSize(void)
    {
        zfint listCellSizeHint = this->listAdapter->listCellSizeHint();
        ZFUISize sizeHint = ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeDialogWidth(), listCellSizeHint);
        ZFUISize size = sizeHint;
        size.height = 0;
        for(zfindex i = 0; i < this->buttonGroup->buttonCount(); ++i)
        {
            ZFUIButton *button = this->buttonGroup->buttonAtIndex(i);
            button->layoutMeasure(sizeHint, ZFUISizeParamWrapWidthWrapHeight);
            if(size.width < button->layoutMeasuredSize().width)
            {
                size.width = button->layoutMeasuredSize().width;
            }
            size.height += zfmMax(listCellSizeHint, button->layoutMeasuredSize().height);
        }
        if(size.height == 0)
        {
            size.height = sizeHint.height;
        }
        this->viewSizePreferedSet(size);
    }

private:
    void updateForChoiceSingle(void)
    {
        this->buttonGroup->buttonGroupTypeSet(ZFUIButtonGroupType::e_Tab);
        for(zfindex i = 0; i < this->owner->choiceCount(); ++i)
        {
            zfblockedAllocWithoutLeakTest(ZFUIListCell, cell);
            this->listAdapter->listCellAdd(cell);

            zfblockedAllocWithoutLeakTest(ZFUIButtonRatio, button);
            cell->cellViewSet(button);
            this->buttonGroup->buttonAdd(button);

            button->buttonLabelTextStringSet(this->owner->choiceNameAtIndex(i));
        }
        this->buttonGroup->buttonTabCheckedSet(this->owner->choiceSelectedIndexList().getFirst());
    }
    void updateForChoiceMultiple(void)
    {
        this->buttonGroup->buttonGroupTypeSet(ZFUIButtonGroupType::e_Normal);
        for(zfindex i = 0; i < this->owner->choiceCount(); ++i)
        {
            zfblockedAllocWithoutLeakTest(ZFUIListCell, cell);
            this->listAdapter->listCellAdd(cell);

            zfblockedAllocWithoutLeakTest(ZFUIButtonCheckBox, button);
            cell->cellViewSet(button);
            this->buttonGroup->buttonAdd(button);

            button->buttonLabelTextStringSet(this->owner->choiceNameAtIndex(i));
        }
        ZFCoreArrayPOD<zfindex> selectedIndexList = this->owner->choiceSelectedIndexList();
        for(zfindex i = 0; i < selectedIndexList.count(); ++i)
        {
            this->buttonGroup->buttonAtIndex(selectedIndexList[i])->buttonCheckedSet(zftrue);
        }
    }

private:
    ZFLISTENER_MEMBER_DECLARE(buttonEvent)
    {
        zfindex index = listenerData.param1->to<ZFValue *>()->indexValue();
        switch(this->owner->choiceMode())
        {
            case ZFUIDialogForChoiceMode::e_ChoiceSingle:
                this->owner->_ZFP_ZFUIDialogForChoice_choiceChange(this->owner->choiceSelectedIndexList().getFirst(), zffalse);
                this->owner->_ZFP_ZFUIDialogForChoice_choiceChange(index, zftrue);
                this->owner->choiceChangeNotify();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceSingleQuickly:
                this->owner->_ZFP_ZFUIDialogForChoice_choiceChange(this->owner->choiceSelectedIndexList().getFirst(), zffalse);
                this->owner->_ZFP_ZFUIDialogForChoice_choiceChange(index, zftrue);
                this->owner->choiceChangeNotify();
                this->owner->choiceConfirmNotify();
                break;
            case ZFUIDialogForChoiceMode::e_ChoiceMultiple:
            {
                ZFUIButton *button = listenerData.sender->to<ZFUIButton *>();
                this->owner->_ZFP_ZFUIDialogForChoice_choiceChange(index, button->buttonChecked());
                this->owner->choiceChangeNotify();
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
    }

public:
    zfoverride
    virtual ZFObject *objectOnInit(void)
    {
        zfsuper::objectOnInit();

        this->listBounceSet(zffalse);

        this->listAdapter = zfAllocWithoutLeakTest(ZFUIListAdapterArray);
        this->listAdapterSet(this->listAdapter);

        this->updateTaskListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(zfself, updateTask));
        this->updateTaskId = zfidentityInvalid;

        this->buttonEventListener = ZFCallbackForMemberMethod(this, ZFMethodAccessClassMember(zfself, buttonEvent));
        this->buttonGroup = zfAllocWithoutLeakTest(ZFUIButtonGroupArray);
        this->buttonGroup->observerAdd(ZFUIButton::EventButtonOnClick(), this->buttonEventListener);

        this->updateListSize();

        return this;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->listAdapterSet(zfnull);
        zfRelease(this->listAdapter);
        this->listAdapter = zfnull;

        zfReleaseWithoutLeakTest(this->buttonGroup);
        this->buttonGroup = zfnull;

        zfsuper::objectOnDealloc();
    }
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogForChoiceDataHolder, ZFLevelZFFrameworkNormal)
{
    this->dialogOnConfirmListener = ZFCallbackForRawFunction(zfself::dialogOnConfirm);
}
ZFListener dialogOnConfirmListener;
static ZFLISTENER_PROTOTYPE_EXPAND(dialogOnConfirm)
{
    userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialogForChoice *>()->choiceConfirmNotify();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogForChoiceDataHolder)

ZFObject *ZFUIDialogForChoice::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfAllocWithoutLeakTest(_ZFP_ZFUIDialogForChoicePrivate);
    d->owner = this;
    this->dialogContentContainer()->childAdd(d);

    return this;
}
void ZFUIDialogForChoice::objectOnDealloc(void)
{
    ZFThreadTaskCancel(d->updateTaskId);
    d->viewRemoveFromParent();
    zfReleaseWithoutLeakTest(d);
    zfsuper::objectOnDealloc();
}

void ZFUIDialogForChoice::dialogButtonOnAdd(ZF_IN ZFUIButton *button)
{
    zfsuper::dialogButtonOnAdd(button);
    if(button == this->dialogButton_Yes(zfHint("autoCreateIfNotExist")zffalse))
    {
        button->observerAdd(
            ZFUIButton::EventButtonOnClick(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogForChoiceDataHolder)->dialogOnConfirmListener,
            this->objectHolder());
    }
}
void ZFUIDialogForChoice::dialogButtonOnRemove(ZF_IN ZFUIButton *button)
{
    if(button == this->dialogButton_Yes(zfHint("autoCreateIfNotExist")zffalse))
    {
        button->observerRemove(
            ZFUIButton::EventButtonOnClick(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogForChoiceDataHolder)->dialogOnConfirmListener);
    }
    zfsuper::dialogButtonOnRemove(button);
}

void ZFUIDialogForChoice::choiceListOnChange(void)
{
    zfsuperI(ZFUIDialogForChoiceStyle)::choiceListOnChange();
    zfblockedAllocWithoutLeakTest(ZFThreadTaskRequestData, taskRequestData);
    taskRequestData->taskCallbackSet(d->updateTaskListener);
    d->updateTaskId = ZFThreadTaskRequest(taskRequestData, ZFThreadTaskRequestMergeCallbackIgnoreOldTask);
}
void ZFUIDialogForChoice::choiceSelectedListOnChange(void)
{
    zfsuperI(ZFUIDialogForChoiceStyle)::choiceSelectedListOnChange();
    zfblockedAllocWithoutLeakTest(ZFThreadTaskRequestData, taskRequestData);
    taskRequestData->taskCallbackSet(d->updateTaskListener);
    d->updateTaskId = ZFThreadTaskRequest(taskRequestData, ZFThreadTaskRequestMergeCallbackIgnoreOldTask);

    this->choiceChangeNotify();
}

ZF_NAMESPACE_GLOBAL_END


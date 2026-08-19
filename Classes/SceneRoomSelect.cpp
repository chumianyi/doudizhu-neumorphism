#include "SceneRoomSelect.h"
#include "SceneMenu.h"
#include "SceneGame.h"
#include "HappyBeanManager.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* SceneRoomSelect::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneRoomSelect::create();
    scene->addChild(layer);
    return scene;
}

bool SceneRoomSelect::init()
{
    if (!Layer::init()) return false;
    
    _selectedRoom = 0;
    _bonusDialog = nullptr;
    
    // 背景
    auto bg = Sprite::create("beijing.png");
    bg->setPosition(600, 337);
    bg->setScale(1200.0 / bg->getContentSize().width);
    this->addChild(bg, 0);
    
    // 标题
    auto title = Label::createWithTTF("选择场次", "fonts/FZCuYuan-M03S.ttf", 42);
    title->setPosition(600, 600);
    title->setColor(Color3B(52, 84, 130));
    this->addChild(title, 1);
    
    // 快乐豆显示
    auto beanBg = Sprite::create("bottomCardZone.png");
    beanBg->setScale(0.5);
    beanBg->setPosition(600, 530);
    this->addChild(beanBg, 1);
    
    auto beanIcon = Sprite::create("icon_dou.png");
    beanIcon->setPosition(510, 530);
    this->addChild(beanIcon, 2);
    
    _beanLabel = Label::createWithTTF("0", "fonts/FZCuYuan-M03S.ttf", 32);
    _beanLabel->setPosition(620, 530);
    _beanLabel->setColor(Color3B(52, 84, 130));
    this->addChild(_beanLabel, 2);
    updateBeanLabel();
    
    // 5个场次按钮 - 垂直排列
    const char* roomNames[] = {
        "初级场  门槛50",
        "中级场  门槛2000",
        "高级场  门槛1万",
        "顶级场  门槛10万",
        "包场  门槛1亿"
    };
    float roomY[] = {450, 370, 290, 210, 130};
    
    for (int i = 0; i < 5; i++)
    {
        auto item = customMenuItem("item_begin.png", "item_begin.png", 
            [this, i](Ref* sender) {
                this->menuRoomCallback(sender, i);
            });
        item->setScale(0.9);
        
        auto label = Label::createWithTTF(roomNames[i], "fonts/FZCuYuan-M03S.ttf", 24);
        label->setPosition(item->getContentSize().width/2, item->getContentSize().height/2);
        label->setColor(Color3B(255, 255, 255));
        item->addChild(label);
        
        auto menu = Menu::create(item, NULL);
        menu->setPosition(600, roomY[i]);
        this->addChild(menu, 2);
    }
    
    // 返回按钮
    auto backItem = customMenuItem("item_back.png", "item_back.png", 
        CC_CALLBACK_1(SceneRoomSelect::menuBackCallback, this));
    backItem->setScale(0.8);
    auto backMenu = Menu::create(backItem, NULL);
    backMenu->setPosition(80, 600);
    this->addChild(backMenu, 2);
    
    // 检查是否破产
    this->schedule(schedule_selector(SceneRoomSelect::checkBankrupt), 0.5f);
    
    return true;
}

void SceneRoomSelect::updateBeanLabel()
{
    int beans = HappyBeanManager::getInstance()->getBeans();
    char buf[64];
    if (beans >= 100000000)
        sprintf(buf, "%d亿", beans / 100000000);
    else if (beans >= 10000)
        sprintf(buf, "%d万", beans / 10000);
    else
        sprintf(buf, "%d", beans);
    _beanLabel->setString(buf);
}

void SceneRoomSelect::checkBankrupt(float dt)
{
    if (HappyBeanManager::getInstance()->getBeans() <= 0 && 
        HappyBeanManager::getInstance()->canClaimDailyBonus() &&
        _bonusDialog == nullptr)
    {
        showBonusDialog();
    }
}

void SceneRoomSelect::showBonusDialog()
{
    _bonusDialog = Layer::create();
    
    auto mask = LayerColor::create(Color4B(0, 0, 0, 150));
    _bonusDialog->addChild(mask, 0);
    
    auto dialogBg = Sprite::create("gameover/nt_result_base.png");
    dialogBg->setScale(0.7);
    dialogBg->setPosition(600, 337);
    _bonusDialog->addChild(dialogBg, 1);
    
    auto title = Label::createWithTTF("今日补偿", "fonts/FZCuYuan-M03S.ttf", 38);
    title->setPosition(600, 400);
    title->setColor(Color3B(200, 100, 100));
    _bonusDialog->addChild(title, 2);
    
    auto content = Label::createWithTTF("快乐豆已用完\n赠送你200快乐豆\n明天还可以再来领取哦", "fonts/FZCuYuan-M03S.ttf", 26);
    content->setPosition(600, 337);
    content->setColor(Color3B(52, 84, 130));
    _bonusDialog->addChild(content, 2);
    
    auto claimItem = customMenuItem("item_begin.png", "item_begin.png", 
        CC_CALLBACK_1(SceneRoomSelect::menuBonusCallback, this));
    claimItem->setScale(0.8);
    auto claimLabel = Label::createWithTTF("领取", "fonts/FZCuYuan-M03S.ttf", 28);
    claimLabel->setPosition(claimItem->getContentSize().width/2, claimItem->getContentSize().height/2);
    claimLabel->setColor(Color3B(255, 255, 255));
    claimItem->addChild(claimLabel);
    
    auto claimMenu = Menu::create(claimItem, NULL);
    claimMenu->setPosition(600, 250);
    _bonusDialog->addChild(claimMenu, 2);
    
    this->addChild(_bonusDialog, 100);
}

void SceneRoomSelect::hideBonusDialog()
{
    if (_bonusDialog)
    {
        _bonusDialog->removeFromParentAndCleanup(true);
        _bonusDialog = nullptr;
    }
}

void SceneRoomSelect::menuBonusCallback(Ref* pSender)
{
    int bonus = HappyBeanManager::getInstance()->claimDailyBonus();
    updateBeanLabel();
    hideBonusDialog();
}

void SceneRoomSelect::menuRoomCallback(Ref* pSender, int roomType)
{
    int thresholds[] = {50, 2000, 10000, 100000, 100000000};
    int threshold = thresholds[roomType];
    
    if (!HappyBeanManager::getInstance()->hasEnoughBeans(threshold))
    {
        if (HappyBeanManager::getInstance()->canClaimDailyBonus())
        {
            showBonusDialog();
        }
        return;
    }
    
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(SceneGame::createScene());
}

void SceneRoomSelect::menuBackCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(SceneMenu::createScene());
}

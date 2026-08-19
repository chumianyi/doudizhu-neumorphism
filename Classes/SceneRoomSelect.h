#ifndef __SCENE_ROOM_SELECT_H__
#define __SCENE_ROOM_SELECT_H__
#include "cocos2d.h"
#include "AppCommon.h"

class SceneRoomSelect : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void menuRoomCallback(cocos2d::Ref* pSender, int roomType);
    void menuBackCallback(cocos2d::Ref* pSender);
    void menuBonusCallback(cocos2d::Ref* pSender);
    
    void updateBeanLabel();
    void showBonusDialog();
    void hideBonusDialog();
    void checkBankrupt(float dt);
    
    CREATE_FUNC(SceneRoomSelect);
    
private:
    cocos2d::Label* _beanLabel;
    cocos2d::Layer* _bonusDialog;
    int _selectedRoom;
};

#endif

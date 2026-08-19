#ifndef __HAPPY_BEAN_MANAGER_H__
#define __HAPPY_BEAN_MANAGER_H__
#include "cocos2d.h"

class HappyBeanManager
{
public:
    static HappyBeanManager* getInstance();
    
    int getBeans();
    void addBeans(int count);
    bool spendBeans(int count);
    bool hasEnoughBeans(int count);
    
    // 每日补偿
    bool canClaimDailyBonus();
    int claimDailyBonus();
    std::string getLastBonusDate();
    
    // 场次门槛
    static const int ROOM_BEGINNER = 50;
    static const int ROOM_INTERMEDIATE = 2000;
    static const int ROOM_ADVANCED = 10000;
    static const int ROOM_PREMIUM = 100000;
    static const int ROOM_VIP = 100000000;
    
private:
    HappyBeanManager();
    static HappyBeanManager* _instance;
    int _beans;
    std::string _lastBonusDate;
    
    void load();
    void save();
    std::string getTodayDate();
};

#endif

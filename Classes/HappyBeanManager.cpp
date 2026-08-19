#include "HappyBeanManager.h"
#include "cocos2d.h"
#include <time.h>

USING_NS_CC;

HappyBeanManager* HappyBeanManager::_instance = nullptr;

HappyBeanManager* HappyBeanManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new HappyBeanManager();
    }
    return _instance;
}

HappyBeanManager::HappyBeanManager()
{
    _beans = 2000;
    _lastBonusDate = "";
    load();
}

std::string HappyBeanManager::getTodayDate()
{
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    char buf[32];
    sprintf(buf, "%04d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    return std::string(buf);
}

void HappyBeanManager::load()
{
    auto ud = UserDefault::getInstance();
    _beans = ud->getIntegerForKey("happy_beans", 2000);
    _lastBonusDate = ud->getStringForKey("last_bonus_date", "");
}

void HappyBeanManager::save()
{
    auto ud = UserDefault::getInstance();
    ud->setIntegerForKey("happy_beans", _beans);
    ud->setStringForKey("last_bonus_date", _lastBonusDate);
    ud->flush();
}

int HappyBeanManager::getBeans()
{
    return _beans;
}

void HappyBeanManager::addBeans(int count)
{
    _beans += count;
    if (_beans < 0) _beans = 0;
    save();
}

bool HappyBeanManager::spendBeans(int count)
{
    if (_beans >= count)
    {
        _beans -= count;
        save();
        return true;
    }
    return false;
}

bool HappyBeanManager::hasEnoughBeans(int count)
{
    return _beans >= count;
}

bool HappyBeanManager::canClaimDailyBonus()
{
    return _beans <= 0 && _lastBonusDate != getTodayDate();
}

int HappyBeanManager::claimDailyBonus()
{
    if (canClaimDailyBonus())
    {
        _beans = 200;
        _lastBonusDate = getTodayDate();
        save();
        return 200;
    }
    return 0;
}

std::string HappyBeanManager::getLastBonusDate()
{
    return _lastBonusDate;
}

#include "SceneGame.h"
#include "HappyBeanManager.h"
#include "Player.h"
#include "SceneMenu.h"
#include "SimpleAudioEngine.h"   //������������ͷ�ļ�  
#include "AppMacros.h"

USING_NS_CC;  
using namespace CocosDenshion;//ʹ�ø���������������ռ�

//////////////////////////////////////////////////////////////////////////
// ����ʱ�����Ϣ�洢

struct PlayerInfo
{
	std::string _name;
	int			_score;
};

struct RunTimeData
{
	RunTimeData()
	{
		_isReady = false;
	}

	bool		_isReady;	

	PlayerInfo	_playerinfo1;
	PlayerInfo	_playerinfo2;
	PlayerInfo	_playerinfo3;
};

RunTimeData s_runtimeData;

//
// SceneGame
//

Scene* SceneGame::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneGame::create();
    scene->addChild(layer);

    return scene;
}

bool SceneGame::init()
{
    // ��ʼ������----------------------------------------
    if ( !Layer::init() )
    {
        return false;
    }
    
	// ���ű�������
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/MusicEx_Normal.ogg", true);

	//

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ����----------------------------------------------
	auto bg = Sprite::create("bg_table.jpg");
	bg->setScale(1/0.8);
	bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);

	auto flag = Sprite::create("d_flag.png");
	flag->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + 100/SCALE_FACTOR));
	this->addChild(flag, 0);

	// ���ز˵�

	auto itemBack = customMenuItem("item_back.png", "item_back.png", CC_CALLBACK_1(SceneGame::menuBackCallback, this));
	itemBack->setPosition(visibleSize.width/2+200/SCALE_FACTOR, visibleSize.height-50/SCALE_FACTOR);

	_menuBack = Menu::create(itemBack, NULL);
	_menuBack->setPosition(origin);
	this->addChild(_menuBack, 1);
	
	// ׼���˵�

	auto itemReady = customMenuItem("item_zhunbei.png", "item_zhunbei.png", CC_CALLBACK_1(SceneGame::menuReadyCallback, this));
	_menuReady = Menu::create(itemReady, NULL);
	this->addChild(_menuReady, 1);

	// �������˵�

	auto itemQiang = customMenuItem("item_qiangdizhu.png", "item_qiangdizhu.png", CC_CALLBACK_1(SceneGame::menuQiangCallback, this));
	itemQiang->setPosition(-80/SCALE_FACTOR, -50/SCALE_FACTOR);
	auto itemBuQiang = customMenuItem("item_buqiang.png", "item_buqiang.png", CC_CALLBACK_1(SceneGame::menuBuQiangCallback, this));
	itemBuQiang->setPosition(80/SCALE_FACTOR, -50/SCALE_FACTOR);

	_menuQiangDiZhu = Menu::create(itemBuQiang, itemQiang, NULL);
	this->addChild(_menuQiangDiZhu, 1);
	_menuQiangDiZhu->setVisible(false);

    // ���Ʋ˵�

	auto itemBuChu = customMenuItem("item_buchu.png", "item_buchu_d.png", CC_CALLBACK_1(SceneGame::menuBuChuCallback, this));
	itemBuChu->setPosition(100/SCALE_FACTOR-50/SCALE_FACTOR, -50/SCALE_FACTOR);
    auto itemChuPai = customMenuItem("item_chupai.png", "item_chupai_d.png", CC_CALLBACK_1(SceneGame::menuChuPaiCallback, this));
	itemChuPai->setPosition(300/SCALE_FACTOR-50/SCALE_FACTOR, -50/SCALE_FACTOR);
	auto itemTiShi = customMenuItem("item_tishi.png", "item_tishi_d.png", CC_CALLBACK_1(SceneGame::menuTiShiCallback, this));
	itemTiShi->setPosition(-100/SCALE_FACTOR-50/SCALE_FACTOR, -50/SCALE_FACTOR);

	_menuChuPai = Menu::create();
	_menuChuPai->addChild(itemBuChu,2,0);
	_menuChuPai->addChild(itemChuPai,2,1);
	_menuChuPai->addChild(itemTiShi,2,2);
	this->addChild(_menuChuPai, 1);
	_menuChuPai->setVisible(false);

	// �����Ϣ��ȡ
	srand(time(0));

	auto name_list = FileUtils::getInstance()->getValueMapFromFile("strings.xml").at("name_list").asValueVector();
	// ����״ν���˴�������������Ϣ
	if (!s_runtimeData._isReady)
	{
		int name_index_1 = rand()%name_list.size();
		int name_index_2 = rand()%name_list.size();
		int name_index_3 = rand()%name_list.size();

		while (name_index_2 == name_index_1)
		{
			name_index_2 = rand()%name_list.size();
		}

		while (name_index_2 == name_index_1 || name_index_3 == name_index_1)
		{
			name_index_3 = rand()%name_list.size();
		}

		s_runtimeData._playerinfo1._name = name_list[name_index_1].asString();
		s_runtimeData._playerinfo1._score = 5000;
		s_runtimeData._playerinfo2._name = name_list[name_index_2].asString();
		s_runtimeData._playerinfo2._score = 5000;
		s_runtimeData._playerinfo3._name = name_list[name_index_3].asString();
		s_runtimeData._playerinfo3._score = 5000;

		s_runtimeData._isReady = true;
	}

	// ���

	_player1 = Player::create(s_runtimeData._playerinfo1._name, s_runtimeData._playerinfo1._score, true);
	_player1->setPosition(70/SCALE_FACTOR, 300/SCALE_FACTOR);
	_player1->SetPlayerID(1);
	this->addChild(_player1);

	_player2 = Player::create(s_runtimeData._playerinfo2._name, s_runtimeData._playerinfo2._score, false);
	_player2->setPosition(1000/SCALE_FACTOR, 600/SCALE_FACTOR);
	_player2->SetPlayerID(2);
	this->addChild(_player2);

	_player3 = Player::create(s_runtimeData._playerinfo3._name, s_runtimeData._playerinfo3._score, false);
	_player3->setPosition(70/SCALE_FACTOR, 600/SCALE_FACTOR);
	_player3->SetPlayerID(3);
	this->addChild(_player3);

	// ������
	_bottomCardZone = BottomCardZone::create();
	_bottomCardZone->setPosition(600/SCALE_FACTOR, 610/SCALE_FACTOR);
	this->addChild(_bottomCardZone, 1);

	// ��ʼ���ƶ�
	initCards();
    
	_arrPlayerOut = CCArray::create();
	_arrPlayerOut->retain();

	_state = 1;
	_gameover = false;
	_begin = false;

	//jiesuan(1);

    return true;
}

void SceneGame::initCards()
{
	for (int i=0; i<13; i++)
	{
		for (int j=0; j<4; j++)
		{
			PokeInfo info;
			info._num = (PokeNum)i;
			info._tag = (PokeTag)j;
			_pokeInfo.push_back(info);
		}
	}

	PokeInfo info;
	info._num = (PokeNum)13;
	info._tag = (PokeTag)0;
	_pokeInfo.push_back(info);
	info._num = (PokeNum)14;
	_pokeInfo.push_back(info);
}

static int index_fapai = 0; //����ʱ������

void SceneGame::faPai()
{
	// ϴ��
	index_fapai = 0;

	srand(time(0));
	std::random_shuffle(_pokeInfo.begin(), _pokeInfo.end());

	// ����
	auto delay = DelayTime::create(0.04);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackFaPai,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	// �Զ��ж�����
	schedule(schedule_selector(SceneGame::OutCard),0.1);
}

void SceneGame::callbackFaPai(cocos2d::Node* node)
{
	if (index_fapai < 51)
	{
		if (index_fapai%3==0)
		{
			// ��ɫ1
			_player1->FaPai(this, _pokeInfo.at(index_fapai));
		} else if (index_fapai%3==1)
		{
			// ��ɫ2
			_player2->FaPai(this, _pokeInfo.at(index_fapai));
		} else if (index_fapai%3==2)
		{
			// ��ɫ3
			_player3->FaPai(this, _pokeInfo.at(index_fapai));
		}

		index_fapai++;
	}

	if (index_fapai<51)
	{
		auto delay = DelayTime::create(0.04);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackFaPai,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}
	else
	{
		_menuQiangDiZhu->setVisible(true);
	}
}

void SceneGame::FaDiPai(Player* player)
{
	for (int i=index_fapai; i<54; i++)
	{
		//����
		_bottomCardZone->Show(_pokeInfo.at(i));
		_vecDiPai.push_back(_pokeInfo.at(i));
	}

	player->setDiZhu();
	for (int i=0; i<_vecDiPai.size(); i++)
	{
		player->FaPai(this, _vecDiPai[i]);
	}
}

void SceneGame::menuBackCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(SceneMenu::createScene());
}

void SceneGame::menuReadyCallback(Ref* pSender)
{
	faPai();

	_menuReady->setVisible(false);
}

void SceneGame::menuRestartCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(SceneGame::createScene());
}

void SceneGame::menuQiangCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/Man/Order.ogg");

	// �ַ�����
	FaDiPai(_player1);

	// �л��˵��ɼ�
	_menuQiangDiZhu->setVisible(false);
	_menuChuPai->setVisible(true);
}

void SceneGame::menuBuQiangCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/Man/NoOrder.ogg");

	// �л��˵��ɼ�
	_menuQiangDiZhu->setVisible(false);
	//_menuChuPai->setVisible(true);

	auto delay = DelayTime::create(2);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackQiangDiZhu2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void SceneGame::callbackQiangDiZhu2(cocos2d::Node* node)
{
	// �ַ�����

	// �����Ҳ��������������¼����
	if (_player2->IsQiangDiZhu())
	{
		FaDiPai(_player2);

		if (!_gameover)
		{
			auto delay = DelayTime::create(3);
			auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
			auto seq = Sequence::createWithTwoActions(delay, callback);
			this->runAction(seq);

			SimpleAudioEngine::getInstance()->playEffect("sound/Man/Rob1.ogg");

			if (!_begin)
			{
				_begin = true;
			}
		}
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect("sound/Man/NoRob.ogg");

		auto delay = DelayTime::create(2);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackQiangDiZhu3,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}
}

void SceneGame::callbackQiangDiZhu3(cocos2d::Node* node)
{
	FaDiPai(_player3);

	if (!_gameover)
	{
		auto delay = DelayTime::create(3);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai3,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
		SimpleAudioEngine::getInstance()->playEffect("sound/Man/Rob3.ogg");

		if (!_begin)
		{
			_begin = true;
		}
	}
}

void SceneGame::menuTiShiCallback(cocos2d::Ref* pSender)
{
	CARDS_DATA card_data = PanDuanPaiXing(_arrPlayerOut);

	auto player3_outcards = _player3->GetOutCards();
	CARDS_DATA player3_card_data = PanDuanPaiXing(player3_outcards);

	if (player3_outcards.empty())
	{
		auto player2_outcards = _player2->GetOutCards();
		CARDS_DATA player2_card_data = PanDuanPaiXing(player2_outcards);
		if (player2_outcards.empty())
		{
			_player1->ShowTipInfo(false, player2_card_data._type, player2_card_data._cards.size(), player2_card_data._value);
		}
		else
		{
			// ��ʾ����δ����
			_player1->ShowTipInfo(true, player2_card_data._type, player2_card_data._cards.size(), player2_card_data._value);
		}
	}
	else
	{
		_player1->ShowTipInfo(true, player3_card_data._type, player3_card_data._cards.size(), player3_card_data._value);
	} 
}

void SceneGame::menuBuChuCallback(Ref* pSender)
{
	_player3->clearCards();
	_player1->BuChu();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_menuChuPai->setVisible(false);
	SimpleAudioEngine::getInstance()->playEffect("sound/Man/buyao4.ogg");
}

void SceneGame::menuChuPaiCallback(Ref* pSender)
{
	if (!_begin)
	{
		_begin = true;
	}

	// ����֮ǰ���ж��ϼҵ�����
	auto player3_outcards = _player3->GetOutCards();
	CARDS_DATA player3_card_data = PanDuanPaiXing(player3_outcards);

	if (player3_outcards.empty())
	{
		auto player2_outcards = _player2->GetOutCards();
		CARDS_DATA player2_card_data = PanDuanPaiXing(player2_outcards);
		if (player2_outcards.empty())
		{
			_player1->ChuPai(this, false, ERROR_CARD, 0, 1);// ����
		}
		else
		{
			_player1->ChuPai(this, true, player2_card_data._type, player2_outcards.size(), player2_card_data._value);// ����
		}
	}
	else
	{
		_player1->ChuPai(this, true, player3_card_data._type, player3_outcards.size(), player3_card_data._value);// ����
	}

	// ��������ж���
	_arrPlayerOut->removeAllObjects();

	if (!_gameover)
	{
		auto delay = DelayTime::create(1);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}

	_menuChuPai->setVisible(false);
}

void SceneGame::callbackChuPai2(cocos2d::Node* node)
{
	// ����֮ǰ���ж��ϼҵ�����
	auto player1_outcards = _player1->GetOutCards();
	CARDS_DATA player1_card_data = PanDuanPaiXing(player1_outcards);

	if (player1_outcards.empty())
	{
		auto player3_outcards = _player3->GetOutCards();
		CARDS_DATA player3_card_data = PanDuanPaiXing(player3_outcards);
		if (player3_outcards.empty())
		{
			_player2->ChuPai(this, false, ERROR_CARD, 0, 1);// ����
		}
		else
		{
			_player2->ChuPai(this, true, player3_card_data._type, player3_outcards.size(), player3_card_data._value);// ����
		}
	}
	else
	{
		_player2->ChuPai(this, true, player1_card_data._type, player1_outcards.size(), player1_card_data._value);// ����
	}

	if (!_gameover)
	{
		auto delay = DelayTime::create(1);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai3,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}
}

void SceneGame::callbackChuPai3(cocos2d::Node* node)
{
	// ����֮ǰ���ж��ϼҵ�����
	auto player2_outcards = _player2->GetOutCards();
	CARDS_DATA player2_card_data = PanDuanPaiXing(player2_outcards);

	if (player2_outcards.empty())
	{
		auto player1_outcards = _player1->GetOutCards();
		CARDS_DATA player1_card_data = PanDuanPaiXing(player1_outcards);
		if (player1_outcards.empty())
		{
			_player3->ChuPai(this, false, ERROR_CARD, 0, 1);// ����
		}
		else
		{
			_player3->ChuPai(this, true, player1_card_data._type, player1_outcards.size(), player1_card_data._value);// ����
		}
	}
	else
	{
		_player3->ChuPai(this, true, player2_card_data._type, player2_outcards.size(), player2_card_data._value);// ����
	}

	if (!_gameover)
	{
		_menuChuPai->setVisible(true);
	}
}

void SceneGame::update(float delta)
{

}

void SceneGame::setChuPaiMenuEnabled(bool isChuPai, bool isBuChu)
{
	((MenuItemFont *)_menuChuPai->getChildByTag(1))->setEnabled(isChuPai);//����
	((MenuItemFont *)_menuChuPai->getChildByTag(0))->setEnabled(isBuChu);//����
}

void SceneGame::OutCard(float delta)
{
	if (_state == 1)
	{
		// ����
		if (!_begin)
		{
			CARDS_DATA card_data = PanDuanPaiXing(_arrPlayerOut);
			setChuPaiMenuEnabled(card_data._type != ERROR_CARD, false);
		}
		// �Ѿ�����
		else
		{
			CARDS_DATA card_data = PanDuanPaiXing(_arrPlayerOut);

			auto player3_outcards = _player3->GetOutCards();
			CARDS_DATA player3_card_data = PanDuanPaiXing(player3_outcards);

			if (player3_outcards.empty())
			{
				auto player2_outcards = _player2->GetOutCards();
				CARDS_DATA player2_card_data = PanDuanPaiXing(player2_outcards);
				if (player2_outcards.empty())
				{
					setChuPaiMenuEnabled(card_data._type != ERROR_CARD, false);
				}
				else
				{
					// ��ʾ����δ����
					//_player1->ShowTipInfo(true, player2_card_data._type, player2_card_data._cards.size(), player2_card_data._value);

					if (
						// ��ͬ����
						(card_data._type == player2_card_data._type && card_data._value > player2_card_data._value) ||
						// ��ͬ������ը������
						(card_data._type != player2_card_data._type && player2_card_data._type != MISSILE_CARD && 
						(card_data._type == BOMB_CARD || card_data._type == MISSILE_CARD) && card_data._value > player2_card_data._value))
					{
						setChuPaiMenuEnabled(true, true);
					}
					else
					{
						setChuPaiMenuEnabled(false, true);
					}
				}
			}
			else
			{
				//_player1->ShowTipInfo(true, player3_card_data._type, player3_card_data._cards.size(), player3_card_data._value);

				if (
					// ��ͬ����
					(card_data._type == player3_card_data._type && card_data._value > player3_card_data._value) ||
					// ��ͬ������ը������
					(card_data._type != player3_card_data._type && player3_card_data._type != MISSILE_CARD && 
					(card_data._type == BOMB_CARD || card_data._type == MISSILE_CARD) && card_data._value > player3_card_data._value))
				{
					setChuPaiMenuEnabled(true, true);
				}
				else
				{
					setChuPaiMenuEnabled(false, true);
				}
			}
		}
	}
}

void SceneGame::gameover(int winID)
{
	_gameover = true;

	unschedule(schedule_selector(SceneGame::OutCard));

	//_menuRestart->setVisible(true);
	_menuChuPai->setVisible(false);
	_menuQiangDiZhu->setVisible(false);
	_menuReady->setVisible(false);

	jiesuan(winID);
}

int player1_delta_score = 0;
int player2_delta_score = 0;
int player3_delta_score = 0;
bool isPlayerWin = false;

void SceneGame::jiesuan(int winID)
{
	// �������

	int base_score = 100;
	player1_delta_score = 0;
	player2_delta_score = 0;
	player3_delta_score = 0;

	if (winID == 1)
	{
		isPlayerWin = true;

		if (_player1->getIsDiZhu())
		{
			player1_delta_score = base_score * 2;
			player2_delta_score = base_score * -1;
			player3_delta_score = base_score * -1;
		}
		else if (_player2->getIsDiZhu())
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * -1 * 2;
			player3_delta_score = base_score * 1;
		}
		else
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * -1 * 2;
		}
	}
	else if (winID == 2)
	{
		isPlayerWin = false;

		if (_player1->getIsDiZhu())
		{
			player1_delta_score = base_score * -1 * 2;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * 1;
		}
		else if (_player2->getIsDiZhu())
		{
			player1_delta_score = base_score * -1;
			player2_delta_score = base_score * 2;
			player3_delta_score = base_score * -1;
		}
		else
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * -1 * 2;
		}
	}
	else if (winID == 3)
	{
		isPlayerWin = false;

		if (_player1->getIsDiZhu())
		{
			player1_delta_score = base_score * -1 * 2;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * 1;
		}
		else if (_player2->getIsDiZhu())
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * -1 * 2;
			player3_delta_score = base_score * 1;
		}
		else
		{
			player1_delta_score = base_score * -1;
			player2_delta_score = base_score * -1;
			player3_delta_score = base_score * 2;
		}
	}

	// �洢����
	_player1->Score(player1_delta_score);
	// 同步快乐豆
	if (player1_delta_score != 0)
	{
		HappyBeanManager::getInstance()->addBeans(player1_delta_score);
	}
	_player2->Score(player2_delta_score);
	_player3->Score(player3_delta_score);

	s_runtimeData._playerinfo1._score = _player1->GetScore();
	s_runtimeData._playerinfo2._score = _player2->GetScore();
	s_runtimeData._playerinfo3._score = _player3->GetScore();

	// �������
	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackJieSuan,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void SceneGame::callbackJieSuan(cocos2d::Node* node)
{
	// �������в˵�
	_menuBack->setEnabled(false);
	_menuChuPai->setEnabled(false);
	_menuQiangDiZhu->setEnabled(false);
	_menuReady->setEnabled(false);

	//------------------------------------------------------------

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SimpleAudioEngine::getInstance()->playEffect(isPlayerWin ? "sound/MusicEx_Win.ogg" : "sound/MusicEx_Lose.ogg");

	// ����----------------------------------------------
	auto bg = Sprite::create(isPlayerWin ? "gameover/ddz_diploma_win_big_bg.jpg" : "gameover/ddz_diploma_lose_big_bg.jpg");
	bg->setScale(1.4);
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 1000);

	// ����ҳ��

	auto jiesuanBg = Sprite::create("gameover/bg_small.png");
	jiesuanBg->setPosition(Vec2(visibleSize.width * 2 / 3 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(jiesuanBg, 1001);

	auto tiaofu = Sprite::create(isPlayerWin ? "gameover/gameresult_win_tiaofu.png" : "gameover/gameresult_lose_tiaofu.png");
	tiaofu->setPosition(Vec2(350/SCALE_FACTOR, 400/SCALE_FACTOR));
	jiesuanBg->addChild(tiaofu, 0);

	auto shengli = Sprite::createWithSpriteFrameName(isPlayerWin ? "ddz_result_base_word_win.png" : "ddz_result_base_word_lose.png");
	shengli->setPosition(Vec2(350/SCALE_FACTOR, 430/SCALE_FACTOR));
	jiesuanBg->addChild(shengli, 0);

	auto player = Sprite::create(
		isPlayerWin ? 
		(_player1->getIsDiZhu() ? "gameover/owner1_win.png" : "gameover/farmer1_win.png") 
		: 
		(_player1->getIsDiZhu() ? "gameover/owner1_lose.png" : "gameover/farmer1_lose.png"));
	player->setPosition(Vec2(-200/SCALE_FACTOR, 200/SCALE_FACTOR));
	jiesuanBg->addChild(player, 0);

	// ��������

	// �ǳ�

	auto label_name = Label::createWithTTF(FileUtils::getInstance()->getValueMapFromFile("strings.xml").at("name").asString(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_name->setColor(Color3B(52, 84, 130));
	label_name->setPosition(Vec2(150/SCALE_FACTOR,300/SCALE_FACTOR));
	jiesuanBg->addChild(label_name,1);

	// ������־
	auto dizhu_flag = Sprite::createWithSpriteFrameName("ddz_result_base_dizhu_flag.png");
	dizhu_flag->setPosition(Vec2(90/SCALE_FACTOR, _player1->getIsDiZhu() ? 250/SCALE_FACTOR : (_player2->getIsDiZhu() ? 200/SCALE_FACTOR : (_player3->getIsDiZhu() ? 150/SCALE_FACTOR : 0))));
	jiesuanBg->addChild(dizhu_flag, 0);

	auto label_name_player1 = Label::createWithTTF(_player1->GetName().c_str(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_name_player1->setColor(Color3B(52, 84, 130));
	label_name_player1->setPosition(Vec2(150/SCALE_FACTOR,250/SCALE_FACTOR));
	jiesuanBg->addChild(label_name_player1,1);

	auto label_name_player2 = Label::createWithTTF(_player2->GetName().c_str(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_name_player2->setColor(Color3B(52, 84, 130));
	label_name_player2->setPosition(Vec2(150/SCALE_FACTOR,200/SCALE_FACTOR));
	jiesuanBg->addChild(label_name_player2,1);

	auto label_name_player3 = Label::createWithTTF(_player3->GetName().c_str(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_name_player3->setColor(Color3B(52, 84, 130));
	label_name_player3->setPosition(Vec2(150/SCALE_FACTOR,150/SCALE_FACTOR));
	jiesuanBg->addChild(label_name_player3,1);

	// ���ֶ�

	auto label_huanledou = Label::createWithTTF(FileUtils::getInstance()->getValueMapFromFile("strings.xml").at("score").asString(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_huanledou->setColor(Color3B(52, 84, 130));
	label_huanledou->setPosition(Vec2(350/SCALE_FACTOR,300/SCALE_FACTOR));
	jiesuanBg->addChild(label_huanledou,1);

	auto label_huanledou_player1 = Label::createWithTTF(_player1->GetScoreString().c_str(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_huanledou_player1->setColor(Color3B(52, 84, 130));
	label_huanledou_player1->setPosition(Vec2(350/SCALE_FACTOR,250/SCALE_FACTOR));
	jiesuanBg->addChild(label_huanledou_player1,1);

	auto label_huanledou_player2 = Label::createWithTTF(_player2->GetScoreString().c_str(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_huanledou_player2->setColor(Color3B(52, 84, 130));
	label_huanledou_player2->setPosition(Vec2(350/SCALE_FACTOR,200/SCALE_FACTOR));
	jiesuanBg->addChild(label_huanledou_player2,1);

	auto label_huanledou_player3 = Label::createWithTTF(_player3->GetScoreString().c_str(), "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_huanledou_player3->setColor(Color3B(52, 84, 130));
	label_huanledou_player3->setPosition(Vec2(350/SCALE_FACTOR,150/SCALE_FACTOR));
	jiesuanBg->addChild(label_huanledou_player3,1);

	// �����Ӽ�
	char str_score_delta_player1[255] = {0};
	char str_score_delta_player2[255] = {0};
	char str_score_delta_player3[255] = {0};

	sprintf(str_score_delta_player1, "%+d", player1_delta_score);
	sprintf(str_score_delta_player2, "%+d", player2_delta_score);
	sprintf(str_score_delta_player3, "%+d", player3_delta_score);

	auto label_huanledou_d_player1 = Label::createWithTTF(str_score_delta_player1, "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_huanledou_d_player1->setColor(Color3B(52, 84, 130));
	label_huanledou_d_player1->setPosition(Vec2(480/SCALE_FACTOR,250/SCALE_FACTOR));
	jiesuanBg->addChild(label_huanledou_d_player1,1);

	auto label_huanledou_d_player2 = Label::createWithTTF(str_score_delta_player2, "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_huanledou_d_player2->setColor(Color3B(52, 84, 130));
	label_huanledou_d_player2->setPosition(Vec2(480/SCALE_FACTOR,200/SCALE_FACTOR));
	jiesuanBg->addChild(label_huanledou_d_player2,1);

	auto label_huanledou_d_player3 = Label::createWithTTF(str_score_delta_player3, "fonts/FZCuYuan-M03S.ttf", 24/SCALE_FACTOR);
	label_huanledou_d_player3->setColor(Color3B(52, 84, 130));
	label_huanledou_d_player3->setPosition(Vec2(480/SCALE_FACTOR,150/SCALE_FACTOR));
	jiesuanBg->addChild(label_huanledou_d_player3,1);

	// �ؿ��˵�

	auto itemRestart = customMenuItemWithSpriteFrameName("ddzsingle_maplvl_btn_restart.png", "ddzsingle_maplvl_btn_restart.png", CC_CALLBACK_1(SceneGame::menuRestartCallback, this));
	itemRestart->setPosition(200/SCALE_FACTOR, -150/SCALE_FACTOR);
	auto menuRestart = Menu::create(itemRestart, NULL);
	this->addChild(menuRestart, 1002);
}
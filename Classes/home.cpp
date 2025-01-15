#include "home.h"
#include "player.h"
#include "enemy.h"
#include "battle.h"
#include "CharacterSelectLayer.h"
#include "ShopLayer.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

Scene* home::createScene() {
	return home::create();
}

bool home::init() {  //主函数

	if (!Scene::init())
	{
		return false;
	}

	CSL = CharacterSelectLayer::create();
	this->addChild(CSL);
	this->schedule(schedule_selector(home::endCSL));
	

	//背景设定
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	background->setScale(1.2);
	this->addChild(background, -2);

	
	return true;
}


void home::endCSL(float delta) 
{
	if (CSL->end == false) 
	{
		this->removeChild(CSL);
		b = battle::create();
		this->addChild(b);
		this->unschedule(schedule_selector(home::endCSL));
		this->schedule(schedule_selector(home::endWave));

	}
}

void home::endWave(float delta)
{
	if (b->totalOver() != 0)
	{
		gameOverLayer = GameOverLayer::create(b->totalOver());
		this->addChild(gameOverLayer);
		gameOverLayer->showInfo(b->getKillNum());
		this->removeChild(b);
		this->unschedule(schedule_selector(home::endWave));
		this->schedule(schedule_selector(home::retryGame));
	}
	else if (b->gameover())
	{
		b->setVisible(false);
		shopLayer = ShopLayer::create(b->getPlayer());
		this->addChild(shopLayer);
		this->unschedule(schedule_selector(home::endWave));
		this->schedule(schedule_selector(home::endShop));
	}
}

void home::endShop(float delta)
{
	if (shopLayer->toEnd() == true)
	{
		this->removeChild(shopLayer);
		b->setVisible(true);
		b->startSchedule();

		this->unschedule(schedule_selector(home::endShop));
		this->schedule(schedule_selector(home::endWave));
	}
}

void home::endGame(float delta)
{
	if (!b->totalOver())
	{
		this->removeChild(b);
		gameOverLayer = GameOverLayer::create(b->totalOver());
		this->addChild(gameOverLayer);
		this->unschedule(schedule_selector(home::endGame));
		this->schedule(schedule_selector(home::retryGame));
	}
}

void home::retryGame(float delta)
{
	if (gameOverLayer->toEnd())
	{
		this->removeChild(gameOverLayer);
		CSL = CharacterSelectLayer::create();
		this->addChild(CSL);
		this->unschedule(schedule_selector(home::retryGame));
		this->schedule(schedule_selector(home::endCSL));
	}
}

void home::test(float delta) {  //测试函数，无用
	//if (b->gameover()) 
		//this->addChild(b);
	if (b->dead()) {
		b->setVisible(false);
		/*
		this->removeChild(b);
		Layer* l = Layer::create();
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto s = Sprite::create("buildings/houses.png");
		s->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7));
		s->setScale(2.5);
		l->addChild(s);
		this->addChild(l);
		*/
	}
}

/*
void home::playermove(float delta) {   //键盘控制角色移动
	auto w = EventKeyboard::KeyCode::KEY_W;
	auto s = EventKeyboard::KeyCode::KEY_S;
	auto a = EventKeyboard::KeyCode::KEY_A;
	auto d = EventKeyboard::KeyCode::KEY_D;
	if (keyMap[w]==true) {
		if (Brotato.sprite->getPositionY() <  1080)
			Brotato.sprite->setPositionY(Brotato.sprite->getPositionY() + Brotato.speed);
	}
	if (keyMap[a] == true) {
		if(Brotato.sprite->getPositionX()>0)
			Brotato.sprite->setPositionX(Brotato.sprite->getPositionX() - Brotato.speed);
	}
	if (keyMap[s] == true) {
		if (Brotato.sprite->getPositionY() > 0)
			Brotato.sprite->setPositionY(Brotato.sprite->getPositionY() - Brotato.speed);
	}
	if (keyMap[d] == true) {
		if (Brotato.sprite->getPositionX() < 1920)
			Brotato.sprite->setPositionX(Brotato.sprite->getPositionX() + Brotato.speed);
	}
}

void home::playermove2(float delta) {   //键盘控制角色移动
	auto i = EventKeyboard::KeyCode::KEY_I;
	auto k = EventKeyboard::KeyCode::KEY_K;
	auto j = EventKeyboard::KeyCode::KEY_J;
	auto l = EventKeyboard::KeyCode::KEY_L;
	if (keyMap[i] == true) {
		if (Brotato.sprite->getPositionY() < 1080)
			Brotato.sprite->setPositionY(Brotato.sprite->getPositionY() + 10);
	}
	if (keyMap[j] == true) {
		if (Brotato.sprite->getPositionX() > 0)
			Brotato.sprite->setPositionX(Brotato.sprite->getPositionX() - 10);
	}
	if (keyMap[k] == true) {
		if (Brotato.sprite->getPositionY() > 0)
			Brotato.sprite->setPositionY(Brotato.sprite->getPositionY() - 10);
	}
	if (keyMap[l] == true) {
		if (Brotato.sprite->getPositionX() < 1920)
			Brotato.sprite->setPositionX(Brotato.sprite->getPositionX() + 10);
	}
}




void home::OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {  //记录键盘按下的状态
	keyMap[keyCode] = true;
}

void home::OnKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {  //撤销键盘按下的状态
	keyMap[keyCode] = false;
}


void home::test(float delta) {  //测试函数，无用
	Layer* l = Layer::create();
	auto s = Sprite::create("buildings/houses.png");
	s->setPosition(Brotato.countdown * 50, Brotato.countdown * 30);
	s->setScale(4);
	l->addChild(s);
	this->addChild(l);
}


void home::update_per_frame(float delta) {   //所有每帧都要做的操作
	
	Brotato.hurt(enemylist.hit_damage());
	Brotato.showInfo();

	enemylist.update(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY());
	enemylist.move();

	if (Brotato.dead()) {
		auto scene_helloworld = HelloWorld::createScene();
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.8f, scene_helloworld));
	}
	if (Brotato.gameover()) {
		//auto scene_helloworld = home::createScene();
		//Director::getInstance()->replaceScene(CCTransitionFade::create(0.8f, scene_helloworld));
		this->removeChild(battle);
	}
}

void home::update_per_second(float delta) {  //所有每秒都要做的操作
	Brotato.countdown--;
	//Brotato.hurt(enemylist.hit_damage());
	//Brotato.showInfo();
}



void home::generate_enemy(float delta) {  //生成敌人
	battle->addChild(enemylist.generate_enemy());
	Brotato.hurt(-1);
}
*/
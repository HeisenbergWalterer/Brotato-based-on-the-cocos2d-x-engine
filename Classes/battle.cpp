#include "cocos2d.h"
#include "battle.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

Layer* battle::createlayer() {
	return Layer::create();
}

bool battle::init() {

	if (!Layer::init())
	{
		return false;
	}
	int wave = 1;

	killTenFlag = true;

	//创建玩家角色
	Brotato.createPlayer();
	this->addChild(Brotato.sprite);

	//创建信息面板
	Brotato.createInfo();
	this->addChild(Brotato.label);

	weapon.init(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY());
	set_weapon();

	//bgm
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("bgm/battle.wav", true);

	achieve = Label::createWithTTF("Achievent:\nYou have killed ten enemies!!", "fonts/arial.ttf", 36);
	achieve->setPosition(1600, 100);
	achieve->setVisible(false);
	this->addChild(achieve);

	//创建键盘监听
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(battle::OnKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(battle::OnKeyReleased, this);
	this->schedule(schedule_selector(battle::playermove));
	this->schedule(schedule_selector(battle::playermove2));
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	//每帧要做的函数
	this->schedule(schedule_selector(battle::update_per_frame));

	//每秒要做的函数
	this->schedule(schedule_selector(battle::update_per_second), 1);

	//玩家受到伤害结算
	this->schedule(schedule_selector(battle::playerHurt), 0.2);
	this->schedule(schedule_selector(battle::white), 0.5);
	this->schedule(schedule_selector(battle::update_per_attack), 1 / (Brotato.attackSpeed + weapon.attackSpeed[0]));
	this->schedule(schedule_selector(battle::update_per_attack1), 1 / (Brotato.attackSpeed + weapon.attackSpeed[1]));
	this->schedule(schedule_selector(battle::update_per_attack2), 1 / (Brotato.attackSpeed + weapon.attackSpeed[2]));
	this->schedule(schedule_selector(battle::update_per_attack3), 1 / (Brotato.attackSpeed + weapon.attackSpeed[3]));
	this->schedule(schedule_selector(battle::update_per_attack4), 1 / (Brotato.attackSpeed + weapon.attackSpeed[4]));
	this->schedule(schedule_selector(battle::update_per_attack5), 1 / (Brotato.attackSpeed + weapon.attackSpeed[5]));
	this->schedule(schedule_selector(battle::generate_enemy), 2);
	this->schedule(schedule_selector(battle::generate_bullet), 4);
	
	this->schedule(schedule_selector(battle::achievement), 4);

	return true;
}

void battle::playermove(float delta) {   //键盘控制角色移动
	auto w = EventKeyboard::KeyCode::KEY_W;
	auto s = EventKeyboard::KeyCode::KEY_S;
	auto a = EventKeyboard::KeyCode::KEY_A;
	auto d = EventKeyboard::KeyCode::KEY_D;
	if (keyMap[w] == true) {
		if (Brotato.sprite->getPositionY() < 1080)
			Brotato.sprite->setPositionY(Brotato.sprite->getPositionY() + Brotato.speed);
	}
	if (keyMap[a] == true) {
		if (Brotato.sprite->getPositionX() > 0)
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

void battle::playermove2(float delta) {   //键盘控制角色移动
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

void battle::OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {  //记录键盘按下的状态
	keyMap[keyCode] = true;
}

void battle::OnKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {  //撤销键盘按下的状态
	keyMap[keyCode] = false;
}

void battle::updatePurchasedItemsUI()
{
	// 获取窗口大小
	auto winSize = Director::getInstance()->getWinSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// 获取玩家对象
	player* p = getPlayer();

	// 遍历已购买物品并显示
	for (int i = 0; i < p->purchasedItems.size(); i++)
	{
		string itemImage = (p->purchasedItems[i].isWeapon ? "weapon/" : "Item/") + p->purchasedItems[i].name + (p->purchasedItems[i].isWeapon ? "_icon.png" : ".png");
		auto itemSprite = Sprite::create(itemImage);

		itemSprite->setPosition(Vec2(winSize.width - 50, winSize.height - 50 - i * 70));  // 在右侧依次排列
		itemSprite->setScale(0.5f); // 缩小显示尺寸
		this->addChild(itemSprite); // 将物品图标添加到当前层
	}
}

// 处理购买武器的逻辑
void battle::purchaseWeapon(Item item)
{
	// 判断武器槽是否有空位，若有空位则加入，否则升级已有的武器
	if (weapon.addWeapon(item))
	{
		// 武器添加成功，标记为购买完成
		Brotato.updateToBuyWeapon();
		Brotato.updateIfBuyWeapon();

		/*bool* ptr1 = Brotato.getIfBuyWeapon();
		*ptr1 = false;*/
	}
	else
	{
		// 购买失败（例如，武器槽已满）
		CCLOG("Cannot add more weapons or no matching weapon found.");
	}

}


void battle::update_per_frame(float delta) {   //所有每帧都要做的操作

	//Brotato.hurt(enemylist.hit_damage());
	Brotato.showInfo();

	// 更新已购买物品 UI 
	if(Brotato.toBuyItem)
	{
		updatePurchasedItemsUI();
		Brotato.toBuyItem = false;
	}

	enemylist.update(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY());
	enemylist.move();

	// 检查玩家是否有武器购买请求
	if (Brotato.getToBuyWeapon())
	{
		Item item = Brotato.getCurrentItem();
		purchaseWeapon(item);
	}

	remove_weapon();
	weapon.create(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY());
	set_weapon();

	weapon.bullet_move();
	enemyhurt();

	/*
	if (Brotato.dead()) {
		auto scene_helloworld = HelloWorld::createScene();
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.8f, scene_helloworld));
	}
	if (Brotato.gameover()) {
		//auto scene_helloworld = home::createScene();
		//Director::getInstance()->replaceScene(CCTransitionFade::create(0.8f, scene_helloworld));
		this->removeChild(battle);
	}

	*/
}

void battle::update_per_second(float delta) {  //所有每秒都要做的操作
	Brotato.countdown--;
}



void battle::generate_enemy(float delta) {  //生成敌人
	this->addChild(enemylist.generate_enemy());
}

void battle::generate_bullet(float delta) {
	enemylist.generate_bullet();
	Sprite* p;
	for (int i = 0; i < 100; i++) {
		p = enemylist.newBullet[i];
		if (p == NULL)
			break;
		else
			this->addChild(p);
	}
}

void battle::set_weapon() {
	for (int i = 0; i < 6; i++) {
		if (weapon.num[i]) {
			this->addChild(weapon.weapon[i]);
		}
	}
}

void battle::remove_weapon() {
	for (int i = 0; i < 6; i++) {
		if (weapon.num[i]) {
			this->removeChild(weapon.weapon[i]);
		}
	}
}

void battle::bullet(float X, float Y, float enemy_X, float enemy_Y, int damage, int i) {
	weapon.generate_bullet(X, Y, enemy_X, enemy_Y, damage, i);
	Sprite* p;
	p = weapon.newbullet;
	if (p != NULL)
		this->addChild(p);
}

void battle::enemyhurt() {
	while (weapon.bullet_hit() != weapon.last) {
		Sprite* p = weapon.bullet_hit()->bullet_s;
		enemylist.hurt(p->getPositionX(), p->getPositionY(), 150, weapon.bullet_hit()->bullet_damage);
		weapon.bullet_clear(weapon.bullet_hit());
		this->removeChild(p);
	}
}

void battle::white(float delta) {
	enemylist.white();
}

void battle::update_per_attack(float delta) {
	if (weapon.num[0]) {
		if (enemylist.nearestDistance(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()) <= (Brotato.range + weapon.range[0])) {
			if (weapon.isRanged[0] == true) {
				bullet(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(),enemylist.nearestX(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()),enemylist.nearestY(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), Brotato.Strength + weapon.damage[0], 0);//发射弹幕
			}
			else {
				enemylist.hurt(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(),(Brotato.range + weapon.range[0]), (Brotato.Strength + weapon.damage[0]));
			}
		}
	}
}

void battle::update_per_attack1(float delta) {
	if (weapon.num[1]) {
		if (enemylist.nearestDistance(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()) <= (Brotato.range + weapon.range[1])) {
			if (weapon.isRanged[1] == true) {
				bullet(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), enemylist.nearestX(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), enemylist.nearestY(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), Brotato.Strength + weapon.damage[1], 1);//发射弹幕
			}
			else {
				enemylist.hurt(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), (Brotato.range + weapon.range[1]), (Brotato.Strength + weapon.damage[1]));
			}
		}
	}
}

void battle::update_per_attack2(float delta) {
	if (weapon.num[2]) {
		if (enemylist.nearestDistance(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()) <= (Brotato.range + weapon.range[2])) {
			if (weapon.isRanged[2] == true) {
				bullet(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), enemylist.nearestX(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), enemylist.nearestY(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), Brotato.Strength + weapon.damage[2], 2);//发射弹幕
			}
			else {
				enemylist.hurt(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), (Brotato.range + weapon.range[2]), (Brotato.Strength + weapon.damage[2]));
			}
		}
	}
}

void battle::update_per_attack3(float delta) {
	if (weapon.num[3]) {
		if (enemylist.nearestDistance(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()) <= (Brotato.range + weapon.range[3])) {
			if (weapon.isRanged[3] == true) {
				bullet(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), enemylist.nearestX(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), enemylist.nearestY(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), Brotato.Strength + weapon.damage[3], 3);//发射弹幕
			}
			else {
				enemylist.hurt(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), (Brotato.range + weapon.range[3]), (Brotato.Strength + weapon.damage[3]));
			}
		}
	}
}

void battle::update_per_attack4(float delta) {
	if (weapon.num[4]) {
		if (enemylist.nearestDistance(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()) <= (Brotato.range + weapon.range[4])) {
			if (weapon.isRanged[4] == true) {
				bullet(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), enemylist.nearestX(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), enemylist.nearestY(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), Brotato.Strength + weapon.damage[4], 4);//发射弹幕
			}
			else {
				enemylist.hurt(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), (Brotato.range + weapon.range[4]), (Brotato.Strength + weapon.damage[4]));
			}
		}
	}
}

void battle::update_per_attack5(float delta) {
	if (weapon.num[5]) {
		if (enemylist.nearestDistance(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()) <= (Brotato.range + weapon.range[5])) {
			if (weapon.isRanged[5] == true) {
				bullet(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), enemylist.nearestX(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), enemylist.nearestY(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY()), Brotato.Strength + weapon.damage[5], 5);//发射弹幕
			}
			else {
				enemylist.hurt(Brotato.sprite->getPositionX(), Brotato.sprite->getPositionY(), (Brotato.range + weapon.range[5]), (Brotato.Strength + weapon.damage[5]));
			}
		}
	}
}

	//玩家受到伤害结算
void battle::playerHurt(float delta) {
	Brotato.hurt(enemylist.hit_damage());
	getCoin();
}

//拾取金币
void battle::getCoin() {
	int t = enemylist.collectCoin();
	Brotato.addMoney(t);
	Brotato.addExperience(t * 3);
	Sprite* p;
	for (int i = 0; i < 100; i++) {
		p = enemylist.newCoin[i];
		if (p == NULL)
			break;
		else {
			this->addChild(p);
		}
	}
}


//停止所有函数运行
void battle::stopSchedule() {
	this->unschedule(schedule_selector(battle::update_per_frame));
	this->unschedule(schedule_selector(battle::update_per_second));
	this->unschedule(schedule_selector(battle::playerHurt));
	this->unschedule(schedule_selector(battle::update_per_attack));
	this->unschedule(schedule_selector(battle::update_per_attack1));
	this->unschedule(schedule_selector(battle::update_per_attack2));
	this->unschedule(schedule_selector(battle::update_per_attack3));
	this->unschedule(schedule_selector(battle::update_per_attack4));
	this->unschedule(schedule_selector(battle::update_per_attack5));
	this->unschedule(schedule_selector(battle::generate_enemy));
	this->unschedule(schedule_selector(battle::generate_bullet));
}

//开始所有函数运行
void battle::startSchedule() {
	this->schedule(schedule_selector(battle::update_per_frame));

	//每秒要做的函数
	this->schedule(schedule_selector(battle::update_per_second), 1);

	//玩家受到伤害结算
	this->schedule(schedule_selector(battle::playerHurt), 0.2);
	this->schedule(schedule_selector(battle::update_per_attack), 1 / (Brotato.attackSpeed + weapon.attackSpeed[0]));
	this->schedule(schedule_selector(battle::update_per_attack1), 1 / (Brotato.attackSpeed + weapon.attackSpeed[1]));
	this->schedule(schedule_selector(battle::update_per_attack2), 1 / (Brotato.attackSpeed + weapon.attackSpeed[2]));
	this->schedule(schedule_selector(battle::update_per_attack3), 1 / (Brotato.attackSpeed + weapon.attackSpeed[3]));
	this->schedule(schedule_selector(battle::update_per_attack4), 1 / (Brotato.attackSpeed + weapon.attackSpeed[4]));
	this->schedule(schedule_selector(battle::update_per_attack5), 1 / (Brotato.attackSpeed + weapon.attackSpeed[5]));
	this->schedule(schedule_selector(battle::generate_enemy), 2);
	this->schedule(schedule_selector(battle::generate_bullet), 4);
}

// 波结束
bool battle::gameover() 
{
	if (Brotato.gameover())
	{
		wave++;
		Brotato.totalTime += 10;
		Brotato.countdown = Brotato.totalTime;
		enemylist.clear();
		stopSchedule();
		return true;
	}
	else
		return false;
}

// 局结束
int battle::totalOver()
{
	if (Brotato.dead()) // 角色死亡
	{
		return -1;
	}
	else if (wave == 5) // 战斗结束
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool battle::dead() 
{
	if (Brotato.dead())
		return true;
	else
		return false;
}

player* battle::getPlayer()
{
	player* p = &Brotato;
	return p;
}

void battle::achievement(float delta) {
	if (killTenFlag) {
		if (enemylist.killTen()) {
			achieve->setVisible(true);
			killTenFlag = false;
		}
	}
	else {
		achieve->setVisible(false);
		this->unschedule(schedule_selector(battle::achievement));
	}
	
}
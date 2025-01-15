#ifndef __BATTLE_H__
#define __BATTLE_H__

#include "cocos2d.h"
#include "player.h"
#include "enemy.h"
#include "weapon.h"
#include "Item.h"
using namespace cocos2d;

class battle :public Layer {
private:
	int wave;
	player Brotato;
	List enemylist;
	Weapon weapon;
	bool killTenFlag;
	Label* achieve;
	std::map<EventKeyboard::KeyCode, bool> keyMap;
public:
	Layer* createlayer();
	virtual bool init();
	void OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void OnKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void playermove(float delta);
	void playermove2(float delta);
	void playerHurt(float delta);
	void white(float delta);
	void update_per_frame(float delta);
	void update_per_second(float delta);
	void update_per_attack(float delta);
	void update_per_attack1(float delta);
	void update_per_attack2(float delta);
	void update_per_attack3(float delta);
	void update_per_attack4(float delta);
	void update_per_attack5(float delta);

	// 更新已购买物品的 UI 显示
	void updatePurchasedItemsUI();

	// 购买武器
	void purchaseWeapon(Item item);

	void generate_enemy(float delta);
	void generate_bullet(float delta);

	void set_weapon();
	void remove_weapon();
	void bullet(float X, float Y, float enemy_X, float enemy_Y, int damage,int i);
	void enemyhurt();

	void getCoin();

	bool gameover();
	void stopSchedule();
	void startSchedule();
	bool dead();

	void achievement(float delta);

	int totalOver();
	int getKillNum() const { return enemylist.getDeadNum(); }

	// 获取
	player* getPlayer();

	CREATE_FUNC(battle);
};


#endif 
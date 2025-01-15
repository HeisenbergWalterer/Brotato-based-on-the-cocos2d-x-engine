#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "cocos2d.h"
#include "player.h"
#include "enemy.h"
#include "CharacterSelectLayer.h"
#include "battle.h"
#include "GameOverLayer.h"
#include "ShopLayer.h"

using namespace cocos2d;

class home :public Scene {
private:
	battle* b;
	CharacterSelectLayer* CSL;
	ShopLayer* shopLayer;
	GameOverLayer* gameOverLayer;
	//player Brotato;
	//List enemylist;
	//std::map<EventKeyboard::KeyCode, bool> keyMap;
public:
	static Scene* createScene();
	virtual bool init();


	void test(float delta);
	void endCSL(float delta);
	void endWave(float delta);
	void endShop(float delta);

	void endGame(float delta);
	void retryGame(float delta);
	/*
	void OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void OnKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void playermove(float delta);
	void playermove2(float delta);

	void update_per_frame(float delta);
	void update_per_second(float delta);

	void generate_enemy(float delta);

	void test(float delta);
	*/
	CREATE_FUNC(home);

};

#endif
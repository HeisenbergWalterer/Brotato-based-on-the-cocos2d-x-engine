#ifndef SHOPLAYER_H
#define SHOPLAYER_H

#include "cocos2d.h"
#include "player.h"
#include <vector>

USING_NS_CC;

class ShopLayer : public Layer
{
private:
	player* p;   // 玩家对象
	Label* coinLabel;
	bool end;
	vector<Item> shopItems;

	vector<bool> itemPurchased;    // 当前商店中物品的购买状态

public:
	// 自定义创建场景函数，传入 player 参数
	static Scene* createScene(player* p);
	// 自定义初始化函数
	static ShopLayer* create(player* p);
	// 初始化函数
	virtual bool init(player* p);

	// 初始化商店界面
	void createShopUI();
	bool toEnd();
};


#endif

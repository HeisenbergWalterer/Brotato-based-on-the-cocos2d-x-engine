#include "cocos2d.h"
#include "ShopLayer.h"
#include "player.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* ShopLayer::createScene(player* p)
{
	auto scene = Scene::create();
	auto layer = ShopLayer::create(p); // 需要手动创建对象
	layer->init(p);
	scene->addChild(layer);

	return scene;
}

ShopLayer* ShopLayer::create(player* p)
{
	// 手动创建对象
	ShopLayer* layer = new (nothrow) ShopLayer();
	if (layer && layer->init(p))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool ShopLayer::init(player* p)
{
	if (!Layer::init())
	{
		return false;
	}
	this->p = p;
	end = false;

	// 加载物品
	p->loadItems();
	srand(time(0)); // 初始化随机种子

	bool weaponAdded = false; // 用于标记是否已经添加了武器
	while (shopItems.size() < 5)
	{
		int randomIndex = rand() % p->items.size();
		Item randomItem = p->items[randomIndex];

		// 检查是否已添加过武器
		if (randomItem.isWeapon && weaponAdded)
		{
			continue; // 如果已经有武器了，跳过该物品
		}

		shopItems.push_back(randomItem);

		// 如果是武器，更新标记
		if (randomItem.isWeapon)
		{
			weaponAdded = true;
		}
	}

	// 创建商店界面UI
	createShopUI();

	return true;
}

void ShopLayer::createShopUI()
{
	auto winSize = Director::getInstance()->getWinSize();

	// 显示玩家的当前金币
	coinLabel = Label::createWithTTF("Coins: " + std::to_string(p->getMoney()), "fonts/Marker Felt.ttf", 75);
	coinLabel->setPosition(Vec2(1280, winSize.height - 100));
	this->addChild(coinLabel);

	auto infoLabel = Label::createWithTTF("Shop", "fonts/Marker Felt.ttf", 100);
	infoLabel->setPosition(Vec2(960, winSize.height - 100));  // 设置标签位置为左上角
	this->addChild(infoLabel);

	auto background = Sprite::create("menu/background.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	background->setScale(1.2);
	this->addChild(background, -2);

	// 创建购买物品的按钮
	for (int i = 0; i < shopItems.size(); i++)
	{
		string itemImage = (shopItems[i].isWeapon ? "weapon/" : "Item/") + shopItems[i].name + (shopItems[i].isWeapon ? "_icon.png" : ".png");
		auto buyButton = ui::Button::create(itemImage, itemImage, itemImage);
		buyButton->setScale(1.0f); // 根据需要调整按钮的大小

		// 设置按钮位置（横向排列）
		float buttonSpacing = 350; // 按钮间距，可以根据需要调整
		float totalWidth = buttonSpacing * (shopItems.size() - 1); // 总间距：按钮间距 * (按钮数 - 1)
		float startX = (winSize.width - totalWidth) / 2; // 起始X坐标，确保按钮居中
		buyButton->setPosition(Vec2(startX + i * buttonSpacing, winSize.height - 400));

		// 创建标签，显示按钮的文本
		auto label = Label::createWithTTF(shopItems[i].name, "fonts/Marker Felt.ttf", 50);
		label->setPosition(Vec2(startX + i * buttonSpacing, winSize.height - 400 - buyButton->getContentSize().height / 2 - 30)); 

		// 创建标签，显示装备的属性加成
		string attributesText = "Cost: " + std::to_string(shopItems[i].cost) +
			"\nHealth: " + std::to_string(shopItems[i].health) +
			"\nStrength: " + std::to_string(shopItems[i].strength) +
			"\nAttack Speed: " + std::to_string(shopItems[i].attackSpeed) +
			"\nRange: " + std::to_string(shopItems[i].range) +
			"\nLife Regen: " + std::to_string(shopItems[i].lifeRegen);
		auto attributesLabel = Label::createWithTTF(attributesText, "fonts/Marker Felt.ttf", 32);
		attributesLabel->setPosition(Vec2(startX + i * buttonSpacing, winSize.height - 500 - buyButton->getContentSize().height / 2 - 150));

		buyButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				if (type == ui::Widget::TouchEventType::ENDED)
				{
					// 调用购买物品的逻辑
					if (p->buyItem(shopItems[i].id)) {
						coinLabel->setString("Coins: " + std::to_string(p->getMoney()));

						// 更新按钮状态
						buyButton->setEnabled(false);
						buyButton->setColor(Color3B(128, 128, 128)); // 使图片变暗
						buyButton->setTitleText("Sold");
					}
				}
			});
		this->addChild(buyButton);
		this->addChild(label);
		this->addChild(attributesLabel); // 添加装备属性标签
	}

	// 创建关闭商店按钮
	auto closeButton = ui::Button::create("menu/EndGame.png");
	closeButton->setScale(2.2f);
	closeButton->setPosition(Vec2(winSize.width - 80, winSize.height - 70));
	closeButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type)
		{
			if (type == ui::Widget::TouchEventType::ENDED)
			{
				end = true;
			}
		});
	this->addChild(closeButton);

}

bool ShopLayer::toEnd() { return end; }



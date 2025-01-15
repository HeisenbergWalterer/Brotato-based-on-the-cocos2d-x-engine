#include "GameOverLayer.h"
#include "CharacterSelectLayer.h"
#include <string>
#include "ui/CocosGUI.h"

using namespace std;
USING_NS_CC;

Scene* GameOverLayer::createScene(int playerWon)
{
	auto scene = Scene::create();
	auto layer = GameOverLayer::create(playerWon);
	layer->init(playerWon);
	scene->addChild(layer);

	return scene;
}

GameOverLayer* GameOverLayer::create(int playerWon)
{
	GameOverLayer* layer = new GameOverLayer();
	if (layer && layer->init(playerWon))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool GameOverLayer::init(int playeWon)
{
	if (!Layer::init())
	{
		return false;
	}
	end = false;

	auto winSize = Director::getInstance()->getWinSize();

	auto background = Sprite::create("menu/background.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	background->setScale(1.2);
	this->addChild(background, -2);

	// 创建结算label
	auto label = Label::createWithTTF(playeWon == 1 ? "You Win!" : "You Lose!", "fonts/Marker Felt.ttf", 80);
	label->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(label);
	
	// 创建按钮
	auto retryButton = ui::Button::create("menu/start.png", "menu/start.png", "menu/start.png");
	retryButton->setScale(2.2f);
	retryButton->setPosition(Vec2(winSize.width - 80, winSize.height - 70));
	//retryButton->setTitleText("Retry");
	retryButton->setTitleFontSize(24);
	retryButton->setPosition(Vec2(winSize.width - 50, winSize.height - 50));
	retryButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			if (type == ui::Widget::TouchEventType::ENDED)
			{
				end = true;
			}
		});
	this->addChild(retryButton);

	return true;
}

void GameOverLayer::showInfo(int n) {
	killNum = n;
	auto winSize = Director::getInstance()->getWinSize();
	auto label2 = Label::createWithTTF("KillNum:" + to_string(killNum), "fonts/Marker Felt.ttf", 80);
	label2->setPosition(Vec2(winSize.width / 2, winSize.height / 1.2));
	this->addChild(label2);
}

bool GameOverLayer::toEnd() { return end; }



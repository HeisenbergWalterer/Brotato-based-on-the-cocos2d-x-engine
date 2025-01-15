#include "CharacterSelectLayer.h"
//#include "GameLayer.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;

Scene* CharacterSelectLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = CharacterSelectLayer::create();
	scene->addChild(layer);

	return scene;
}

bool CharacterSelectLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();

	auto background = Sprite::create("menu/background.png");
	background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	background->setScale(1.2);
	this->addChild(background, -2);

	// 添加五个角色选择按钮
	for (int i = 0; i < 5; i++)
	{
		// 创建按钮，设置正常状态、选中状态和禁用状态的图片
		string ButtonImage = "character/character0" + to_string(i) + ".png";
		auto button = ui::Button::create(ButtonImage);
		button->setScale(1.0f); // 可根据需要调整按钮的大小

		// 设置按钮位置
		float buttonSpacing = 300; // 按钮间距，可以根据需要调整
		float totalWidth = buttonSpacing * (5 - 1); // 总间距：按钮间距 * (按钮数 - 1)
		float startX = (winSize.width - totalWidth) / 2; // 起始X坐标，确保按钮居中
		button->setPosition(Vec2(startX + i * buttonSpacing, winSize.height / 2));
		
		// 创建标签显示按钮的文本
		char buttonText[16];
		sprintf(buttonText, "Character %d", i + 1);
		auto label = Label::createWithTTF(buttonText, "fonts/Marker Felt.ttf", 50);
		label->setPosition(Vec2(startX + i * buttonSpacing, winSize.height / 2 - button->getContentSize().height / 2 - 30)); // 设置标签在按钮下方

		// 添加触摸事件监听器
		button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED)
			{
				// 调用角色选择的处理逻辑
				this->SelectCharacter(i);
				end = false;
			}

			});

		// 将按钮添加到当前层
		this->addChild(button);
		// 将标签添加到当前层
		this->addChild(label);
	}

	// 创建左上角的功能信息标签
	auto infoLabel = Label::createWithTTF("Character Select", "fonts/Marker Felt.ttf", 100);
	infoLabel->setPosition(Vec2(960, winSize.height - 100));  // 设置标签位置为左上角
	this->addChild(infoLabel);

	return true;
}

void CharacterSelectLayer::SelectCharacter(int characterIndex)
{
	// 将所选角色信息传递给 GameLayer
	UserDefault::getInstance()->setIntegerForKey("SelectedCharacter", characterIndex); // 保存所选择的角色索引
	//auto scene = GameLayer::createScene();
	//Director::getInstance()->replaceScene(scene);
}
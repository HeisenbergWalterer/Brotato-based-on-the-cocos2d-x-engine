#include "cocos2d.h"
#include "player.h"
#include "enemy.h"
#include <string>
#include <cmath>
using namespace cocos2d;
using namespace std;



//void player::test() {
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//	enemy = Sprite::create("buildings/houses.png");
//	enemy->setPosition(Vec2(origin.x + visibleSize.width / 3, origin.y + visibleSize.height * 0.3));
//	enemy->setScale(2.5);
//}

void player::init() 
{

	FULLHP = init_FULLHP;
	HP = init_FULLHP;
	Level = 1;
    totalTime = 20;
    countdown = totalTime;
	speed = 10;
	Strength = 10;
	attackSpeed = 0; // 每秒攻击次数
	range = 0;
	lifeRegen = 2.0f;   // 每秒恢复2点生命
	experience = 0;
	money = 0;

    toBuyItem = true;
    toBuyWeapon = false;
    ifBuyWeapon = false;

    regenCooldown = 1.0f;

	initPlayerAttributes();
}

int player::getCharacterIndex()
{
    // 根据玩家选择初始化角色
    int selectedCharacter = UserDefault::getInstance()->getIntegerForKey("SelectedCharacter", 0);
    return selectedCharacter;
}

void player::initPlayerAttributes()
{
    switch (getCharacterIndex())
    {
    case 0: // 角色 1
        HP = 100;
        Strength = 10;
        speed = 12;
        //attackSpeed = 1.0f;
        //range = 100.0f;
        lifeRegen = 5.0f;
        break;
    case 1: // 角色 2
        HP = 120;
        Strength = 8;
        speed = 10;
        //attackSpeed = 1.2f;
        //range = 120.0f;
        lifeRegen = 3.0f;
        break;
    case 2: // 角色 3
        HP = 80;
        Strength = 12;
        speed = 15;
        //attackSpeed = 0.8f;
        //range = 80.0f;
        lifeRegen = 2.0f;
        break;
    case 3: // 角色 4
        HP = 150;
        Strength = 8;
        speed = 8;
        //attackSpeed = 1.5f;
        //range = 150.0f;
        lifeRegen = 10.0f;
        break;
    case 4: // 角色 5
        HP = 90;
        Strength = 12;
        speed = 10;
        //attackSpeed = 1.1f;
        //range = 90.0f;
        lifeRegen = 4.0f;
        break;
    default:
        HP = 100;
        Strength = 10;
        speed = 10;
        //attackSpeed = 1.0f;
        //range = 100.0f;
        lifeRegen = 5.0f;
        break;
    }
}

void player::checkCollisions(vector<Node*>& coins)
{
    // 获取玩家的边界框
    Rect playerBoundingBox = sprite->getBoundingBox();

    // 遍历所有金币
    for (auto it = coins.begin(); it != coins.end();)
    {
        // 获取当前金币的边界框
        Rect coinBoundingBox = (*it)->getBoundingBox();

        // 检查玩家与当前金币是否发生碰撞
        if (playerBoundingBox.intersectsRect(coinBoundingBox))
        {
            // 玩家拾取金币，增加金币数量
            addMoney(1);
            (*it)->removeFromParent(); // 从场景中移除当前金币
            it = coins.erase(it); // 使用erase返回新的迭代器，以继续遍历下一个元素
        }
        else
        {
            it++;
        }
    }
}

bool player::dead() 
{
	if (HP <= 0) 
		return true;
	else
		return false;
}

bool player::gameover() {
	if (countdown <= 0)
		return true;
	else
		return false;
}

void player::createPlayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 根据用户选择加载不同的角色图片
    string playerImage;
    switch (getCharacterIndex())
    {
    case 0:
        playerImage = "character/character00.png"; // 角色1的图片
        break;
    case 1:
        playerImage = "character/character01.png"; // 角色2的图片
        break;
    case 2:
        playerImage = "character/character02.png"; // 角色3的图片
        break;
    case 3:
        playerImage = "character/character03.png"; // 角色3的图片
        break;
    case 4:
        playerImage = "character/character04.png"; // 角色3的图片
        break;
    default:
        playerImage = "character/character00.png"; // 默认角色图片
        break;
    }
    sprite = Sprite::create(playerImage);
    sprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7));
    sprite->setScale(1);
    init();
}

void player::createInfo()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建标签内容
    label = Label::createWithTTF("HP: " + to_string(HP) + "\n" +
        "Level: " + to_string(Level) + "\n" +
        "Time: " + to_string(countdown) + "\n" +
        "Money: " + to_string(money) + "\n\n\n" +
        "FULLHP: " + to_string(FULLHP) + "\n" +
        "Strength: " + to_string(Strength) + "\n" +
        "AttackSpeed: " + to_string(attackSpeed) + "\n" +
        "Range: " + to_string(range) + "\n" +
        "LifeRegen: " + to_string(lifeRegen),
        "fonts/Marker Felt.ttf", 40);

    label->setPosition(Vec2(origin.x + visibleSize.width * 0.1, origin.y + visibleSize.height * 0.7));
}

void player::showInfo() {
    label->setString("HP: " + to_string(HP) + "\n" +
        "Level: " + to_string(Level) + "\n" +
        "Time: " + to_string(countdown) + "\n" +
        "Money: " + to_string(money) + "\n\n\n" +
        "FULLHP: " + to_string(FULLHP) + "\n" +
        "Strength: " + to_string(Strength) + "\n" +
        "AttackSpeed: " + to_string(attackSpeed) + "\n" +
        "Range: " + to_string(range) + "\n" +
        "LifeRegen: " + to_string(lifeRegen));
}

void player::hurt(int total_damage) {
	HP -= total_damage;
    if (total_damage > 0)
        sprite->setColor(Color3B::RED);
    else
        sprite->setColor(Color3B::WHITE);
}

void player::addMoney(int amount)
{
    money += amount;
}

void player::regenerateHealth(float deltaTime)
{
    regenCooldown -= deltaTime;
    if (regenCooldown <= 0)
    {
        HP += lifeRegen;
        if (HP > FULLHP) HP = FULLHP;
        regenCooldown = 1.0f;
    }
}

void player::addExperience(int exp)
{
    experience += exp;
    if (experience >= Level * 100)
    {
        levelUp();
    }
}

void player::levelUp()
{
    Level++;
    FULLHP += 5;
    Strength += 1;
    attackSpeed += 0.1f;
    HP = FULLHP; // 升级恢复生命
}

void player::loadItems()
{
    items.push_back(Item(1, "acid_icon", 15, 8, 0, -0.02f, 0, 0.0f));  
    items.push_back(Item(2, "adrenaline_icon", 30, 0, 0, 0.05f, 0, 0.0f)); 
    items.push_back(Item(3, "alien_baby_icon", 20, 15, 0, 0, 0, 0.0f)); 
    items.push_back(Item(4, "alien_magic_icon", 20, 0, 3, 0, 0, 0.0f));
    items.push_back(Item(5, "alien_tongue_icon", 25, 8, 0, 0, 3, 0.0f));
    items.push_back(Item(6, "alien_worm_icon", 25, 0, 0, 0, 0, 1.0f));
    items.push_back(Item(7, "ball_and_chain_icon", 15, 3, 0, 0, 2, 0.0f));
    items.push_back(Item(8, "bandana_icon", 30, 0, 3, 0, 0, 0.0f));
    items.push_back(Item(9, "banner_icon", 50, 0, 0, 0, 50, 5.0f));
    items.push_back(Item(10, "bat_icon", 22, 0, 0, 0.1, 0, 0.0f));
    items.push_back(Item(11, "bean_teacher_icon", 18, 0, 3, 0.1, 0, 0.0f));
    items.push_back(Item(12, "big_arms_icon", 35, 0, 0, 0, 100, 0.0f));
    items.push_back(Item(13, "black_belt_icon", 15, 10, 0, 0, 0, 0.0f));
    items.push_back(Item(14, "blood_donation_icon", 25, 0, 8, 0, 0, 0.0f));
    items.push_back(Item(15, "bowler_hat_icon", 45, 0, 5, 0.05f, 5, 0.0f));
    items.push_back(Item(16, "boxing_glove_icon", 45, 0, 0, 0.1f, 0, 0.0f));
    items.push_back(Item(17, "butterfly_icon", 55, 0, 0, 0.1f, 0, 0.0f));
    items.push_back(Item(18, "cake_icon", 25, 0, 0, 0, 3, 0.0f));
    items.push_back(Item(19, "chameleon_icon", 20, 0, 2, 0, 0, 0.0f));
    items.push_back(Item(20, "anvil_icon", 30, 0, 0, 0, 0, 0.0f));

    items.push_back(Item(21, "cactus_mace", 30, 0, 50, 0.5, 200, 0.0f, true, false));
    items.push_back(Item(22, "chopper", 30, 0, 25, 1.5, 200, 0.0f, true, false));
    items.push_back(Item(23, "claw", 30, 0, 30, 1, 200, 0.0f, true, false));
    items.push_back(Item(24, "double_barrel_shotgun", 30, 0, 90, 0.3, 800, 0.0f, true, true));
    items.push_back(Item(25, "medical_gun", 30, 0, 15, 1, 800, 5.0f, true, true));
    items.push_back(Item(26, "minigun", 30, 0, 5, 2, 800, 0.0f, true, true));
    items.push_back(Item(27, "revolver", 30, 0, 50, 0.5, 800, 0.0f, true, true));
}

void player::updateAttribute(int health, int strength, float attackSpeed_, int range_, float lifeRegen_)
{
    FULLHP += health;
    Strength += strength;
    attackSpeed += attackSpeed_;
    range += range_;
    lifeRegen += lifeRegen_;
}

bool player::buyItem(int itemId)
{
    Item item = items[itemId - 1]; // 获取物品

    if (money >= item.cost)
    {
        if (item.isWeapon)
        {
            toBuyWeapon = true;
            currentItem = item;  // 保存当前选中的武器

        }
        else {
            // 常规物品，更新玩家属性
            updateAttribute(item.health, item.strength, item.attackSpeed, item.range, item.lifeRegen);
        }
        money -= item.cost; // 扣除金币
        purchasedItems.push_back(item);  // 将物品添加到已购买的列表中
        toBuyItem = true;

        return true;
    }
    return false;
}


int player::getAttackSpeed() const { return attackSpeed; }
int player::getLifeRegen() const { return lifeRegen; }
int player::getMoney() const { return money; }

bool player::getToBuyWeapon() const { return toBuyWeapon; }
Item player::getCurrentItem() const { return currentItem; }  // 获取当前选中的物品

void player::updateIfBuyWeapon()
{
    ifBuyWeapon = !ifBuyWeapon;
}
void player::updateToBuyWeapon()
{
    toBuyWeapon = !toBuyWeapon;
}
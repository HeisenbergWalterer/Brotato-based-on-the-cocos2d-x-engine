#include "cocos2d.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"
#include <string>
#include <cmath>
using namespace cocos2d;
using namespace std;
#define L 25

Weapon::Weapon() {
	newbullet = NULL;
}

void Weapon::init(float x,float y){
	last = new bullet(-1, -1, -1, -1, 0, NULL, 0,0);
	first = new bullet(-1, -1, -1, -1, 0, last, 0,0);

	num[0] = 1;
	for (int i = 1; i < 6; i++)
	{
		num[i] = 0;
	}

	int selectedCharacter = UserDefault::getInstance()->getIntegerForKey("SelectedCharacter", 0);
	switch (selectedCharacter)
	{
	case 0:
	case 1:
	case 2:
		items[0].id = 22;
		items[0].name = "chopper";
		items[0].cost = 30;
		items[0].health = 0;
		items[0].strength = 25;
		items[0].attackSpeed = 1.5f;
		items[0].range = 200;
		items[0].isWeapon = true;
		items[0].isRanged = false;
		break;
	case 3:
	case 4:
		items[0].id = 25;
		items[0].name = "medical_gun";
		items[0].cost = 30;
		items[0].health = 0;
		items[0].strength = 15;
		items[0].attackSpeed = 1;
		items[0].range = 800;
		items[0].isWeapon = true;
		items[0].isRanged = true;
		break;
	}

	create(x, y);
}

bool Weapon::addWeapon(const Item& item)
{
	// 查找空的武器槽
	for (int i = 1; i < 6; i++)
	{
		if (num[i] == 0)
		{
			// 找到空槽，添加武器
			num[i] = 1;
			items[i] = item;
			return true;
		}
	}
	//// 如果没有空槽，检查是否有相同武器
	//for (int i = 0; i < 6; i++) 
	//{
	//	if (num[i] == type) 
	//	{
	//		// 升级武器
	//		upgradeWeapon(i);
	//		return true;
	//	}
	//}
	//// 如果武器槽已满且没有相同武器，返回失败
	//return false;

	// 如果所有槽都满且没有匹配的武器，则购买失败
	return false;
}


void Weapon::select_data(int i) {
	// .................................................
	damage[i] = items[i].strength;
	attackSpeed[i] = items[i].attackSpeed;
	range[i] = items[i].range;
	isRanged[i] = items[i].isRanged;
}

Sprite* Weapon::select_image(int i) {
	string weaponImage = "weapon/" + items[i].name + ".png";
	return Sprite::create(weaponImage);
}

void Weapon::create(float x, float y) {
	for (int i = 0; i < 6; i++) 
	{
		if (num[i]) 
		{
			select_data(i);
			Sprite* p = select_image(i);
			weapon[i] = p;
			switch (i) 
			{
			case 0:
				weapon[i]->setPosition(x - L, y + L * sqrt(3));
				break;
			case 1:
				weapon[i]->setPosition(x - 2 * L, y);
				break;
			case 2:
				weapon[i]->setPosition(x - L, y - L * sqrt(3));
				break;
			case 3:
				weapon[i]->setPosition(x + L, y - L * sqrt(3));
				break;
			case 4:
				weapon[i]->setPosition(x + 2 * L, y);
				break;
			case 5:
				weapon[i]->setPosition(x + L, y + L * sqrt(3));
				break;
			default:
				weapon[i]->setPosition(x, y);
				break;
			}
			weapon[i]->setScale(1);
		}
	}
}

void Weapon::generate_bullet(float X, float Y, float enemy_X, float enemy_Y, int damage, int i) {
	bullet* new_bullet = new bullet(X, Y, enemy_X, enemy_Y, damage, first->link, i,items[i].id);
	first->link = new_bullet;
	newbullet = new_bullet->bullet_s;
}

void Weapon::bullet_move() {
	bullet* p = first->link;
	while (p != last) {
		p->move();
		p = p->link;
	}
}

bullet* Weapon::bullet_hit() {
	bullet* p = first->link;
	while (p != last) {
		if (p->hit()) {
			return p;
		}
		p = p->link;
	}
	return p;
}

void Weapon::bullet_clear(bullet* k) {
	bullet* p = first,* q;
	while (p != last) {
		if (p==k) {
			q->link = p->link;
			break;
		}
		q = p;
		p = p->link;
	}
}

bullet::bullet(float X, float Y, float enemy_X, float enemy_Y,int damage, bullet* next, int i,int id) {
	bullet_damage = damage;
	link = next;
	enemy_x = enemy_X;
	enemy_y = enemy_Y;
	bullet_s = select_bullet(i,id);
	switch (i) {
	case 0:
		bullet_s->setPosition(X - L, Y + L * sqrt(3));
		break;
	case 1:
		bullet_s->setPosition(X - 2 * L, Y);
		break;
	case 2:
		bullet_s->setPosition(X - L, Y - L * sqrt(3));
		break;
	case 3:
		bullet_s->setPosition(X + L, Y - L * sqrt(3));
		break;
	case 4:
		bullet_s->setPosition(X + 2 * L, Y);
		break;
	case 5:
		bullet_s->setPosition(X + L, Y + L * sqrt(3));
		break;
	default:
		bullet_s->setPosition(X, Y);
		break;
	}
	bullet_s->setScale(1);
	x = bullet_s->getPositionX();
	y = bullet_s->getPositionY();
	int dx = enemy_x - x;
	int dy = enemy_y - y;
	if (dx == 0 && dx == 0) {
		direction_x = 0.6;
		direction_y = 0.6;
	}
	else {
		//单位化每个方向向量
		direction_x = dx / (sqrt(dx * dx + dy * dy));
		direction_y = dy / (sqrt(dx * dx + dy * dy));
	}

}

Sprite* bullet::select_bullet(int i,int id) {
	Sprite* bulletSprite = nullptr;

	if (id == 24) {
		bulletSprite = Sprite::create("weapon/bullet_shredder_0000.png");
		bulletSprite->setScale(0.5f);
	}
	else if (id == 25) {
		bulletSprite = Sprite::create("weapon/bullet_medical.png");
	}
	else if (id == 26) {
		bulletSprite = Sprite::create("weapon/frame0001.png");
	}
	else {
		bulletSprite = Sprite::create("weapon/bullet_gun.png");
	}
	// 调整弹幕精灵的大小为原来的 0.5 倍

	return bulletSprite;
}

void bullet::move() {
	bullet_s->setPositionX(bullet_s->getPositionX() + 20 * direction_x);
	bullet_s->setPositionY(bullet_s->getPositionY() + 20 * direction_y);
}

bool bullet::hit() {
	if (bullet_s->getPositionX() <= enemy_x + 10 && bullet_s->getPositionX() >= enemy_x - 10 && bullet_s->getPositionY() <= enemy_y + 10 && bullet_s->getPositionY() >= enemy_y - 10) {
		return true;
	}
	else {
		return false;
	}
}
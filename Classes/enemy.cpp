#include "cocos2d.h"
#include "enemy.h"
#include <string>
#include <cmath>
#include <ctime>
#include <vector>
#include <random>
using namespace cocos2d;
using namespace std;

float distance(float x1, float y1, float x2, float y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int randType() {   
	srand(time(0));
	int enemyType;
	int r = rand() % 10;
	if (r >= 0 && r <= 1)
		enemyType = FIGHTER;
	else if (r >= 2 && r <= 3)
		enemyType = BUTCHER;
	else if (r >= 4 && r <= 7)
		enemyType = ARCHER;
	else
		enemyType = ELITE;
	return enemyType;
}

enemy::enemy(float myX, float myY, float playerX, float playerY, enemy* l, int Type) {
	//初始化参数
	dead = false;
	x = myX;
	y = myY;
	player_x = playerX;
	player_y = playerY;
	direction_x = -1;
	direction_y = -1;
	link = l;
	enemyType = Type;

	//生成敌人sprite
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (enemyType == FIGHTER) {
		HP = 100;
		damage = 5;
		speed = 4;
		x = rand() % int(visibleSize.width);
		y = rand() % int(visibleSize.height);
		sprite = Sprite::create("enemy/bruiser.png");
		sprite->setPosition(x, y);
		sprite->setScale(1);
	}
	else if (enemyType == BUTCHER) {
		HP = 60;
		damage = 8;
		speed = 5;
		x = rand() % int(visibleSize.width);
		y = rand() % int(visibleSize.height);
		sprite = Sprite::create("enemy/butcher.png");
		sprite->setPosition(x, y);
		sprite->setScale(1);
	}
	else if (enemyType == ARCHER) {
		HP = 50;
		damage = 1;
		speed = 2;
		x = rand() % int(visibleSize.width);
		y = rand() % int(visibleSize.height);
		sprite = Sprite::create("enemy/baby_alien.png");
		sprite->setPosition(x, y);
		sprite->setScale(1);
	}
	else if (enemyType == ELITE) {
		HP = 75;
		damage = 1;
		speed = 2;
		x = rand() % int(visibleSize.width);
		y = rand() % int(visibleSize.height);
		sprite = Sprite::create("enemy/buffer.png");
		sprite->setPosition(x, y);
		sprite->setScale(1);
	}
	else {
		HP = 1;
		damage = 3;
		speed = 4;
		sprite = Sprite::create("enemy/archerBullet.png");
		sprite->setPosition(myX, myY);
		sprite->setScale(1);
	}
}

void enemy::move() {
	if (!dead) {
		sprite->setPositionX(sprite->getPositionX() + speed * direction_x);
		sprite->setPositionY(sprite->getPositionY() + speed * direction_y);
	}
}

List::List() {
	last = new enemy(-1, -1, -1, -1, NULL, -1);
	first = new enemy(-1, -1, -1, -1, last, -1);
	coinLast = new coin(-1, -1, NULL);
	coinFirst = new coin(-1, -1, coinLast);
	deadNum = 0;
	for (int i = 0; i < 100; i++)
		newBullet[i] = NULL;
	for (int i = 0; i < 100; i++)
		newCoin[i] = NULL;
}

void List::update(float x, float y) {
	update_playerlocation(x, y);
	update_mylocation();
	update_direction();
}

void List::update_playerlocation(float x, float y) {  //记录玩家的位置
	playerX = x;
	playerY = y;
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			p->player_x = x;
			p->player_y = y;
		}
		p = p->link;
	}
}

void List::update_mylocation() {  //记录自己的位置
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			p->x = p->sprite->getPositionX();
			p->y = p->sprite->getPositionY();
		}
		p = p->link;
	}
}

void List::update_direction() {
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			if (p->enemyType != BULLET) {
				int X = p->player_x - p->x;
				int Y = p->player_y - p->y;
				if (X == 0 && Y == 0) {
					p->direction_x = 0.6;
					p->direction_y = 0.6;
				}
				else {
					//单位化每个方向向量
					p->direction_x = X / (sqrt(X * X + Y * Y));
					p->direction_y = Y / (sqrt(X * X + Y * Y));
				}
			}
		}
		p = p->link;
	}
}

int List::hit_damage() {  //计算对玩家造成的伤害
	int total_damage = 0;
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			if (p->enemyType == FIGHTER || p->enemyType == BUTCHER||p->enemyType == BULLET) {
				if (distance(p->x, p->y, p->player_x, p->player_y) < 60) {
					total_damage += p->damage;
					//p->sprite->setVisible(false);
				}
			}
		}
		p = p->link;
	}
	return total_damage;
}

Sprite* List::generate_enemy() {   //生成敌人
	enemy* new_enemy = new enemy(900, 900, playerX, playerY, first->link, randType());
	first->link = new_enemy;
	return new_enemy->sprite;
}

void List::generate_bullet() {
	for (int i = 0; i < 100; i++)
		newBullet[i] = NULL;
	int pos = 0;
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			if (p->enemyType == ARCHER) {
				enemy* new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				//确定子弹行进方向
				int X = p->player_x - p->x;
				int Y = p->player_y - p->y;
				new_enemy->direction_x = X / (sqrt(X * X + Y * Y));
				new_enemy->direction_y = Y / (sqrt(X * X + Y * Y));

				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;
			}
			if (p->enemyType == ELITE) {
				enemy* new_enemy;
				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = 0.6;
				new_enemy->direction_y = 0.6;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;

				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = 0.6;
				new_enemy->direction_y = -0.6;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;

				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = -0.6;
				new_enemy->direction_y = 0.6;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;

				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = -0.6;
				new_enemy->direction_y = -0.6;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;

				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = 0;
				new_enemy->direction_y = 0.85;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;

				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = 0.85;
				new_enemy->direction_y = 0.0;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;

				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = 0;
				new_enemy->direction_y = -0.85;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;

				new_enemy = new enemy(p->x, p->y, playerX, playerY, first->link, BULLET);
				new_enemy->direction_x = -0.85;
				new_enemy->direction_y = 0;
				first->link = new_enemy;
				newBullet[pos] = new_enemy->sprite;
				pos++;
			}
		}
		
		p = p->link;
	}
}

void List::move() {
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			p->move();
		}
		p = p->link;
	}
}

float List::nearestDistance(float X,float Y) {//寻找最近的敌人
	float nearest = 99999;
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			if (p->enemyType != BULLET) {
				float d = distance(p->x, p->y, X, Y);
				if (d < nearest)
					nearest = d;
			}
		}
		p = p->link;
	}
	return nearest;
}

float List::nearestX(float X, float Y) {
	enemy* p = first->link;
	float nearest = nearestDistance(X,Y);
	while (p != last) {
		if (distance(p->x, p->y,X, Y) == nearest) {
			if (p->enemyType != BULLET) {
				return p->x;
			}
		}
		p = p->link;
	}
}

float List::nearestY(float X, float Y) {
	enemy* p = first->link;
	float nearest = nearestDistance(X,Y);
	while (p != last) {
		if (distance(p->x, p->y, X, Y) == nearest) {
			if (p->enemyType != BULLET) {
				return p->y;
			}
		}
		p = p->link;
	}
}

void List::hurt(float X, float Y,int range,int damage) {//敌人受伤
	enemy* p = first->link;
	float nearest = nearestDistance(X,Y);
	
	if (nearest <= range) {
		while (p != last) {
			if (distance(p->x, p->y, X, Y) == nearest) {
				if (p->enemyType != BULLET) {
					p->HP -= damage;
					p->sprite->setColor(Color3B::RED);
					if (p->HP <= 0) {
						p->dead = true;
						deadNum++;
						p->sprite->setVisible(false);                  //敌人死亡
						
						coin* newCoin = new coin(p->x, p->y, coinFirst->link);
						coinFirst->link = newCoin;
					}
					break;
				}
			}
			p = p->link;
		}
	}
}

void List::white() {
	enemy* p = first->link;
	while (p != last) {
		if (!p->dead) {
			if (p->enemyType != BULLET) {
				p->sprite->setColor(Color3B::WHITE);
			}
		}
		p = p->link;
	}
}

coin::coin(float x, float y, coin* next) {
	coinX = x;
	coinY = y;
	collected = false;
	newGenerated = true;
	link = next;
	coinSprite = Sprite::create("Enemy/coin.png");
	coinSprite->setPosition(x, y);
	coinSprite->setScale(1);
}


//拾取金币
int List::collectCoin() {
	for (int i = 0; i < 100; i++)
		newCoin[i] = NULL;
	int pos = 0;
	int totalCoin = 0;  
	
	coin* p = coinFirst->link;
	while (p != coinLast) {
		if (!p->collected) {
			if (p->newGenerated) {
				p->coinSprite = Sprite::create("Enemy/coin.png");
				p->coinSprite->setPosition(p->coinX, p->coinY);
				p->coinSprite->setScale(1);
				newCoin[pos] = p->coinSprite;
				pos++;
				p->newGenerated = false;
			}
			
			if (distance(p->coinSprite->getPositionX(), p->coinSprite->getPositionY(), playerX, playerY) <= 100) {
				totalCoin += 6;
				p->coinSprite->setVisible(false);
				p->collected = true;
			}
			
		}
		
		p = p->link;
	}
	

	return totalCoin;
}


//清屏
void List::clear() {
	enemy* p = first->link;
	while (p != last) {
		p->sprite->setVisible(false);
		p->dead = true;

		p = p->link;
	}

	coin* t = coinFirst->link;
	while (t != coinLast) {
		t->coinSprite->setVisible(false);
		t->collected = true;

		t = t->link;
	}
}

bool List::killTen() {
	if (deadNum == 10)
		return true;
	else
		return false;
}
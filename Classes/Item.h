#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>

using namespace std;

class Item
{
public:
    int id;               // 物品ID
    string name;          // 物品名称
    int cost;             // 物品价格
    int health;           // 生命值加成
    int strength;         // 力量加成
    float attackSpeed;    // 攻击速度加成
    int range;            // 攻击范围加成
    float lifeRegen;    // 生命恢复加成

    bool isWeapon;      // 是否为武器
    bool isRanged;     // 武器类型（刀/枪）

    Item(int id = 0, string name = "", int cost = 0, int health = 0, int strength = 0, float attackSpeed = 0.0f,
        int range = 0, float lifeRegen = 0.0f, bool isWeapon = false, bool isRanged = false)
        : id(id), name(name), cost(cost), health(health), strength(strength), attackSpeed(attackSpeed), range(range), lifeRegen(lifeRegen), isWeapon(isWeapon), isRanged(isRanged) {}

    // 重载函数调用运算符
    void operator()(int id, string name, int cost, int health, int strength,
        float attackSpeed, int range, float lifeRegen, bool isWeapon, bool isRanged)
    {
        this->id = id;
        this->name = name;
        this->cost = cost;
        this->health = health;
        this->strength = strength;
        this->attackSpeed = attackSpeed;
        this->range = range;
        this->lifeRegen = lifeRegen;
        this->isWeapon = isWeapon;
        this->isRanged = isRanged;
    }

};


#endif 

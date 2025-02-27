# Brotato 游戏项目

2024同济大学程序设计范式期末大作业

------



## 项目名称

Brotato

## 项目简介

该项目是一个基于 [cocos2d-x 3.17.2](https://docs.cocos.com/cocos2d-x/manual/) 开发的 roguelike 动作射击游戏。 在这个游戏中，玩家将控制一个名为 Brotato 的角色，在敌人不断涌现的环境中生存并进行战斗。玩家可以通过选择不同的角色和武器，在游戏过程中升级和调整策略，击败越来越强大的敌人。

## 成员分工

| 姓名   | 学号    | 分工                                                         |
| ------ | ------- | ------------------------------------------------------------ |
| 姜政言 | 2353594 | Battle层、player类中和战斗相关部分、Enemy类、主场景home类 |
| 朱会佳 | 2353922 | CharacterSelectLayer层、player类中和武器装备相关部分、ShopLayer层、GameOverLayer层，Item类 |
| 汤诚阳 | 2350434 | Weapon类 |

## 成员贡献

| 姓名   | 贡献量 |
| ------ | ------ |
| 姜政言 | 35%    |
| 朱会佳 | 35%    |
| 汤诚阳 | 30%    |

## 项目组成

- `/Classes` 类定义与类实现
- `/Resources` 资源文件
- `Brotato.msi` 游戏程序安装文件

## 项目实现功能

有初始界面，支持5种人物选择

支持背景音效

支持近战、远程等多种武器类型

包含20余中局内装备

包含近战、远程、精英等至少4种敌人

支持玩家自动瞄准攻击处在攻击范围内最近的敌人

支持敌人自动向玩家移动并攻击玩家

支持玩家/敌人的受击动画显示

支持玩家同时持有多把武器

支持局内物品栏显示（显示现有装备及武器）

支持玩家状态栏显示（包括当前属性，金币）

敌人死亡掉落金币（每波战斗结束后，可以在商店用金币购买武器及装备）

支持角色升级功能（玩家击败敌人会获得经验值，每升一级可获得属性加成）

每局游戏结束显示结算画面，显示击杀数等数据

支持成就系统

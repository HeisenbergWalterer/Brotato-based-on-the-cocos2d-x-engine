#ifndef GAMEOVERLAYER_H
#define GAMEOVERLAYER_H

#include "cocos2d.h"
USING_NS_CC;

class GameOverLayer : public Layer 
{
private:
    bool end;
    int killNum;

public:
    static Scene* createScene(int playerWon);
    static GameOverLayer* create(int playerWon);
    virtual bool init(int playerWon);

    bool toEnd();
    void showInfo(int n);
};

#endif
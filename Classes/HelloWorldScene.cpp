/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Home.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "menu/EndGame.png",
        "menu/EndGame.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width * 0.6 - closeItem->getContentSize().width / 2;
        float y = origin.y + visibleSize.height * 0.3 + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
        closeItem->setScale(2.5);
    }

    auto startItem = MenuItemImage::create(
        "menu/Start.png",
        "menu/Start.png",
        CC_CALLBACK_1(HelloWorld::menuStartCallback, this));
    float x = origin.x + visibleSize.width * 0.4 - startItem->getContentSize().width / 2;
    float y = origin.y + visibleSize.height * 0.3 + startItem->getContentSize().height / 2;
    startItem->setPosition(Vec2(x, y));
    startItem->setScale(2.5);

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, startItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    /*
    auto label = Label::createWithTTF("Star Valley", "fonts/Marker Felt.ttf", 30);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("buildings/Barn.png");
    if (sprite == nullptr)
    {
        problemLoading("'buildings/Barn.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        //sprite->setScale(3);
        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    */

    auto background = Sprite::create("menu/background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    background->setScale(1.2);
    this->addChild(background, -2);

    auto title = Sprite::create("menu/logo.png");
    title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height * 0.7 + origin.y));
    title->setScale(1);
    this->addChild(title, -1);

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("bgm/helloworld.wav", true);


    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::menuStartCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    auto scene_home = home::createScene();
    Director::getInstance()->replaceScene(CCTransitionFade::create(0.8f, scene_home));

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
#ifndef __CHARACTERSELECTLAYER_H__
#define __CHARACTERSELECTLAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class CharacterSelectLayer : public Layer
{
public:
	bool end = true;
	static Scene* createScene();
	virtual bool init();
	void SelectCharacter(int characterIndex);

	CREATE_FUNC(CharacterSelectLayer);
};

#endif 
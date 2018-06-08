#ifndef LOADING_SCENE__H_
#define LOADING_SCENE__H_
#include "cocos2d.h"
#include"BaseLayer.h"
USING_NS_CC;
class loadingScene : public cocos2d::Layer 
{
public:
	//cocos2d::TMXTiledMap* tilemap;
 
	static cocos2d::Scene*createScene();
	virtual bool init();
	CREATE_FUNC(loadingScene);
	void onClickCallBack(Ref* pSender);
};





#endif
#ifndef __login_Scnen__H__
#define __login_Scene__H__
#include"choose_to_beScene.h"
#include"cocos2d.h"
#include"ui\CocosGUI.h"
#include"cocos-ext.h"
#include"choose_to_beScene.h"
USING_NS_CC;
USING_NS_CC_EXT;
class loginScene :public cocos2d::Layer,public EditBoxDelegate
{
	int             map_index;     
	std::string     _playerName;    
	     
	MenuItemLabel *        _pleaseStartButton;
	
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	//void menuCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(loginScene);
	void OKCallback(Ref* pSender);
	void backBack(Ref* pSender);
	void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	void editBoxEditingDidBegin(EditBox* editBox);
	void editBoxEditingDidEnd(EditBox* editBox);
	void editBoxTextChanged(EditBox* editBox, const std::string& text);
	void editBoxReturn(EditBox* editBox);
};





#endif
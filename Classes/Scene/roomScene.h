#ifndef __ROOM_SCENE__H_
#define __ROOM_SCENE__H_
#include"cocos2d.h"
#include<vector>
#include<tuple>
#include<string>
#include"my_Network\client.h"
//#include"my_Network\client.h"
#include"ui\CocosGUI.h"
#include"utility\information.h"
USING_NS_CC;

class roomScene : public cocos2d::Layer
{
	//bool am_i_host;

	//typedef std::tuple<std::string, int, int> player_imfor;
	//std::vector<player_imfor> players;
	void tell_map_changed(Ref *pSender);
	void tell_nation_changed(Ref *pSender);
	void tell_team_changed(Ref *pSender);
	cocos2d::ui::TextField* _chatWindow;
	void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	void send_message(Ref *pSender);
	void shut_down_callback(Ref *pSender)
	{
		auto this_buttom = dynamic_cast<cocos2d::ui::Button*>(pSender);
		this->removeChild(this_buttom);
	}
	//talk_to_svr::ptr pClient;
public:
	static cocos2d::Scene*createScene(client*);
	virtual bool init();
	CREATE_FUNC(roomScene);
	virtual void update(float dt) override;






};





#endif
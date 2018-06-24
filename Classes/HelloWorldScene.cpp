#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <math.h>
#include"soldier.h"
#include"cocos2d.h"

cocos2d::Vector<soldier*> HelloWorld::vec_soldier;
cocos2d::Vector<soldier*> HelloWorld::vec_enemy;
cocos2d::Vector<soldier*> HelloWorld::vec_chosed_soldier;


USING_NS_CC;

Scene* HelloWorld::createScene()
{
	
	auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
	
}
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
	
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	
	//�����˵���ť
	MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(15);
	

	MenuItemFont* item1 = MenuItemFont::create("create friend", CC_CALLBACK_1(HelloWorld::menuItem1Callback, this));
	item1->setPositionX(origin.x + item1->getContentSize().width / 2);
	item1->setPositionY(origin.y + item1->getContentSize().height / 2);

	MenuItemFont* item2 = MenuItemFont::create("create enemy", CC_CALLBACK_1(HelloWorld::menuItem2Callback, this));
	item2->setPositionX(origin.x - item2->getContentSize().width / 2 + visibleSize.width);
	item2->setPositionY(origin.y + item2->getContentSize().height / 2);

	Menu* mn = Menu::create(item1,item2, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	//�򿪼��Ѫ���ĵ�����
	this->scheduleUpdate();
	

	//������������ļ�����
	auto _mouseListen = EventListenerMouse::create();
	//��갴�£���¼����ʱ�̵�λ��

	_mouseListen->onMouseDown = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		mouseDownPosition.set(e->getCursorX(), e->getCursorY());
	};
	//����ɿ�
	_mouseListen->onMouseUp = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		//����ɿ�ʱ��λ��
		cocos2d::Vec2 mouseUpPosition(e->getCursorX(), e->getCursorY());
		//������º��ɿ���������ͬһ��λ��
		if (sqrt(pow((mouseUpPosition.x - mouseDownPosition.x), 2) + pow((mouseUpPosition.y - mouseDownPosition.y), 2)) <= 5)
		{
			if (e->getMouseButton() == 0)//�����������
			{
				bool is_click_on_a_friend = 0;
				for (int i = 0;vec_soldier.size() != 0 && i <= vec_soldier.size() - 1;i++)
				{
					//�ж��Ƿ������
					Rect rect = vec_soldier.at(i)->getBoundingBox();
					if (rect.containsPoint(mouseDownPosition))//���������
					{
						Texture2D* textureNo = Director::getInstance()->getTextureCache()->addImage("CloseSelected.png");
						for (int i = 0;vec_soldier.size() != 0 && i <= vec_soldier.size() - 1;i++)
						{
							vec_soldier.at(i)->setTexture(textureNo);
						}

						Texture2D* textureYes = Director::getInstance()->getTextureCache()->addImage("CloseNormal.png");
						vec_soldier.at(i)->setTexture(textureYes);
						vec_chosed_soldier.clear();
						vec_chosed_soldier.pushBack(vec_soldier.at(i));
						is_click_on_a_friend = 1;
					}
					if (is_click_on_a_friend == 1)
						break;
				}
			}
			else if (e->getMouseButton() == 1)//���������Ҽ�
			{
				//�ж��Ҽ��Ƿ�����˵о���
				int enemy_index = -1;
				for (int i = 0;vec_enemy.size() != 0 && i <= vec_enemy.size() - 1;i++)
				{
					cocos2d::Rect rect = vec_enemy.at(i)->getBoundingBox();
					if (rect.containsPoint(mouseUpPosition))
					{
						enemy_index = i;
						break;
					}
				}
				if (enemy_index < 0)//������Ҽ�ʱû�е㵽�о�
				{
					//�ƶ�ʿ��
					for (int i = 0;vec_chosed_soldier.size() != 0 && i <= vec_chosed_soldier.size() - 1;i++)
					{
						vec_chosed_soldier.at(i)->stopAction(vec_chosed_soldier.at(i)->move_action);
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateMove));
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateAttack));
						double distance = sqrt(pow((e->getCursorX() - vec_chosed_soldier.at(i)->getPositionX()), 2) + pow((e->getCursorY() - vec_chosed_soldier.at(i)->getPositionY()), 2));
						vec_chosed_soldier.at(i)->move_action = MoveTo::create(distance / vec_chosed_soldier.at(i)->speed, cocos2d::Vec2(e->getCursorX(), e->getCursorY()));
						vec_chosed_soldier.at(i)->runAction(vec_chosed_soldier.at(i)->move_action);
					
					}
				}
				else//������Ҽ�ʱ�㵽�˵з�
				{
					//�ƶ�ʿ��ֱ�����Թ����з�
					for (int i = 0;vec_chosed_soldier.size() != 0 && i <= vec_chosed_soldier.size() - 1;i++)
					{
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateMove));
						vec_chosed_soldier.at(i)->unschedule(schedule_selector(soldier::updateAttack));

						vec_chosed_soldier.at(i)->enemy_target = vec_enemy.at(enemy_index);
						vec_chosed_soldier.at(i)->schedule(schedule_selector(soldier::updateMove),0.1f);
					}
				}
			}
		}
		else//�����ɿ�����ͬһλ�ã�����ѡ��
		{
			if (e->getMouseButton() == 0)//�����������
			{
				vec_chosed_soldier.clear();
				for (int i = 0;vec_soldier.size()!=0 && i <= vec_soldier.size() - 1;i++)
				{
					//������껭�ķ���rect
					float rectX =mouseDownPosition.x < mouseUpPosition.x ? mouseDownPosition.x : mouseUpPosition.x;
					float rectY = mouseDownPosition.y < mouseUpPosition.y ? mouseDownPosition.y : mouseUpPosition.y;
					Rect mouseRect (rectX, rectY, abs(mouseDownPosition.x - mouseUpPosition.x), abs(mouseDownPosition.y - mouseUpPosition.y));
					//�жϿ�ѡ����Щ
					if (vec_soldier.at(i)->getBoundingBox().intersectsRect(mouseRect))
					{
						Texture2D* textureYes = Director::getInstance()->getTextureCache()->addImage("CloseNormal.png");
						vec_soldier.at(i)->setTexture(textureYes);
						vec_chosed_soldier.pushBack(vec_soldier.at(i));
					}
					else
					{
						Texture2D* textureNo = Director::getInstance()->getTextureCache()->addImage("CloseSelected.png");
						vec_soldier.at(i)->setTexture(textureNo);
					}
				}
			}
		}
	};
	//ע�������
	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListen, this);

    return true;
}

void HelloWorld::menuItem1Callback(cocos2d::Ref* pSender)//����һ���ѷ���λ
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	soldier* a_soldier = soldier::create("CloseSelected.png");
	vec_soldier.pushBack(a_soldier);
	a_soldier->setPosition(cocos2d::Vec2(a_soldier->getContentSize().width/2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(a_soldier, 0);
	//a_soldier->scheduleUpdate();//�򿪸���������Ҫ�Ż���
};
void HelloWorld::menuItem2Callback(cocos2d::Ref* pSender)//����һ���з���λ
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	soldier* an_enemy = soldier::create("enemy.png");
	vec_enemy.pushBack(an_enemy);
	an_enemy->setPosition(cocos2d::Vec2(visibleSize.width - an_enemy->getContentSize().width/2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(an_enemy, 0);
	an_enemy->country = 1;

	an_enemy->schedule(schedule_selector(soldier::move_all_time), 0.1f);
}
void HelloWorld::update(float dt)
{
	//ɾ��ûѪ���ѷ�
	for (int i = 0;vec_soldier.size() > 0 && i <= vec_soldier.size() - 1;)
	{
		if (vec_soldier.at(i)->health <= 0)
		{
			vec_soldier.at(i)->removeFromParent();
			vec_soldier.erase(i);
			continue;
		}
		i++;
	}
	//ɾ��ûѪ�ĵз�
	for (int i = 0;vec_enemy.size() > 0 && i <= vec_enemy.size() - 1;)
	{
		if (vec_enemy.at(i)->health <= 0)//����õ�λûѪ
		{
			//�������ڹ����õ�λ���Ѿ�
			for (int j = 0;vec_soldier.size() > 0 && j <= vec_soldier.size() - 1;j++)
			{
				if (vec_enemy.at(i) == vec_soldier.at(j)->enemy_target)
				{
					vec_soldier.at(j)->enemy_target = NULL;
				}
			}
			vec_enemy.at(i)->removeFromParent();
			vec_enemy.erase(i);
			continue;
		}
		i++;
	}
}
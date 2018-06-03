#include"cocos2d.h"
#include"loginScnen.h"

USING_NS_CC;
Scene* loginScene::createScene()
{
	auto scene = Scene::create();
	auto layer = loginScene::create();

	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
bool loginScene::init()
{
	if (!Layer::init())
	{
		return 0;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background_color = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background_color, -1);

	cocos2d::ui::TextField* textField = cocos2d::ui::TextField::create("favorite band (alphabat only) ", "fonts/Marker Felt.ttf", 80);
	textField->setMaxLengthEnabled(true);
	textField->setMaxLength(20);
	textField->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 50));
	textField->setColor(Color3B(0, 0, 0));
	textField->addEventListener(CC_CALLBACK_2(loginScene::textFieldEvent, this));
	this->addChild(textField);
	auto inputBox = Sprite::create("scene/basic-red-heart.jpg");
	inputBox->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	inputBox->setScale(0.3, 0.3);
	this->addChild(inputBox, -1);
	auto pleaseStartButton = MenuItemLabel::create(Label::createWithTTF("OK", "fonts/Marker Felt.ttf", 60),
		CC_CALLBACK_1(loginScene::OKCallback, this));
	pleaseStartButton->setScale(1.0);
	pleaseStartButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.5 - 80));
	pleaseStartButton->setVisible(false);
	pleaseStartButton->setColor(Color3B(0, 0, 0));
	_pleaseStartButton = pleaseStartButton;
	//this->addChild(pleaseStartButton);
	auto labbel_back = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 60);
	auto back = MenuItemLabel::create(labbel_back, CC_CALLBACK_1(loginScene::backBack, this));
	back->setScale(1.0);
	back->setVisible(true);
	back->setPosition(Vec2(origin.x + 0.8*visibleSize.width, origin.y + 100));
	back->setColor(Color3B(0, 0, 0));
	auto mn = Menu::create(back,pleaseStartButton, nullptr);
	mn->setPosition(0.0f, 0.0f);
	this->addChild(mn);
	auto BackEventListener = EventListenerKeyboard::create();
	BackEventListener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			Director::getInstance()->popScene();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(BackEventListener, this);
	return 1;
}
void loginScene::backBack(Ref* pSender)
{
	Director::getInstance()->popScene();
}
void loginScene::OKCallback(Ref* pSender)
{
	if (_playerName.length())
	{
		UserDefault::getInstance()->setStringForKey("UserName", _playerName);
		auto sc = choose_to_beScene::createScene();
		auto reScene = TransitionFade::create(1.0f, sc);
		Director::getInstance()->replaceScene(reScene);
	}
}

void loginScene::textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		Size screenSize = CCDirector::getInstance()->getWinSize();

		_pleaseStartButton->setVisible(true);
	}
	break;

	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);

		_playerName = textField->getString();
	}
	break;

	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		//_pleaseStartButton->setVisible(false);
		_playerName = textField->getString();

	}
	break;

	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
	{
		cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);

		_playerName = textField->getString();
	}
	break;

	default:
		break;
	}
}
void loginScene::editBoxEditingDidBegin(EditBox *editBox)
{
}
void loginScene::editBoxEditingDidEnd(EditBox *editBox)
{
}
void loginScene::editBoxReturn(EditBox *editBox)
{
}
void loginScene::editBoxTextChanged(EditBox *editBox, const std::string &text)
{
}
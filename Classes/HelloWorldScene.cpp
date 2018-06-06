#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

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
    if ( !Scene::init() )
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
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	
    /*auto label = Label::createWithTTF("you are eating", "fonts/Marker Felt.ttf", 48);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height-50));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

	auto label1 = Label::createWithTTF("but i'm waiting", "fonts/Marker Felt.ttf", 48);
	if (label1 == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label1->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + label1->getContentSize().height+50));

		// add the label as a child to this layer
		this->addChild(label1, 1);
	}

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("joke1.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'joke1.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }*/
    //////////////////
	// try for menu
	/*MenuItemFont::setFontName("Times New Roman");
	MenuItemFont::setFontSize(60);

	MenuItemFont * item1 = MenuItemFont::create("Start",
		CC_CALLBACK_1(HelloWorld::menuItem1Callback, this));

	MenuItemFont::setFontName("Cooper Black");
	MenuItemFont::setFontSize(40);
	MenuItemFont * item2 = MenuItemFont::create("help",
		CC_CALLBACK_1(HelloWorld::menuItem2Callback, this));
	

	Menu * mn = Menu::create(item1, item2, NULL);
	mn->alignItemsVertically();
	item2->setPosition(Director::getInstance()->convertToGL(Vec2(200,200)));
	this->addChild(mn);*/
	/////////////////
	//try for toggle
	/*auto toggle = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(HelloWorld::menuItem1Callback, this),
		MenuItemFont::create("on"),
		MenuItemFont::create("off"),
		NULL);
	toggle->setPosition(
		Vec2(origin.x,origin.y));
	Menu* mn = Menu::create(toggle, NULL);
	this->addChild(mn);*/
	//////////////////
	//try for sprite
	/*auto bg = Sprite::create("sprite4.png");
	bg->setAnchorPoint(Vec2::ZERO);
	this->addChild(bg, 0);

	auto tree1 = Sprite::create("sprite2.png", Rect(638, 51, 325, 385));
	tree1->setPosition(Vec2(200,265));
	this->addChild(tree1, 0);

	Texture2D* cache = Director::getInstance()->getTextureCache()->addImage("sprite2.png");
	auto tree2 = Sprite::create();
	tree2->setTexture(cache);
	tree2->setTextureRect(Rect(70, 99, 208, 340));
	tree2->setPosition(Vec2(600, 245));
	this->addChild(tree2, 0);*/
	/////////////////
	//try for scene
	Sprite * bg = Sprite::create("scene1.jpg");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(bg, 0);

	MenuItemImage *setting = MenuItemImage::create(
		"setting1.jpg",
		"setting1.jpg",
		CC_CALLBACK_1(HelloWorld::menuItemsettingCallback, this));
	//setting->setAnchorPoint(Vec2(0, 0));
	setting->setPosition(Director::getInstance()->convertToGL(Vec2(402,355)));

	MenuItemImage *go = MenuItemImage::create(
		"start-up.png",
		"start-down.png",
		CC_CALLBACK_1(HelloWorld::menuItemgoCallback, this));
	go->setPosition(Director::getInstance()->convertToGL(Vec2(587, 152)));

	Menu* mn = Menu::create(setting, go,NULL);
	mn->setPosition(Vec2::ZERO);
	addChild(mn);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void HelloWorld::menuItem1Callback(Ref* pSender)
{
	MenuItem* item = (MenuItem*)pSender;
	log("Touch Start Menu Item %p", item);
}

void HelloWorld::menuItemsettingCallback(Ref* pSender)
{
	auto sc = setting::createscene();
	Director::getInstance()->pushScene(sc);
}

void HelloWorld::menuItemgoCallback(Ref* pSender)
{
	auto sc = go::createscene();
	Director::getInstance()->pushScene(sc);
}
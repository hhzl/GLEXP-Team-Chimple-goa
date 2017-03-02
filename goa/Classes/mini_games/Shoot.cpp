#include "Shoot.h"
#include "../menu/HelpLayer.h"
#include "../util/CommonLabelTTF.h"

#define COCOS2D_DEBUG 1;
using namespace std;
using namespace cocos2d;

Scene* Shoot::createScene()
{
	auto scene = Scene::create();
	auto layer = Shoot::create();
	scene->addChild(layer);
	layer->_menuContext = MenuContext::create(layer, Shoot::gameName());
	scene->addChild(layer->_menuContext);
	return scene;
}

Shoot *Shoot::create() {
	Shoot *blast = new (std::nothrow) Shoot();
	if (blast && blast->init()) {
		blast->autorelease();
		return blast;
	}
	CC_SAFE_DELETE(blast);
	return nullptr;

}

bool Shoot::init()
{
	if (!Layer::init()) { return false; }

	return true;
}

void Shoot::onEnterTransitionDidFinish() {

	std::string playerGUI = "";
	int heightTolrence = 0;
	_menuContext->setMaxPoints(10);

	std::string gameRand[] = { "pinatacity" ,"pinatacream","pinatajungle" };
	int gameCurrentLevel = _menuContext->getCurrentLevel();
	std::tuple<int, int, int> levelKeyNumber = levelAllInfo(gameCurrentLevel, 3, 5, 3, 10);
	std::string gameTheme = gameRand[std::get<1>(levelKeyNumber) - 1];

	auto topBoard = Sprite::create();
	topBoard->setTextureRect(Rect(0, 0, Director::getInstance()->getVisibleSize().width / 2, 165));
	topBoard->setColor(Color3B(128, 196, 206));
	topBoard->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height*0.92));
	addChild(topBoard, 4);
	topBoard->setName("topBoard");

	auto upText = CommonLabelTTF::create(TextGenerator::getInstance()->translateString("choose same sounding word"), "res/fonts/BalooBhai-Regular.ttf", topBoard->getContentSize().height * 0.5);
	upText->setPosition(topBoard->getContentSize().width / 2, topBoard->getContentSize().height / 2);
	topBoard->addChild(upText);

	backUp.category = std::get<0>(levelKeyNumber);
	backUp.level = std::get<2>(levelKeyNumber);

	if (std::get<0>(levelKeyNumber) == 1) {
		_map = TextGenerator::getInstance()->getHomonyms(15, std::get<2>(levelKeyNumber));
		setKeyValueFromMap(_map);

		while (_data_key.size() != 15) {
			_map = TextGenerator::getInstance()->getHomonyms(15, std::get<2>(levelKeyNumber));
			setKeyValueFromMap(_map);
		}
		upText->setString(TextGenerator::getInstance()->translateString("choose same sounding word"));
	}
	else if (std::get<0>(levelKeyNumber) == 2) {
		_map = TextGenerator::getInstance()->getAntonyms(15, std::get<2>(levelKeyNumber));
		setKeyValueFromMap(_map);

		while (_data_key.size() != 15) {
			_map = TextGenerator::getInstance()->getAntonyms(15, std::get<2>(levelKeyNumber));
			setKeyValueFromMap(_map);
		}
		upText->setString(TextGenerator::getInstance()->translateString("choose opposite word"));
	}
	else if (std::get<0>(levelKeyNumber) == 3) {
		_map = TextGenerator::getInstance()->getSynonyms(15, std::get<2>(levelKeyNumber));
		setKeyValueFromMap(_map);

		while (_data_key.size() != 15) {
			_map = TextGenerator::getInstance()->getSynonyms(15, std::get<2>(levelKeyNumber));
			setKeyValueFromMap(_map);
		}
		upText->setString(TextGenerator::getInstance()->translateString("choose meaning word"));
	}
	else {
		CCLOG("ERROR :: Your category is wrong , please check your code : line no : 91");
	}

	if (gameTheme.compare("pinatacream") == 0) {

		Node* gameBg = CSLoader::createNode("pinatacream/pinatacream.csb");
		addChild(gameBg);
		gameBg->setName("bg");

		auto targetA = gameBg->getChildByName("targeta");
		heightTolrence = targetA->getContentSize().height * 0.25;
		stringColor = Color4F(Color4B(158, 45, 45, 255));
		topBoard->setOpacity(164);
		playerGUI = "pinatacream/player.png";
		((Sprite*)this->getChildByName("topBoard"))->setColor(Color3B(251, 107, 124));
	}
	else if (gameTheme.compare("pinatacity") == 0) {

		Node* gameBg = CSLoader::createNode("pinatacity/pinatacity.csb");
		addChild(gameBg);
		gameBg->setName("bg");

		stringColor = Color4F(Color4B(140, 234, 19, 255));
		topBoard->setOpacity(180);
		playerGUI = "pinatacity/player.png";
		((Sprite*)this->getChildByName("topBoard"))->setColor(Color3B(36, 184, 184));
	}
	else if (gameTheme.compare("pinatajungle") == 0) {
		Node* gameBg = CSLoader::createNode("jungle/pinatajungle.csb");
		addChild(gameBg);
		gameBg->setName("bg");
		stringColor = Color4F(Color4B(124, 252, 0, 255));
		topBoard->setOpacity(180);
		playerGUI = "jungle/junglec/player.png";
		((Sprite*)this->getChildByName("topBoard"))->setColor(Color3B(228, 171, 80));
	}

	if (Director::getInstance()->getVisibleSize().width > 2560) {
		this->xPosi = Director::getInstance()->getVisibleSize().width - 2560;
		this->getChildByName("bg")->setPositionX(this->xPosi / 2);
	}

	mapKey = _data_key[getRandomInt(0, 2)];

	if (gameCurrentLevel == 1) {
		mapKey = _data_key[1];
	}
	auto board = this->getChildByName("bg")->getChildByName("board");
	auto boardText = CommonLabelTTF::create(mapKey, "res/fonts/BalooBhai-Regular.ttf", 120);
	boardText->setName(board->getName());
	boardText->setPosition(Vec2(board->getContentSize().width / 2, board->getContentSize().height / 2));
	board->addChild(boardText);
	if (gameTheme.compare("pinatacream") == 0) { boardText->setColor(Color3B(155, 42, 50)); }
	if (gameTheme.compare("pinatacity") == 0) { boardText->setColor(Color3B(0, 0, 0)); }

	auto targetA = this->getChildByName("bg")->getChildByName("targeta");
	auto targetAText = CommonLabelTTF::create(_map[_data_key[0]], "res/fonts/BalooBhai-Regular.ttf", 80);
	if (gameTheme.compare("pinatajungle") == 0) targetAText->setFontSize(80);
	if (gameTheme.compare("pinatacity") == 0) { targetAText->setColor(Color3B(0, 0, 0)); }
	targetAText->setName(targetA->getName());
	targetAText->setPosition(Vec2(targetA->getContentSize().width / 2, targetA->getContentSize().height / 2 - heightTolrence));
	targetA->addChild(targetAText);
	targetA->setTag(0);

	auto targetB = this->getChildByName("bg")->getChildByName("targetb");
	auto targetBText = CommonLabelTTF::create(_map[_data_key[1]], "res/fonts/BalooBhai-Regular.ttf", 80);
	if (gameTheme.compare("pinatajungle") == 0) targetBText->setFontSize(80);
	if (gameTheme.compare("pinatacity") == 0) { targetBText->setColor(Color3B(0, 0, 0)); }
	targetBText->setName(targetB->getName());
	targetBText->setPosition(targetB->getContentSize().width / 2, targetB->getContentSize().height / 2 - heightTolrence);
	targetB->addChild(targetBText);
	targetB->setTag(0);

	auto targetC = this->getChildByName("bg")->getChildByName("targetc");
	auto targetCText = CommonLabelTTF::create(_map[_data_key[2]], "res/fonts/BalooBhai-Regular.ttf", 80);
	if (gameTheme.compare("pinatajungle") == 0) targetCText->setFontSize(80);
	if (gameTheme.compare("pinatacity") == 0) { targetCText->setColor(Color3B(0, 0, 0)); }
	targetCText->setName(targetC->getName());
	targetCText->setPosition(Vec2(targetC->getContentSize().width / 2, targetC->getContentSize().height / 2 - heightTolrence));
	targetC->addChild(targetCText);
	targetC->setTag(0);

	bubblePlayer = Sprite::createWithSpriteFrameName(playerGUI);
	bubblePlayer->setName(gameTheme);
	bubblePlayer->setPosition(Vec2((xPosi / 2) + (this->getChildByName("bg")->getChildByName("left")->getPositionX() + this->getChildByName("bg")->getChildByName("right")->getPositionX()) / 2, this->getChildByName("bg")->getChildByName("right")->getPositionY()));
	addChild(bubblePlayer, 1);
	player.x = bubblePlayer->getPositionX();    player.y = bubblePlayer->getPositionY();

	rightLine = DrawNode::create();
	rightLine->drawSegment(Vec2(this->getChildByName("bg")->getChildByName("right")->getPositionX() + (xPosi / 2), this->getChildByName("bg")->getChildByName("right")->getPositionY()),Vec2(player.x + (bubblePlayer->getContentSize().width / 2), player.y),10,stringColor);
	this->addChild(rightLine,3);
	rightLine->setName("rightLine");

	leftLine = DrawNode::create();
	leftLine->drawSegment(Vec2(this->getChildByName("bg")->getChildByName("left")->getPositionX() + (xPosi / 2), this->getChildByName("bg")->getChildByName("left")->getPositionY()), Vec2(player.x - (bubblePlayer->getContentSize().width / 2), player.y),10,stringColor);
	this->addChild(leftLine);
	leftLine->setName("leftLine");

	targetYcoordSave = targetB->getPositionX();
	bubblePlayer->setVisible(false);
	rightLine->setVisible(false);
	leftLine->setVisible(false);

	this->getChildByName("bg")->getChildByName("right")->setVisible(false);
	this->getChildByName("bg")->getChildByName("left")->setVisible(false);
	this->getChildByName("bg")->getChildByName("board")->setTag(0);
	if (bubblePlayer->getName() == "pinatacity")
		this->getChildByName("bg")->getChildByName("slingshot_16")->setVisible(false);

	if (_menuContext->getCurrentLevel() == 1) {
		auto help = HelpLayer::create(Rect((xPosi / 2) + targetB->getPositionX(), targetB->getPositionY(), targetB->getContentSize().width + targetB->getContentSize().width * 0.3, targetB->getContentSize().height + targetB->getContentSize().height * 0.1), Rect((xPosi / 2) + board->getPositionX(), board->getPositionY(), board->getContentSize().width, board->getContentSize().height));
		help->click(Vec2((xPosi / 2) + targetB->getPositionX(), targetB->getPositionY()));
		help->setName("helpLayer");
		addChild(help, 4);
	}

	bgListner();
	choosingListner();

	this->scheduleUpdate();
}

void Shoot::setKeyValueFromMap(std::map<std::string, std::string> _data) {
	int count = 0;
	_data_key.clear();
	_data_value.clear();
	for (std::map<std::string, std::string>::iterator it = _data.begin(); it != _data.end(); ++it) {
		auto key = (it->first);
		auto value = (it->second);

		_data_key.push_back(key);
		_data_value.push_back(value);
		count++;
		CCLOG("index = %d   key :  %s ----> %s", count, key.c_str(), value.c_str());
	}
}

string Shoot::getConvertInUpperCase(string data)
{
	std::ostringstream blockName;
	int i = 0;
	while (data[i])
	{
		blockName << (char)toupper(data[i]);
		i++;
	}
	return blockName.str();
}
string Shoot::getConvertInLowerCase(string data)
{
	std::ostringstream blockName;
	int i = 0;
	while (data[i])
	{
		blockName << (char)tolower(data[i]);
		i++;
	}
	return blockName.str();
}

void Shoot::update(float dt) {
	//commet
	if (!this->shootingFlag && _menuContext->isGamePaused() && (this->getChildByName("bg")->getChildByName("board")->getTag() == 1)) {
		this->bubblePlayer->setPosition((this->xPosi / 2) + (this->getChildByName("bg")->getChildByName("left")->getPositionX() + this->getChildByName("bg")->getChildByName("right")->getPositionX()) / 2, this->getChildByName("bg")->getChildByName("right")->getPositionY());
		if (this->rightLine) {
			this->removeChild(this->rightLine);
		}
		this->rightLine = DrawNode::create();
		this->rightLine->drawSegment(Vec2((this->xPosi / 2) + this->getChildByName("bg")->getChildByName("right")->getPositionX(), this->getChildByName("bg")->getChildByName("right")->getPositionY()), Vec2(this->bubblePlayer->getPositionX() + (this->bubblePlayer->getContentSize().width / 2), this->bubblePlayer->getPositionY()),10,this->stringColor);
		this->addChild(this->rightLine);

		if (this->leftLine) {
			this->removeChild(this->leftLine);
		}
		this->leftLine = DrawNode::create();
		this->leftLine->drawSegment(Vec2((this->xPosi / 2) + this->getChildByName("bg")->getChildByName("left")->getPositionX(), this->getChildByName("bg")->getChildByName("left")->getPositionY()), Vec2(this->bubblePlayer->getPositionX() - (this->bubblePlayer->getContentSize().width / 2), this->bubblePlayer->getPositionY()),10,this->stringColor);
		this->addChild(this->leftLine);

	}

	if (this->shootingFlag && !_menuContext->isGamePaused()) {
		this->stateShootBubble(dt);
		if (!(this->bubblePlayer->getPositionY() >= 0)) {
			this->bubblePlayer->setPosition(Vec2((this->xPosi / 2) + (this->getChildByName("bg")->getChildByName("left")->getPositionX() + this->getChildByName("bg")->getChildByName("right")->getPositionX()) / 2, this->getChildByName("bg")->getChildByName("right")->getPositionY()));
			this->player.x = this->bubblePlayer->getPositionX();    this->player.y = this->bubblePlayer->getPositionY();
			this->shootingFlag = false;
			this->getChildByName("bg")->getChildByName("board")->setTag(1);
		}
	}
	if (this->shootingFlag) {
		auto path = "";
		float size = 0.5;
		if (this->bubblePlayer->getName() == "pinatacity") {
			path = "pinatacity/pinatacityanim.csb";
			size = 0.7;
		}
		else if (this->bubblePlayer->getName() == "pinatacream") {
			path = "pinatacream/pinatacreamanim.csb";
			size = 0.5;
		}
		else if (this->bubblePlayer->getName() == "pinatajungle") {
			path = "jungle/target.csb";
			size = 0.9;
		}

		auto firstX = this->targetPlayer->getPositionX() - (this->targetPlayer->getContentSize().width*size / 2) + (this->xPosi / 2);
		auto firstY = this->targetPlayer->getPositionY() - (this->targetPlayer->getContentSize().height*size / 2);
		auto height = this->targetPlayer->getContentSize().height*size;
		auto width = this->targetPlayer->getContentSize().width*size;

		auto firstXs = this->bubblePlayer->getPositionX() - (this->bubblePlayer->getContentSize().width / 4);
		auto firstYs = this->bubblePlayer->getPositionY() - (this->bubblePlayer->getContentSize().height / 4);
		auto heights = this->bubblePlayer->getContentSize().width / 2;
		auto widths = this->bubblePlayer->getContentSize().height / 2;

		auto targetObject = Rect(this->targetPlayer->getPositionX() - (this->targetPlayer->getContentSize().width*size / 2) + (this->xPosi / 2), this->targetPlayer->getPositionY() - (this->targetPlayer->getContentSize().height*size / 2), this->targetPlayer->getContentSize().width*size, this->targetPlayer->getContentSize().height*size);
		auto playerObject = Rect(this->bubblePlayer->getPositionX() - (this->bubblePlayer->getContentSize().width / 4), this->bubblePlayer->getPositionY() - (this->bubblePlayer->getContentSize().height / 4), this->bubblePlayer->getContentSize().width / 2, this->bubblePlayer->getContentSize().height / 2);

		if (targetObject.intersectsRect(playerObject)) {
			this->shootingFlag = false;
			this->flagSingleTouchFirst = false;
			this->runAnimations(CSLoader::createNode(path), this->targetPlayer->getPositionX(), this->targetPlayer->getPositionY(), path);
			this->getChildByName("bg")->getChildByName("board")->setTag(0);
			this->bubblePlayer->setVisible(false);
			this->targetPlayer->setVisible(false);
			auto classReference = this;
			auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
			audioEngine->playEffect("sounds/sfx/pinata_select.ogg",false);

			auto checkGameCompleteOrNot = CallFunc::create([=]()
			{
				if (classReference->counterlevelStatus == 5) {
					_menuContext->setMaxPoints(classReference->counterHit);
					_menuContext->showScore();
				}
				else {
					classReference->counterlevelStatus++;
					classReference->reCreateSceneElement();
				}
			});
			this->runAction(Sequence::create(DelayTime::create(1.2),checkGameCompleteOrNot,NULL));

		}
	}
}

void Shoot::choosingListner() {

	auto classRefer = this;
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	listener->onTouchBegan = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto target = event->getCurrentTarget();
		Size targetSize = target->getContentSize();
		auto location = target->convertToNodeSpace(touch->getLocation());
		Rect targetRect = Rect(0, 0, targetSize.width, targetSize.height);

		if (target->getBoundingBox().containsPoint(touch->getLocation()) && (classRefer->getChildByName("bg")->getChildByName("board")->getTag() == 0) && !classRefer->shootingFlag && classRefer->flagSingleTouchFirst) {

			classRefer->flagSingleTouchFirst = false;

			if (_menuContext->getCurrentLevel() == 1) {
				classRefer->removeChildByName("helpLayer");
			}
			std::string path = "";

			if (classRefer->bubblePlayer->getName() == "pinatacity") {
				path = "pinatacity/pinatacityanim.csb";
			}
			else if (classRefer->bubblePlayer->getName() == "pinatacream") {
				path = "pinatacream/pinatacreamanim.csb";
			}
			else if (classRefer->bubblePlayer->getName() == "pinatajungle") {
				path = "jungle/target.csb";
			}
			auto boardText = classRefer->getChildByName("bg")->getChildByName("board")->getChildByName("board");

			auto stringmap = ((CommonLabelTTF *)boardText)->getString();
			if (classRefer->_map[stringmap] == ((CommonLabelTTF *)target->getChildByName(target->getName()))->getString()) {

				auto targetA = classRefer->getChildByName("bg")->getChildByName("targeta");
				auto targetB = classRefer->getChildByName("bg")->getChildByName("targetb");
				auto targetC = classRefer->getChildByName("bg")->getChildByName("targetc");

				auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
				if (target->getName() == "targetc") {
					if (targetA->getTag() == 0) { classRefer->runAnimations(CSLoader::createNode(path), targetA->getPositionX(), targetA->getPositionY(), path); targetA->setVisible(false); }
					if (targetB->getTag() == 0) { classRefer->runAnimations(CSLoader::createNode(path), targetB->getPositionX(), targetB->getPositionY(), path); targetB->setVisible(false); }
					classRefer->targetXcoordSave = targetC->getPositionX();
					audioEngine->playEffect("sounds/sfx/pinata_select.ogg", false);
					classRefer->counterHit++;
					_menuContext->addPoints(2);
					classRefer->gamePlay(targetC);
				}
				else if (target->getName() == "targetb") {
					if (targetA->getTag() == 0) { classRefer->runAnimations(CSLoader::createNode(path), targetA->getPositionX(), targetA->getPositionY(), path); targetA->setVisible(false); }
					if (targetC->getTag() == 0) { classRefer->runAnimations(CSLoader::createNode(path), targetC->getPositionX(), targetC->getPositionY(), path); targetC->setVisible(false); }
					classRefer->targetXcoordSave = targetB->getPositionX();
					classRefer->gamePlay(targetB);
					audioEngine->playEffect("sounds/sfx/pinata_select.ogg", false);
					classRefer->counterHit++;
					_menuContext->addPoints(2);
				}
				else if (target->getName() == "targeta") {
					if (targetC->getTag() == 0) { classRefer->runAnimations(CSLoader::createNode(path), targetC->getPositionX(), targetC->getPositionY(), path); targetC->setVisible(false); }
					if (targetB->getTag() == 0) { classRefer->runAnimations(CSLoader::createNode(path), targetB->getPositionX(), targetB->getPositionY(), path); targetB->setVisible(false); }
					classRefer->targetXcoordSave = targetA->getPositionX();
					classRefer->gamePlay(targetA);
					audioEngine->playEffect("sounds/sfx/pinata_select.ogg", false);
					classRefer->counterHit++;
					_menuContext->addPoints(2);
				}

			}
			else {
				//    console.log("its wrong answer");
				auto targetA = (Node*)classRefer->getChildByName("bg")->getChildByName("targeta");
				auto targetB = (Node*)classRefer->getChildByName("bg")->getChildByName("targetb");
				auto targetC = (Node*)classRefer->getChildByName("bg")->getChildByName("targetc");

				if (target->getName() == "targetc") {
					if (targetC->getTag() == 0) {
						classRefer->counterHit++;
						_menuContext->addPoints(-1);
						classRefer->runAnimations(CSLoader::createNode(path), targetC->getPositionX(), targetC->getPositionY(), path);
						targetC->setVisible(false);
						auto audioEngine1 = CocosDenshion::SimpleAudioEngine::getInstance();
						audioEngine1->playEffect("sounds/sfx/pinata_select.ogg", false);
						auto audioEngine2 = CocosDenshion::SimpleAudioEngine::getInstance();
						audioEngine2->playEffect("sounds/sfx/pinata_select.ogg", false);
						targetC->setTag(1);
					}

				}
				else if (target->getName() == "targetb") {
					if (targetB->getTag() == 0) {
						classRefer->counterHit++;
						_menuContext->addPoints(-1);
						classRefer->runAnimations(CSLoader::createNode(path), targetB->getPositionX(), targetB->getPositionY(), path);
						targetB->setTag(1);
						auto audioEngine1 = CocosDenshion::SimpleAudioEngine::getInstance();
						audioEngine1->playEffect("sounds/sfx/pinata_select.ogg", false);
						auto audioEngine2 = CocosDenshion::SimpleAudioEngine::getInstance();
						audioEngine2->playEffect("sounds/sfx/pinata_select.ogg", false);
						targetB->setVisible(false);
					}

				}
				else if (target->getName() == "targeta") {
					if (targetA->getTag() == 0) {
						classRefer->counterHit++;
						_menuContext->addPoints(-1);
						classRefer->runAnimations(CSLoader::createNode(path), targetA->getPositionX(), targetA->getPositionY(), path);
						targetA->setTag(1);
						auto audioEngine1 = CocosDenshion::SimpleAudioEngine::getInstance();
						audioEngine1->playEffect("sounds/sfx/pinata_select.ogg", false);
						auto audioEngine2 = CocosDenshion::SimpleAudioEngine::getInstance();
						audioEngine2->playEffect("sounds/sfx/pinata_select.ogg", false);
						targetA->setVisible(false);
					}
				}
			}

			auto changeFlagInTouch = CallFunc::create([=]()
			{
				classRefer->flagSingleTouchFirst = true;
			});
			classRefer->runAction(Sequence::create(DelayTime::create(1), changeFlagInTouch, NULL));

			return false;
		}
		return false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, classRefer->getChildByName("bg")->getChildByName("targeta"));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), classRefer->getChildByName("bg")->getChildByName("targetb"));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), classRefer->getChildByName("bg")->getChildByName("targetc"));
}

void Shoot::bgListner() {
	auto classRefer = this;
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	listener->onTouchBegan = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto target = event->getCurrentTarget();
		Size targetSize = target->getContentSize();
		auto location = target->convertToNodeSpace(touch->getLocation());
		Rect targetRect = Rect(0, 0, targetSize.width, targetSize.height);
		//target->getBoundingBox().containsPoint(touch->getLocation())
		if (classRefer->getChildByName("bg")->getChildByName("board")->getTag() == 1) {
			if (target->getBoundingBox().containsPoint(touch->getLocation())) {
				player.prevX = touch->getLocation().x;
				player.prevY = touch->getLocation().y;
				return true;
			}
		}
		return false;
	};

	listener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
		auto classReference = this;
		auto target = event->getCurrentTarget();
		
		classReference->checkBoundaryBall(target, touch);
		if (classReference->rightLine) {
			classReference->removeChild(classReference->rightLine);
		}
		classReference->rightLine = DrawNode::create();
		classReference->rightLine->drawSegment(Vec2((classReference->xPosi / 2) + classReference->getChildByName("bg")->getChildByName("right")->getPositionX(), classReference->getChildByName("bg")->getChildByName("right")->getPositionY()), Vec2(classReference->bubblePlayer->getPositionX() + (classReference->bubblePlayer->getContentSize().width / 2), classReference->bubblePlayer->getPositionY()),10,classReference->stringColor);
		classReference->addChild(classReference->rightLine);

		if (classReference->leftLine) {
			classReference->removeChild(classReference->leftLine);
		}
		classReference->leftLine = DrawNode::create();
		classReference->leftLine->drawSegment(Vec2((classReference->xPosi / 2) + classReference->getChildByName("bg")->getChildByName("left")->getPositionX(), classReference->getChildByName("bg")->getChildByName("left")->getPositionY()), Vec2(classReference->bubblePlayer->getPositionX() - (classReference->bubblePlayer->getContentSize().width / 2), classReference->bubblePlayer->getPositionY()),10,classReference->stringColor);
		classReference->addChild(classReference->leftLine);
		
		return true;

	};

	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto classReference = this;

		auto prex = classReference->player.prevX;
		auto prey = classReference->player.prevY;
		auto currentX = touch->getLocation().x;
		auto currentY = touch->getLocation().y;
		auto playerdatay = classReference->player.y;
		auto playerdatax = classReference->player.x;

		auto angless = atan2((currentY - playerdatay), (-currentX + playerdatax));
		auto convertDegree = radToDeg(angless);

		classReference->player.angle = classReference->radToDeg(atan2((touch->getLocation().y - classReference->player.y), (-touch->getLocation().x + classReference->player.x)));
		auto angle = classReference->player.angle;
		classReference->player.prevX = abs(classReference->player.prevX - touch->getLocation().x);
		classReference->player.prevY = abs(classReference->player.prevY - touch->getLocation().y);


		if (classReference->rightLine) {
			classReference->removeChild(classReference->rightLine);
		}
		classReference->rightLine = DrawNode::create();
		classReference->rightLine->drawSegment(Vec2((classReference->xPosi / 2) + classReference->getChildByName("bg")->getChildByName("right")->getPositionX(), classReference->getChildByName("bg")->getChildByName("right")->getPositionY()), Vec2(classReference->player.x - 10, classReference->player.y),10,classReference->stringColor);
		classReference->addChild(classReference->rightLine);

		if (classReference->leftLine) {
			classReference->removeChild(classReference->leftLine);
		}
		classReference->leftLine = DrawNode::create();
		classReference->leftLine->drawSegment(Vec2((classReference->xPosi / 2) + classReference->getChildByName("bg")->getChildByName("left")->getPositionX(), classReference->getChildByName("bg")->getChildByName("left")->getPositionY()), Vec2(classReference->player.x + 10, classReference->player.y),10,classReference->stringColor);
		classReference->addChild(classReference->leftLine);

		if (classReference->player.prevY != 0 && classReference->player.prevX != 0) {
			classReference->shootingFlag = true;
			classReference->getChildByName("bg")->getChildByName("board")->setTag(0);
			classReference->soundReleaseBall = true;
			auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
			audioEngine->playEffect("sounds/sfx/ball_release_sound.ogg");
			if (!((abs(classReference->player.angle) < 175) && (abs(classReference->player.angle) > 5))) {
				//   console.log("the range is not correct ");

				auto againSetToOriginalPosition = CallFunc::create([=]()
				{
					classReference->bubblePlayer->setPosition(Vec2((classReference->xPosi / 2) + (classReference->getChildByName("bg")->getChildByName("left")->getPositionX() + classReference->getChildByName("bg")->getChildByName("right")->getPositionX()) / 2, classReference->getChildByName("bg")->getChildByName("right")->getPositionY()));
					classReference->player.x = classReference->bubblePlayer->getPositionX();    classReference->player.y = classReference->bubblePlayer->getPositionY();
					classReference->shootingFlag = false;
					classReference->getChildByName("bg")->getChildByName("board")->setTag(1);
				});
				classReference->runAction(Sequence::create(DelayTime::create(3), againSetToOriginalPosition,NULL));
			}
		}

		if (!classReference->shootingFlag && (classReference->getChildByName("bg")->getChildByName("board")->getTag() == 1)) {
			auto xPositionForBall = (classReference->xPosi / 2) + (classReference->getChildByName("bg")->getChildByName("left")->getPositionX() + classReference->getChildByName("bg")->getChildByName("right")->getPositionX()) / 2;
			auto yPositionForBall = classReference->getChildByName("bg")->getChildByName("right")->getPositionY();

			if (classReference->bubblePlayer->getPositionX() != xPositionForBall && classReference->bubblePlayer->getPositionY() != yPositionForBall) {
				auto ballTouchMovementAllow = CallFunc::create([=]()
				{
					classReference->isItinOriginalPosition = true;
				});
				classReference->isItinOriginalPosition = false;
				classReference->bubblePlayer->runAction(Sequence::create(MoveTo::create(0.2, Vec2(xPositionForBall, yPositionForBall)), ballTouchMovementAllow,NULL));
			}
		}
		auto xPositionForBall = (classReference->xPosi / 2) + (classReference->getChildByName("bg")->getChildByName("left")->getPositionX() + classReference->getChildByName("bg")->getChildByName("right")->getPositionX()) / 2;
		auto yPositionForBall = classReference->getChildByName("bg")->getChildByName("right")->getPositionY();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, bubblePlayer);
}

void Shoot::reCreateSceneElement() {

	this->getChildByName("topBoard")->setVisible(true);
	auto optionValue = this->getBoardAndOptionWord();

	auto targetA = this->getChildByName("bg")->getChildByName("targeta");
	auto targetB = this->getChildByName("bg")->getChildByName("targetb");
	auto targetC = this->getChildByName("bg")->getChildByName("targetc");

	((CommonLabelTTF*)targetA->getChildByName(targetA->getName()))->setString(std::get<0>(optionValue));
	((CommonLabelTTF*)targetB->getChildByName(targetB->getName()))->setString(std::get<1>(optionValue));
	((CommonLabelTTF*)targetC->getChildByName(targetC->getName()))->setString(std::get<2>(optionValue));

	auto board = this->getChildByName("bg")->getChildByName("board");
	auto boardText = board->getChildByName(board->getName());
	((CommonLabelTTF*)boardText)->setString(this->mapKey);
	this->getChildByName("bg")->getChildByName("board")->setTag(0);

	this->targetPlayer->setPositionX(this->targetXcoordSave);
	this->targetPlayer->getActionManager()->removeAllActions();

	this->targetPlayer->setScaleX(1);   this->targetPlayer->setScaleY(1);

	if (this->bubblePlayer->getName() == "pinatacream") {
		this->targetPlayer->setScaleX(1); this->targetPlayer->setScaleY(1);
		this->targetPlayer->setPositionY(this->targetYcoordSave - Director::getInstance()->getVisibleSize().height * 0.11);
	}
	if (this->bubblePlayer->getName() == "pinatajungle") {
		this->targetPlayer->setScaleX(1.2); this->targetPlayer->setScaleY(1.2);
		this->targetPlayer->setPositionY(this->targetYcoordSave - Director::getInstance()->getVisibleSize().height * 0.2);
	}
	if (this->bubblePlayer->getName() == "pinatacity") {
		this->targetPlayer->setScaleX(1); this->targetPlayer->setScaleY(1);
		this->targetPlayer->setPositionY(this->targetYcoordSave - Director::getInstance()->getVisibleSize().height * 0.1);
	}

	targetA->setVisible(true);   targetA->setTag(0);
	targetB->setVisible(true);   targetB->setTag(0);
	targetC->setVisible(true);   targetC->setTag(0);

	this->flagSingleTouchFirst = true;

	this->bubblePlayer->setPosition((this->xPosi / 2) + (this->getChildByName("bg")->getChildByName("left")->getPositionX() + this->getChildByName("bg")->getChildByName("right")->getPositionX()) / 2, this->getChildByName("bg")->getChildByName("right")->getPositionY());

	this->getChildByName("bg")->getChildByName("board")->setVisible(true);
	if (this->bubblePlayer->getName() == "pinatacream")
		this->getChildByName("bg")->getChildByName("Panel_2")->setVisible(true);

	this->bubblePlayer->setVisible(false);
	this->rightLine->setVisible(false);
	this->leftLine->setVisible(false);
	if (this->bubblePlayer->getName() == "pinatajungle") {
		this->getChildByName("bg")->getChildByName("rightshoot")->setVisible(false);
		this->getChildByName("bg")->getChildByName("leftshoot")->setVisible(false);
	}
	this->getChildByName("bg")->getChildByName("right")->setVisible(false);
	this->getChildByName("bg")->getChildByName("left")->setVisible(false);
	if (this->bubblePlayer->getName() == "pinatacity")
		this->getChildByName("bg")->getChildByName("slingshot_16")->setVisible(false);
	this->getChildByName("bg")->getChildByName("board")->setTag(0);

}

std::tuple<string, string, string> Shoot::getBoardAndOptionWord() {

	struct optionWord {
		std::string first = "";
		std::string second = "";
		std::string third = "";
	}optionWord;

	if (this->counterlevelStatus == 2) {
		auto index = this->getRandomInt(3, 5);
		this->mapKey = _data_key[index];
		optionWord.first = this->_map[_data_key[3]];
		optionWord.second = this->_map[_data_key[4]];
		optionWord.third = this->_map[_data_key[5]];

	}
	else if (this->counterlevelStatus == 3) {
		auto index = this->getRandomInt(6, 8);
		this->mapKey = _data_key[index];
		optionWord.first = this->_map[_data_key[6]];
		optionWord.second = this->_map[_data_key[7]];
		optionWord.third = this->_map[_data_key[8]];

	}
	else if (this->counterlevelStatus == 4) {
		auto index = this->getRandomInt(9, 11);
		this->mapKey = _data_key[index];
		optionWord.first = this->_map[_data_key[9]];
		optionWord.second = this->_map[_data_key[10]];
		optionWord.third = this->_map[_data_key[11]];
	}
	else if (this->counterlevelStatus == 5) {
		auto index = this->getRandomInt(12, 14);
		this->mapKey = _data_key[index];
		optionWord.first = this->_map[_data_key[12]];
		optionWord.second = this->_map[_data_key[13]];
		optionWord.third = this->_map[_data_key[14]];
	}
	else {
		auto index = this->getRandomInt(10, 12);
		this->mapKey = _data_key[10];
		optionWord.first = this->_map[_data_key[10]];
		optionWord.second = this->_map[_data_key[11]];
		optionWord.third = this->_map[_data_key[12]];

	}

	if (this->mapKey.empty()) {
		CCLOG("---------- game crashed ----------");
		auto mapKeyArrayss = _data_key;

		if (this->backUp.category == 1) {
			this->_map = TextGenerator::getInstance()->getHomonyms(15, this->backUp.level);
		}
		else if (this->backUp.category == 2) {
			this->_map = TextGenerator::getInstance()->getAntonyms(15, this->backUp.level);
		}
		else if (this->backUp.category == 3) {
			this->_map = TextGenerator::getInstance()->getSynonyms(15, this->backUp.level);
		}
		else {
			//  console.log("ERROR :: Your category is wrong , please check your code : line no : 23");
		}
		auto index = this->getRandomInt(3, 8);
		this->mapKey = _data_key[index];
		optionWord.first = this->_map[_data_key[index + 1]];
		optionWord.second = this->_map[_data_key[index]];
		optionWord.third = this->_map[_data_key[index + 2]];
	}
	return std::make_tuple(optionWord.first, optionWord.second, optionWord.third);
}

void Shoot::runAnimations(Node* AnimNode, int x, int y, std::string path) {

	auto animation = CSLoader::createTimeline(path);
	AnimNode->runAction(animation);
	animation->gotoFrameAndPlay(0, false);
	AnimNode->setPosition(Vec2(x + (xPosi / 2), y));
	addChild(AnimNode);
	auto classRefer = this;

	auto removeAnimaCallFunc = CallFunc::create([=]() 
	{
		classRefer->removeChild(AnimNode);
	});
	auto sequence = Sequence::create(DelayTime::create(0.8), removeAnimaCallFunc, NULL);
	this->runAction(sequence);
	auto ss = 20;
}

void Shoot::stateShootBubble(float dt) {

	auto currentx = this->bubblePlayer->getPositionX();
	auto currenttxangle = cos(this->degToRad(this->player.angle));
	auto currenty = this->bubblePlayer->getPositionY();
	auto currenttyangle = -1 * sin(this->degToRad(this->player.angle));
	auto angless = this->player.angle;
	auto radianAngle = this->degToRad(this->player.angle);
	auto addingy = ((1.0 / 60) * 2500 * -1 * sin(this->degToRad(this->player.angle)));
	auto addingx = ((1.0 / 60) * 2500 * cos(this->degToRad(this->player.angle)));
	
	auto nextY = this->bubblePlayer->getPositionY() + ((1.0f / 60.0f) * 2500 * -1 * sin(this->degToRad(this->player.angle)));
	auto nextX = this->bubblePlayer->getPositionX() + ((1.0f / 60.0f) * 2500 * cos(this->degToRad(this->player.angle)));
	this->bubblePlayer->setPositionX(nextX);
	this->bubblePlayer->setPositionY(nextY);
	
	if (this->bubblePlayer->getPositionX() < (this->bubblePlayer->getContentSize().width / 2)) {
		// Left edge
		this->player.angle = 180 - this->player.angle;
		auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
		audioEngine->playEffect("sounds/sfx/collide_ball_wall.ogg", false);
	}
	else if (this->bubblePlayer->getPositionX() > Director::getInstance()->getVisibleSize().width - (this->bubblePlayer->getContentSize().width / 2)) {
		// Right edge
		this->player.angle = 180 - this->player.angle;
		auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
		audioEngine->playEffect("sounds/sfx/collide_ball_wall.ogg", false);
	}
	if (this->bubblePlayer->getPositionY() > Director::getInstance()->getVisibleSize().height - (this->bubblePlayer->getContentSize().width / 2)) {
		// Top collision
		this->player.angle = 360 - this->player.angle;
		auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
		audioEngine->playEffect("sounds/sfx/collide_ball_wall.ogg", false);
	}
}

 void Shoot::gamePlay(Node* correctObject) {
		((Sprite*)this->getChildByName("topBoard"))->setVisible(false);
		float size = 0.5;
		if (bubblePlayer->getName() == "pinatacity") { size = 0.7; };
		if (bubblePlayer->getName() == "pinatajungle") { size = 1.0; };
		this->targetPlayer = correctObject;
		auto classReference = this;
	
		auto sequenceForRepeatMovement = CallFunc::create([=]()
		{
			auto  leftTOright = MoveTo::create(4, Vec2(((correctObject->getContentSize().width * size) / 2) - (classReference->xPosi / 2), Director::getInstance()->getVisibleSize().height * 0.85));
			auto  rightTOleft = MoveTo::create(4, Vec2(Director::getInstance()->getVisibleSize().width - (correctObject->getContentSize().width * size / 2) - (classReference->xPosi / 2), Director::getInstance()->getVisibleSize().height * 0.85));
			auto repeatForeverAction = RepeatForever::create(Sequence::create(rightTOleft, leftTOright,NULL));
			correctObject->runAction(repeatForeverAction);
		});
		auto SequenceVal = Sequence::create(ScaleTo::create(0.3, size), MoveTo::create(0.5, Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 0.85)), MoveTo::create(2, Vec2(((correctObject->getContentSize().width * size) / 2) - (xPosi / 2), Director::getInstance()->getVisibleSize().height * 0.85)), sequenceForRepeatMovement,NULL);
		correctObject->runAction(SequenceVal);

		this->getChildByName("bg")->getChildByName("board")->setVisible(false);
		if (bubblePlayer->getName() == "pinatacream")
		this->getChildByName("bg")->getChildByName("Panel_2")->setVisible(false);

		bubblePlayer->setVisible(true);
		rightLine->setVisible(true);
		leftLine->setVisible(true);
		if (bubblePlayer->getName() == "pinatajungle") {
			this->getChildByName("bg")->getChildByName("rightshoot")->setVisible(true);
			this->getChildByName("bg")->getChildByName("leftshoot")->setVisible(true);
		}
		this->getChildByName("bg")->getChildByName("right")->setVisible(true);
		this->getChildByName("bg")->getChildByName("left")->setVisible(true);
		if (bubblePlayer->getName() == "pinatacity")
		this->getChildByName("bg")->getChildByName("slingshot_16")->setVisible(true);

		auto changeFlagInTouchBoard = CallFunc::create([=]()
		{
			classReference->getChildByName("bg")->getChildByName("board")->setTag(1);
		});
		this->runAction(Sequence::create(DelayTime::create(0.8),changeFlagInTouchBoard,NULL));
}

void Shoot::checkBoundaryBall(Node* target, cocos2d::Touch* touch) {

	 if ((touch->getLocation().x >= (Director::getInstance()->getVisibleSize().width * 0.1)) && (touch->getLocation().x <= (Director::getInstance()->getVisibleSize().height * 0.05)) && (touch->getLocation().y <= (Director::getInstance()->getVisibleSize().height * 0.5))) {
		 target->setPosition(touch->getLocation());
			CCLOG("default if touch condition - 660");
	 }

	 if (touch->getLocation().x < (Director::getInstance()->getVisibleSize().width * 0.1)) {
		 if ((bubblePlayer->getPositionY() >= (Director::getInstance()->getVisibleSize().height * 0.05)) && (bubblePlayer->getPositionY() <= (Director::getInstance()->getVisibleSize().height * 0.5))) {
			 target->setPositionY(touch->getLocation().y);
			 CCLOG(" left X fixed , Y coordinate Changes if condition - 666");
		 }
	 }
	 if (touch->getLocation().x > (Director::getInstance()->getVisibleSize().width * 0.9)) {
		 if ((bubblePlayer->getPositionY() >= (Director::getInstance()->getVisibleSize().height * 0.05)) && (bubblePlayer->getPositionY() <= (Director::getInstance()->getVisibleSize().height * 0.5))) {
			 target->setPositionY(touch->getLocation().y);
			 CCLOG(" right X fixed , Y coordinate Changes if condition - 672");
		 }
	 }

	 if ((touch->getLocation().y > (Director::getInstance()->getVisibleSize().height * 0.05))) {
		 if ((touch->getLocation().x >= (Director::getInstance()->getVisibleSize().width * 0.1)) && (touch->getLocation().x <= (Director::getInstance()->getVisibleSize().width * 0.9))) {
			 target->setPositionX(touch->getLocation().x);
			 CCLOG(" bottom Y fixed , X coordinate changes if condition - 679");
		 }
	 }

	 if ((touch->getLocation().y < (Director::getInstance()->getVisibleSize().height * 0.5))) {
		 if ((touch->getLocation().x >= (Director::getInstance()->getVisibleSize().width * 0.1)) && (touch->getLocation().x <= (Director::getInstance()->getVisibleSize().width * 0.9))) {
			 target->setPosition(touch->getLocation());
			 CCLOG("CONDITION XXXX changes done here - 686");
		 }
 }

 if (touch->getLocation().y >= (Director::getInstance()->getVisibleSize().height * 0.5)) {
	 target->setPositionY(Director::getInstance()->getVisibleSize().height * 0.5);
	 CCLOG("CONDITION YYYY -  692");
 }
}

float Shoot::radToDeg(float angle) {
	return angle * (180.0f / M_PI);
}

float Shoot::degToRad(float angle) {
	return angle * (M_PI / 180.0f);
}

int Shoot::getRandomInt(int min, int max) {
	auto randomValue = floor(rand_0_1() * (max - min + 1)) + min;
	
	if (randomValue >= max)
		return max;
	if (randomValue <= min)
		return min;

	return  randomValue;
}

std::tuple<int, int, int> Shoot::levelAllInfo(int currentLevel, int totalCategory, int eachCategoryGroup, int totalSceneTheme, int SceneChangeAfterLevel)
{
	float currentLevelInFloat = static_cast<float>(currentLevel);
	int categoryBase = static_cast<int>(std::ceil(currentLevelInFloat / eachCategoryGroup));

	int categoryNo = totalCategory;

	if (categoryBase != totalCategory) {
		categoryNo = categoryBase % totalCategory;
		if (categoryNo == 0)
			categoryNo = totalCategory;
	}

	if (currentLevel % eachCategoryGroup == 0)
		categoryNo = (categoryBase - 1) % totalCategory + 1;

	int sceneBase = static_cast<int>(std::ceil(currentLevelInFloat / SceneChangeAfterLevel));
	int sceneNo = sceneBase % totalSceneTheme;

	int totalInterationLevel = totalCategory * eachCategoryGroup;
	int Iteration = static_cast<int>(std::floor(currentLevel / totalInterationLevel));
	int level = currentLevel % eachCategoryGroup;
	if (level == 0)
		level = eachCategoryGroup;
	int categoryLevel = (Iteration * eachCategoryGroup) + level;

	if (sceneNo == 0)
		sceneNo = totalSceneTheme;

	if (categoryLevel >= 7) {
		categoryLevel = 7;
	}

	return std::make_tuple(categoryNo, sceneNo, categoryLevel);
}

void Shoot::setSpriteProperties(Sprite* ImageObject, float positionX, float positionY, float scaleX, float scaleY, float anchorX, float anchorY, float rotation, int zorder) {
	ImageObject->setPosition(Vec2(positionX, positionY));
	ImageObject->setScaleX(scaleX);
	ImageObject->setScaleY(scaleY);
	ImageObject->setAnchorPoint(Vec2(anchorX, anchorY));
	ImageObject->setRotation(rotation);
	ImageObject->setZOrder(zorder);

	auto flagTest = shootingFlag;
	auto f = this->shootingFlag;
	auto flagTests = shootingFlag;
}

vector<int> Shoot::getRandomValueRange(int min, int max, int getValue) {
	int count = 0;
	vector<int> objectVector;
	while (count < getValue) {
		int temp = RandomHelper::random_int(min, max);
		bool flag = true;

		for (size_t index = 0; index < objectVector.size(); index++) {
			if (objectVector[index] == temp) {
				flag = false;
				break;
			}
		}

		if (flag) {
			objectVector.push_back(temp);
			count++;
		}
	}

	sort(objectVector.begin(), objectVector.end());
	return objectVector;
}

Shoot::~Shoot(void)
{
	this->removeAllChildrenWithCleanup(true);
}

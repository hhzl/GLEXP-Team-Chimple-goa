
//  DuelScene.cpp
//  rpg
//
//  Created by Srikanth Talapadi on 22/06/16.
//
//

#include "DuelScene.h"
#include "../alphamon/HPMeter.h"
#include "AlphabetGrid.h"
#include "CharGenerator.h"
#include "../alphamon/Alphamon.h"
#include "../alphamon/SelectAlphamonScene.h"
#include "../effects/FShake.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"

USING_NS_CC;

const std::string DuelScene::PANEL_NAME = "Panel_1";
const std::string DuelScene::SLIDER_NAME = "Slider_2";
const std::string DuelScene::LEFT_STAND_NAME = "rocks_2";
const std::string DuelScene::RIGHT_STAND_NAME = "rocks_1";
const int DuelScene::MAX_POINTS_PER_TURN = 20;

Scene* DuelScene::createScene(char myMonChar, char otherMonChar)
{
    auto scene = Scene::create();
    
    auto layer = DuelScene::create(myMonChar, otherMonChar);
    
    scene->addChild(layer);
    
    return scene;
}

DuelScene* DuelScene::create(char myMonChar, char otherMonChar)
{
    DuelScene* duelScene = new (std::nothrow) DuelScene();
    if(duelScene && duelScene->init(myMonChar, otherMonChar))
    {
        duelScene->autorelease();
        return duelScene;
    }
    CC_SAFE_DELETE(duelScene);
    return nullptr;
}

bool DuelScene::init(char myMonChar, char otherMonChar)
{
    if (!Node::init()) {
        return false;
    }
    auto background = CSLoader::createNode("battle_ground.csb");
    addChild(background);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    const int numRows = 4;
    const int numCols = 12;
    const float squareLength = 200.0;
    _grid = AlphabetGrid::create(squareLength * numCols, squareLength * numRows, numRows, numCols);
    auto panel = background->getChildByName(PANEL_NAME);
    panel->addChild(_grid);
    _grid->setPosition(Vec2((panel->getContentSize().width - squareLength * numCols) / 2, (panel->getContentSize().height - squareLength * numRows) / 2));

    _timer = HPMeter::createWithTextureAndPercent("battle_ground/bar_out.png", "battle_ground/bar_in.png", "battle_ground/clock.png", 100);
    _timer->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    addChild(_timer);

    _myMon = Alphamon::createWithAlphabet(myMonChar);
    auto leftStand = background->getChildByName(LEFT_STAND_NAME);
    _myMon->setPosition(leftStand->getPosition() + Vec2(0, 250));
    addChild(_myMon);
    _myMon->setHealth(100);
    _eventDispatcher->addCustomEventListener("alphabet_pressed", CC_CALLBACK_1(DuelScene::onAlphabetSelected, this));
    
    _otherMon = Alphamon::createWithAlphabet(otherMonChar);
    auto rightStand = background->getChildByName(RIGHT_STAND_NAME);
    addChild(_otherMon);
    _otherMon->setPosition(rightStand->getPosition() + Vec2(0, 250));
    _otherMon->setHealth(100);
//    auto lg = LayerGradient::create(Color4B(0.0, 0.0, 0.0, 128.0), Color4B(0.0, 0.0, 0.0, 0.0), Vec2(-1, 0));
//    lg->changeWidthAndHeight(1280.0, 900.0);
//    addChild(lg);
    
    
//    auto amon = CSLoader::createNode("english/A.csb");
//    addChild(amon);
//    amon->setScale(0.5);
//    amon->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
//    auto eyeTimeline = CSLoader::createTimeline("eye_ani/eye_h.csb");
//    amon->getChildByName("FileNode_2")->runAction(eyeTimeline);
//    eyeTimeline->gotoFrameAndPlay(0, true);
//    auto mouthTimeline = CSLoader::createTimeline("mouth_ani/mouth_e.csb");
//    amon->getChildByName("FileNode_4")->runAction(mouthTimeline);
////    mouthTimeline->gotoFrameAndPlay(0, true);
//    mouthTimeline->play("eat", true);
    
    startMyTurn();
    return true;
}

void DuelScene::startMyTurn() {
    if(_myMon->getHealth() <= 0) {
        CCLOG("MyMon game over");
        gameOver();
    } else if (_otherMon->getHealth() <= 0) {
        CCLOG("OtherMon game over");
        gameOver();
    } else {
        auto charArray = CharGenerator::getInstance()->generateMatrixForChoosingAChar(_myMon->getAlphabet(), 4, 12, 50);
        _grid->setCharacters(charArray);
        _grid->enableTouch(true);
        _powerIncr = 100 / _grid->getCountOfAlphabetsWhichMatch(_myMon->getAlphabet());

        _timer->setPercent(100);
        
        auto actionTimer = ActionTween::create(10, "percent", 100.0, 0.0);
        auto armMyMonFunc = CallFunc::create(CC_CALLBACK_0(DuelScene::armMyMon, this));
        
        _timer->runAction(Sequence::create(actionTimer, armMyMonFunc, NULL));
        _myMon->startMyTurn();
        CCLOG("%s", TextureCache::sharedTextureCache()->getCachedTextureInfo().c_str());
    }
}

void DuelScene::gameOver() {
    auto winner = (_myMon->getHealth() <= 0) ? _otherMon : _myMon;
    auto scaleBy = ScaleBy::create(1.0, 1.2);
    auto moveTo = MoveTo::create(1.0, Vec2(1280.0, 900.0));
    auto spawn = TargetedAction::create(winner, Spawn::createWithTwoActions(scaleBy, moveTo));
    auto callbackStart = CallFunc::create(CC_CALLBACK_0(DuelScene::returnToPrevScene, this));
    auto sequence = Sequence::createWithTwoActions(spawn, callbackStart);
    runAction(sequence);
}

void DuelScene::returnToPrevScene() {
    stopAllActions();
    _eventDispatcher->removeCustomEventListeners("alphabet_pressed");
    Director::getInstance()->replaceScene(SelectAlphamon::createScene());
}

void DuelScene::armMyMon() {
    _grid->enableTouch(false);
    auto matchingAlphabets = _grid->getAlphabetsWhichMatch(_myMon->getAlphabet());
    for(auto alpha: matchingAlphabets) {
        CCLOG("meteor %c", alpha->getChar());
        auto particle = cocos2d::ParticleMeteor::create();
        particle->setPosition(alpha->getPosition());
        addChild(particle);
        auto moveTo = JumpTo::create(2, _myMon->getPosition(), 25.0, 1);
        auto callbackJump = CallFunc::create(CC_CALLBACK_0(DuelScene::endMeteor, this, particle));
        
        auto sequence = Sequence::create(moveTo, callbackJump, NULL);
        particle->runAction(sequence);
        
    }
    auto callbackAttack = CallFunc::create(CC_CALLBACK_0(DuelScene::attackOtherMon, this));
    this->runAction(Sequence::create(DelayTime::create(2), callbackAttack, nullptr));
}

void DuelScene::attackOtherMon() {
    auto particle = cocos2d::ParticleMeteor::create();
    particle->setPosition(_myMon->getPosition());
    addChild(particle);
    auto moveTo = TargetedAction::create(particle, JumpTo::create(2, _otherMon->getPosition(), 25.0, 1));
    auto callbackJump = CallFunc::create(CC_CALLBACK_0(DuelScene::endMeteor, this, particle));
    auto callbackAttack = CallFunc::create(CC_CALLBACK_0(DuelScene::attackMyMon, this));
    auto callbackReduceHP = CallFunc::create(CC_CALLBACK_0(DuelScene::reduceHP, this, _otherMon, _myMon->getPower() * MAX_POINTS_PER_TURN / 100));
    auto sequence = Sequence::create(moveTo, callbackJump, _otherMon->shakeAction(), callbackReduceHP, callbackAttack, NULL);
    runAction(sequence);
    _myMon->endMyTurn();
}

void DuelScene::attackMyMon() {
    auto particle = cocos2d::ParticleMeteor::create();
    particle->setPosition(_otherMon->getPosition());
    addChild(particle);
    auto moveTo = TargetedAction::create(particle, JumpTo::create(2, _myMon->getPosition(), 25.0, 1));
    auto callbackJump = CallFunc::create(CC_CALLBACK_0(DuelScene::endMeteor, this, particle));
    auto callbackStart = CallFunc::create(CC_CALLBACK_0(DuelScene::startMyTurn, this));
    auto callbackReduceHP = CallFunc::create(CC_CALLBACK_0(DuelScene::reduceHP, this, _myMon, rand() % MAX_POINTS_PER_TURN));
    auto sequence = Sequence::create(moveTo, callbackJump, _myMon->shakeAction(), callbackReduceHP, callbackStart, NULL);
    runAction(sequence);
    
}

void DuelScene::reduceHP(Alphamon *amon, int numPoints) {
    amon->changeHealth(numPoints * -1);
}

void DuelScene::endMeteor(Node* node) {
    removeChild(node);
}

void DuelScene::onAlphabetSelected(EventCustom *event) {
    char* buf = static_cast<char*>(event->getUserData());
    CCLOG("Pressed %c",buf[0]);
    if(_myMon->getAlphabet() == buf[0]) {
        _myMon->changePower(_powerIncr);
    } else {
        _myMon->changePower(-_powerIncr);
    }
}

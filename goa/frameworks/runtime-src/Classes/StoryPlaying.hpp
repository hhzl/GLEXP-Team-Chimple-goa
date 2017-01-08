//
//  StoryPlaying.hpp
//  goa
//
//  Created by Shyamal.Upadhyaya on 07/01/17.
//
//

#ifndef StoryPlaying_hpp
#define StoryPlaying_hpp

#include <stdio.h>

#include <stdio.h>
#include "menu/MenuContext.h"
#include "cocos2d.h"
#include "RPGConfig.h"
#include "menu/MenuContext.h"
#include "lang/LangUtil.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "editor-support/cocostudio/ActionTimeline/CCFrame.h"
#include "editor-support/cocostudio/CCComExtensionData.h"
#include "ui/UIWidget.h"
#include "ui/GUIExport.h"
#include "cocos-ext.h"
#include "storage/local-storage/LocalStorage.h"


#define NEXT_BUTTON "next"
#define PREVIOUS_BUTTON "previous"
#define CLOSE_BUTTON "close"
#define SOUND_BUTTON "sound"
#define SOUND_BUTTON_WORD "sound_button_2"
#define TEXT_FIELD_WORD "TextField_1"


#define STORY_TEXT "titleText"


class StoryPlaying : public cocos2d::Layer {
public:
    
    static cocos2d::Scene* createScene(int pageIndex, std::string storyId);
    static StoryPlaying* create(int pageIndex, std::string storyId);
    
CC_CONSTRUCTOR_ACCESS:
    virtual bool init(int pageIndex, std::string storyId);
    
    StoryPlaying();
    
    virtual ~StoryPlaying();
    
    void load();
    
    void loadContentPage(std::string contentPageUrl);
    
    void nextStory(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void previousStory(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void onExitTransitionDidStart();
    
    void onEnterTransitionDidFinish();
    
    
    
private:
    std::string _storyInformation;
    std::string _baseDir;
    MenuContext* _menuContext;
    std::string _soundFile;
    std::string _soundEnabled;
    std::string _storyId;
    int _pageIndex;
    bool _isPlayEnded;
    bool _isPlayStarted;
    int _totalStoryPages;
    Node* _talkBubbleNode;
    Node* _contentPageNode;
    Node* _wordBubbleNode;
    float _offsetInY;
    float _offsetInX;
    bool _isNodeDraggable;
    bool _textDisplayAnimationRunning;
    std::string _animationToPlayWhenTouched;
    Color3B _originalSpriteColor;
    std::string _pronouceWord;
    int _zOrder;
    
    std::map<std::string, Color3B> skinColors;
    std::map<std::string, std::string> _wordMappings;
    std::map<std::string, std::string> _pixelPerfectMapping;
    std::map<std::string, std::vector<std::vector<cocos2d::Point>>> _nodesToTringlePointsMapping;
    
    
    std::vector<std::string> _loadedEffects;
    
    cocostudio::timeline::ActionTimeline* _mainTimeLine;
    
    cocos2d::ui::Button* _nextButton;
    cocos2d::ui::Button* _prevButton;
    
    void playEnded();
    
    void onFrameEvent(cocostudio::timeline::Frame* pSender);
    
    void playFrameEventEffect(std::string eventData);
    
    
    
    void createNextAndPreviousButtons();
    
    void createDialogBubble();
    
    void playSound(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void closeDialog(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void narrateDialog(float dt);
    
    void processScene(cocos2d::Node* parent);
    
    void bindListenerToSkeletonNode(cocostudio::timeline::SkeletonNode* skeletonNode);
    
    bool onTouchBeganOnSkeleton(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void onTouchEndedOnSkeleton(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void onTouchMovedOnSkeleton(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void bindListenerToCompositeNode(cocos2d::Node* node) ;
    
    bool onTouchBeganOnComposite(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void onTouchEndedOnComposite(cocos2d::Touch *touch, cocos2d::Event *event) ;
    
    void onTouchMovedOnComposite(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void bindListenerToNode(cocos2d::Node* node);
    
    bool onTouchBeganOnNode(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void onTouchEndedOnNode(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void onTouchMovedOnNode(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void bindEventsToTarget(cocos2d::Node* node);
    
    void playAnimationOnNode(std::string animationName, Node* node);
    
    void cleanUpWhenTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void changeSkinColor(cocostudio::timeline::BoneNode* bone, bool revert);
    
    void changeBoneColor(cocostudio::timeline::BoneNode* skeleton, bool revert);
    
    void createWordBubble();
    
    void showText(std::string nodeName);
    
    bool translatedText(std::string text);
    
    void displayTextAnimationFinished();
    
    void removeWordBubble(float dt);
    
    void wordBubbleDisappeared();
    
    void pronounceWord(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void processPixelPerfectNodes(cocos2d::Node* parent);
    
    void playMasterAnimation();
    
    bool pointInTriangle(cocos2d::Point p0, cocos2d::Point p1, cocos2d::Point p2, cocos2d::Point p3);

};


#endif /* StoryPlaying_hpp */






/**
 * @file CardView.cpp
 * @brief 卡牌视图类实现
 */

#include "views/CardView.h"
#include "configs/CardTypes.h"
#include "utils/CardUtils.h"

USING_NS_CC;

CardView* CardView::create(const CardModel& cardModel)
{
    CardView* view = new (std::nothrow) CardView();
    if (view && view->init(cardModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(const CardModel& cardModel)
{
    if (!Node::init())
    {
        return false;
    }
    
    _cardId = cardModel.getCardId();
    _suit = cardModel.getSuit();
    _face = cardModel.getFace();
    _isFaceUp = cardModel.isFaceUp();
    _isClickable = cardModel.isClickable();
    
    _backgroundSprite = nullptr;
    _bigNumberSprite = nullptr;
    _smallNumberSprite = nullptr;
    _suitSprite = nullptr;
    _frontNode = nullptr;
    _backNode = nullptr;
    _touchListener = nullptr;
    
    // 设置卡牌大小
    this->setContentSize(Size(GameConstants::kCardWidth, GameConstants::kCardHeight));
    
    // 创建正面和背面
    createFrontView();
    createBackView();
    
    // 根据状态显示正面或背面
    setFaceUp(_isFaceUp);
    
    // 设置触摸监听
    setupTouchListener();
    
    // 设置位置
    this->setPosition(cardModel.getPosition());
    
    return true;
}

void CardView::createFrontView()
{
    _frontNode = Node::create();
    _frontNode->setContentSize(this->getContentSize());
    this->addChild(_frontNode, 1);
    
    // 创建卡牌背景
    _backgroundSprite = Sprite::create(GameConstants::kResPath + "card_general.png");
    if (_backgroundSprite)
    {
        _backgroundSprite->setPosition(this->getContentSize() / 2);
        _frontNode->addChild(_backgroundSprite, 0);
    }
    
    updateFrontView();
}

void CardView::createBackView()
{
    _backNode = Node::create();
    _backNode->setContentSize(this->getContentSize());
    this->addChild(_backNode, 0);
    
    // 创建卡牌背面（使用背景图或纯色）
    auto backSprite = Sprite::create(GameConstants::kResPath + "card_general.png");
    if (backSprite)
    {
        backSprite->setPosition(this->getContentSize() / 2);
        backSprite->setColor(Color3B(100, 100, 150)); // 背面用不同颜色区分
        _backNode->addChild(backSprite, 0);
    }
}

void CardView::updateFrontView()
{
    if (!_frontNode)
    {
        return;
    }
    
    bool isRed = CardUtils::isRedSuit(_suit);
    
    // 移除旧的精灵
    if (_bigNumberSprite)
    {
        _bigNumberSprite->removeFromParent();
        _bigNumberSprite = nullptr;
    }
    if (_smallNumberSprite)
    {
        _smallNumberSprite->removeFromParent();
        _smallNumberSprite = nullptr;
    }
    if (_suitSprite)
    {
        _suitSprite->removeFromParent();
        _suitSprite = nullptr;
    }
    
    // 创建大号数字（居中显示）
    std::string bigNumPath = getNumberImagePath(_face, isRed, true);
    _bigNumberSprite = Sprite::create(bigNumPath);
    if (_bigNumberSprite)
    {
        _bigNumberSprite->setPosition(Vec2(
            this->getContentSize().width / 2,
            this->getContentSize().height / 2
        ));
        _frontNode->addChild(_bigNumberSprite, 1);
    }
    
    // 创建小号数字（左上角）
    std::string smallNumPath = getNumberImagePath(_face, isRed, false);
    _smallNumberSprite = Sprite::create(smallNumPath);
    if (_smallNumberSprite)
    {
        _smallNumberSprite->setPosition(Vec2(25, this->getContentSize().height - 30));
        _frontNode->addChild(_smallNumberSprite, 1);
    }
    
    // 创建花色（左上角数字下方）
    std::string suitPath = getSuitImagePath(_suit);
    _suitSprite = Sprite::create(suitPath);
    if (_suitSprite)
    {
        _suitSprite->setPosition(Vec2(25, this->getContentSize().height - 60));
        _suitSprite->setScale(0.5f);
        _frontNode->addChild(_suitSprite, 1);
    }
}

void CardView::setupTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    
    _touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool
    {
        if (!_isClickable || !_isFaceUp)
        {
            return false;
        }
        
        //转换为卡牌节点的本地坐标
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        // 判断是否点中卡牌
        if (rect.containsPoint(locationInNode))
        {
            return true;// 点中了
        }
        return false;
    };
    
    _touchListener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        if (rect.containsPoint(locationInNode))
        {
            if (_clickCallback)
            {
                _clickCallback(_cardId);
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void CardView::updateView(const CardModel& cardModel)
{
    _cardId = cardModel.getCardId();
    _suit = cardModel.getSuit();
    _face = cardModel.getFace();
    
    updateFrontView();
    setFaceUp(cardModel.isFaceUp());
    setClickable(cardModel.isClickable());
}

void CardView::setFaceUp(bool faceUp)
{
    _isFaceUp = faceUp;
    if (_frontNode)
    {
        _frontNode->setVisible(faceUp);
    }
    if (_backNode)
    {
        _backNode->setVisible(!faceUp);
    }
}

void CardView::setClickable(bool clickable)
{
    _isClickable = clickable;
}

void CardView::moveTo(const Vec2& targetPos, float duration, 
                      const std::function<void()>& callback)
{
    auto moveAction = MoveTo::create(duration, targetPos);
    
    if (callback)
    {
        auto sequence = Sequence::create(
            moveAction,
            CallFunc::create(callback),
            nullptr
        );
        this->runAction(sequence);
    }
    else
    {
        this->runAction(moveAction);
    }
}

void CardView::setPositionImmediate(const Vec2& position)
{
    this->stopAllActions();
    this->setPosition(position);
}

std::string CardView::getNumberImagePath(CardFaceType face, bool isRed, bool isBig) const
{
    std::string colorStr = isRed ? "red" : "black";
    std::string sizeStr = isBig ? "big" : "small";
    std::string faceStr = CardUtils::getFaceString(face);
    
    return GameConstants::kNumberPath + sizeStr + "_" + colorStr + "_" + faceStr + ".png";
}

std::string CardView::getSuitImagePath(CardSuitType suit) const
{
    return GameConstants::kSuitsPath + CardUtils::getSuitFileName(suit);
}

/**
 * @file StackView.cpp
 * @brief 手牌区视图类实现
 */

#include "views/StackView.h"
#include "configs/CardTypes.h"

USING_NS_CC;

StackView* StackView::create()
{
    StackView* view = new (std::nothrow) StackView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool StackView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    _topCardView = nullptr;
    _reserveNode = nullptr;
    _reserveSprite = nullptr;
    _reserveCountLabel = nullptr;
    
    // 设置手牌区大小
    this->setContentSize(Size(GameConstants::kStackAreaWidth, GameConstants::kStackAreaHeight));
    
    // 设置顶部牌位置（中间偏左）
    _topCardPos = Vec2(this->getContentSize().width / 2 - 100, 
                       this->getContentSize().height / 2);
    
    // 设置备用牌堆位置（中间偏右）
    _reservePos = Vec2(this->getContentSize().width / 2 + 100,
                       this->getContentSize().height / 2);
    
    // 创建备用牌堆显示
    createReserveView();
    
    return true;
}

void StackView::initStack(const GameModel* gameModel)
{
    if (!gameModel)
    {
        return;
    }
    
    // 设置顶部牌
    if (gameModel->hasStackTopCard())
    {
        setTopCard(gameModel->getStackTopCard());
    }
    
    // 更新备用牌堆显示
    updateReserveDisplay(gameModel->getReserveCardCount());
}

void StackView::createReserveView()
{
    _reserveNode = Node::create();
    _reserveNode->setPosition(_reservePos);
    this->addChild(_reserveNode, 0);
    
    // 创建备用牌堆背景（使用卡牌背面样式）
    _reserveSprite = Sprite::create(GameConstants::kResPath + "card_general.png");
    if (_reserveSprite)
    {
        _reserveSprite->setColor(Color3B(80, 80, 120));
        _reserveNode->addChild(_reserveSprite, 0);
    }
    
    // 创建数量标签
    _reserveCountLabel = Label::createWithSystemFont("0", "Arial", 32);
    if (_reserveCountLabel)
    {
        _reserveCountLabel->setPosition(Vec2(0, -GameConstants::kCardHeight / 2 - 30));
        _reserveCountLabel->setColor(Color3B::WHITE);
        _reserveNode->addChild(_reserveCountLabel, 1);
    }
    
    // 设置触摸监听
    setupReserveTouchListener();
}

void StackView::setupReserveTouchListener()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool
    {
        if (!_reserveNode)
        {
            return false;
        }
        
        Vec2 locationInNode = _reserveNode->convertToNodeSpace(touch->getLocation());
        Size size = Size(GameConstants::kCardWidth, GameConstants::kCardHeight);
        Rect rect = Rect(-size.width / 2, -size.height / 2, size.width, size.height);
        
        if (rect.containsPoint(locationInNode))
        {
            return true;
        }
        return false;
    };
    
    touchListener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        Vec2 locationInNode = _reserveNode->convertToNodeSpace(touch->getLocation());
        Size size = Size(GameConstants::kCardWidth, GameConstants::kCardHeight);
        Rect rect = Rect(-size.width / 2, -size.height / 2, size.width, size.height);
        
        if (rect.containsPoint(locationInNode))
        {
            if (_reserveClickCallback)
            {
                _reserveClickCallback();
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _reserveNode);
}

void StackView::setTopCard(const CardModel& cardModel)
{
    // 移除旧的顶部牌
    if (_topCardView)
    {
        _topCardView->removeFromParent();
        _topCardView = nullptr;
    }
    
    // 创建新的顶部牌视图
    CardModel topCard = cardModel;
    topCard.setFaceUp(true);
    topCard.setClickable(true);
    topCard.setPosition(_topCardPos);
    
    _topCardView = CardView::create(topCard);
    if (_topCardView)
    {
        _topCardView->setClickCallback([this](int cardId) {
            if (_topCardClickCallback)
            {
                _topCardClickCallback(cardId);
            }
        });
        this->addChild(_topCardView, 1);
    }
}

void StackView::updateTopCard(const CardModel& cardModel)
{
    if (_topCardView)
    {
        _topCardView->updateView(cardModel);
    }
    else
    {
        setTopCard(cardModel);
    }
}

Vec2 StackView::getTopCardPosition() const
{
    return this->convertToWorldSpace(_topCardPos);
}

void StackView::updateReserveDisplay(size_t remainingCount)
{
    if (_reserveCountLabel)
    {
        _reserveCountLabel->setString(std::to_string(remainingCount));
    }
    
    // 根据剩余数量调整显示
    if (_reserveSprite)
    {
        if (remainingCount == 0)
        {
            _reserveSprite->setOpacity(100); // 半透明表示无牌
        }
        else
        {
            _reserveSprite->setOpacity(255);
        }
    }
}

Vec2 StackView::getReservePosition() const
{
    return this->convertToWorldSpace(_reservePos);
}

void StackView::playDrawAnimation(const CardModel& newTopCard,
                                  const std::function<void()>& callback)
{
    // 创建临时卡牌视图在备用牌堆位置
    CardModel tempCard = newTopCard;
    tempCard.setFaceUp(false);
    tempCard.setPosition(_reservePos);
    
    CardView* tempCardView = CardView::create(tempCard);
    if (tempCardView)
    {
        this->addChild(tempCardView, 2);
        
        // 移动到顶部牌位置
        tempCardView->moveTo(_topCardPos, GameConstants::kCardMoveTime, 
            [this, tempCardView, newTopCard, callback]() {
                // 移除临时视图
                tempCardView->removeFromParent();
                // 设置新的顶部牌
                this->setTopCard(newTopCard);
                
                if (callback)
                {
                    callback();
                }
            });
    }
    else if (callback)
    {
        callback();
    }
}

void StackView::playReplaceAnimation(CardView* incomingCardView,
                                    const std::function<void()>& callback)
{
    if (!incomingCardView)
    {
        if (callback)
        {
            callback();
        }
        return;
    }
    
    // 移动到顶部牌位置
    incomingCardView->moveTo(_topCardPos, GameConstants::kCardMoveTime, callback);
}

void StackView::playUndoToPlayfieldAnimation(const Vec2& targetWorldPos,
                                             const CardModel& previousTopCard,
                                             const std::function<void()>& callback)
{
    if (!_topCardView)
    {
        // 没有当前顶部牌，直接设置新的顶部牌
        setTopCard(previousTopCard);
        if (callback)
        {
            callback();
        }
        return;
    }
    
    // 转换目标位置为本地坐标
    Vec2 targetLocalPos = this->convertToNodeSpace(targetWorldPos);
    
    // 保存当前顶部牌视图的引用
    CardView* movingCard = _topCardView;
    _topCardView = nullptr;  // 清空引用，但不移除节点
    
    // 先创建之前的顶部牌（但不可见）
    setTopCard(previousTopCard);
    if (_topCardView)
    {
        _topCardView->setOpacity(0);  // 初始不可见
    }
    
    // 移动当前牌到目标位置
    movingCard->moveTo(targetLocalPos, GameConstants::kCardMoveTime, 
        [this, movingCard, callback]() {
            // 移除移动的卡牌
            movingCard->removeFromParent();
            
            // 显示之前的顶部牌
            if (_topCardView)
            {
                _topCardView->setOpacity(255);
            }
            
            if (callback)
            {
                callback();
            }
        });
}

void StackView::playUndoToReserveAnimation(const CardModel& previousTopCard,
                                           const std::function<void()>& callback)
{
    if (!_topCardView)
    {
        // 没有当前顶部牌，直接设置新的顶部牌
        setTopCard(previousTopCard);
        if (callback)
        {
            callback();
        }
        return;
    }
    
    // 保存当前顶部牌视图的引用
    CardView* movingCard = _topCardView;
    _topCardView = nullptr;  // 清空引用，但不移除节点
    
    // 先创建之前的顶部牌（但不可见）
    setTopCard(previousTopCard);
    if (_topCardView)
    {
        _topCardView->setOpacity(0);  // 初始不可见
    }
    
    // 移动当前牌到备用牌堆位置
    movingCard->moveTo(_reservePos, GameConstants::kCardMoveTime, 
        [this, movingCard, callback]() {
            // 移除移动的卡牌
            movingCard->removeFromParent();
            
            // 显示之前的顶部牌
            if (_topCardView)
            {
                _topCardView->setOpacity(255);
            }
            
            if (callback)
            {
                callback();
            }
        });
}

void StackView::setReserveClickCallback(const ReserveClickCallback& callback)
{
    _reserveClickCallback = callback;
}

void StackView::setTopCardClickCallback(const TopCardClickCallback& callback)
{
    _topCardClickCallback = callback;
}

/**
 * @file PlayFieldView.cpp
 * @brief 主牌区视图类实现
 */

#include "views/PlayFieldView.h"
#include "configs/CardTypes.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create()
{
    PlayFieldView* view = new (std::nothrow) PlayFieldView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool PlayFieldView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    // 设置主牌区大小
    this->setContentSize(Size(GameConstants::kPlayFieldWidth, GameConstants::kPlayFieldHeight));
    
    return true;
}

void PlayFieldView::initCards(const GameModel* gameModel)
{
    if (!gameModel)
    {
        return;
    }
    
    clearAllCards();
    
    // 为每张主牌区卡牌创建视图
    const auto& cards = gameModel->getPlayfieldCards();
    for (const auto& cardModel : cards)
    {
        addCard(cardModel);
    }
}

void PlayFieldView::addCard(const CardModel& cardModel)
{
    int cardId = cardModel.getCardId();
    
    // 检查是否已存在
    if (_cardViews.find(cardId) != _cardViews.end())
    {
        CCLOG("PlayFieldView: Card %d already exists", cardId);
        return;
    }
    
    // 创建卡牌视图
    CardView* cardView = CardView::create(cardModel);
    if (cardView)
    {
        cardView->setClickCallback([this](int id) {
            this->onCardClicked(id);
        });
        
        this->addChild(cardView, cardId); // 使用cardId作为zOrder
        _cardViews[cardId] = cardView;
    }
}

void PlayFieldView::removeCard(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        it->second->removeFromParent();
        _cardViews.erase(it);
    }
}

CardView* PlayFieldView::getCardViewById(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        return it->second;
    }
    return nullptr;
}

void PlayFieldView::updateCardView(const CardModel& cardModel)
{
    CardView* cardView = getCardViewById(cardModel.getCardId());
    if (cardView)
    {
        cardView->updateView(cardModel);
    }
}

void PlayFieldView::playMoveAnimation(int cardId, const Vec2& targetPos,
                                      const std::function<void()>& callback)
{
    CardView* cardView = getCardViewById(cardId);
    if (cardView)
    {
        // 移动时禁止点击
        cardView->setClickable(false);
        
        cardView->moveTo(targetPos, GameConstants::kCardMoveTime, [this, cardId, callback]() {
            // 动画完成后从视图中移除
            this->removeCard(cardId);
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

void PlayFieldView::playMoveBackAnimation(int cardId, const Vec2& originalPos,
                                          const std::function<void()>& callback)
{
    CardView* cardView = getCardViewById(cardId);
    if (cardView)
    {
        cardView->moveTo(originalPos, GameConstants::kCardMoveTime, [cardView, callback]() {
            cardView->setClickable(true);
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

void PlayFieldView::setCardClickCallback(const CardClickCallback& callback)
{
    _cardClickCallback = callback;
}

void PlayFieldView::clearAllCards()
{
    for (auto& pair : _cardViews)
    {
        if (pair.second)
        {
            pair.second->removeFromParent();
        }
    }
    _cardViews.clear();
}

void PlayFieldView::onCardClicked(int cardId)
{
    if (_cardClickCallback)
    {
        _cardClickCallback(cardId);
    }
}

/**
 * @file UndoManager.cpp
 * @brief 回退操作管理器实现
 */

#include "managers/UndoManager.h"
#include "cocos2d.h"

USING_NS_CC;

UndoManager::UndoManager()
    : _gameModel(nullptr)
{
}

UndoManager::~UndoManager()
{
}

void UndoManager::init(GameModel* gameModel)
{
    _gameModel = gameModel;
    clearUndoStack();
}

void UndoManager::recordAction(const UndoModel& undoModel)
{
    if (undoModel.isValid())
    {
        _undoStack.push_back(undoModel);
        CCLOG("UndoManager: Recorded action, stack size: %zu", _undoStack.size());
    }
}

void UndoManager::recordPlayfieldToStack(const CardModel& movedCard,
                                         const CardModel& previousTopCard,
                                         const Vec2& originalPos,
                                         const Vec2& targetPos)
{
    UndoModel undoModel(CardOperationType::PLAYFIELD_TO_STACK);
    undoModel.setMovedCard(movedCard);
    undoModel.setPreviousStackTopCard(previousTopCard);
    undoModel.setOriginalPosition(originalPos);
    undoModel.setTargetPosition(targetPos);
    
    recordAction(undoModel);
}

void UndoManager::recordReserveToStack(const CardModel& drawnCard,
                                       const CardModel& previousTopCard)
{
    UndoModel undoModel(CardOperationType::RESERVE_TO_STACK);
    undoModel.setMovedCard(drawnCard);
    undoModel.setPreviousStackTopCard(previousTopCard);
    
    recordAction(undoModel);
}

bool UndoManager::undo()
{
    if (!canUndo())
    {
        CCLOG("UndoManager: Cannot undo, stack is empty");
        return false;
    }
    
    // 获取最后一次操作
    UndoModel undoModel = _undoStack.back();
    _undoStack.pop_back();
    
    // 根据操作类型执行撤销
    switch (undoModel.getOperationType())
    {
        case CardOperationType::PLAYFIELD_TO_STACK:
            undoPlayfieldToStack(undoModel);
            break;
            
        case CardOperationType::RESERVE_TO_STACK:
            undoReserveToStack(undoModel);
            break;
            
        default:
            CCLOG("UndoManager: Unknown operation type");
            return false;
    }
    
    // 通知回调执行视图更新
    if (_undoExecuteCallback)
    {
        _undoExecuteCallback(undoModel);
    }
    
    CCLOG("UndoManager: Undo executed, remaining stack size: %zu", _undoStack.size());
    return true;
}

bool UndoManager::canUndo() const
{
    return !_undoStack.empty();
}

void UndoManager::clearUndoStack()
{
    _undoStack.clear();
}

void UndoManager::setUndoExecuteCallback(const UndoExecuteCallback& callback)
{
    _undoExecuteCallback = callback;
}

void UndoManager::undoPlayfieldToStack(const UndoModel& undoModel)
{
    if (!_gameModel)
    {
        return;
    }
    
    const CardModel& movedCard = undoModel.getMovedCard();
    const CardModel& previousTopCard = undoModel.getPreviousStackTopCard();
    
    // 1. 将当前顶部牌（被移动的牌）放回主牌区
    CardModel cardToRestore = movedCard;
    cardToRestore.setArea(CardAreaType::PLAYFIELD);
    cardToRestore.setPosition(undoModel.getOriginalPosition());
    cardToRestore.setFaceUp(true);
    cardToRestore.setClickable(true);
    _gameModel->addPlayfieldCard(cardToRestore);
    
    // 2. 恢复原来的顶部牌
    _gameModel->setStackTopCard(previousTopCard);
    
    CCLOG("UndoManager: Undone PLAYFIELD_TO_STACK for card %d", movedCard.getCardId());
}

void UndoManager::undoReserveToStack(const UndoModel& undoModel)
{
    if (!_gameModel)
    {
        return;
    }
    
    const CardModel& drawnCard = undoModel.getMovedCard();
    const CardModel& previousTopCard = undoModel.getPreviousStackTopCard();
    
    // 1. 将当前顶部牌（抽取的牌）放回备用牌堆
    CardModel cardToRestore = drawnCard;
    cardToRestore.setArea(CardAreaType::RESERVE);
    cardToRestore.setFaceUp(false);
    cardToRestore.setClickable(false);
    _gameModel->pushReserveCard(cardToRestore);
    
    // 2. 恢复原来的顶部牌
    _gameModel->setStackTopCard(previousTopCard);
    
    CCLOG("UndoManager: Undone RESERVE_TO_STACK for card %d", drawnCard.getCardId());
}

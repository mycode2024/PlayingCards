/**
 * @file GameController.cpp
 * @brief 游戏控制器类实现
 */

#include "controllers/GameController.h"
#include "services/GameModelGenerator.h"
#include "configs/CardTypes.h"
#include "cocos2d.h"

USING_NS_CC;

GameController::GameController()
    : _gameView(nullptr)
    , _isAnimating(false)
{
}

GameController::~GameController()
{
}

bool GameController::init(GameView* gameView)
{
    if (!gameView)
    {
        CCLOG("GameController: GameView is null");
        return false;
    }
    
    _gameView = gameView;
    
    // 初始化撤销管理器
    _undoManager.init(&_gameModel);
    _undoManager.setUndoExecuteCallback([this](const UndoModel& undoModel) {
        this->onUndoExecuted(undoModel);
    });
    
    // 设置视图回调
    setupViewCallbacks();
    
    return true;
}

void GameController::setupViewCallbacks()
{
    if (!_gameView)
    {
        return;
    }
    
    // 主牌区卡牌点击回调
    PlayFieldView* playFieldView = _gameView->getPlayFieldView();
    if (playFieldView)
    {
        playFieldView->setCardClickCallback([this](int cardId) {
            this->handlePlayfieldCardClick(cardId);
        });
    }
    
    // 手牌区相关回调
    StackView* stackView = _gameView->getStackView();
    if (stackView)
    {
        // 备用牌堆点击回调
        stackView->setReserveClickCallback([this]() {
            this->handleReserveClick();
        });
    }
    
    // 回退按钮回调
    _gameView->setUndoClickCallback([this]() {
        this->handleUndoClick();
    });
}

bool GameController::startGame()
{
    // 使用测试数据生成游戏模型
    if (!GameModelGenerator::generateTestModel(_gameModel))
    {
        CCLOG("GameController: Failed to generate test model");
        return false;
    }
    
    // 初始化视图
    if (_gameView)
    {
        _gameView->initGame(&_gameModel);
    }
    
    // 清空撤销栈
    _undoManager.clearUndoStack();
    updateUndoButtonState();
    
    CCLOG("GameController: Game started");
    return true;
}

bool GameController::startGame(const LevelConfig& levelConfig)
{
    // 从配置生成游戏模型
    if (!GameModelGenerator::generate(levelConfig, _gameModel))
    {
        CCLOG("GameController: Failed to generate game model from config");
        return false;
    }
    
    // 初始化视图
    if (_gameView)
    {
        _gameView->initGame(&_gameModel);
    }
    
    // 清空撤销栈
    _undoManager.clearUndoStack();
    updateUndoButtonState();
    
    CCLOG("GameController: Game started from config");
    return true;
}

bool GameController::handlePlayfieldCardClick(int cardId)
{
    if (_isAnimating)
    {
        CCLOG("GameController: Animation in progress, ignoring click");
        return false;
    }
    
    // 获取被点击的卡牌
    CardModel* clickedCard = _gameModel.getPlayfieldCardById(cardId);
    if (!clickedCard)
    {
        CCLOG("GameController: Card %d not found in playfield", cardId);
        return false;
    }
    
    // 检查卡牌是否可点击（未被遮挡）
    if (!clickedCard->isClickable())
    {
        CCLOG("GameController: Card %d is blocked by other cards", cardId);
        return false;
    }
    
    // 检查是否可以与顶部牌匹配
    const CardModel& topCard = _gameModel.getStackTopCard();
    if (!clickedCard->canMatchWith(topCard))
    {
        CCLOG("GameController: Card %d cannot match with top card", cardId);
        return false;
    }
    
    // 执行移动
    executePlayfieldToStack(cardId);
    return true;
}

void GameController::executePlayfieldToStack(int cardId)
{
    _isAnimating = true; // 加锁，防止重复点击
    
    // 保存原始数据用于撤销
    CardModel* clickedCard = _gameModel.getPlayfieldCardById(cardId);
    if (!clickedCard)
    {
        _isAnimating = false;
        return;
    }
    
    CardModel movedCard = *clickedCard;
    CardModel previousTopCard = _gameModel.getStackTopCard();
    Vec2 originalPos = movedCard.getPosition();
    
    // 获取目标位置
    Vec2 targetPos = Vec2::ZERO;
    if (_gameView && _gameView->getStackView())
    {
        targetPos = _gameView->getStackView()->getTopCardPosition();
        // 转换为主牌区视图的本地坐标
        if (_gameView->getPlayFieldView())
        {
            targetPos = _gameView->getPlayFieldView()->convertToNodeSpace(targetPos);
        }
    }
    
    // 记录撤销操作
    _undoManager.recordPlayfieldToStack(movedCard, previousTopCard, originalPos, targetPos);
    
    // 更新模型：从主牌区移除卡牌
    _gameModel.removePlayfieldCard(cardId);
    
    // 更新模型：设置新的顶部牌
    movedCard.setArea(CardAreaType::STACK);
    _gameModel.setStackTopCard(movedCard);
    
    // 播放视图动画
    if (_gameView && _gameView->getPlayFieldView())
    {
        _gameView->getPlayFieldView()->playMoveAnimation(cardId, targetPos, [this, movedCard]() {
            // 动画完成后更新手牌区视图
            if (_gameView && _gameView->getStackView())
            {
                _gameView->getStackView()->setTopCard(movedCard);
            }
            
            // 更新主牌区卡牌的可点击状态
            GameModelGenerator::updatePlayfieldClickable(_gameModel);
            updatePlayfieldCardViews();
            
            _isAnimating = false;
            updateUndoButtonState();
            
            CCLOG("GameController: Card moved to stack");
        });
    }
    else
    {
        _isAnimating = false;
        updateUndoButtonState();
    }
}

bool GameController::handleReserveClick()
{
    if (_isAnimating)
    {
        CCLOG("GameController: Animation in progress, ignoring click");
        return false;
    }
    
    // 检查备用牌堆是否有牌
    if (_gameModel.isReserveEmpty())
    {
        CCLOG("GameController: Reserve is empty");
        return false;
    }
    
    executeReserveDraw();
    return true;
}

void GameController::executeReserveDraw()
{
    _isAnimating = true;
    
    // 保存当前顶部牌用于撤销
    CardModel previousTopCard = _gameModel.getStackTopCard();
    
    // 从备用牌堆抽取一张牌
    CardModel drawnCard;
    if (!_gameModel.drawReserveCard(drawnCard))
    {
        _isAnimating = false;
        return;
    }
    
    // 记录撤销操作
    _undoManager.recordReserveToStack(drawnCard, previousTopCard);
    
    // 更新模型：设置新的顶部牌
    drawnCard.setArea(CardAreaType::STACK);
    drawnCard.setFaceUp(true);
    _gameModel.setStackTopCard(drawnCard);
    
    // 更新视图
    if (_gameView && _gameView->getStackView())
    {
        _gameView->getStackView()->playDrawAnimation(drawnCard, [this]() {
            // 更新备用牌堆显示
            if (_gameView && _gameView->getStackView())
            {
                _gameView->getStackView()->updateReserveDisplay(_gameModel.getReserveCardCount());
            }
            
            _isAnimating = false;
            updateUndoButtonState();
            
            CCLOG("GameController: Drew card from reserve");
        });
    }
    else
    {
        _isAnimating = false;
        updateUndoButtonState();
    }
}

bool GameController::handleUndoClick()
{
    if (_isAnimating)
    {
        CCLOG("GameController: Animation in progress, ignoring undo");
        return false;
    }
    
    if (!canUndo())
    {
        CCLOG("GameController: Nothing to undo");
        return false;
    }
    
    _isAnimating = true;
    
    // 执行撤销
    bool success = _undoManager.undo();
    
    if (!success)
    {
        _isAnimating = false;
    }
    
    return success;
}

void GameController::onUndoExecuted(const UndoModel& undoModel)
{
    // 根据操作类型更新视图
    switch (undoModel.getOperationType())
    {
        case CardOperationType::PLAYFIELD_TO_STACK:
        {
            // 恢复卡牌到主牌区
            const CardModel& movedCard = undoModel.getMovedCard();
            const CardModel& previousTopCard = undoModel.getPreviousStackTopCard();
            Vec2 originalPos = undoModel.getOriginalPosition();
            
            // 计算目标世界坐标
            Vec2 targetWorldPos = Vec2::ZERO;
            if (_gameView && _gameView->getPlayFieldView())
            {
                targetWorldPos = _gameView->getPlayFieldView()->convertToWorldSpace(originalPos);
            }
            
            // 播放手牌区回退动画
            if (_gameView && _gameView->getStackView())
            {
                _gameView->getStackView()->playUndoToPlayfieldAnimation(
                    targetWorldPos, 
                    previousTopCard,
                    [this, movedCard, originalPos]() {
                        // 动画完成后，在主牌区添加卡牌视图
                        if (_gameView && _gameView->getPlayFieldView())
                        {
                            CardModel restoredCard = movedCard;
                            restoredCard.setPosition(originalPos);
                            restoredCard.setFaceUp(true);
                            restoredCard.setClickable(true);
                            _gameView->getPlayFieldView()->addCard(restoredCard);
                        }
                        
                        // 更新主牌区卡牌的可点击状态
                        GameModelGenerator::updatePlayfieldClickable(_gameModel);
                        updatePlayfieldCardViews();
                        
                        _isAnimating = false;
                        updateUndoButtonState();
                        CCLOG("GameController: Undo PLAYFIELD_TO_STACK completed");
                    });
            }
            else
            {
                _isAnimating = false;
                updateUndoButtonState();
            }
            break;
        }
        
        case CardOperationType::RESERVE_TO_STACK:
        {
            const CardModel& previousTopCard = undoModel.getPreviousStackTopCard();
            
            // 播放手牌区回退动画（移回备用牌堆）
            if (_gameView && _gameView->getStackView())
            {
                _gameView->getStackView()->playUndoToReserveAnimation(
                    previousTopCard,
                    [this]() {
                        // 动画完成后更新备用牌堆显示
                        if (_gameView && _gameView->getStackView())
                        {
                            _gameView->getStackView()->updateReserveDisplay(
                                _gameModel.getReserveCardCount());
                        }
                        
                        _isAnimating = false;
                        updateUndoButtonState();
                        CCLOG("GameController: Undo RESERVE_TO_STACK completed");
                    });
            }
            else
            {
                _isAnimating = false;
                updateUndoButtonState();
            }
            break;
        }
        
        default:
            _isAnimating = false;
            updateUndoButtonState();
            break;
    }
    
    CCLOG("GameController: Undo view animation started");
}

bool GameController::canUndo() const
{
    return _undoManager.canUndo();
}

void GameController::updateUndoButtonState()
{
    if (_gameView)
    {
        _gameView->updateUndoButtonState(canUndo());
    }
}

void GameController::updatePlayfieldCardViews()
{
    if (!_gameView || !_gameView->getPlayFieldView())
    {
        return;
    }
    
    // 更新每张卡牌视图的可点击状态
    const auto& cards = _gameModel.getPlayfieldCards();
    for (const auto& card : cards)
    {
        _gameView->getPlayFieldView()->updateCardView(card);
    }
    
    CCLOG("GameController: Updated %zu card views", cards.size());
}

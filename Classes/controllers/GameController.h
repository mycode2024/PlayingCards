/**
 * @file GameController.h
 * @brief 游戏控制器类
 * 
 * 负责协调游戏的整体流程，包括：
 * - 初始化游戏
 * - 处理主牌区卡牌点击（匹配逻辑）
 * - 处理备用牌堆点击（翻牌逻辑）
 * - 处理回退操作
 * - 协调模型和视图的更新
 */

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"
#include "configs/LevelConfig.h"
#include <memory>

/**
 * @brief 游戏控制器类
 * 
 * 作为游戏的核心控制器，处理所有用户操作和业务逻辑。
 * 协调GameModel和GameView之间的交互。
 */
class GameController
{
public:
    /**
     * @brief 构造函数
     */
    GameController();
    
    /**
     * @brief 析构函数
     */
    ~GameController();
    
    // ========== 初始化方法 ==========
    
    /**
     * @brief 初始化控制器
     * @param gameView 游戏视图指针
     * @return 初始化成功返回true
     */
    bool init(GameView* gameView);
    
    /**
     * @brief 开始游戏（使用测试数据）
     * @return 成功返回true
     */
    bool startGame();
    
    /**
     * @brief 开始游戏（从配置加载）
     * @param levelConfig 关卡配置
     * @return 成功返回true
     */
    bool startGame(const LevelConfig& levelConfig);
    
    // ========== 事件处理方法 ==========
    
    /**
     * @brief 处理主牌区卡牌点击
     * @param cardId 被点击的卡牌ID
     * @return 处理成功返回true
     */
    bool handlePlayfieldCardClick(int cardId);
    
    /**
     * @brief 处理备用牌堆点击
     * @return 处理成功返回true
     */
    bool handleReserveClick();
    
    /**
     * @brief 处理回退按钮点击
     * @return 处理成功返回true
     */
    bool handleUndoClick();
    
    // ========== 状态查询方法 ==========
    
    /**
     * @brief 检查是否可以回退
     * @return 可以回退返回true
     */
    bool canUndo() const;
    
    /**
     * @brief 获取游戏模型（只读）
     * @return 游戏模型的const指针
     */
    const GameModel* getGameModel() const { return &_gameModel; }

private:
    /**
     * @brief 设置视图回调
     */
    void setupViewCallbacks();
    
    /**
     * @brief 更新回退按钮状态
     */
    void updateUndoButtonState();
    
    /**
     * @brief 处理撤销执行完成
     * @param undoModel 撤销数据
     */
    void onUndoExecuted(const UndoModel& undoModel);
    
    /**
     * @brief 执行主牌区到手牌区的移动
     * @param cardId 要移动的卡牌ID
     */
    void executePlayfieldToStack(int cardId);
    
    /**
     * @brief 执行备用牌堆翻牌
     */
    void executeReserveDraw();
    
    /**
     * @brief 更新主牌区卡牌视图的可点击状态
     */
    void updatePlayfieldCardViews();

private:
    GameModel _gameModel;           ///< 游戏数据模型
    GameView* _gameView;            ///< 游戏视图指针
    UndoManager _undoManager;       ///< 撤销管理器
    bool _isAnimating;              ///< 是否正在播放动画
};

#endif // __GAME_CONTROLLER_H__

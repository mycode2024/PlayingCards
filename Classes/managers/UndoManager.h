/**
 * @file UndoManager.h
 * @brief 回退操作管理器
 * 
 * 负责管理游戏中的撤销操作，包括：
 * - 记录可撤销的操作
 * - 执行撤销操作
 * - 管理撤销栈
 * 
 * 作为controller的成员变量使用，可持有model数据。
 */

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/UndoModel.h"
#include "models/GameModel.h"
#include <vector>
#include <functional>

/**
 * @brief 回退操作管理器类
 * 
 * 管理撤销栈，提供记录和执行撤销的功能。
 * 符合managers层的设计规范：
 * - 作为controller的成员变量
 * - 可持有model数据
 * - 禁止单例模式
 * - 通过回调与其他模块交互
 */
class UndoManager
{
public:
    /// 撤销执行回调类型
    using UndoExecuteCallback = std::function<void(const UndoModel& undoModel)>;
    
    /**
     * @brief 构造函数
     */
    UndoManager();
    
    /**
     * @brief 析构函数
     */
    ~UndoManager();
    
    // ========== 初始化方法 ==========
    
    /**
     * @brief 初始化
     * @param gameModel 游戏数据模型指针（用于执行撤销时修改数据）
     */
    void init(GameModel* gameModel);
    
    // ========== 记录操作 ==========
    
    /**
     * @brief 记录一次操作（用于后续撤销）
     * @param undoModel 撤销数据模型
     */
    void recordAction(const UndoModel& undoModel);
    
    /**
     * @brief 记录主牌区到手牌区的移动操作
     * @param movedCard 被移动的卡牌
     * @param previousTopCard 原来的手牌区顶部牌
     * @param originalPos 原始位置
     * @param targetPos 目标位置
     */
    void recordPlayfieldToStack(const CardModel& movedCard,
                                const CardModel& previousTopCard,
                                const cocos2d::Vec2& originalPos,
                                const cocos2d::Vec2& targetPos);
    
    /**
     * @brief 记录备用牌堆到手牌区的翻牌操作
     * @param drawnCard 抽取的卡牌
     * @param previousTopCard 原来的手牌区顶部牌
     */
    void recordReserveToStack(const CardModel& drawnCard,
                              const CardModel& previousTopCard);
    
    // ========== 撤销操作 ==========
    
    /**
     * @brief 执行撤销
     * @return 撤销成功返回true
     */
    bool undo();
    
    /**
     * @brief 检查是否可以撤销
     * @return 可以撤销返回true
     */
    bool canUndo() const;
    
    /**
     * @brief 获取撤销栈大小
     * @return 撤销记录数量
     */
    size_t getUndoStackSize() const { return _undoStack.size(); }
    
    // ========== 清理方法 ==========
    
    /**
     * @brief 清空撤销栈
     */
    void clearUndoStack();
    
    // ========== 回调设置 ==========
    
    /**
     * @brief 设置撤销执行回调
     * @param callback 回调函数（用于通知controller执行视图更新）
     */
    void setUndoExecuteCallback(const UndoExecuteCallback& callback);

private:
    /**
     * @brief 执行主牌区到手牌区的撤销
     * @param undoModel 撤销数据
     */
    void undoPlayfieldToStack(const UndoModel& undoModel);
    
    /**
     * @brief 执行备用牌堆到手牌区的撤销
     * @param undoModel 撤销数据
     */
    void undoReserveToStack(const UndoModel& undoModel);

private:
    GameModel* _gameModel;                      // 游戏数据模型指针
    std::vector<UndoModel> _undoStack;          // 撤销栈
    UndoExecuteCallback _undoExecuteCallback;   // 撤销执行回调
};

#endif // __UNDO_MANAGER_H__

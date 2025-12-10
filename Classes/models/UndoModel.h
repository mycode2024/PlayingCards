/**
 * @file UndoModel.h
 * @brief 回退操作数据模型
 * 
 * 存储单次操作的回退数据，用于实现撤销功能。
 * 记录操作类型、涉及的卡牌和位置信息。
 */

#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "models/CardModel.h"
#include "configs/CardTypes.h"
#include "json/document.h"

/**
 * @brief 回退操作数据模型类
 * 
 * 存储一次可撤销操作的完整信息。
 * 支持两种操作类型：
 * 1. PLAYFIELD_TO_STACK: 从主牌区移动到手牌区（匹配消除）
 * 2. RESERVE_TO_STACK: 从备用牌堆移动到手牌区（翻牌）
 */
class UndoModel
{
public:
    /**
     * @brief 默认构造函数
     */
    UndoModel();
    
    /**
     * @brief 带参数的构造函数
     * @param operationType 操作类型
     */
    explicit UndoModel(CardOperationType operationType);
    
    /**
     * @brief 析构函数
     */
    ~UndoModel();
    
    // ========== Getter方法 ==========
    
    /**
     * @brief 获取操作类型
     * @return 操作类型
     */
    CardOperationType getOperationType() const { return _operationType; }
    
    /**
     * @brief 获取被移动的卡牌数据
     * @return 卡牌模型的只读引用
     */
    const CardModel& getMovedCard() const { return _movedCard; }
    
    /**
     * @brief 获取原来的手牌区顶部牌数据
     * @return 卡牌模型的只读引用
     */
    const CardModel& getPreviousStackTopCard() const { return _previousStackTopCard; }
    
    /**
     * @brief 获取卡牌原始位置
     * @return 原始位置
     */
    cocos2d::Vec2 getOriginalPosition() const { return _originalPosition; }
    
    /**
     * @brief 获取目标位置
     * @return 目标位置
     */
    cocos2d::Vec2 getTargetPosition() const { return _targetPosition; }
    
    // ========== Setter方法 ==========
    
    /**
     * @brief 设置操作类型
     * @param operationType 操作类型
     */
    void setOperationType(CardOperationType operationType) { _operationType = operationType; }
    
    /**
     * @brief 设置被移动的卡牌数据
     * @param card 卡牌模型
     */
    void setMovedCard(const CardModel& card) { _movedCard = card; }
    
    /**
     * @brief 设置原来的手牌区顶部牌数据
     * @param card 卡牌模型
     */
    void setPreviousStackTopCard(const CardModel& card) { _previousStackTopCard = card; }
    
    /**
     * @brief 设置卡牌原始位置
     * @param position 原始位置
     */
    void setOriginalPosition(const cocos2d::Vec2& position) { _originalPosition = position; }
    
    /**
     * @brief 设置目标位置
     * @param position 目标位置
     */
    void setTargetPosition(const cocos2d::Vec2& position) { _targetPosition = position; }
    
    // ========== 序列化方法 ==========
    
    /**
     * @brief 序列化为JSON对象
     * @param allocator JSON分配器
     * @return JSON值对象
     */
    rapidjson::Value serialize(rapidjson::Document::AllocatorType& allocator) const;
    
    /**
     * @brief 从JSON对象反序列化
     * @param json JSON值对象
     * @return 是否成功
     */
    bool deserialize(const rapidjson::Value& json);
    
    // ========== 工具方法 ==========
    
    /**
     * @brief 检查回退数据是否有效
     * @return 有效返回true
     */
    bool isValid() const;

private:
    CardOperationType _operationType;       ///< 操作类型
    CardModel _movedCard;                   ///< 被移动的卡牌
    CardModel _previousStackTopCard;        ///< 操作前的手牌区顶部牌
    cocos2d::Vec2 _originalPosition;        ///< 卡牌原始位置
    cocos2d::Vec2 _targetPosition;          ///< 卡牌目标位置
};

#endif // __UNDO_MODEL_H__

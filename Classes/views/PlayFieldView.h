/**
 * @file PlayFieldView.h
 * @brief 主牌区视图类
 * 
 * 负责主牌区（桌面牌区）的可视化显示，包括：
 * - 管理桌面上所有卡牌的显示
 * - 处理卡牌点击事件并通过回调通知Controller
 * - 卡牌移动动画
 */

#ifndef __PLAYFIELD_VIEW_H__
#define __PLAYFIELD_VIEW_H__

#include "cocos2d.h"
#include "views/CardView.h"
#include "models/GameModel.h"
#include <map>
#include <functional>

/**
 * @brief 主牌区视图类
 * 
 * 管理桌面上所有卡牌的显示和交互。
 * 视图层只负责显示和接收用户输入，不包含业务逻辑。
 */
class PlayFieldView : public cocos2d::Node
{
public:
    /// 卡牌点击回调类型：参数为卡牌ID
    using CardClickCallback = std::function<void(int cardId)>;
    
    /**
     * @brief 创建主牌区视图
     * @return 主牌区视图实例
     */
    static PlayFieldView* create();
    
    /**
     * @brief 初始化
     * @return 初始化成功返回true
     */
    virtual bool init() override;
    
    // ========== 卡牌管理方法 ==========
    
    /**
     * @brief 初始化卡牌显示
     * @param gameModel 游戏数据模型（只读）
     */
    void initCards(const GameModel* gameModel);
    
    /**
     * @brief 添加卡牌视图
     * @param cardModel 卡牌数据模型
     */
    void addCard(const CardModel& cardModel);
    
    /**
     * @brief 移除卡牌视图
     * @param cardId 卡牌ID
     */
    void removeCard(int cardId);
    
    /**
     * @brief 根据ID获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图指针，未找到返回nullptr
     */
    CardView* getCardViewById(int cardId);
    
    /**
     * @brief 更新卡牌视图
     * @param cardModel 卡牌数据模型
     */
    void updateCardView(const CardModel& cardModel);
    
    // ========== 动画方法 ==========
    
    /**
     * @brief 播放卡牌匹配移动动画
     * @param cardId 要移动的卡牌ID
     * @param targetPos 目标位置
     * @param callback 动画完成回调
     */
    void playMoveAnimation(int cardId, const cocos2d::Vec2& targetPos,
                          const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 将卡牌移动回原位（用于回退）
     * @param cardId 卡牌ID
     * @param originalPos 原始位置
     * @param callback 动画完成回调
     */
    void playMoveBackAnimation(int cardId, const cocos2d::Vec2& originalPos,
                               const std::function<void()>& callback = nullptr);
    
    // ========== 回调设置 ==========
    
    /**
     * @brief 设置卡牌点击回调
     * @param callback 回调函数
     */
    void setCardClickCallback(const CardClickCallback& callback);
    
    // ========== 清理方法 ==========
    
    /**
     * @brief 清除所有卡牌
     */
    void clearAllCards();

private:
    /**
     * @brief 处理卡牌点击
     * @param cardId 被点击的卡牌ID
     */
    void onCardClicked(int cardId);

private:
    std::map<int, CardView*> _cardViews;    // 卡牌ID到视图的映射
    CardClickCallback _cardClickCallback;    // 卡牌点击回调
};

#endif // __PLAYFIELD_VIEW_H__

/**
 * @file StackView.h
 * @brief 手牌区视图类
 * 
 * 负责手牌区的可视化显示，包括：
 * - 手牌区顶部牌显示
 * - 备用牌堆显示
 * - 点击备用牌堆翻牌的交互
 */

#ifndef __STACK_VIEW_H__
#define __STACK_VIEW_H__

#include "cocos2d.h"
#include "views/CardView.h"
#include "models/GameModel.h"
#include <functional>

/**
 * @brief 手牌区视图类
 * 
 * 管理手牌区顶部牌和备用牌堆的显示与交互。
 * 视图层只负责显示和接收用户输入，不包含业务逻辑。
 */
class StackView : public cocos2d::Node
{
public:
    /// 备用牌堆点击回调类型
    using ReserveClickCallback = std::function<void()>;
    /// 顶部牌点击回调类型
    using TopCardClickCallback = std::function<void(int cardId)>;
    
    /**
     * @brief 创建手牌区视图
     * @return 手牌区视图实例
     */
    static StackView* create();
    
    /**
     * @brief 初始化
     * @return 初始化成功返回true
     */
    virtual bool init() override;
    
    // ========== 初始化方法 ==========
    
    /**
     * @brief 初始化手牌区显示
     * @param gameModel 游戏数据模型（只读）
     */
    void initStack(const GameModel* gameModel);
    
    // ========== 顶部牌操作 ==========
    
    /**
     * @brief 设置顶部牌
     * @param cardModel 卡牌数据模型
     */
    void setTopCard(const CardModel& cardModel);
    
    /**
     * @brief 更新顶部牌显示
     * @param cardModel 卡牌数据模型
     */
    void updateTopCard(const CardModel& cardModel);
    
    /**
     * @brief 获取顶部牌位置（世界坐标）
     * @return 顶部牌中心位置
     */
    cocos2d::Vec2 getTopCardPosition() const;
    
    /**
     * @brief 获取顶部牌视图
     * @return 顶部牌视图指针
     */
    CardView* getTopCardView() const { return _topCardView; }
    
    // ========== 备用牌堆操作 ==========
    
    /**
     * @brief 更新备用牌堆显示
     * @param remainingCount 剩余卡牌数量
     */
    void updateReserveDisplay(size_t remainingCount);
    
    /**
     * @brief 获取备用牌堆位置（世界坐标）
     * @return 备用牌堆中心位置
     */
    cocos2d::Vec2 getReservePosition() const;
    
    // ========== 动画方法 ==========
    
    /**
     * @brief 播放翻牌动画（从备用牌堆到顶部牌位置）
     * @param newTopCard 新的顶部牌数据
     * @param callback 动画完成回调
     */
    void playDrawAnimation(const CardModel& newTopCard,
                          const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放卡牌替换动画
     * @param incomingCardView 进入的卡牌视图
     * @param callback 动画完成回调
     */
    void playReplaceAnimation(CardView* incomingCardView,
                             const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放回退到主牌区动画（当前顶部牌移出，然后显示之前的顶部牌）
     * @param targetWorldPos 目标位置（主牌区世界坐标）
     * @param previousTopCard 之前的顶部牌数据
     * @param callback 动画完成回调
     */
    void playUndoToPlayfieldAnimation(const cocos2d::Vec2& targetWorldPos,
                                      const CardModel& previousTopCard,
                                      const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放回退到备用牌堆动画（当前顶部牌移回备用牌堆，然后显示之前的顶部牌）
     * @param previousTopCard 之前的顶部牌数据
     * @param callback 动画完成回调
     */
    void playUndoToReserveAnimation(const CardModel& previousTopCard,
                                    const std::function<void()>& callback = nullptr);
    
    // ========== 回调设置 ==========
    
    /**
     * @brief 设置备用牌堆点击回调
     * @param callback 回调函数
     */
    void setReserveClickCallback(const ReserveClickCallback& callback);
    
    /**
     * @brief 设置顶部牌点击回调
     * @param callback 回调函数
     */
    void setTopCardClickCallback(const TopCardClickCallback& callback);

private:
    /**
     * @brief 创建备用牌堆显示
     */
    void createReserveView();
    
    /**
     * @brief 设置备用牌堆触摸事件
     */
    void setupReserveTouchListener();

private:
    CardView* _topCardView;                     ///< 顶部牌视图
    cocos2d::Node* _reserveNode;                ///< 备用牌堆容器
    cocos2d::Sprite* _reserveSprite;            ///< 备用牌堆背景
    cocos2d::Label* _reserveCountLabel;         ///< 剩余数量标签
    
    cocos2d::Vec2 _topCardPos;                  ///< 顶部牌位置
    cocos2d::Vec2 _reservePos;                  ///< 备用牌堆位置
    
    ReserveClickCallback _reserveClickCallback; ///< 备用牌堆点击回调
    TopCardClickCallback _topCardClickCallback; ///< 顶部牌点击回调
};

#endif // __STACK_VIEW_H__

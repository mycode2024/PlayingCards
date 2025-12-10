/**
 * @file CardView.h
 * @brief 卡牌视图类
 * 
 * 负责单张卡牌的可视化显示，包括：
 * - 卡牌正面（显示花色和点数）
 * - 卡牌背面
 * - 点击事件处理
 * - 移动动画
 */

#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

/**
 * @brief 卡牌视图类
 * 
 * 继承自cocos2d::Node，负责卡牌的渲染和交互。
 * 视图层只负责显示和接收用户输入，不包含业务逻辑。
 */
class CardView : public cocos2d::Node
{
public:
    // 点击回调函数类型：参数为卡牌ID
    using ClickCallback = std::function<void(int cardId)>;
    
    /**
     * @brief 创建卡牌视图
     * @param cardModel 卡牌数据模型（只读）
     * @return 卡牌视图实例
     */
    static CardView* create(const CardModel& cardModel);
    
    /**
     * @brief 初始化
     * @param cardModel 卡牌数据模型
     * @return 初始化成功返回true
     */
    bool init(const CardModel& cardModel);
    
    // ========== 视图更新方法 ==========
    
    /**
     * @brief 更新卡牌显示
     * @param cardModel 卡牌数据模型
     */
    void updateView(const CardModel& cardModel);
    
    /**
     * @brief 设置是否显示正面
     * @param faceUp true显示正面，false显示背面
     */
    void setFaceUp(bool faceUp);
    
    /**
     * @brief 设置是否可点击
     * @param clickable true表示可点击
     */
    void setClickable(bool clickable);
    
    // ========== 动画方法 ==========
    
    /**
     * @brief 移动到指定位置（带动画）
     * @param targetPos 目标位置
     * @param duration 动画时长
     * @param callback 动画完成回调
     */
    void moveTo(const cocos2d::Vec2& targetPos, float duration, 
                const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 立即移动到指定位置（无动画）
     * @param position 目标位置
     */
    void setPositionImmediate(const cocos2d::Vec2& position);
    
    // ========== 回调设置 ==========
    
    /**
     * @brief 设置点击回调
     * @param callback 回调函数
     */
    void setClickCallback(const ClickCallback& callback) { _clickCallback = callback; }
    
    // ========== Getter方法 ==========
    
    /**
     * @brief 获取卡牌ID
     * @return 卡牌ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 是否正面朝上
     * @return true表示正面朝上
     */
    bool isFaceUp() const { return _isFaceUp; }
    
    /**
     * @brief 是否可点击
     * @return true表示可点击
     */
    bool isClickable() const { return _isClickable; }

private:
    /**
     * @brief 创建卡牌正面显示
     */
    void createFrontView();
    
    /**
     * @brief 创建卡牌背面显示
     */
    void createBackView();
    
    /**
     * @brief 更新正面显示内容
     */
    void updateFrontView();
    
    /**
     * @brief 设置触摸事件
     */
    void setupTouchListener();
    
    /**
     * @brief 获取卡牌数字图片路径
     * @param face 点数
     * @param isRed 是否红色
     * @param isBig 是否大号
     * @return 图片路径
     */
    std::string getNumberImagePath(CardFaceType face, bool isRed, bool isBig) const;
    
    /**
     * @brief 获取花色图片路径
     * @param suit 花色
     * @return 图片路径
     */
    std::string getSuitImagePath(CardSuitType suit) const;

private:
    int _cardId;                            // 卡牌ID
    CardSuitType _suit;                     // 花色
    CardFaceType _face;                     // 点数
    bool _isFaceUp;                         // 是否正面朝上
    bool _isClickable;                      // 是否可点击
    
    cocos2d::Sprite* _backgroundSprite;     // 卡牌背景
    cocos2d::Sprite* _bigNumberSprite;      // 大号数字精灵
    cocos2d::Sprite* _smallNumberSprite;    // 小号数字精灵
    cocos2d::Sprite* _suitSprite;           // 花色精灵
    cocos2d::Node* _frontNode;              // 正面容器
    cocos2d::Node* _backNode;               // 背面容器
    
    ClickCallback _clickCallback;           // 点击回调
    cocos2d::EventListenerTouchOneByOne* _touchListener; // 触摸监听器
};

#endif // __CARD_VIEW_H__

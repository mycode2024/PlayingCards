/**
 * @file CardModel.h
 * @brief 卡牌数据模型
 * 
 * 表示单张卡牌的数据结构，包含花色、点数、位置等信息。
 * 支持序列化和反序列化以实现存档功能。
 */

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "configs/CardTypes.h"
#include "cocos2d.h"
#include "json/document.h"
#include <utils/CardUtils.h>

/**
 * @brief 卡牌数据模型类
 * 
 * 存储单张卡牌的所有数据信息，不包含业务逻辑。
 * 用于表示游戏中每一张卡牌的状态。
 */
class CardModel
{
public:
    /**
     * @brief 默认构造函数
     */
    CardModel();
    
    /**
     * @brief 带参数的构造函数
     * @param cardId 卡牌唯一标识
     * @param suit 花色
     * @param face 点数
     */
    CardModel(int cardId, CardSuitType suit, CardFaceType face);
    
    /**
     * @brief 析构函数
     */
    ~CardModel();
    
    // ========== Getter方法 ==========
    
    /**
     * @brief 获取卡牌ID
     * @return 卡牌唯一标识
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 获取花色
     * @return 花色类型
     */
    CardSuitType getSuit() const { return _suit; }
    
    /**
     * @brief 获取点数
     * @return 点数类型
     */
    CardFaceType getFace() const { return _face; }
    
    /**
     * @brief 获取位置
     * @return 卡牌在场景中的位置
     */
    cocos2d::Vec2 getPosition() const { return _position; }
    
    /**
     * @brief 获取卡牌所在区域
     * @return 区域类型
     */
    CardAreaType getArea() const { return _area; }
    
    /**
     * @brief 是否正面朝上
     * @return true表示正面朝上
     */
    bool isFaceUp() const { return _isFaceUp; }
    
    /**
     * @brief 是否可以被点击
     * @return true表示可以点击
     */
    bool isClickable() const { return _isClickable; }
    
    /**
     * @brief 是否是红色花色（红桃或方块）
     * @return true表示红色
     */
    bool isRed() const { return CardUtils::isRedSuit(_suit); }
    
    // ========== Setter方法 ==========
    
    /**
     * @brief 设置卡牌ID
     * @param cardId 卡牌唯一标识
     */
    void setCardId(int cardId) { _cardId = cardId; }
    
    /**
     * @brief 设置花色
     * @param suit 花色类型
     */
    void setSuit(CardSuitType suit) { _suit = suit; }
    
    /**
     * @brief 设置点数
     * @param face 点数类型
     */
    void setFace(CardFaceType face) { _face = face; }
    
    /**
     * @brief 设置位置
     * @param position 场景中的位置
     */
    void setPosition(const cocos2d::Vec2& position) { _position = position; }
    
    /**
     * @brief 设置所在区域
     * @param area 区域类型
     */
    void setArea(CardAreaType area) { _area = area; }
    
    /**
     * @brief 设置是否正面朝上
     * @param faceUp true表示正面朝上
     */
    void setFaceUp(bool faceUp) { _isFaceUp = faceUp; }
    
    /**
     * @brief 设置是否可点击
     * @param clickable true表示可以点击
     */
    void setClickable(bool clickable) { _isClickable = clickable; }
    
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
     * @brief 判断是否可以与另一张牌匹配
     * @param other 另一张卡牌
     * @return 可以匹配返回true
     */
    bool canMatchWith(const CardModel& other) const;
    
    /**
     * @brief 复制卡牌数据
     * @return 复制后的卡牌模型
     */
    CardModel clone() const;

private:
    int _cardId;                ///< 卡牌唯一标识
    CardSuitType _suit;         ///< 花色
    CardFaceType _face;         ///< 点数
    cocos2d::Vec2 _position;    ///< 位置
    CardAreaType _area;         ///< 所在区域
    bool _isFaceUp;             ///< 是否正面朝上
    bool _isClickable;          ///< 是否可点击
};

#endif // __CARD_MODEL_H__

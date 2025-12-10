/**
 * @file CardTypes.h
 * @brief 卡牌类型定义文件
 * 
 * 定义卡牌的花色类型和点数类型枚举，
 * 以及相关的常量配置。
 */

#ifndef __CARD_TYPES_H__
#define __CARD_TYPES_H__

#include <string>

/**
 * @brief 卡牌花色类型枚举
 * 
 * 定义扑克牌的四种花色：梅花、方块、红桃、黑桃
 */
enum class CardSuitType
{
    NONE = -1,      ///< 无效花色
    CLUBS = 0,      ///< 梅花 (黑色)
    DIAMONDS = 1,   ///< 方块 (红色)
    HEARTS = 2,     ///< 红桃 (红色)
    SPADES = 3,     ///< 黑桃 (黑色)
    COUNT = 4       ///< 花色总数
};

/**
 * @brief 卡牌点数类型枚举
 * 
 * 定义扑克牌的点数：A, 2-10, J, Q, K
 */
enum class CardFaceType
{
    NONE = -1,      ///< 无效点数
    ACE = 0,        ///< A
    TWO = 1,        ///< 2
    THREE = 2,      ///< 3
    FOUR = 3,       ///< 4
    FIVE = 4,       ///< 5
    SIX = 5,        ///< 6
    SEVEN = 6,      ///< 7
    EIGHT = 7,      ///< 8
    NINE = 8,       ///< 9
    TEN = 9,        ///< 10
    JACK = 10,      ///< J
    QUEEN = 11,     ///< Q
    KING = 12,      ///< K
    COUNT = 13      ///< 点数总数
};

/**
 * @brief 卡牌操作类型枚举
 * 
 * 用于记录回退操作的类型
 */
enum class CardOperationType
{
    NONE = 0,               ///< 无操作
    PLAYFIELD_TO_STACK,     ///< 从主牌区移动到手牌区（匹配消除）
    RESERVE_TO_STACK        ///< 从备用牌堆移动到手牌区（翻牌）
};

/**
 * @brief 卡牌区域类型枚举
 */
enum class CardAreaType
{
    NONE = 0,       ///< 无效区域
    PLAYFIELD,      ///< 主牌区（桌面）
    STACK,          ///< 手牌区（底牌堆顶部）
    RESERVE         ///< 备用牌堆
};

/**
 * @brief 游戏常量配置
 */
namespace GameConstants
{
    // 设计分辨率
    constexpr float kDesignWidth = 1080.0f;
    constexpr float kDesignHeight = 2080.0f;
    
    // 区域尺寸
    constexpr float kPlayFieldWidth = 1080.0f;
    constexpr float kPlayFieldHeight = 1500.0f;
    constexpr float kStackAreaWidth = 1080.0f;
    constexpr float kStackAreaHeight = 580.0f;
    
    // 卡牌尺寸（根据实际资源调整）
    constexpr float kCardWidth = 150.0f;
    constexpr float kCardHeight = 210.0f;
    
    // 动画时长
    constexpr float kCardMoveTime = 0.3f;
    
    // 资源路径前缀
    const std::string kResPath = "res/res/";
    const std::string kNumberPath = "res/res/number/";
    const std::string kSuitsPath = "res/res/suits/";
}

#endif // __CARD_TYPES_H__

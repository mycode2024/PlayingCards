/**
 * @file CardUtils.h
 * @brief 卡牌工具函数
 * 
 * 提供卡牌相关的通用工具函数
 */

#ifndef __CARD_UTILS_H__
#define __CARD_UTILS_H__

#include <string>
#include <cstdlib>
#include "configs/CardTypes.h"

/**
 * @brief 卡牌工具函数命名空间
 */
namespace CardUtils
{
    /**
     * @brief 判断花色是否为红色
     * @param suit 花色类型
     * @return 红桃和方块返回true，其他返回false
     */
    inline bool isRedSuit(CardSuitType suit)
    {
        return suit == CardSuitType::HEARTS || suit == CardSuitType::DIAMONDS;
    }
    
    /**
     * @brief 获取点数对应的字符串
     * @param face 点数类型
     * @return 点数字符串 (A, 2-10, J, Q, K)
     */
    inline std::string getFaceString(CardFaceType face)
    {
        switch (face)
        {
            case CardFaceType::ACE:   return "A";
            case CardFaceType::TWO:   return "2";
            case CardFaceType::THREE: return "3";
            case CardFaceType::FOUR:  return "4";
            case CardFaceType::FIVE:  return "5";
            case CardFaceType::SIX:   return "6";
            case CardFaceType::SEVEN: return "7";
            case CardFaceType::EIGHT: return "8";
            case CardFaceType::NINE:  return "9";
            case CardFaceType::TEN:   return "10";
            case CardFaceType::JACK:  return "J";
            case CardFaceType::QUEEN: return "Q";
            case CardFaceType::KING:  return "K";
            default: return "";
        }
    }
    
    /**
     * @brief 获取花色对应的资源文件名
     * @param suit 花色类型
     * @return 花色资源文件名
     */
    inline std::string getSuitFileName(CardSuitType suit)
    {
        switch (suit)
        {
            case CardSuitType::CLUBS:    return "club.png";
            case CardSuitType::DIAMONDS: return "diamond.png";
            case CardSuitType::HEARTS:   return "heart.png";
            case CardSuitType::SPADES:   return "spade.png";
            default: return "";
        }
    }
    
    /**
     * @brief 判断两张牌是否可以匹配（点数差1）
     * @param face1 第一张牌的点数
     * @param face2 第二张牌的点数
     * @return 可以匹配返回true
     */
    inline bool canMatch(CardFaceType face1, CardFaceType face2)
    {
        if (face1 == CardFaceType::NONE || face2 == CardFaceType::NONE)
        {
            return false;
        }
        
        int value1 = static_cast<int>(face1);
        int value2 = static_cast<int>(face2);
        int diff = std::abs(value1 - value2);
        
        // 点数差1可以匹配
        if (diff == 1)
        {
            return true;
        }
        
        // A和K可以匹配（循环匹配）
        if ((face1 == CardFaceType::ACE && face2 == CardFaceType::KING) ||
            (face1 == CardFaceType::KING && face2 == CardFaceType::ACE))
        {
            return true;
        }
        
        return false;
    }
}

#endif // __CARD_UTILS_H__

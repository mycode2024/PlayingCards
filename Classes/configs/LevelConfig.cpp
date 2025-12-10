/**
 * @file LevelConfig.cpp
 * @brief 关卡配置数据结构实现
 */

#include "configs/LevelConfig.h"

LevelConfig::LevelConfig()
    : _levelId(0)
{
}

LevelConfig::~LevelConfig()
{
}

void LevelConfig::addPlayfieldCard(const CardConfigData& cardConfig)
{
    _playfieldCards.push_back(cardConfig);
}

void LevelConfig::addStackCard(const CardConfigData& cardConfig)
{
    _stackCards.push_back(cardConfig);
}

void LevelConfig::clear()
{
    _levelId = 0;
    _playfieldCards.clear();
    _stackCards.clear();
}

bool LevelConfig::isValid() const
{
    // 至少需要有主牌区的牌和备用牌堆的牌
    if (_playfieldCards.empty())
    {
        return false;
    }
    
    if (_stackCards.empty())
    {
        return false;
    }
    
    // 验证每张牌的配置是否有效
    for (const auto& card : _playfieldCards)
    {
        if (card.face == CardFaceType::NONE || card.suit == CardSuitType::NONE)
        {
            return false;
        }
    }
    
    for (const auto& card : _stackCards)
    {
        if (card.face == CardFaceType::NONE || card.suit == CardSuitType::NONE)
        {
            return false;
        }
    }
    
    return true;
}

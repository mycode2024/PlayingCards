/**
 * @file GameModelGenerator.cpp
 * @brief 游戏模型生成服务实现
 */

#include "services/GameModelGenerator.h"
#include "cocos2d.h"

USING_NS_CC;

bool GameModelGenerator::generate(const LevelConfig& levelConfig, GameModel& outGameModel)
{
    if (!levelConfig.isValid())
    {
        CCLOG("GameModelGenerator: Invalid level config");
        return false;
    }
    
    outGameModel.clear();
    
    // 生成主牌区卡牌
    const auto& playfieldConfigs = levelConfig.getPlayfieldCards();
    for (const auto& config : playfieldConfigs)
    {
        int cardId = outGameModel.getNextCardId();
        CardModel card = createCardModel(config, cardId, CardAreaType::PLAYFIELD);
        card.setFaceUp(true);       // 主牌区的牌默认正面朝上
        card.setClickable(false);   // 先设置为不可点击，稍后统一更新
        outGameModel.addPlayfieldCard(card);
    }
    
    // 更新可点击状态（基于遮挡检测）
    updatePlayfieldClickable(outGameModel);
    
    // 生成备用牌堆卡牌
    const auto& stackConfigs = levelConfig.getStackCards();
    if (stackConfigs.empty())
    {
        CCLOG("GameModelGenerator: No stack cards in config");
        return false;
    }
    
    // 第一张牌作为初始顶部牌
    const auto& firstStackConfig = stackConfigs[0];
    int topCardId = outGameModel.getNextCardId();
    CardModel topCard = createCardModel(firstStackConfig, topCardId, CardAreaType::STACK);
    topCard.setFaceUp(true);
    topCard.setClickable(false);    // 顶部牌不可直接点击
    outGameModel.setStackTopCard(topCard);
    
    // 剩余的牌放入备用牌堆
    for (size_t i = 1; i < stackConfigs.size(); i++)
    {
        int cardId = outGameModel.getNextCardId();
        CardModel card = createCardModel(stackConfigs[i], cardId, CardAreaType::RESERVE);
        card.setFaceUp(false);      // 备用牌背面朝上
        card.setClickable(false);
        outGameModel.addReserveCard(card);
    }
    
    CCLOG("GameModelGenerator: Generated %zu playfield cards, 1 top card, %zu reserve cards",
          outGameModel.getPlayfieldCardCount(), outGameModel.getReserveCardCount());
    
    return true;
}

bool GameModelGenerator::generateTestModel(GameModel& outGameModel)
{
    outGameModel.clear();
    
    // 创建测试主牌区卡牌 - 按文档示例
    // 左侧列
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::CLUBS, CardFaceType::KING);
        card.setPosition(Vec2(250, 1000));
        card.setArea(CardAreaType::PLAYFIELD);
        card.setFaceUp(true);
        card.setClickable(true);
        outGameModel.addPlayfieldCard(card);
    }
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::CLUBS, CardFaceType::THREE);
        card.setPosition(Vec2(300, 800));
        card.setArea(CardAreaType::PLAYFIELD);
        card.setFaceUp(true);
        card.setClickable(true);
        outGameModel.addPlayfieldCard(card);
    }
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::DIAMONDS, CardFaceType::THREE);
        card.setPosition(Vec2(350, 600));
        card.setArea(CardAreaType::PLAYFIELD);
        card.setFaceUp(true);
        card.setClickable(true);
        outGameModel.addPlayfieldCard(card);
    }
    
    // 右侧列
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::CLUBS, CardFaceType::THREE);
        card.setPosition(Vec2(850, 1000));
        card.setArea(CardAreaType::PLAYFIELD);
        card.setFaceUp(true);
        card.setClickable(true);
        outGameModel.addPlayfieldCard(card);
    }
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::CLUBS, CardFaceType::THREE);
        card.setPosition(Vec2(800, 800));
        card.setArea(CardAreaType::PLAYFIELD);
        card.setFaceUp(true);
        card.setClickable(true);
        outGameModel.addPlayfieldCard(card);
    }
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::SPADES, CardFaceType::TWO);
        card.setPosition(Vec2(750, 600));
        card.setArea(CardAreaType::PLAYFIELD);
        card.setFaceUp(true);
        card.setClickable(true);
        outGameModel.addPlayfieldCard(card);
    }
    
    // 创建手牌区顶部牌 - 4号牌
    {
        CardModel topCard(outGameModel.getNextCardId(), CardSuitType::CLUBS, CardFaceType::FOUR);
        topCard.setArea(CardAreaType::STACK);
        topCard.setFaceUp(true);
        topCard.setClickable(false);
        outGameModel.setStackTopCard(topCard);
    }
    
    // 创建备用牌堆
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::HEARTS, CardFaceType::ACE);
        card.setArea(CardAreaType::RESERVE);
        card.setFaceUp(false);
        card.setClickable(false);
        outGameModel.addReserveCard(card);
    }
    {
        CardModel card(outGameModel.getNextCardId(), CardSuitType::DIAMONDS, CardFaceType::FOUR);
        card.setArea(CardAreaType::RESERVE);
        card.setFaceUp(false);
        card.setClickable(false);
        outGameModel.addReserveCard(card);
    }
    
    // 更新可点击状态
    updatePlayfieldClickable(outGameModel);
    
    CCLOG("GameModelGenerator: Generated test model");
    return true;
}

CardModel GameModelGenerator::createCardModel(const CardConfigData& configData, 
                                              int cardId, 
                                              CardAreaType area)
{
    CardModel card(cardId, configData.suit, configData.face);
    card.setPosition(configData.position);
    card.setArea(area);
    return card;
}

bool GameModelGenerator::isCardOverlapping(const CardModel& card1, const CardModel& card2)
{
    // 获取卡牌尺寸
    float cardWidth = GameConstants::kCardWidth;
    float cardHeight = GameConstants::kCardHeight;
    
    // 获取两张卡牌的位置
    Vec2 pos1 = card1.getPosition();
    Vec2 pos2 = card2.getPosition();
    
    // 计算卡牌边界（位置是中心点）
    float left1 = pos1.x - cardWidth / 2;
    float right1 = pos1.x + cardWidth / 2;
    float bottom1 = pos1.y - cardHeight / 2;
    float top1 = pos1.y + cardHeight / 2;
    
    float left2 = pos2.x - cardWidth / 2;
    float right2 = pos2.x + cardWidth / 2;
    float bottom2 = pos2.y - cardHeight / 2;
    float top2 = pos2.y + cardHeight / 2;
    
    // 检查是否有重叠（水平和垂直方向都有交集）
    bool overlapX = (left1 < right2) && (right1 > left2);
    bool overlapY = (bottom1 < top2) && (top1 > bottom2);
    
    return overlapX && overlapY;
}

void GameModelGenerator::updatePlayfieldClickable(GameModel& gameModel)
{
    auto& cards = gameModel.getPlayfieldCardsMutable();
    
    // 遍历每张卡牌，检查是否被其他卡牌遮挡
    for (auto& card : cards)
    {
        bool isBlocked = false;
        
        // 检查是否被其他卡牌遮挡
        // 在游戏中，y坐标较小的卡牌在上面（靠近玩家）
        for (const auto& otherCard : cards)
        {
            if (card.getCardId() == otherCard.getCardId())
            {
                continue;  // 跳过自己
            }
            
            // 如果另一张卡牌的y坐标更小（在上面）且与当前卡牌重叠
            if (otherCard.getPosition().y < card.getPosition().y)
            {
                if (isCardOverlapping(card, otherCard))
                {
                    isBlocked = true;
                    break;
                }
            }
        }
        
        // 设置可点击状态：未被遮挡的卡牌可以点击
        card.setClickable(!isBlocked);
    }
    
    CCLOG("GameModelGenerator: Updated clickable state for %zu cards", cards.size());
}

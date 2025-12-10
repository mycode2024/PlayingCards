/**
 * @file GameModel.cpp
 * @brief 游戏运行时数据模型实现
 */

#include "models/GameModel.h"
#include <algorithm>

GameModel::GameModel()
    : _nextCardId(0)
{
}

GameModel::~GameModel()
{
}

void GameModel::addPlayfieldCard(const CardModel& card)
{
    _playfieldCards.push_back(card);
}

bool GameModel::removePlayfieldCard(int cardId)
{
    auto it = std::find_if(_playfieldCards.begin(), _playfieldCards.end(),
        [cardId](const CardModel& card) {
            return card.getCardId() == cardId;
        });
    
    if (it != _playfieldCards.end())
    {
        _playfieldCards.erase(it);
        return true;
    }
    return false;
}

CardModel* GameModel::getPlayfieldCardById(int cardId)
{
    auto it = std::find_if(_playfieldCards.begin(), _playfieldCards.end(),
        [cardId](const CardModel& card) {
            return card.getCardId() == cardId;
        });
    
    if (it != _playfieldCards.end())
    {
        return &(*it);
    }
    return nullptr;
}

void GameModel::setStackTopCard(const CardModel& card)
{
    _stackTopCard = card;
    _stackTopCard.setArea(CardAreaType::STACK);
}

void GameModel::addReserveCard(const CardModel& card)
{
    _reserveCards.push_back(card);
}

bool GameModel::drawReserveCard(CardModel& outCard)
{
    if (_reserveCards.empty())
    {
        return false;
    }
    
    outCard = _reserveCards.back();
    _reserveCards.pop_back();
    return true;
}

void GameModel::pushReserveCard(const CardModel& card)
{
    _reserveCards.push_back(card);
}

CardModel* GameModel::findCardById(int cardId)
{
    // 先在主牌区查找
    CardModel* card = getPlayfieldCardById(cardId);
    if (card)
    {
        return card;
    }
    
    // 检查手牌区顶部牌
    if (_stackTopCard.getCardId() == cardId)
    {
        return &_stackTopCard;
    }
    
    // 在备用牌堆查找
    auto it = std::find_if(_reserveCards.begin(), _reserveCards.end(),
        [cardId](const CardModel& c) {
            return c.getCardId() == cardId;
        });
    
    if (it != _reserveCards.end())
    {
        return &(*it);
    }
    
    return nullptr;
}

void GameModel::clear()
{
    _playfieldCards.clear();
    _stackTopCard = CardModel();
    _reserveCards.clear();
    _nextCardId = 0;
}

int GameModel::getNextCardId()
{
    return _nextCardId++;
}

rapidjson::Value GameModel::serialize(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value json(rapidjson::kObjectType);
    
    // 序列化主牌区
    rapidjson::Value playfieldArray(rapidjson::kArrayType);
    for (const auto& card : _playfieldCards)
    {
        playfieldArray.PushBack(card.serialize(allocator), allocator);
    }
    json.AddMember("playfieldCards", playfieldArray, allocator);
    
    // 序列化手牌区顶部牌
    json.AddMember("stackTopCard", _stackTopCard.serialize(allocator), allocator);
    
    // 序列化备用牌堆
    rapidjson::Value reserveArray(rapidjson::kArrayType);
    for (const auto& card : _reserveCards)
    {
        reserveArray.PushBack(card.serialize(allocator), allocator);
    }
    json.AddMember("reserveCards", reserveArray, allocator);
    
    json.AddMember("nextCardId", _nextCardId, allocator);
    
    return json;
}

bool GameModel::deserialize(const rapidjson::Value& json)
{
    if (!json.IsObject())
    {
        return false;
    }
    
    clear();
    
    // 反序列化主牌区
    if (json.HasMember("playfieldCards") && json["playfieldCards"].IsArray())
    {
        const auto& playfieldArray = json["playfieldCards"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++)
        {
            CardModel card;
            if (card.deserialize(playfieldArray[i]))
            {
                _playfieldCards.push_back(card);
            }
        }
    }
    
    // 反序列化手牌区顶部牌
    if (json.HasMember("stackTopCard"))
    {
        _stackTopCard.deserialize(json["stackTopCard"]);
    }
    
    // 反序列化备用牌堆
    if (json.HasMember("reserveCards") && json["reserveCards"].IsArray())
    {
        const auto& reserveArray = json["reserveCards"];
        for (rapidjson::SizeType i = 0; i < reserveArray.Size(); i++)
        {
            CardModel card;
            if (card.deserialize(reserveArray[i]))
            {
                _reserveCards.push_back(card);
            }
        }
    }
    
    if (json.HasMember("nextCardId") && json["nextCardId"].IsInt())
    {
        _nextCardId = json["nextCardId"].GetInt();
    }
    
    return true;
}

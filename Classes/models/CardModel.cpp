/**
 * @file CardModel.cpp
 * @brief 卡牌数据模型实现
 */

#include "models/CardModel.h"
#include "utils/CardUtils.h"

CardModel::CardModel()
    : _cardId(-1)
    , _suit(CardSuitType::NONE)
    , _face(CardFaceType::NONE)
    , _position(cocos2d::Vec2::ZERO)
    , _area(CardAreaType::NONE)
    , _isFaceUp(false)
    , _isClickable(false)
{
}

CardModel::CardModel(int cardId, CardSuitType suit, CardFaceType face)
    : _cardId(cardId)
    , _suit(suit)
    , _face(face)
    , _position(cocos2d::Vec2::ZERO)
    , _area(CardAreaType::NONE)
    , _isFaceUp(false)
    , _isClickable(false)
{
}

CardModel::~CardModel()
{
}

rapidjson::Value CardModel::serialize(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value json(rapidjson::kObjectType);
    
    json.AddMember("cardId", _cardId, allocator);
    json.AddMember("suit", static_cast<int>(_suit), allocator);
    json.AddMember("face", static_cast<int>(_face), allocator);
    
    rapidjson::Value posObj(rapidjson::kObjectType);
    posObj.AddMember("x", _position.x, allocator);
    posObj.AddMember("y", _position.y, allocator);
    json.AddMember("position", posObj, allocator);
    
    json.AddMember("area", static_cast<int>(_area), allocator);
    json.AddMember("isFaceUp", _isFaceUp, allocator);
    json.AddMember("isClickable", _isClickable, allocator);
    
    return json;
}

bool CardModel::deserialize(const rapidjson::Value& json)
{
    if (!json.IsObject())
    {
        return false;
    }
    
    if (json.HasMember("cardId") && json["cardId"].IsInt())
    {
        _cardId = json["cardId"].GetInt();
    }
    
    if (json.HasMember("suit") && json["suit"].IsInt())
    {
        _suit = static_cast<CardSuitType>(json["suit"].GetInt());
    }
    
    if (json.HasMember("face") && json["face"].IsInt())
    {
        _face = static_cast<CardFaceType>(json["face"].GetInt());
    }
    
    if (json.HasMember("position") && json["position"].IsObject())
    {
        const auto& posObj = json["position"];
        if (posObj.HasMember("x") && posObj.HasMember("y"))
        {
            _position.x = posObj["x"].GetFloat();
            _position.y = posObj["y"].GetFloat();
        }
    }
    
    if (json.HasMember("area") && json["area"].IsInt())
    {
        _area = static_cast<CardAreaType>(json["area"].GetInt());
    }
    
    if (json.HasMember("isFaceUp") && json["isFaceUp"].IsBool())
    {
        _isFaceUp = json["isFaceUp"].GetBool();
    }
    
    if (json.HasMember("isClickable") && json["isClickable"].IsBool())
    {
        _isClickable = json["isClickable"].GetBool();
    }
    
    return true;
}

bool CardModel::canMatchWith(const CardModel& other) const
{
    return CardUtils::canMatch(_face, other._face);
}

CardModel CardModel::clone() const
{
    CardModel copy;
    copy._cardId = _cardId;
    copy._suit = _suit;
    copy._face = _face;
    copy._position = _position;
    copy._area = _area;
    copy._isFaceUp = _isFaceUp;
    copy._isClickable = _isClickable;
    return copy;
}

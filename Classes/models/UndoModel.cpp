/**
 * @file UndoModel.cpp
 * @brief 回退操作数据模型实现
 */

#include "models/UndoModel.h"

UndoModel::UndoModel()
    : _operationType(CardOperationType::NONE)
    , _originalPosition(cocos2d::Vec2::ZERO)
    , _targetPosition(cocos2d::Vec2::ZERO)
{
}

UndoModel::UndoModel(CardOperationType operationType)
    : _operationType(operationType)
    , _originalPosition(cocos2d::Vec2::ZERO)
    , _targetPosition(cocos2d::Vec2::ZERO)
{
}

UndoModel::~UndoModel()
{
}

rapidjson::Value UndoModel::serialize(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value json(rapidjson::kObjectType);
    
    json.AddMember("operationType", static_cast<int>(_operationType), allocator);
    json.AddMember("movedCard", _movedCard.serialize(allocator), allocator);
    json.AddMember("previousStackTopCard", _previousStackTopCard.serialize(allocator), allocator);
    
    rapidjson::Value origPosObj(rapidjson::kObjectType);
    origPosObj.AddMember("x", _originalPosition.x, allocator);
    origPosObj.AddMember("y", _originalPosition.y, allocator);
    json.AddMember("originalPosition", origPosObj, allocator);
    
    rapidjson::Value targetPosObj(rapidjson::kObjectType);
    targetPosObj.AddMember("x", _targetPosition.x, allocator);
    targetPosObj.AddMember("y", _targetPosition.y, allocator);
    json.AddMember("targetPosition", targetPosObj, allocator);
    
    return json;
}

bool UndoModel::deserialize(const rapidjson::Value& json)
{
    if (!json.IsObject())
    {
        return false;
    }
    
    if (json.HasMember("operationType") && json["operationType"].IsInt())
    {
        _operationType = static_cast<CardOperationType>(json["operationType"].GetInt());
    }
    
    if (json.HasMember("movedCard"))
    {
        _movedCard.deserialize(json["movedCard"]);
    }
    
    if (json.HasMember("previousStackTopCard"))
    {
        _previousStackTopCard.deserialize(json["previousStackTopCard"]);
    }
    
    if (json.HasMember("originalPosition") && json["originalPosition"].IsObject())
    {
        const auto& posObj = json["originalPosition"];
        if (posObj.HasMember("x") && posObj.HasMember("y"))
        {
            _originalPosition.x = posObj["x"].GetFloat();
            _originalPosition.y = posObj["y"].GetFloat();
        }
    }
    
    if (json.HasMember("targetPosition") && json["targetPosition"].IsObject())
    {
        const auto& posObj = json["targetPosition"];
        if (posObj.HasMember("x") && posObj.HasMember("y"))
        {
            _targetPosition.x = posObj["x"].GetFloat();
            _targetPosition.y = posObj["y"].GetFloat();
        }
    }
    
    return true;
}

bool UndoModel::isValid() const
{
    // 操作类型必须有效
    if (_operationType == CardOperationType::NONE)
    {
        return false;
    }
    
    // 被移动的卡牌ID必须有效
    if (_movedCard.getCardId() < 0)
    {
        return false;
    }
    
    return true;
}

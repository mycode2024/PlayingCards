/**
 * @file LevelConfigLoader.cpp
 * @brief 关卡配置加载器实现
 */

#include "configs/LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC;

bool LevelConfigLoader::loadFromFile(const std::string& filePath, LevelConfig& outConfig)
{
    // 读取文件内容
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    if (fullPath.empty())
    {
        CCLOG("LevelConfigLoader: File not found - %s", filePath.c_str());
        return false;
    }
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(fullPath);
    if (fileContent.empty())
    {
        CCLOG("LevelConfigLoader: Failed to read file - %s", filePath.c_str());
        return false;
    }
    
    return loadFromString(fileContent, outConfig);
}

bool LevelConfigLoader::loadFromString(const std::string& jsonString, LevelConfig& outConfig)
{
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());
    
    if (doc.HasParseError())
    {
        CCLOG("LevelConfigLoader: JSON parse error");
        return false;
    }
    
    outConfig.clear();
    
    // 解析主牌区配置 (Playfield)
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const auto& playfieldArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++)
        {
            const auto& cardObj = playfieldArray[i];
            CardConfigData cardConfig;
            
            // 解析点数
            if (cardObj.HasMember("CardFace") && cardObj["CardFace"].IsInt())
            {
                cardConfig.face = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
            }
            
            // 解析花色
            if (cardObj.HasMember("CardSuit") && cardObj["CardSuit"].IsInt())
            {
                cardConfig.suit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());
            }
            
            // 解析位置
            if (cardObj.HasMember("Position") && cardObj["Position"].IsObject())
            {
                const auto& posObj = cardObj["Position"];
                float x = 0, y = 0;
                if (posObj.HasMember("x"))
                {
                    x = posObj["x"].IsInt() ? 
                        static_cast<float>(posObj["x"].GetInt()) : 
                        posObj["x"].GetFloat();
                }
                if (posObj.HasMember("y"))
                {
                    y = posObj["y"].IsInt() ? 
                        static_cast<float>(posObj["y"].GetInt()) : 
                        posObj["y"].GetFloat();
                }
                cardConfig.position = Vec2(x, y);
            }
            
            outConfig.addPlayfieldCard(cardConfig);
        }
    }
    
    // 解析备用牌堆配置 (Stack)
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const auto& stackArray = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArray.Size(); i++)
        {
            const auto& cardObj = stackArray[i];
            CardConfigData cardConfig;
            
            // 解析点数
            if (cardObj.HasMember("CardFace") && cardObj["CardFace"].IsInt())
            {
                cardConfig.face = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
            }
            
            // 解析花色
            if (cardObj.HasMember("CardSuit") && cardObj["CardSuit"].IsInt())
            {
                cardConfig.suit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());
            }
            
            // 备用牌堆的位置通常不重要，设为默认值
            cardConfig.position = Vec2::ZERO;
            
            outConfig.addStackCard(cardConfig);
        }
    }
    
    // 验证配置
    if (!outConfig.isValid())
    {
        CCLOG("LevelConfigLoader: Invalid level config");
        return false;
    }
    
    CCLOG("LevelConfigLoader: Loaded %zu playfield cards, %zu stack cards",
          outConfig.getPlayfieldCardCount(), outConfig.getStackCardCount());
    
    return true;
}

std::string LevelConfigLoader::getLevelConfigPath(int levelId)
{
    char path[256];
    snprintf(path, sizeof(path), "levels/level_%d.json", levelId);
    return std::string(path);
}

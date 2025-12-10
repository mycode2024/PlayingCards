/**
 * @file GameModelGenerator.h
 * @brief 游戏模型生成服务
 * 
 * 负责将静态配置（LevelConfig）转换为运行时数据（GameModel）。
 * 这是一个无状态的服务类，提供静态方法。
 */

#ifndef __GAME_MODEL_GENERATOR_H__
#define __GAME_MODEL_GENERATOR_H__

#include "configs/LevelConfig.h"
#include "models/GameModel.h"
#include <memory>

/**
 * @brief 游戏模型生成器类
 * 
 * 提供从关卡配置生成游戏模型的静态方法。
 * 符合services层的设计规范：无状态、不持有数据。
 */
class GameModelGenerator
{
public:
    /**
     * @brief 从关卡配置生成游戏模型
     * @param levelConfig 关卡配置
     * @param outGameModel 输出的游戏模型
     * @return 生成成功返回true
     */
    static bool generate(const LevelConfig& levelConfig, GameModel& outGameModel);
    
    /**
     * @brief 生成测试用的游戏模型
     * @param outGameModel 输出的游戏模型
     * @return 生成成功返回true
     */
    static bool generateTestModel(GameModel& outGameModel);

private:
    /**
     * @brief 私有构造函数，禁止实例化
     */
    GameModelGenerator() = delete;
    
    /**
     * @brief 从配置数据创建卡牌模型
     * @param configData 卡牌配置数据
     * @param cardId 卡牌ID
     * @param area 卡牌区域
     * @return 卡牌模型
     */
    static CardModel createCardModel(const CardConfigData& configData, 
                                     int cardId, 
                                     CardAreaType area);
    
    /**
     * @brief 检查两张卡牌是否重叠
     * @param card1 第一张卡牌
     * @param card2 第二张卡牌
     * @return 重叠返回true
     */
    static bool isCardOverlapping(const CardModel& card1, const CardModel& card2);

public:
    /**
     * @brief 更新主牌区卡牌的可点击状态
     * @param gameModel 游戏模型
     * 
     * 检查每张卡牌是否被其他卡牌遮挡，更新其可点击状态
     */
    static void updatePlayfieldClickable(GameModel& gameModel);
};

#endif // __GAME_MODEL_GENERATOR_H__

/**
 * @file LevelConfig.h
 * @brief 关卡配置数据结构
 * 
 * 定义关卡配置的静态数据结构，用于存储从JSON文件加载的关卡数据。
 * 这是只读的配置数据，不包含运行时状态。
 */

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include <vector>
#include "configs/CardTypes.h"
#include "cocos2d.h"

/**
 * @brief 单张卡牌的配置数据
 * 
 * 存储关卡配置中每张卡牌的初始设置
 */
struct CardConfigData
{
    CardFaceType face;          ///< 点数
    CardSuitType suit;          ///< 花色
    cocos2d::Vec2 position;     ///< 初始位置
    
    CardConfigData()
        : face(CardFaceType::NONE)
        , suit(CardSuitType::NONE)
        , position(cocos2d::Vec2::ZERO)
    {
    }
    
    CardConfigData(CardFaceType f, CardSuitType s, const cocos2d::Vec2& pos)
        : face(f)
        , suit(s)
        , position(pos)
    {
    }
};

/**
 * @brief 关卡配置类
 * 
 * 存储一个关卡的完整配置数据，包括：
 * - 主牌区（Playfield）的卡牌配置
 * - 备用牌堆（Stack）的卡牌配置
 */
class LevelConfig
{
public:
    /**
     * @brief 默认构造函数
     */
    LevelConfig();
    
    /**
     * @brief 析构函数
     */
    ~LevelConfig();
    
    // ========== Getter方法 ==========
    
    /**
     * @brief 获取关卡ID
     * @return 关卡唯一标识
     */
    int getLevelId() const { return _levelId; }
    
    /**
     * @brief 获取主牌区卡牌配置列表
     * @return 主牌区卡牌配置的只读引用
     */
    const std::vector<CardConfigData>& getPlayfieldCards() const { return _playfieldCards; }
    
    /**
     * @brief 获取备用牌堆卡牌配置列表
     * @return 备用牌堆卡牌配置的只读引用
     */
    const std::vector<CardConfigData>& getStackCards() const { return _stackCards; }
    
    /**
     * @brief 获取主牌区卡牌数量
     * @return 卡牌数量
     */
    size_t getPlayfieldCardCount() const { return _playfieldCards.size(); }
    
    /**
     * @brief 获取备用牌堆卡牌数量
     * @return 卡牌数量
     */
    size_t getStackCardCount() const { return _stackCards.size(); }
    
    // ========== Setter方法 ==========
    
    /**
     * @brief 设置关卡ID
     * @param levelId 关卡唯一标识
     */
    void setLevelId(int levelId) { _levelId = levelId; }
    
    /**
     * @brief 添加主牌区卡牌配置
     * @param cardConfig 卡牌配置数据
     */
    void addPlayfieldCard(const CardConfigData& cardConfig);
    
    /**
     * @brief 添加备用牌堆卡牌配置
     * @param cardConfig 卡牌配置数据
     */
    void addStackCard(const CardConfigData& cardConfig);
    
    /**
     * @brief 清空所有配置
     */
    void clear();
    
    // ========== 验证方法 ==========
    
    /**
     * @brief 验证配置是否有效
     * @return 配置有效返回true
     */
    bool isValid() const;

private:
    int _levelId;                               ///< 关卡ID
    std::vector<CardConfigData> _playfieldCards; ///< 主牌区卡牌配置
    std::vector<CardConfigData> _stackCards;     ///< 备用牌堆卡牌配置
};

#endif // __LEVEL_CONFIG_H__

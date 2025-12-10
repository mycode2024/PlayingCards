/**
 * @file GameModel.h
 * @brief 游戏运行时数据模型
 * 
 * 存储游戏运行时的所有状态数据，包括：
 * - 主牌区（Playfield）的卡牌
 * - 手牌区顶部牌（Stack top card）
 * - 备用牌堆（Reserve cards）
 * 支持序列化以实现存档功能。
 */

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include <vector>
#include <map>
#include "models/CardModel.h"
#include "json/document.h"

/**
 * @brief 游戏数据模型类
 * 
 * 管理游戏运行时的所有卡牌数据和状态。
 * 纯数据类，不包含业务逻辑。
 */
class GameModel
{
public:
    /**
     * @brief 默认构造函数
     */
    GameModel();
    
    /**
     * @brief 析构函数
     */
    ~GameModel();
    
    // ========== 主牌区（Playfield）操作 ==========
    
    /**
     * @brief 添加卡牌到主牌区
     * @param card 卡牌模型
     */
    void addPlayfieldCard(const CardModel& card);
    
    /**
     * @brief 从主牌区移除卡牌
     * @param cardId 卡牌ID
     * @return 移除成功返回true
     */
    bool removePlayfieldCard(int cardId);
    
    /**
     * @brief 获取主牌区所有卡牌
     * @return 卡牌列表的只读引用
     */
    const std::vector<CardModel>& getPlayfieldCards() const { return _playfieldCards; }
    
    /**
     * @brief 获取主牌区所有卡牌（可修改）
     * @return 卡牌列表的引用
     */
    std::vector<CardModel>& getPlayfieldCardsMutable() { return _playfieldCards; }
    
    /**
     * @brief 根据ID获取主牌区卡牌
     * @param cardId 卡牌ID
     * @return 卡牌指针，未找到返回nullptr
     */
    CardModel* getPlayfieldCardById(int cardId);
    
    /**
     * @brief 获取主牌区卡牌数量
     * @return 卡牌数量
     */
    size_t getPlayfieldCardCount() const { return _playfieldCards.size(); }
    
    // ========== 手牌区顶部牌（Stack）操作 ==========
    
    /**
     * @brief 设置手牌区顶部牌
     * @param card 卡牌模型
     */
    void setStackTopCard(const CardModel& card);
    
    /**
     * @brief 获取手牌区顶部牌
     * @return 顶部牌的只读引用
     */
    const CardModel& getStackTopCard() const { return _stackTopCard; }
    
    /**
     * @brief 获取手牌区顶部牌（可修改）
     * @return 顶部牌的引用
     */
    CardModel& getStackTopCardMutable() { return _stackTopCard; }
    
    /**
     * @brief 检查是否有手牌区顶部牌
     * @return 有顶部牌返回true
     */
    bool hasStackTopCard() const { return _stackTopCard.getCardId() >= 0; }
    
    // ========== 备用牌堆（Reserve）操作 ==========
    
    /**
     * @brief 添加卡牌到备用牌堆
     * @param card 卡牌模型
     */
    void addReserveCard(const CardModel& card);
    
    /**
     * @brief 从备用牌堆抽取一张牌
     * @param outCard 输出的卡牌
     * @return 抽取成功返回true
     */
    bool drawReserveCard(CardModel& outCard);
    
    /**
     * @brief 将卡牌放回备用牌堆顶部（用于回退）
     * @param card 卡牌模型
     */
    void pushReserveCard(const CardModel& card);
    
    /**
     * @brief 获取备用牌堆所有卡牌
     * @return 卡牌列表的只读引用
     */
    const std::vector<CardModel>& getReserveCards() const { return _reserveCards; }
    
    /**
     * @brief 获取备用牌堆剩余数量
     * @return 卡牌数量
     */
    size_t getReserveCardCount() const { return _reserveCards.size(); }
    
    /**
     * @brief 检查备用牌堆是否为空
     * @return 为空返回true
     */
    bool isReserveEmpty() const { return _reserveCards.empty(); }
    
    // ========== 通用操作 ==========
    
    /**
     * @brief 根据ID查找卡牌（在所有区域中搜索）
     * @param cardId 卡牌ID
     * @return 卡牌指针，未找到返回nullptr
     */
    CardModel* findCardById(int cardId);
    
    /**
     * @brief 清空所有数据
     */
    void clear();
    
    /**
     * @brief 获取下一个可用的卡牌ID
     * @return 新的卡牌ID
     */
    int getNextCardId();
    
    // ========== 序列化方法 ==========
    
    /**
     * @brief 序列化为JSON对象
     * @param allocator JSON分配器
     * @return JSON值对象
     */
    rapidjson::Value serialize(rapidjson::Document::AllocatorType& allocator) const;
    
    /**
     * @brief 从JSON对象反序列化
     * @param json JSON值对象
     * @return 是否成功
     */
    bool deserialize(const rapidjson::Value& json);

private:
    std::vector<CardModel> _playfieldCards;     ///< 主牌区卡牌
    CardModel _stackTopCard;                     ///< 手牌区顶部牌
    std::vector<CardModel> _reserveCards;        ///< 备用牌堆
    int _nextCardId;                             ///< 下一个可用的卡牌ID
};

#endif // __GAME_MODEL_H__

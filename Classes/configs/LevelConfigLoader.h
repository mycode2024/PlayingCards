/**
 * @file LevelConfigLoader.h
 * @brief 关卡配置加载器
 * 
 * 负责从JSON文件加载关卡配置数据。
 * 提供静态方法，不持有状态。
 */

#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "configs/LevelConfig.h"
#include <string>
#include <memory>

/**
 * @brief 关卡配置加载器类
 * 
 * 提供从JSON文件加载关卡配置的静态方法。
 * 符合services层的设计规范：无状态、可静态调用。
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 从JSON文件加载关卡配置
     * @param filePath JSON配置文件路径
     * @param outConfig 输出的关卡配置对象
     * @return 加载成功返回true
     */
    static bool loadFromFile(const std::string& filePath, LevelConfig& outConfig);
    
    /**
     * @brief 从JSON字符串加载关卡配置
     * @param jsonString JSON字符串内容
     * @param outConfig 输出的关卡配置对象
     * @return 加载成功返回true
     */
    static bool loadFromString(const std::string& jsonString, LevelConfig& outConfig);
    
    /**
     * @brief 获取关卡配置文件的默认路径
     * @param levelId 关卡ID
     * @return 配置文件路径
     */
    static std::string getLevelConfigPath(int levelId);

private:
    /**
     * @brief 私有构造函数，禁止实例化
     */
    LevelConfigLoader() = delete;
};

#endif // __LEVEL_CONFIG_LOADER_H__

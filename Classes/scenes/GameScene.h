/**
 * @file GameScene.h
 * @brief 游戏主场景
 * 
 * 作为游戏的入口场景，负责：
 * - 创建并初始化GameView
 * - 创建并初始化GameController
 * - 启动游戏
 */

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "views/GameView.h"
#include "controllers/GameController.h"
#include <memory>

/**
 * @brief 游戏主场景类
 * 
 * 继承自cocos2d::Scene，作为游戏的主要场景。
 * 负责创建和管理GameView和GameController的生命周期。
 */
class GameScene : public cocos2d::Scene
{
public:
    /**
     * @brief 创建场景
     * @return 场景实例
     */
    static cocos2d::Scene* createScene();
    
    /**
     * @brief 初始化
     * @return 初始化成功返回true
     */
    virtual bool init() override;
    
    // 实现静态create方法
    CREATE_FUNC(GameScene);

private:
    GameView* _gameView;                            // 游戏视图
    std::unique_ptr<GameController> _gameController; // 游戏控制器
};

#endif // __GAME_SCENE_H__

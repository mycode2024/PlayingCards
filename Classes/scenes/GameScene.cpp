/**
 * @file GameScene.cpp
 * @brief 游戏主场景实现
 */

#include "scenes/GameScene.h"
#include "configs/CardTypes.h"
#include "configs/LevelConfigLoader.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    _gameView = nullptr;
    _gameController = nullptr;
    
    // 创建游戏视图
    _gameView = GameView::create();
    if (!_gameView)
    {
        CCLOG("GameScene: Failed to create GameView");
        return false;
    }
    
    // 设置视图位置
    _gameView->setPosition(Vec2::ZERO);
    this->addChild(_gameView, 0);
    
    // 创建游戏控制器
    _gameController = std::make_unique<GameController>();
    if (!_gameController->init(_gameView))
    {
        CCLOG("GameScene: Failed to init GameController");
        return false;
    }
    
    // 加载关卡配置并启动游戏
    LevelConfig levelConfig;
    std::string levelPath = LevelConfigLoader::getLevelConfigPath(1);  // 加载第1关
    
    if (LevelConfigLoader::loadFromFile(levelPath, levelConfig))
    {
        // 使用JSON配置启动游戏
        if (!_gameController->startGame(levelConfig))
        {
            CCLOG("GameScene: Failed to start game with level config");
            return false;
        }
        CCLOG("GameScene: Game started with level config: %s", levelPath.c_str());
    }
    else
    {
        // 加载失败，使用测试数据
        CCLOG("GameScene: Failed to load level config, using test data");
        if (!_gameController->startGame())
        {
            CCLOG("GameScene: Failed to start game");
            return false;
        }
    }
    
    CCLOG("GameScene: Initialized successfully");
    return true;
}

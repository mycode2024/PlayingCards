/**
 * @file GameView.h
 * @brief 游戏主视图类
 * 
 * 负责整个游戏界面的组织，包括：
 * - 组合主牌区视图和手牌区视图
 * - 回退按钮
 * - 游戏状态显示
 */

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"
#include "models/GameModel.h"
#include <functional>

/**
 * @brief 游戏主视图类
 * 
 * 作为游戏界面的根视图，组合管理所有子视图。
 * 视图层只负责显示和接收用户输入，不包含业务逻辑。
 */
class GameView : public cocos2d::Node
{
public:
    /// 回退按钮点击回调类型
    using UndoClickCallback = std::function<void()>;
    
    /**
     * @brief 创建游戏主视图
     * @return 游戏主视图实例
     */
    static GameView* create();
    
    /**
     * @brief 初始化
     * @return 初始化成功返回true
     */
    virtual bool init() override;
    
    // ========== 初始化方法 ==========
    
    /**
     * @brief 初始化游戏界面
     * @param gameModel 游戏数据模型（只读）
     */
    void initGame(const GameModel* gameModel);
    
    // ========== 子视图访问 ==========
    
    /**
     * @brief 获取主牌区视图
     * @return 主牌区视图指针
     */
    PlayFieldView* getPlayFieldView() const { return _playFieldView; }
    
    /**
     * @brief 获取手牌区视图
     * @return 手牌区视图指针
     */
    StackView* getStackView() const { return _stackView; }
    
    // ========== UI更新方法 ==========
    
    /**
     * @brief 更新回退按钮状态
     * @param canUndo 是否可以回退
     */
    void updateUndoButtonState(bool canUndo);
    
    // ========== 回调设置 ==========
    
    /**
     * @brief 设置回退按钮点击回调
     * @param callback 回调函数
     */
    void setUndoClickCallback(const UndoClickCallback& callback);

private:
    /**
     * @brief 创建回退按钮
     */
    void createUndoButton();
    
    /**
     * @brief 创建背景
     */
    void createBackground();
    
    /**
     * @brief 创建关闭按钮
     */
    void createCloseButton();
    
    /**
     * @brief 关闭按钮点击回调
     */
    void onCloseButtonClicked(cocos2d::Ref* sender);

private:
    PlayFieldView* _playFieldView;          // 主牌区视图
    StackView* _stackView;                   // 手牌区视图
    cocos2d::ui::Button* _undoButton;        // 回退按钮（备用）
    cocos2d::Node* _undoButtonNode;          // 回退按钮背景节点
    cocos2d::Label* _undoLabel;              // 回退按钮文字
    bool _undoEnabled;                       // 回退按钮是否启用
    
    UndoClickCallback _undoClickCallback;    // 回退按钮点击回调
    cocos2d::Menu* _closeMenu;                // 关闭按钮菜单
};

#endif // __GAME_VIEW_H__

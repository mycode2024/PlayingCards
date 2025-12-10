/**
 * @file GameView.cpp
 * @brief 游戏主视图类实现
 */

#include "views/GameView.h"
#include "configs/CardTypes.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

GameView* GameView::create()
{
    GameView* view = new (std::nothrow) GameView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    _playFieldView = nullptr;
    _stackView = nullptr;
    _undoButton = nullptr;
    _undoButtonNode = nullptr;
    _undoLabel = nullptr;
    _undoEnabled = false;
    _closeMenu = nullptr;
    
    // 设置整体大小
    this->setContentSize(Size(GameConstants::kDesignWidth, GameConstants::kDesignHeight));
    
    // 创建背景
    createBackground();
    
    // 创建主牌区视图（上方）
    _playFieldView = PlayFieldView::create();
    if (_playFieldView)
    {
        _playFieldView->setPosition(Vec2(0, GameConstants::kStackAreaHeight));
        this->addChild(_playFieldView, 1);
    }
    
    // 创建手牌区视图（下方）
    _stackView = StackView::create();
    if (_stackView)
    {
        _stackView->setPosition(Vec2(0, 0));
        this->addChild(_stackView, 1);
    }
    
    // 创建回退按钮
    createUndoButton();
    
    // 创建关闭按钮
    createCloseButton();
    
    return true;
}

void GameView::createBackground()
{
    // 创建纯色背景
    auto background = LayerColor::create(Color4B(34, 85, 34, 255)); // 深绿色
    background->setContentSize(this->getContentSize());
    this->addChild(background, 0);
    
    // 添加主牌区和手牌区的分隔线
    auto separator = DrawNode::create();
    separator->drawLine(
        Vec2(0, GameConstants::kStackAreaHeight),
        Vec2(GameConstants::kDesignWidth, GameConstants::kStackAreaHeight),
        Color4F(1, 1, 1, 0.3f)
    );
    this->addChild(separator, 0);
}

void GameView::createUndoButton()
{
    // 创建按钮背景
    auto buttonBg = DrawNode::create();
    float btnWidth = 120.0f;
    float btnHeight = 50.0f;
    buttonBg->drawSolidRect(Vec2(0, 0), Vec2(btnWidth, btnHeight), Color4F(0.3f, 0.3f, 0.5f, 0.8f));
    buttonBg->drawRect(Vec2(0, 0), Vec2(btnWidth, btnHeight), Color4F(1, 1, 1, 0.5f));
    
    // 位置：右下角
    buttonBg->setPosition(Vec2(
        GameConstants::kDesignWidth - btnWidth - 20,
        20
    ));
    this->addChild(buttonBg, 2);
    
    // 创建按钮文字
    auto label = Label::createWithSystemFont("Undo", "Arial", 32);
    label->setColor(Color3B::WHITE);
    label->setPosition(Vec2(btnWidth / 2, btnHeight / 2));
    buttonBg->addChild(label);
    
    // 保存引用用于更新状态
    _undoButton = nullptr;  // 不再使用ui::Button
    
    // 创建触摸监听器
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    Rect buttonRect(GameConstants::kDesignWidth - btnWidth - 20, 20, btnWidth, btnHeight);
    
    touchListener->onTouchBegan = [this, buttonRect, buttonBg](Touch* touch, Event* event) {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        if (buttonRect.containsPoint(locationInNode))
        {
            buttonBg->setScale(0.95f);
            return true;
        }
        return false;
    };
    
    touchListener->onTouchEnded = [this, buttonRect, buttonBg](Touch* touch, Event* event) {
        buttonBg->setScale(1.0f);
        if (!_undoEnabled)
        {
            return;
        }
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        if (buttonRect.containsPoint(locationInNode))
        {
            CCLOG("GameView: Undo button clicked");
            if (_undoClickCallback)
            {
                _undoClickCallback();
            }
        }
    };
    
    touchListener->onTouchCancelled = [buttonBg](Touch* touch, Event* event) {
        buttonBg->setScale(1.0f);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    // 保存背景节点用于状态更新
    _undoButtonNode = buttonBg;
    _undoLabel = label;
}

void GameView::initGame(const GameModel* gameModel)
{
    if (!gameModel)
    {
        return;
    }
    
    // 初始化主牌区
    if (_playFieldView)
    {
        _playFieldView->initCards(gameModel);
    }
    
    // 初始化手牌区
    if (_stackView)
    {
        _stackView->initStack(gameModel);
    }
}

void GameView::updateUndoButtonState(bool canUndo)
{
    _undoEnabled = canUndo;
    if (_undoButtonNode)
    {
        _undoButtonNode->setOpacity(canUndo ? 255 : 100);
    }
    if (_undoLabel)
    {
        _undoLabel->setOpacity(canUndo ? 255 : 100);
    }
}

void GameView::setUndoClickCallback(const UndoClickCallback& callback)
{
    _undoClickCallback = callback;
}

void GameView::createCloseButton()
{
    // 创建关闭按钮图片
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(GameView::onCloseButtonClicked, this)
    );
    
    if (closeItem == nullptr || closeItem->getContentSize().width <= 0)
    {
        CCLOG("GameView: Failed to load close button images");
        return;
    }
    
    // 位置：左下角
    float x = 20 + closeItem->getContentSize().width / 2;
    float y = 20 + closeItem->getContentSize().height / 2;
    closeItem->setPosition(Vec2(x, y));
    
    // 创建菜单
    _closeMenu = Menu::create(closeItem, nullptr);
    _closeMenu->setPosition(Vec2::ZERO);
    this->addChild(_closeMenu, 2);
    
    CCLOG("GameView: Close button created");
}

void GameView::onCloseButtonClicked(Ref* sender)
{
    CCLOG("GameView: Close button clicked, exiting application");
    
    // 关闭应用程序
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

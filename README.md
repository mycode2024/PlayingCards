# 纸牌游戏程序设计文档

本文档说明当前程序的架构设计，以及如何在现有代码结构下添加新的卡牌类型。

---

## 1. 程序架构设计

### 1.1 MVC分层架构

```
┌─────────────────────────────────────────────────────────────────┐
│                         Scene (场景层)                           │
│  GameScene - 组装 MVC 组件，管理生命周期                          │
├─────────────────────────────────────────────────────────────────┤
│                         View (视图层)                            │
│  GameView / PlayFieldView / StackView / CardView                │
│  职责：渲染显示、接收用户输入、通过回调向上传递事件                  │
├─────────────────────────────────────────────────────────────────┤
│                       Controller (控制层)                        │
│  GameController / UndoManager                                    │
│  职责：处理业务逻辑、协调 Model 和 View、管理游戏状态               │
├─────────────────────────────────────────────────────────────────┤
│                         Model (数据层)                           │
│  GameModel / CardModel / UndoModel                               │
│  职责：纯数据存储、支持序列化/反序列化、不包含业务逻辑              │
├─────────────────────────────────────────────────────────────────┤
│                        Config (配置层)                           │
│  CardTypes.h / LevelConfig / CardUtils                           │
│  职责：类型定义、常量配置、工具函数                                │
└─────────────────────────────────────────────────────────────────┘
```

### 1.2 卡牌相关核心文件

| 文件 | 职责 |
|------|------|
| `configs/CardTypes.h` | 卡牌花色、点数枚举定义 |
| `models/CardModel.h` | 单张卡牌的数据模型 |
| `utils/CardUtils.h` | 卡牌工具函数（颜色判断、匹配规则等） |
| `views/CardView.h` | 卡牌视图渲染 |

---

## 2. 卡牌系统设计

### 2.1 当前卡牌类型定义

**花色枚举** (`CardTypes.h`)：
```cpp
enum class CardSuitType
{
    NONE = -1,      // 无效
    CLUBS = 0,      // 梅花 (黑色)
    DIAMONDS = 1,   // 方块 (红色)
    HEARTS = 2,     // 红桃 (红色)
    SPADES = 3,     // 黑桃 (黑色)
    COUNT = 4
};
```

**点数枚举** (`CardTypes.h`)：
```cpp
enum class CardFaceType
{
    NONE = -1,
    ACE = 0, TWO = 1, THREE = 2, FOUR = 3, FIVE = 4,
    SIX = 5, SEVEN = 6, EIGHT = 7, NINE = 8, TEN = 9,
    JACK = 10, QUEEN = 11, KING = 12,
    COUNT = 13
};
```

### 2.2 卡牌数据模型

**CardModel 核心属性**：
```cpp
class CardModel
{
private:
    int _cardId;             // 唯一标识
    CardSuitType _suit;      // 花色
    CardFaceType _face;      // 点数
    Vec2 _position;          // 位置
    CardAreaType _area;      // 所在区域
    bool _isFaceUp;          // 是否正面朝上
    bool _isClickable;       // 是否可点击
};
```

### 2.3 卡牌匹配规则

当前规则在 `CardUtils.h` 中定义：
```cpp
// 点数差1可匹配，A和K可循环匹配
inline bool canMatch(CardFaceType face1, CardFaceType face2)
{
    int diff = std::abs(static_cast<int>(face1) - static_cast<int>(face2));
    if (diff == 1) return true;
    if ((face1 == ACE && face2 == KING) || (face1 == KING && face2 == ACE))
        return true;
    return false;
}
```

---

## 3. 添加新卡牌指南

### 3.1 扩展步骤总览

| 步骤 | 文件 | 修改内容 |
|------|------|----------|
| 1 | `configs/CardTypes.h` | 添加新枚举值 |
| 2 | `models/CardModel.h/cpp` | 添加新属性（可选） |
| 3 | `utils/CardUtils.h` | 添加工具函数、匹配规则 |
| 4 | `views/CardView.cpp` | 添加渲染逻辑 |
| 5 | `Resources/` | 添加资源文件 |

### 3.2 步骤详解

#### 步骤1：扩展枚举定义

**文件**：`Classes/configs/CardTypes.h`

```cpp
// 例如：添加 Joker 花色
enum class CardSuitType
{
    // ... 现有花色 ...
    SPADES = 3,
    JOKER_RED = 4,      // 新增
    JOKER_BLACK = 5,    // 新增
    COUNT = 6           // 更新计数
};
```

#### 步骤2：扩展工具函数

**文件**：`Classes/utils/CardUtils.h`

```cpp
namespace CardUtils
{
    // 新增：判断是否为Joker
    inline bool isJokerSuit(CardSuitType suit)
    {
        return suit == CardSuitType::JOKER_RED || 
               suit == CardSuitType::JOKER_BLACK;
    }
    
    // 新增：获取Joker资源文件名
    inline std::string getJokerFileName(CardSuitType suit)
    {
        if (suit == CardSuitType::JOKER_RED) return "joker_red.png";
        if (suit == CardSuitType::JOKER_BLACK) return "joker_black.png";
        return "";
    }
}
```

#### 步骤3：扩展匹配规则

**文件**：`Classes/models/CardModel.cpp`

```cpp
bool CardModel::canMatchWith(const CardModel& other) const
{
    // 新增：Joker可匹配任意牌
    if (CardUtils::isJokerSuit(_suit) || CardUtils::isJokerSuit(other._suit))
    {
        return true;
    }
    
    // 原有匹配规则
    return CardUtils::canMatch(_face, other._face);
}
```

#### 步骤4：扩展视图渲染

**文件**：`Classes/views/CardView.cpp`

```cpp
void CardView::createFrontView()
{
    // 新增：Joker特殊渲染
    if (CardUtils::isJokerSuit(_suit))
    {
        createJokerView();
        return;
    }
    
    // ... 普通牌渲染逻辑 ...
}

void CardView::createJokerView()
{
    _frontNode = Node::create();
    this->addChild(_frontNode, 1);
    
    std::string jokerPath = GameConstants::kResPath + 
                            CardUtils::getJokerFileName(_suit);
    auto jokerSprite = Sprite::create(jokerPath);
    if (jokerSprite)
    {
        jokerSprite->setPosition(Vec2(
            GameConstants::kCardWidth / 2,
            GameConstants::kCardHeight / 2
        ));
        _frontNode->addChild(jokerSprite);
    }
}
```

#### 步骤5：添加资源文件

在 `Resources/res/res/` 目录下添加：
- `joker_red.png`
- `joker_black.png`

---

## 4. 示例：添加Joker牌

### 4.1 完整修改文件清单

```
Classes/
├── configs/
│   └── CardTypes.h          # 添加 JOKER_RED, JOKER_BLACK 枚举
├── models/
│   └── CardModel.cpp        # 修改 canMatchWith() 方法
├── utils/
│   └── CardUtils.h          # 添加 isJokerSuit(), getJokerFileName()
└── views/
    ├── CardView.h           # 添加 createJokerView() 声明
    └── CardView.cpp         # 实现 createJokerView()

Resources/
└── res/res/
    ├── joker_red.png
    └── joker_black.png
```

### 4.2 核心代码修改

**CardTypes.h**：
```cpp
enum class CardSuitType
{
    NONE = -1,
    CLUBS = 0,
    DIAMONDS = 1,
    HEARTS = 2,
    SPADES = 3,
    JOKER_RED = 4,      // 新增
    JOKER_BLACK = 5,    // 新增
    COUNT = 6
};
```

**CardUtils.h**：
```cpp
inline bool isJokerSuit(CardSuitType suit)
{
    return suit == CardSuitType::JOKER_RED || 
           suit == CardSuitType::JOKER_BLACK;
}
```

**CardModel.cpp**：
```cpp
bool CardModel::canMatchWith(const CardModel& other) const
{
    // Joker可匹配任意牌
    if (CardUtils::isJokerSuit(_suit) || CardUtils::isJokerSuit(other._suit))
        return true;
    
    return CardUtils::canMatch(_face, other._face);
}
```

---

# TypeGame

## 项目概述

`TypeGame` 是一个基于 Qt5 的桌面打字教学与小游戏演示程序，包含以下模块：

- **首页导航**：欢迎界面与游戏选择页面
- **拯救苹果**：经典打字输入游戏，玩家通过输入正确字母接住下落苹果
- **太空大战**：字母触发射击敌机的迷你动作游戏，带敌人、子弹、奖励单词、分数与生命值

该项目使用 `CMake` 构建，并依赖 `Qt5` 的 `Core`、`Widgets`、`Gui` 和 `Multimedia` 模块。

## 技术栈

- C++
- Qt5
- CMake
- Qt Widgets UI (`mainwindow.ui`)
- Qt 资源系统 (`image.qrc`)

## 目录结构

- `CMakeLists.txt` - 项目构建配置
- `main.cpp` - 程序入口
- `mainwindow.h` - 主窗口类声明，定义游戏逻辑结构与状态
- `mainwindow.cpp` - 主窗口实现，负责 UI 管理、事件处理和游戏逻辑
- `mainwindow.ui` - Qt Designer 生成的主界面布局
- `image.qrc` - 资源清单，包含图片与音频资源
- `image/` - 测试资源
- `resource/` - 游戏使用的图像和音效资源

## 核心功能

### 1. 主界面导航

- `pageHome`：欢迎页面，显示游戏入口按钮
- `pageGameSelect`：游戏选择页面，提供“拯救苹果”和“太空大战”入口

### 2. 拯救苹果游戏

- `pageSaveApple` 在程序运行时由代码创建
- 通过 `QTimer` 驱动苹果下落与生成逻辑
- 玩家输入字母触发 `handleKeyPressForLetter()`，匹配苹果字母
- 成功接住苹果会统计分数、准确率和等级

### 3. 太空大战游戏

- `pageSpaceBattle` 由代码创建，包含背景、玩家飞船、敌机、子弹和控制面板
- `spawnEnemy()` 生成带字母的敌机
- `handleKeyPressForLetterSpace()` 根据玩家按键发射子弹锁定敌机
- `onSpaceGameTimerTimeout()` 更新敌机位置、子弹运动、碰撞检测和飞船自动摆动
- 提供生命值、分数、时间、难度升级和奖励单词机制

## 主要类与数据结构

### `MainWindow`

主窗口类负责：

- 初始化 UI 与资源
- 管理页面切换
- 响应按键事件
- 启动与暂停游戏逻辑
- 维护游戏状态

### 数据结构

- `AppleItem`：保存苹果标签、字母、速度和活动状态
- `EnemyItem`：保存敌机标签、字母、速度、位置以及唯一 ID
- `BulletItem`：保存子弹标签、当前位置、目标、跟踪敌机 ID
- `RewardWord`：保存奖励单词标签、文本、位置和状态

## 构建说明

### 依赖

- Qt5
- CMake 3.10+
- 支持 MSVC 或其他兼容编译器

### 构建步骤

```bash
cd "C:\Users\18180\Downloads\shixun03\KDevelop-Train\Example\typegame"
cmake -B build
cmake --build build --config Debug
```

如果需要 Release 版本：

```bash
cmake --build build --config Release
```

## 运行方式

构建完成后，可执行生成的可执行文件：

- Windows：`build\typegame.exe`

## 重要实现细节

- `CMakeLists.txt` 自动收集 `*.ui`、`*.qrc` 和源文件，并启用 `AUTOMOC`、`AUTORCC`、`AUTOUIC`
- `mainwindow.h` 包含所有游戏状态、计时器、控件指针及事件处理函数
- `mainwindow.cpp` 将 `saveApple` 与 `spaceBattle` 页面均通过代码动态创建，并与主 `stackedWidget` 进行切换
- `keyPressEvent()` 负责将字母输入分发到当前游戏模式的处理逻辑

## 可扩展点

以下部分适合后续增强：

- `太空大战` 的子弹爆炸动画与敌机爆炸效果
- 敌机行为模式（例如横向移动、波动轨迹、不同类型敌机）
- 音效开关与背景音乐控制面板
- 排行榜与关卡保存
- 本地化支持

## 说明

该项目为教学性质示例，适合用于学习 Qt Widgets 界面开发、信号与槽、计时器驱动游戏循环、资源管理和输入事件处理。
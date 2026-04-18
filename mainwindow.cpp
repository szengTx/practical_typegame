#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QUrl>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>
#include <QEvent>
#include <QRandomGenerator>
#include <QSlider>
#include <QCheckBox>
#include <QLayout>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnNewUser, &QPushButton::clicked, this, &MainWindow::onNewUserClicked);
    connect(ui->btnEnglishTyping, &QPushButton::clicked, this, &MainWindow::onEnglishTypingClicked);
    connect(ui->btnPinyinTyping, &QPushButton::clicked, this, &MainWindow::onPinyinTypingClicked);
    connect(ui->btnWubiTyping, &QPushButton::clicked, this, &MainWindow::onWubiTypingClicked);
    connect(ui->btnTypingGame, &QPushButton::clicked, this, &MainWindow::onTypingGameClicked);
    connect(ui->btnBackHome, &QPushButton::clicked, this, &MainWindow::onBackHomeClicked);
    connect(ui->btnSaveApple, &QPushButton::clicked, this, &MainWindow::onSelectSaveAppleClicked);
    connect(ui->btnSpaceBattle, &QPushButton::clicked, this, &MainWindow::onSelectSpaceBattleClicked);

    ui->stackedWidget->setCurrentIndex(0);

    ui->btnNewUser->setIcon(QIcon(":/resource/Common/Images/PUBLIC_START.png"));
    ui->btnNewUser->setIconSize(QSize(48, 48));
    ui->btnEnglishTyping->setIcon(QIcon(":/resource/Common/Images/PUBLIC_SETUP.png"));
    ui->btnEnglishTyping->setIconSize(QSize(48, 48));
    ui->btnPinyinTyping->setIcon(QIcon(":/resource/Common/Images/MAIN_DLG_REPLAY.png"));
    ui->btnPinyinTyping->setIconSize(QSize(48, 48));
    ui->btnWubiTyping->setIcon(QIcon(":/resource/Common/Images/PUBLIC_PAUSE.png"));
    ui->btnWubiTyping->setIconSize(QSize(48, 48));
    ui->btnTypingGame->setIcon(QIcon(":/resource/Common/Images/MAIN_DLG_REPLAY.png"));
    ui->btnTypingGame->setIconSize(QSize(48, 48));
    ui->btnSaveApple->setIcon(QIcon(":/resource/Common/Images/PUBLIC_START.png"));
    ui->btnSaveApple->setIconSize(QSize(48, 48));
    ui->btnSpaceBattle->setIcon(QIcon(":/resource/Common/Images/PUBLIC_SETUP.png"));
    ui->btnSpaceBattle->setIconSize(QSize(48, 48));
    ui->btnBackHome->setIcon(QIcon(":/resource/Common/Images/PUBLIC_EXIT.png"));
    ui->btnBackHome->setIconSize(QSize(32, 32));

    // 初始化音乐播放器
    appleBgPlayer = new QMediaPlayer(this);
    appleBgPlayer->setMedia(QUrl("qrc:/resource/Apple/Sounds/APPLE_BG.wav"));
    appleBgPlayer->setVolume(60);
    connect(appleBgPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);

    btnClickSound.setSource(QUrl("qrc:/resource/Common/Sounds/BTN_CLICK.wav"));
    btnClickSound.setVolume(0.8f);

    aniBtnEnterSound.setSource(QUrl("qrc:/resource/Common/Sounds/ANIBTN_ENTER.wav"));
    aniBtnEnterSound.setVolume(0.8f);

    actionClickSound.setSource(QUrl("qrc:/resource/Common/Sounds/ANIBTN_CLICK.wav"));
    actionClickSound.setVolume(0.8f);

    glideSound.setSource(QUrl("qrc:/resource/Common/Sounds/GLIDE.wav"));
    glideSound.setVolume(0.8f);

    typeSound.setSource(QUrl("qrc:/resource/Common/Sounds/TYPE.wav"));
    typeSound.setVolume(0.8f);

    appleHitSound.setSource(QUrl("qrc:/resource/Apple/Sounds/APPLE_IN.wav"));
    appleHitSound.setVolume(0.9f);
    appleFailSound.setSource(QUrl("qrc:/resource/Common/Sounds/ANIBTN_CLICK.wav"));
    appleFailSound.setVolume(0.7f);

    // 创建主界面背景图片标签
    homeBackgroundLabel = new QLabel(ui->pageHome);
    QPixmap homePixmap(":/resource/Common/Images/MAIN_DLG_BG.png");
    if (!homePixmap.isNull()) {
        homeBackgroundLabel->setPixmap(homePixmap);
        homeBackgroundLabel->setScaledContents(true);
        homeBackgroundLabel->setGeometry(0, 0, ui->pageHome->width(), ui->pageHome->height());
        homeBackgroundLabel->lower(); // 将背景放在最底层
    }

    // 创建游戏选择页面背景图片标签
    appleBackgroundLabel = new QLabel(ui->pageGameSelect);
    QPixmap applePixmap(":/resource/Apple/Images/APPLE_BACKGROUND.png");
    if (!applePixmap.isNull()) {
        appleBackgroundLabel->setPixmap(applePixmap);
        appleBackgroundLabel->setScaledContents(true);
        appleBackgroundLabel->setGeometry(0, 0, ui->pageGameSelect->width(), ui->pageGameSelect->height());
        appleBackgroundLabel->lower(); // 将背景放在最底层
    }

    createSaveApplePage();

    // 安装事件过滤器到所有按钮，用于悬停声音
    ui->btnNewUser->installEventFilter(this);
    ui->btnEnglishTyping->installEventFilter(this);
    ui->btnPinyinTyping->installEventFilter(this);
    ui->btnWubiTyping->installEventFilter(this);
    ui->btnTypingGame->installEventFilter(this);
    ui->btnBackHome->installEventFilter(this);
    ui->btnSaveApple->installEventFilter(this);
    ui->btnSpaceBattle->installEventFilter(this);
    btnBackToGameSelect->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createSaveApplePage()
{
    pageSaveApple = new QWidget;
    saveAppleBackgroundLabel = new QLabel(pageSaveApple);
    saveAppleBackgroundLabel->setObjectName("saveAppleBackground");
    saveAppleBackgroundLabel->setScaledContents(true);
    QPixmap saveApplePixmap(":/resource/Apple/Images/APPLE_BACKGROUND.png");
    if (!saveApplePixmap.isNull()) {
        saveAppleBackgroundLabel->setPixmap(saveApplePixmap);
        saveAppleBackgroundLabel->setGeometry(0, 0, pageSaveApple->width(), pageSaveApple->height());
        saveAppleBackgroundLabel->lower();
    }

    saveAppleGameArea = new QWidget(pageSaveApple);
    saveAppleGameArea->setAttribute(Qt::WA_TransparentForMouseEvents);
    saveAppleGameArea->setStyleSheet("background: transparent;");

    QVBoxLayout *saveAppleLayout = new QVBoxLayout(pageSaveApple);
    saveAppleLayout->setContentsMargins(0, 0, 0, 0);
    saveAppleLayout->setSpacing(0);

    QWidget *topPanel = new QWidget(pageSaveApple);
    topPanel->setFixedHeight(140);
    topPanel->setStyleSheet("background: rgba(0, 0, 0, 0.5);");
    QHBoxLayout *topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(16, 12, 16, 12);
    topLayout->setSpacing(12);

    statusLabel = new QLabel(tr("状态: 未开始"), topPanel);
    statusLabel->setStyleSheet("color: white; font-size: 18px;");
    scoreLabel = new QLabel(tr("成功: 0"), topPanel);
    scoreLabel->setStyleSheet("color: white; font-size: 16px;");
    failLabel = new QLabel(tr("失败: 0"), topPanel);
    failLabel->setStyleSheet("color: white; font-size: 16px;");
    accuracyLabel = new QLabel(tr("准确率: 0%"), topPanel);
    accuracyLabel->setStyleSheet("color: white; font-size: 16px;");

    topLayout->addWidget(statusLabel);
    topLayout->addStretch();
    topLayout->addWidget(scoreLabel);
    topLayout->addWidget(failLabel);
    topLayout->addWidget(accuracyLabel);

    QWidget *controlPanel = new QWidget(pageSaveApple);
    controlPanel->setFixedHeight(120);
    controlPanel->setStyleSheet("background: rgba(0, 0, 0, 0.35);");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(16, 8, 16, 8);
    controlLayout->setSpacing(16);

    QWidget *sliderPanel = new QWidget(controlPanel);
    QVBoxLayout *sliderLayout = new QVBoxLayout(sliderPanel);
    sliderLayout->setContentsMargins(0, 0, 0, 0);
    sliderLayout->setSpacing(8);
    QLabel *levelLabel = new QLabel(tr("等级"), sliderPanel);
    levelLabel->setStyleSheet("color: white; font-size: 14px;");
    levelSlider = new QSlider(Qt::Horizontal, sliderPanel);
    levelSlider->setRange(1, 10);
    levelSlider->setValue(1);
    levelSlider->setStyleSheet("QSlider::groove:horizontal { height: 12px; background: transparent; } QSlider::handle:horizontal { width: 20px; background: white; border-radius: 10px; } ");
    levelValueLabel = new QLabel(tr("1"), sliderPanel);
    levelValueLabel->setStyleSheet("color: white; font-size: 14px;");
    sliderLayout->addWidget(levelLabel);
    sliderLayout->addWidget(levelSlider);
    sliderLayout->addWidget(levelValueLabel, 0, Qt::AlignRight);

    QWidget *countPanel = new QWidget(controlPanel);
    QVBoxLayout *countLayout = new QVBoxLayout(countPanel);
    countLayout->setContentsMargins(0, 0, 0, 0);
    countLayout->setSpacing(8);
    QLabel *maxLabel = new QLabel(tr("同屏苹果数"), countPanel);
    maxLabel->setStyleSheet("color: white; font-size: 14px;");
    maxAppleSlider = new QSlider(Qt::Horizontal, countPanel);
    maxAppleSlider->setRange(1, 5);
    maxAppleSlider->setValue(3);
    maxAppleSlider->setStyleSheet("QSlider::groove:horizontal { height: 12px; background: transparent; } QSlider::handle:horizontal { width: 20px; background: white; border-radius: 10px; } ");
    currentCountLabel = new QLabel(tr("3"), countPanel);
    currentCountLabel->setStyleSheet("color: white; font-size: 14px;");
    countLayout->addWidget(maxLabel);
    countLayout->addWidget(maxAppleSlider);
    countLayout->addWidget(currentCountLabel, 0, Qt::AlignRight);

    QWidget *targetPanel = new QWidget(controlPanel);
    QVBoxLayout *targetLayout = new QVBoxLayout(targetPanel);
    targetLayout->setContentsMargins(0, 0, 0, 0);
    targetLayout->setSpacing(8);
    QLabel *targetLabelText = new QLabel(tr("过关目标"), targetPanel);
    targetLabelText->setStyleSheet("color: white; font-size: 14px;");
    targetSlider = new QSlider(Qt::Horizontal, targetPanel);
    targetSlider->setRange(1, 20);
    targetSlider->setValue(8);
    targetSlider->setStyleSheet("QSlider::groove:horizontal { height: 12px; background: transparent; } QSlider::handle:horizontal { width: 20px; background: white; border-radius: 10px; } ");
    targetValueLabel = new QLabel(tr("8"), targetPanel);
    targetValueLabel->setStyleSheet("color: white; font-size: 14px;");
    targetLayout->addWidget(targetLabelText);
    targetLayout->addWidget(targetSlider);
    targetLayout->addWidget(targetValueLabel, 0, Qt::AlignRight);

    soundToggle = new QCheckBox(tr("音效开启"), controlPanel);
    soundToggle->setChecked(true);
    soundToggle->setStyleSheet("color: white; font-size: 14px;");

    controlLayout->addWidget(sliderPanel, 2);
    controlLayout->addWidget(countPanel, 2);
    controlLayout->addWidget(targetPanel, 2);
    controlLayout->addWidget(soundToggle, 1, Qt::AlignVCenter);

    QWidget *buttonPanel = new QWidget(pageSaveApple);
    buttonPanel->setFixedHeight(70);
    buttonPanel->setStyleSheet("background: rgba(0, 0, 0, 0.45);");
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonPanel);
    buttonLayout->setContentsMargins(16, 8, 16, 8);
    buttonLayout->setSpacing(12);
    btnStartGame = new QPushButton(tr("开始游戏"), buttonPanel);
    btnStartGame->setIcon(QIcon(":/resource/Common/Images/PUBLIC_START.png"));
    btnStartGame->setIconSize(QSize(24, 24));
    btnPauseGame = new QPushButton(tr("暂停"), buttonPanel);
    btnPauseGame->setIcon(QIcon(":/resource/Common/Images/PUBLIC_PAUSE.png"));
    btnPauseGame->setIconSize(QSize(24, 24));
    btnResetGame = new QPushButton(tr("重置"), buttonPanel);
    btnResetGame->setIcon(QIcon(":/resource/Common/Images/PUBLIC_EXIT.png"));
    btnResetGame->setIconSize(QSize(24, 24));

    buttonLayout->addStretch();
    buttonLayout->addWidget(btnStartGame);
    buttonLayout->addWidget(btnPauseGame);
    buttonLayout->addWidget(btnResetGame);
    buttonLayout->addStretch();

    saveAppleLayout->addWidget(topPanel);
    saveAppleLayout->addWidget(controlPanel);
    saveAppleLayout->addWidget(buttonPanel);
    saveAppleLayout->addWidget(saveAppleGameArea);

    basketLabel = new QLabel(saveAppleGameArea);
    QPixmap basketPixmap(":/resource/Apple/Images/APPLE_BASKET.png");
    if (!basketPixmap.isNull()) {
        basketLabel->setPixmap(basketPixmap.scaled(180, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    basketLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    basketLabel->show();

    btnBackToGameSelect = new QPushButton(tr("返回游戏选择"), pageSaveApple);
    btnBackToGameSelect->setIcon(QIcon(":/resource/Common/Images/PUBLIC_EXIT.png"));
    btnBackToGameSelect->setIconSize(QSize(32, 32));
    btnBackToGameSelect->setMinimumSize(180, 50);
    saveAppleLayout->addWidget(btnBackToGameSelect, 0, Qt::AlignCenter);

    ui->stackedWidget->addWidget(pageSaveApple);

    connect(levelSlider, &QSlider::valueChanged, this, [this](int value){
        currentLevel = value;
        levelValueLabel->setText(QString::number(value));
        updateGameStateLabels();
    });
    connect(maxAppleSlider, &QSlider::valueChanged, this, [this](int value){
        maxAppleCount = value;
        currentCountLabel->setText(QString::number(value));
    });
    connect(targetSlider, &QSlider::valueChanged, this, [this](int value){
        targetSuccessCount = value;
        targetValueLabel->setText(QString::number(value));
        updateGameStateLabels();
    });
    connect(soundToggle, &QCheckBox::toggled, this, [this](bool checked){
        bool enabled = checked;
        btnClickSound.setMuted(!enabled);
        aniBtnEnterSound.setMuted(!enabled);
        actionClickSound.setMuted(!enabled);
        glideSound.setMuted(!enabled);
        typeSound.setMuted(!enabled);
        appleHitSound.setMuted(!enabled);
        appleFailSound.setMuted(!enabled);
        appleBgPlayer->setMuted(!enabled);
    });
    connect(btnStartGame, &QPushButton::clicked, this, &MainWindow::startAppleGame);
    connect(btnPauseGame, &QPushButton::clicked, this, &MainWindow::pauseAppleGame);
    connect(btnResetGame, &QPushButton::clicked, this, &MainWindow::resetAppleGame);
    connect(btnBackToGameSelect, &QPushButton::clicked, this, &MainWindow::onBackToGameSelectClicked);

    gameTimer = new QTimer(this);
    spawnTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::onGameTimerTimeout);
    connect(spawnTimer, &QTimer::timeout, this, &MainWindow::onSpawnTimerTimeout);

    resetAppleGame();
}

void MainWindow::startAppleGame()
{
    if (gameState == GameState::End) {
        resetAppleGame();
    }
    gameState = GameState::Playing;
    statusLabel->setText(tr("状态: 游戏中"));
    gameTimer->start(30);
    spawnTimer->start(800);
    appleBgPlayer->play();
}

void MainWindow::pauseAppleGame()
{
    if (gameState == GameState::Playing) {
        gameState = GameState::Paused;
        statusLabel->setText(tr("状态: 暂停"));
        gameTimer->stop();
        spawnTimer->stop();
        appleBgPlayer->pause();
    } else if (gameState == GameState::Paused) {
        gameState = GameState::Playing;
        statusLabel->setText(tr("状态: 游戏中"));
        gameTimer->start(30);
        spawnTimer->start(800);
        appleBgPlayer->play();
    }
}

void MainWindow::resetAppleGame()
{
    gameState = GameState::Idle;
    successCount = 0;
    failCount = 0;
    totalInputCount = 0;
    currentAppleCount = 0;
    currentLevel = levelSlider ? levelSlider->value() : 1;
    maxAppleCount = maxAppleSlider ? maxAppleSlider->value() : 3;
    targetSuccessCount = targetSlider ? targetSlider->value() : 8;
    for (auto &apple : apples) {
        if (apple.appleLabel) {
            apple.appleLabel->deleteLater();
        }
        if (apple.letterLabel) {
            apple.letterLabel->deleteLater();
        }
    }
    apples.clear();
    gameTimer->stop();
    spawnTimer->stop();
    statusLabel->setText(tr("状态: 未开始"));
    updateGameStateLabels();
}

void MainWindow::spawnApple()
{
    if (gameState != GameState::Playing) {
        return;
    }
    int activeCount = 0;
    for (const auto &apple : apples) {
        if (apple.active) activeCount++;
    }
    if (activeCount >= maxAppleCount) {
        return;
    }

    QList<QChar> letters;
    for (ushort code = 'A'; code <= 'Z'; ++code) {
        QChar c(code);
        bool used = false;
        for (const auto &apple : apples) {
            if (apple.active && apple.letter == c) {
                used = true;
                break;
            }
        }
        if (!used) letters.append(c);
    }
    if (letters.isEmpty()) {
        return;
    }

    int idx = QRandomGenerator::global()->bounded(letters.size());
    QChar letter = letters.at(idx);

    QLabel *appleLabel = new QLabel(saveAppleGameArea);
    QPixmap applePixmap(":/resource/Apple/Images/APPLE_NORMAL.png");
    if (!applePixmap.isNull()) {
        appleLabel->setPixmap(applePixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    appleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    appleLabel->setFixedSize(80, 80);
    appleLabel->show();

    QLabel *letterLabel = new QLabel(QString(letter), saveAppleGameArea);
    letterLabel->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
    letterLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    letterLabel->adjustSize();
    letterLabel->show();

    int x = QRandomGenerator::global()->bounded(20, qMax(21, saveAppleGameArea->width() - 100));
    appleLabel->move(x, -100);
    letterLabel->move(x + appleLabel->width() / 2 - letterLabel->width() / 2, -80);

    AppleItem item;
    item.appleLabel = appleLabel;
    item.letterLabel = letterLabel;
    item.letter = letter;
    item.speed = 3 + currentLevel;
    item.active = true;
    apples.append(item);
}

void MainWindow::updateGameStateLabels()
{
    scoreLabel->setText(tr("成功: %1").arg(successCount));
    failLabel->setText(tr("失败: %1").arg(failCount));
    int accuracy = totalInputCount > 0 ? qRound(successCount * 100.0 / totalInputCount) : 0;
    accuracyLabel->setText(tr("准确率: %1%" ).arg(accuracy));
    if (levelValueLabel) {
        levelValueLabel->setText(QString::number(currentLevel));
    }
    if (targetValueLabel) {
        targetValueLabel->setText(QString::number(targetSuccessCount));
    }
}

void MainWindow::removeApple(int index)
{
    if (index < 0 || index >= apples.size()) return;
    AppleItem apple = apples.at(index);
    if (apple.appleLabel) apple.appleLabel->deleteLater();
    if (apple.letterLabel) apple.letterLabel->deleteLater();
    apples.removeAt(index);
}

void MainWindow::handleKeyPressForLetter(const QChar &letter)
{
    if (gameState != GameState::Playing) return;
    totalInputCount++;
    bool hit = false;
    for (int i = 0; i < apples.size(); ++i) {
        if (apples[i].active && apples[i].letter == letter) {
            apples[i].active = false;
            if (apples[i].appleLabel) apples[i].appleLabel->hide();
            if (apples[i].letterLabel) apples[i].letterLabel->hide();
            successCount++;
            hit = true;
            if (!appleHitSound.isMuted()) appleHitSound.play();
            removeApple(i);
            break;
        }
    }
    if (!hit) {
        if (!appleFailSound.isMuted()) appleFailSound.play();
    }
    updateGameStateLabels();
    if (successCount >= targetSuccessCount) {
        gameState = GameState::End;
        gameTimer->stop();
        spawnTimer->stop();
        statusLabel->setText(tr("状态: 通关!"));
        QMessageBox::information(this, tr("恭喜"), tr("已达到目标，进入下一关！"));
    }
}

void MainWindow::onSpawnTimerTimeout()
{
    spawnApple();
}

void MainWindow::onGameTimerTimeout()
{
    if (gameState != GameState::Playing) return;
    int floorY = saveAppleGameArea->height() * 0.7;
    for (int i = apples.size() - 1; i >= 0; --i) {
        if (!apples[i].active) continue;
        QRect appleRect = apples[i].appleLabel->geometry();
        int newY = appleRect.y() + apples[i].speed;
        apples[i].appleLabel->move(appleRect.x(), newY);
        apples[i].letterLabel->move(appleRect.x() + appleRect.width() / 2 - apples[i].letterLabel->width() / 2,
                                   newY + appleRect.height() / 2 - apples[i].letterLabel->height() / 2);
        if (newY + appleRect.height() >= floorY) {
            failCount++;
            totalInputCount++;
            if (!appleFailSound.isMuted()) appleFailSound.play();
            removeApple(i);
            updateGameStateLabels();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (ui->stackedWidget->currentWidget() == pageSaveApple && !event->text().isEmpty()) {
        QChar letter = event->text().toUpper().at(0);
        if (letter.isLetter()) {
            if (!soundToggle->isChecked()) {
                typeSound.setMuted(true);
            }
            typeSound.play();
            handleKeyPressForLetter(letter);
        }
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::onNewUserClicked()
{
    QMessageBox::information(this, tr("新手入门"), tr("进入新手入门练习页面。"));
}

void MainWindow::onEnglishTypingClicked()
{
    QMessageBox::information(this, tr("英文打字"), tr("进入英文打字练习页面。"));
}

void MainWindow::onPinyinTypingClicked()
{
    QMessageBox::information(this, tr("拼音打字"), tr("进入拼音打字练习页面。"));
}

void MainWindow::onWubiTypingClicked()
{
    QMessageBox::information(this, tr("五笔打字"), tr("进入五笔打字练习页面。"));
}

void MainWindow::onTypingGameClicked()
{
    glideSound.play();
    ui->stackedWidget->setCurrentWidget(ui->pageGameSelect);
}

void MainWindow::onBackHomeClicked()
{
    actionClickSound.play();
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
    appleBgPlayer->stop();
}

void MainWindow::onSelectSaveAppleClicked()
{
    glideSound.play();
    ui->stackedWidget->setCurrentWidget(pageSaveApple);
    appleBgPlayer->play();
}

void MainWindow::onBackToGameSelectClicked()
{
    actionClickSound.play();
    ui->stackedWidget->setCurrentWidget(ui->pageGameSelect);
}

void MainWindow::onSelectSpaceBattleClicked()
{
    actionClickSound.play();
    QMessageBox::information(this, tr("太空大战"), tr("正在进入太空大战游戏页面。"));
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        appleBgPlayer->play(); // 循环播放
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (homeBackgroundLabel) {
        homeBackgroundLabel->setGeometry(0, 0, ui->pageHome->width(), ui->pageHome->height());
        QPixmap scaled = homeBackgroundLabel->pixmap(Qt::ReturnByValue).scaled(ui->pageHome->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        homeBackgroundLabel->setPixmap(scaled);
    }
    if (appleBackgroundLabel) {
        appleBackgroundLabel->setGeometry(0, 0, ui->pageGameSelect->width(), ui->pageGameSelect->height());
        QPixmap scaled = appleBackgroundLabel->pixmap(Qt::ReturnByValue).scaled(ui->pageGameSelect->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        appleBackgroundLabel->setPixmap(scaled);
    }
    if (saveAppleBackgroundLabel && pageSaveApple) {
        saveAppleBackgroundLabel->setGeometry(0, 0, pageSaveApple->width(), pageSaveApple->height());
        QPixmap scaled = saveAppleBackgroundLabel->pixmap(Qt::ReturnByValue).scaled(pageSaveApple->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        saveAppleBackgroundLabel->setPixmap(scaled);
    }
    if (basketLabel && saveAppleGameArea) {
        int x = saveAppleGameArea->width() - basketLabel->width() - 20;
        int y = saveAppleGameArea->height() - basketLabel->height() - 10;
        basketLabel->move(qMax(0, x), qMax(0, y));
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Enter) {
        // 鼠标悬停在按钮上时播放声音
        aniBtnEnterSound.play();
    } else if (event->type() == QEvent::MouseButtonPress) {
        // 鼠标点击按钮时播放声音
        btnClickSound.play();
    }
    return QMainWindow::eventFilter(obj, event);
}

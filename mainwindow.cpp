#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QDialog>
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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

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

    // 初始化太空大战音乐播放器
    spaceBgPlayer = new QMediaPlayer(this);
    spaceBgPlayer->setMedia(QUrl("qrc:/resource/Space/Sounds/SPACE_BG.wav"));
    spaceBgPlayer->setVolume(60);
    connect(spaceBgPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);

    spaceBlastSound.setSource(QUrl("qrc:/resource/Space/Sounds/SPACE_BLAST.wav"));
    spaceBlastSound.setVolume(0.9f);
    spacePlaneOutSound.setSource(QUrl("qrc:/resource/Space/Sounds/SPACE_PLANEOUT.wav"));
    spacePlaneOutSound.setVolume(0.8f);
    spaceShootSound.setSource(QUrl("qrc:/resource/Space/Sounds/SPACE_SHOOT.wav"));
    spaceShootSound.setVolume(0.8f);
    spaceWordOutSound.setSource(QUrl("qrc:/resource/Space/Sounds/SPACE_WORDOUT.wav"));
    spaceWordOutSound.setVolume(0.8f);
    spaceUpgradeSound.setSource(QUrl("qrc:/resource/Space/Sounds/UPGRADE.wav"));
    spaceUpgradeSound.setVolume(0.8f);

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

    createSpaceBattlePage();

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::handleRewardWordsReply);
    llmApiUrl = QString::fromUtf8(qgetenv("LLM_API_URL"));
    llmApiKey = QString::fromUtf8(qgetenv("LLM_API_KEY"));// 可以通过环境变量设置LLM API的URL和Key，方便测试和部署
    if (llmApiUrl.isEmpty()) {
        llmApiUrl = QStringLiteral("https://api.openai.com/v1/chat/completions");// 默认使用OpenAI的ChatGPT接口
    }

    // 初始化本地词库 for reward mode
    localWords << "apple" << "space" << "battle" << "typing" << "game" << "enemy" << "ship" << "bomb" << "score" << "life";

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
    btnBackToGameSelectSpace->installEventFilter(this);
    btnStartSpaceGame->installEventFilter(this);
    btnPauseSpaceGame->installEventFilter(this);
    btnResetSpaceGame->installEventFilter(this);
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
    topPanel->setFixedHeight(100);
    topPanel->setStyleSheet("background: rgba(0, 0, 0, 0.45);");
    QHBoxLayout *topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(14, 10, 14, 10);
    topLayout->setSpacing(10);

    statusLabel = new QLabel(QString::fromUtf16(u"\u72B6\u6001: \u672A\u5F00\u59CB"), topPanel);
    statusLabel->setStyleSheet("color: white; font-size: 16px;");
    scoreLabel = new QLabel(QString::fromUtf16(u"\u6210\u529F: 0"), topPanel);
    scoreLabel->setStyleSheet("color: white; font-size: 14px;");
    failLabel = new QLabel(QString::fromUtf16(u"\u5931\u8D25: 0"), topPanel);
    failLabel->setStyleSheet("color: white; font-size: 14px;");
    accuracyLabel = new QLabel(QString::fromUtf16(u"\u51C6\u786E\u7387: 0%"), topPanel);
    accuracyLabel->setStyleSheet("color: white; font-size: 14px;");

    topLayout->addWidget(statusLabel);
    topLayout->addStretch();
    topLayout->addWidget(scoreLabel);
    topLayout->addWidget(failLabel);
    topLayout->addWidget(accuracyLabel);

    QWidget *controlPanel = new QWidget(pageSaveApple);
    controlPanel->setFixedHeight(90);
    controlPanel->setStyleSheet("background: rgba(0, 0, 0, 0.45);");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(12, 8, 12, 8);
    controlLayout->setSpacing(12);

    QWidget *sliderPanel = new QWidget(controlPanel);
    QVBoxLayout *sliderLayout = new QVBoxLayout(sliderPanel);
    sliderLayout->setContentsMargins(0, 0, 0, 0);
    sliderLayout->setSpacing(8);
    QLabel *levelLabel = new QLabel(QString::fromUtf16(u"\u7B49\u7EA7"), sliderPanel);
    levelLabel->setStyleSheet("color: white; font-size: 14px;");
    levelSlider = new QSlider(Qt::Horizontal, sliderPanel);
    levelSlider->setRange(1, 10);
    levelSlider->setValue(1);
    levelSlider->setStyleSheet("QSlider::groove:horizontal { height: 12px; background: transparent; } QSlider::handle:horizontal { width: 20px; background: white; border-radius: 10px; } ");
    levelValueLabel = new QLabel(QString::number(1), sliderPanel);
    levelValueLabel->setStyleSheet("color: white; font-size: 14px;");
    sliderLayout->addWidget(levelLabel);
    sliderLayout->addWidget(levelSlider);
    sliderLayout->addWidget(levelValueLabel, 0, Qt::AlignRight);

    QWidget *countPanel = new QWidget(controlPanel);
    QVBoxLayout *countLayout = new QVBoxLayout(countPanel);
    countLayout->setContentsMargins(0, 0, 0, 0);
    countLayout->setSpacing(8);
    QLabel *maxLabel = new QLabel(QString::fromUtf16(u"\u540C\u5C4F\u82F9\u679C\u6570"), countPanel);
    maxLabel->setStyleSheet("color: white; font-size: 14px;");
    maxAppleSlider = new QSlider(Qt::Horizontal, countPanel);
    maxAppleSlider->setRange(1, 5);
    maxAppleSlider->setValue(3);
    maxAppleSlider->setStyleSheet("QSlider::groove:horizontal { height: 12px; background: transparent; } QSlider::handle:horizontal { width: 20px; background: white; border-radius: 10px; } ");
    currentCountLabel = new QLabel(QString::number(3), countPanel);
    currentCountLabel->setStyleSheet("color: white; font-size: 14px;");
    countLayout->addWidget(maxLabel);
    countLayout->addWidget(maxAppleSlider);
    countLayout->addWidget(currentCountLabel, 0, Qt::AlignRight);

    QWidget *targetPanel = new QWidget(controlPanel);
    QVBoxLayout *targetLayout = new QVBoxLayout(targetPanel);
    targetLayout->setContentsMargins(0, 0, 0, 0);
    targetLayout->setSpacing(8);
    QLabel *targetLabelText = new QLabel(QString::fromUtf16(u"\u8FC7\u5173\u76EE\u6807"), targetPanel);
    targetLabelText->setStyleSheet("color: white; font-size: 14px;");
    targetSlider = new QSlider(Qt::Horizontal, targetPanel);
    targetSlider->setRange(1, 20);
    targetSlider->setValue(8);
    targetSlider->setStyleSheet("QSlider::groove:horizontal { height: 12px; background: transparent; } QSlider::handle:horizontal { width: 20px; background: white; border-radius: 10px; } ");
    targetValueLabel = new QLabel(QString::number(8), targetPanel);
    targetValueLabel->setStyleSheet("color: white; font-size: 14px;");
    targetLayout->addWidget(targetLabelText);
    targetLayout->addWidget(targetSlider);
    targetLayout->addWidget(targetValueLabel, 0, Qt::AlignRight);

    soundToggle = new QCheckBox(QString::fromUtf16(u"\u97F3\u6548\u5F00\u542F"), controlPanel);
    soundToggle->setChecked(true);
    soundToggle->setStyleSheet("color: white; font-size: 14px;");

    controlLayout->addWidget(sliderPanel, 2);
    controlLayout->addWidget(countPanel, 2);
    controlLayout->addWidget(targetPanel, 2);
    controlLayout->addWidget(soundToggle, 1, Qt::AlignVCenter);

    QWidget *buttonPanel = new QWidget(pageSaveApple);
    buttonPanel->setFixedHeight(60);
    buttonPanel->setStyleSheet("background: rgba(0, 0, 0, 0.35);");
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonPanel);
    buttonLayout->setContentsMargins(12, 6, 12, 6);
    buttonLayout->setSpacing(10);
    btnStartGame = new QPushButton(QString::fromUtf16(u"\u5F00\u59CB\u6E38\u620F"), buttonPanel);
    btnStartGame->setIcon(QIcon(":/resource/Apple/Images/APPLE_DLG_REPLAY.png"));
    btnStartGame->setIconSize(QSize(20, 20));
    btnStartGame->setStyleSheet("font-size: 12px; padding: 8px;");
    btnPauseGame = new QPushButton(QString::fromUtf16(u"\u6682\u505C"), buttonPanel);
    btnPauseGame->setIcon(QIcon(":/resource/Apple/Images/APPLE_DLG_END.png"));
    btnPauseGame->setIconSize(QSize(20, 20));
    btnPauseGame->setStyleSheet("font-size: 12px; padding: 8px;");
    btnResetGame = new QPushButton(QString::fromUtf16(u"\u91CD\u7F6E"), buttonPanel);
    btnResetGame->setIcon(QIcon(":/resource/Apple/Images/APPLE_DLG_NEXT.png"));
    btnResetGame->setIconSize(QSize(20, 20));
    btnResetGame->setStyleSheet("font-size: 12px; padding: 8px;");

    buttonLayout->addStretch();
    buttonLayout->addWidget(btnStartGame);
    buttonLayout->addWidget(btnPauseGame);
    buttonLayout->addWidget(btnResetGame);
    buttonLayout->addStretch();

    saveAppleLayout->addWidget(topPanel);
    saveAppleLayout->addWidget(controlPanel);
    saveAppleLayout->addWidget(buttonPanel);
    saveAppleLayout->addWidget(saveAppleGameArea, 1);

    basketLabel = new QLabel(saveAppleGameArea);
    QPixmap basketPixmap(":/resource/Apple/Images/APPLE_BASKET.png");
    if (!basketPixmap.isNull()) {
        basketLabel->setPixmap(basketPixmap.scaled(220, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    basketLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    basketLabel->show();

    QLabel *smallAppleLabel = new QLabel(saveAppleGameArea);
    QPixmap smallApplePixmap(":/resource/Apple/Images/APPLE_SMALL.png");
    if (!smallApplePixmap.isNull()) {
        smallAppleLabel->setPixmap(smallApplePixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    smallAppleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    smallAppleLabel->move(20, 20);
    smallAppleLabel->show();

    btnBackToGameSelect = new QPushButton(QString::fromUtf16(u"\u8FD4\u56DE\u6E38\u620F\u9009\u62E9"), pageSaveApple);
    btnBackToGameSelect->setIcon(QIcon(":/resource/Apple/Images/APPLE_DLG_END.png"));
    btnBackToGameSelect->setIconSize(QSize(26, 26));
    btnBackToGameSelect->setMinimumSize(150, 42);
    btnBackToGameSelect->setStyleSheet("font-size: 12px; padding: 6px;");
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
    statusLabel->setText(QString::fromUtf16(u"\u72B6\u6001: \u6E38\u620F\u4E2D"));
    gameTimer->start(30);
    spawnTimer->start(600);
    spawnApple();
    appleBgPlayer->play();
}

void MainWindow::pauseAppleGame()
{
    if (gameState == GameState::Playing) {
        gameState = GameState::Paused;
        statusLabel->setText(QString::fromUtf16(u"\u72B6\u6001: \u6682\u505C"));
        gameTimer->stop();
        spawnTimer->stop();
        appleBgPlayer->pause();
    } else if (gameState == GameState::Paused) {
        gameState = GameState::Playing;
        statusLabel->setText(QString::fromUtf16(u"\u72B6\u6001: \u6E38\u620F\u4E2D"));
        gameTimer->start(30);
        spawnTimer->start(600);
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
    statusLabel->setText(QString::fromUtf16(u"\u72B6\u6001: \u672A\u5F00\u59CB"));
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
    scoreLabel->setText(QString::fromUtf16(u"\u6210\u529F: %1").arg(successCount));
    failLabel->setText(QString::fromUtf16(u"\u5931\u8D25: %1").arg(failCount));
    int accuracy = totalInputCount > 0 ? qRound(successCount * 100.0 / totalInputCount) : 0;
    accuracyLabel->setText(QString::fromUtf16(u"\u51C6\u786E\u7387: %1%" ).arg(accuracy));
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
        statusLabel->setText(QString::fromUtf16(u"\u72B6\u6001: \u901A\u5173!"));
        showGameEndDialog(true);
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
            if (apples[i].appleLabel) {
                apples[i].appleLabel->setPixmap(QPixmap(":/resource/Apple/Images/APPLE_BAD.png").scaled(apples[i].appleLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            removeApple(i);
            updateGameStateLabels();
        }
    }
}

void MainWindow::showGameEndDialog(bool success)
{
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setModal(true);
    dialog.resize(420, 280);

    QLabel *background = new QLabel(&dialog);
    background->setPixmap(QPixmap(":/resource/Apple/Images/APPLE_DLG_BG.png").scaled(dialog.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    background->setGeometry(dialog.rect());
    background->lower();

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    QLabel *title = new QLabel(&dialog);
    title->setText(success ? QString::fromUtf16(u"\u6210\u529F\u901A\u5173\uFF01") : QString::fromUtf16(u"\u5F97\u5230\u5931\u8D25\uFF01"));
    title->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QLabel *message = new QLabel(&dialog);
    message->setText(success ? QString::fromUtf16(u"\u5DF2\u62A5\u9500\u6240\u6709\u82F9\u679C\uFF0C\u53EF\u4EE5\u8FDB\u5165\u4E0B\u4E00\u5173\uFF01") : QString::fromUtf16(u"\u82F9\u679C\u843D\u5230\u5730\u9762\uFF0C\u5339\u914D\u6CA1\u6709\u6253\u6B63\u786E\u5B57\u6BCD\uFF01"));
    message->setStyleSheet("color: white; font-size: 16px;");
    message->setWordWrap(true);
    message->setAlignment(Qt::AlignCenter);
    layout->addWidget(message);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    QPushButton *replayButton = new QPushButton(&dialog);
    replayButton->setIcon(QIcon(":/resource/Apple/Images/APPLE_DLG_REPLAY.png"));
    replayButton->setIconSize(QSize(40, 40));
    replayButton->setToolTip(QString::fromUtf16(u"\u518D\u6765\u4E00\u6B21"));
    replayButton->setFixedSize(140, 48);
    buttonLayout->addWidget(replayButton);

    QPushButton *nextButton = new QPushButton(&dialog);
    nextButton->setIcon(QIcon(":/resource/Apple/Images/APPLE_DLG_NEXT.png"));
    nextButton->setIconSize(QSize(40, 40));
    nextButton->setToolTip(QString::fromUtf16(u"\u4E0B\u4E00\u5173"));
    nextButton->setFixedSize(140, 48);
    buttonLayout->addWidget(nextButton);

    QPushButton *endButton = new QPushButton(&dialog);
    endButton->setIcon(QIcon(":/resource/Apple/Images/APPLE_DLG_END.png"));
    endButton->setIconSize(QSize(40, 40));
    endButton->setToolTip(QString::fromUtf16(u"\u7ED3\u675F\u6E38\u620F"));
    endButton->setFixedSize(140, 48);
    buttonLayout->addWidget(endButton);

    layout->addLayout(buttonLayout);

    connect(replayButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(nextButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(endButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (!success) {
            resetAppleGame();
        }
        if (success) {
            levelSlider->setValue(qMin(levelSlider->maximum(), levelSlider->value() + 1));
            resetAppleGame();
            startAppleGame();
        } else {
            resetAppleGame();
            startAppleGame();
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
    } else if (ui->stackedWidget->currentWidget() == pageSpaceBattle && !event->text().isEmpty()) {
        QChar letter = event->text().toUpper().at(0);
        if (letter.isLetter()) {
            handleKeyPressForLetterSpace(letter);
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
    if (ui->stackedWidget->indexOf(pageSpaceBattle) == -1) {
        ui->stackedWidget->addWidget(pageSpaceBattle);
    }
    ui->stackedWidget->setCurrentWidget(pageSpaceBattle);
    if (spaceGameArea) {
        spaceGameArea->setFocus();
    }
    positionSpaceShip();
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
    if (spaceBattleBackgroundLabel && pageSpaceBattle) {
        spaceBattleBackgroundLabel->setGeometry(0, 0, pageSpaceBattle->width(), pageSpaceBattle->height());
        QPixmap scaled = spaceBattleBackgroundLabel->pixmap(Qt::ReturnByValue).scaled(pageSpaceBattle->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        spaceBattleBackgroundLabel->setPixmap(scaled);
        positionSpaceShip();
    }
    if (basketLabel && saveAppleGameArea) {
        int x = saveAppleGameArea->width() - basketLabel->width() - 20;
        int y = saveAppleGameArea->height() - basketLabel->height() - 10;
        basketLabel->move(qMax(0, x), qMax(0, y));
    }
}

void MainWindow::createSpaceBattlePage()
{
    pageSpaceBattle = new QWidget;
    spaceBattleBackgroundLabel = new QLabel(pageSpaceBattle);
    spaceBattleBackgroundLabel->setObjectName("spaceBattleBackground");
    spaceBattleBackgroundLabel->setScaledContents(true);
    QPixmap spacePixmap(":/resource/Space/Images/SPACE_BACKGROUND.png");
    if (!spacePixmap.isNull()) {
        spaceBattleBackgroundLabel->setPixmap(spacePixmap);
        spaceBattleBackgroundLabel->setGeometry(0, 0, 900, 620);
    }

    spaceGameArea = new QWidget(pageSpaceBattle);
    spaceGameArea->setAttribute(Qt::WA_TransparentForMouseEvents);
    spaceGameArea->setFocusPolicy(Qt::StrongFocus);
    spaceGameArea->setStyleSheet("background: transparent;");

    QVBoxLayout *spaceLayout = new QVBoxLayout(pageSpaceBattle);
    spaceLayout->setContentsMargins(0, 0, 0, 0);
    spaceLayout->setSpacing(0);

    QWidget *topPanel = new QWidget(pageSpaceBattle);
    topPanel->setFixedHeight(100);
    topPanel->setStyleSheet("background: rgba(0, 0, 0, 0.45);");
    QHBoxLayout *topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(14, 10, 14, 10);
    topLayout->setSpacing(10);

    spaceStatusLabel = new QLabel(QString::fromUtf16(u"\u72B6\u6001: \u672A\u5F00\u59CB"), topPanel);
    spaceStatusLabel->setStyleSheet("color: white; font-size: 16px;");
    spaceScoreLabel = new QLabel(QString::fromUtf16(u"\u5F97\u5206: 0"), topPanel);
    spaceScoreLabel->setStyleSheet("color: white; font-size: 14px;");
    spaceLifeLabel = new QLabel(QString::fromUtf16(u"\u751F\u547D\u503C: 18"), topPanel);
    spaceLifeLabel->setStyleSheet("color: white; font-size: 14px;");
    spaceTimeLabel = new QLabel(QString::fromUtf16(u"\u65F6\u95F4: 0"), topPanel);
    spaceTimeLabel->setStyleSheet("color: white; font-size: 14px;");

    topLayout->addWidget(spaceStatusLabel);
    topLayout->addStretch();
    topLayout->addWidget(spaceScoreLabel);
    topLayout->addWidget(spaceLifeLabel);
    topLayout->addWidget(spaceTimeLabel);

    QWidget *controlPanel = new QWidget(pageSpaceBattle);
    controlPanel->setFixedHeight(90);
    controlPanel->setStyleSheet("background: rgba(0, 0, 0, 0.45);");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(10, 5, 10, 5);
    controlLayout->setSpacing(10);

    QLabel *levelLabel = new QLabel(QString::fromUtf16(u"\u7EA7\u522B:"), controlPanel);
    levelLabel->setStyleSheet("color: white; font-size: 12px;");
    spaceLevelValueLabel = new QLabel("1", controlPanel);
    spaceLevelValueLabel->setStyleSheet("color: white; font-size: 12px;");

    QLabel *enemyCountLabel = new QLabel(QString::fromUtf16(u"\u6570\u91CF:"), controlPanel);
    enemyCountLabel->setStyleSheet("color: white; font-size: 12px;");
    spaceEnemyCountValueLabel = new QLabel("1", controlPanel);
    spaceEnemyCountValueLabel->setStyleSheet("color: white; font-size: 12px;");

    QLabel *currentCountLabel = new QLabel(QString::fromUtf16(u"\u5F53\u524D:"), controlPanel);
    currentCountLabel->setStyleSheet("color: white; font-size: 12px;");
    spaceCurrentCountLabel = new QLabel("0", controlPanel);
    spaceCurrentCountLabel->setStyleSheet("color: white; font-size: 12px;");

    spaceRewardModeToggle = new QCheckBox(QString::fromUtf16(u"\u5956\u52B1\u6A21\u5F0F"), controlPanel);
    spaceRewardModeToggle->setStyleSheet("color: white; font-size: 12px;");

    controlLayout->addWidget(levelLabel);
    controlLayout->addWidget(spaceLevelValueLabel);
    controlLayout->addWidget(enemyCountLabel);
    controlLayout->addWidget(spaceEnemyCountValueLabel);
    controlLayout->addWidget(currentCountLabel);
    controlLayout->addWidget(spaceCurrentCountLabel);
    controlLayout->addStretch();
    controlLayout->addWidget(spaceRewardModeToggle);

    QWidget *buttonPanel = new QWidget(pageSpaceBattle);
    buttonPanel->setFixedHeight(60);
    buttonPanel->setStyleSheet("background: rgba(0, 0, 0, 0.45);");
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonPanel);
    buttonLayout->setContentsMargins(10, 5, 10, 5);
    buttonLayout->setSpacing(10);

    btnBackToGameSelectSpace = new QPushButton(QString::fromUtf16(u"\u8FD4\u56DE"), buttonPanel);
    btnBackToGameSelectSpace->setIcon(QIcon(":/resource/Common/Images/PUBLIC_EXIT.png"));
    btnBackToGameSelectSpace->setIconSize(QSize(24, 24));
    btnBackToGameSelectSpace->setStyleSheet("QPushButton { background: rgba(0, 0, 0, 0.45); color: white; border: 1px solid white; padding: 5px; } QPushButton:hover { background: rgba(255, 255, 255, 0.2); }");
    connect(btnBackToGameSelectSpace, &QPushButton::clicked, this, &MainWindow::onBackToGameSelectClicked);

    btnStartSpaceGame = new QPushButton(QString::fromUtf16(u"\u5F00\u59CB"), buttonPanel);
    btnStartSpaceGame->setIcon(QIcon(":/resource/Common/Images/PUBLIC_START.png"));
    btnStartSpaceGame->setIconSize(QSize(24, 24));
    btnStartSpaceGame->setStyleSheet("QPushButton { background: rgba(0, 0, 0, 0.45); color: white; border: 1px solid white; padding: 5px; } QPushButton:hover { background: rgba(255, 255, 255, 0.2); }");
    connect(btnStartSpaceGame, &QPushButton::clicked, this, &MainWindow::startSpaceBattle);

    btnPauseSpaceGame = new QPushButton(QString::fromUtf16(u"\u6682\u505C"), buttonPanel);
    btnPauseSpaceGame->setIcon(QIcon(":/resource/Common/Images/PUBLIC_PAUSE.png"));
    btnPauseSpaceGame->setIconSize(QSize(24, 24));
    btnPauseSpaceGame->setStyleSheet("QPushButton { background: rgba(0, 0, 0, 0.45); color: white; border: 1px solid white; padding: 5px; } QPushButton:hover { background: rgba(255, 255, 255, 0.2); }");
    connect(btnPauseSpaceGame, &QPushButton::clicked, this, &MainWindow::pauseSpaceBattle);

    btnResetSpaceGame = new QPushButton(QString::fromUtf16(u"\u91CD\u7F6E"), buttonPanel);
    btnResetSpaceGame->setIcon(QIcon(":/resource/Common/Images/PUBLIC_SETUP.png"));
    btnResetSpaceGame->setIconSize(QSize(24, 24));
    btnResetSpaceGame->setStyleSheet("QPushButton { background: rgba(0, 0, 0, 0.45); color: white; border: 1px solid white; padding: 5px; } QPushButton:hover { background: rgba(255, 255, 255, 0.2); }");
    connect(btnResetSpaceGame, &QPushButton::clicked, this, &MainWindow::resetSpaceBattle);

    buttonLayout->addWidget(btnBackToGameSelectSpace);
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnStartSpaceGame);
    buttonLayout->addWidget(btnPauseSpaceGame);
    buttonLayout->addWidget(btnResetSpaceGame);

    spaceLayout->addWidget(topPanel);
    spaceLayout->addWidget(spaceGameArea, 1);
    spaceLayout->addWidget(controlPanel);
    spaceLayout->addWidget(buttonPanel);

    // 创建玩家飞船
    shipLabel = new QLabel(spaceGameArea);
    QPixmap shipPixmap(":/resource/Space/Images/SPACE_SHIP.png");
    if (!shipPixmap.isNull()) {
        shipLabel->setPixmap(shipPixmap);
        shipLabel->setScaledContents(true);
        shipLabel->setFixedSize(50, 50);
        shipLabel->move(0, 0);
    }

    // 初始化设置面板（隐藏）
    QWidget *settingsPanel = new QWidget(pageSpaceBattle);
    settingsPanel->setFixedSize(400, 300);
    settingsPanel->setStyleSheet("background: rgba(0, 0, 0, 0.8); border: 2px solid white;");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsPanel);
    settingsLayout->setContentsMargins(20, 20, 20, 20);
    settingsLayout->setSpacing(10);

    QLabel *settingsTitle = new QLabel(QString::fromUtf16(u"\u8BBE\u7F6E"), settingsPanel);
    settingsTitle->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    settingsTitle->setAlignment(Qt::AlignCenter);

    QLabel *enemyCountLabel2 = new QLabel(QString::fromUtf16(u"\u654C\u673A\u6570\u91CF (1-10):"), settingsPanel);
    enemyCountLabel2->setStyleSheet("color: white;");
    spaceEnemyCountSlider = new QSlider(Qt::Horizontal, settingsPanel);
    spaceEnemyCountSlider->setRange(1, 10);
    spaceEnemyCountSlider->setValue(1);
    spaceEnemyCountSlider->setStyleSheet("QSlider::groove:horizontal { background: rgba(255, 255, 255, 0.3); height: 8px; } QSlider::handle:horizontal { background: white; width: 18px; margin: -5px 0; }");

    QLabel *enemySpeedLabel = new QLabel(QString::fromUtf16(u"\u654C\u673A\u901F\u5EA6 (1-10):"), settingsPanel);
    enemySpeedLabel->setStyleSheet("color: white;");
    spaceEnemySpeedSlider = new QSlider(Qt::Horizontal, settingsPanel);
    spaceEnemySpeedSlider->setRange(1, 10);
    spaceEnemySpeedSlider->setValue(1);
    spaceEnemySpeedSlider->setStyleSheet("QSlider::groove:horizontal { background: rgba(255, 255, 255, 0.3); height: 8px; } QSlider::handle:horizontal { background: white; width: 18px; margin: -5px 0; }");

    QLabel *upgradeIntervalLabel = new QLabel(QString::fromUtf16(u"\u5347\u7EA7\u95F4\u9694 (\u79D2):"), settingsPanel);
    upgradeIntervalLabel->setStyleSheet("color: white;");
    spaceUpgradeIntervalSlider = new QSlider(Qt::Horizontal, settingsPanel);
    spaceUpgradeIntervalSlider->setRange(10, 120);
    spaceUpgradeIntervalSlider->setValue(60);
    spaceUpgradeIntervalSlider->setStyleSheet("QSlider::groove:horizontal { background: rgba(255, 255, 255, 0.3); height: 8px; } QSlider::handle:horizontal { background: white; width: 18px; margin: -5px 0; }");

    QPushButton *settingsOkBtn = new QPushButton(QString::fromUtf16(u"\u786E\u5B9A"), settingsPanel);
    settingsOkBtn->setStyleSheet("QPushButton { background: rgba(0, 0, 0, 0.8); color: white; border: 1px solid white; padding: 5px; } QPushButton:hover { background: rgba(255, 255, 255, 0.2); }");
    connect(settingsOkBtn, &QPushButton::clicked, [this, settingsPanel]() { settingsPanel->hide(); });

    settingsLayout->addWidget(settingsTitle);
    settingsLayout->addWidget(enemyCountLabel2);
    settingsLayout->addWidget(spaceEnemyCountSlider);
    settingsLayout->addWidget(enemySpeedLabel);
    settingsLayout->addWidget(spaceEnemySpeedSlider);
    settingsLayout->addWidget(upgradeIntervalLabel);
    settingsLayout->addWidget(spaceUpgradeIntervalSlider);
    settingsLayout->addStretch();
    settingsLayout->addWidget(settingsOkBtn);

    settingsPanel->move((900 - 400) / 2, (620 - 300) / 2);
    settingsPanel->hide();

    // 连接设置按钮（假设有一个设置按钮，这里用 btnResetSpaceGame 作为示例）
    connect(btnResetSpaceGame, &QPushButton::clicked, [settingsPanel]() { settingsPanel->show(); });

    // 初始化定时器
    spaceGameTimer = new QTimer(this);
    connect(spaceGameTimer, &QTimer::timeout, this, &MainWindow::onSpaceGameTimerTimeout);

    spaceSpawnTimer = new QTimer(this);
    connect(spaceSpawnTimer, &QTimer::timeout, this, &MainWindow::onSpaceSpawnTimerTimeout);

    spaceRewardTimer = new QTimer(this);
    connect(spaceRewardTimer, &QTimer::timeout, this, &MainWindow::onSpaceRewardTimerTimeout);

    spaceUpgradeTimer = new QTimer(this);
    connect(spaceUpgradeTimer, &QTimer::timeout, this, &MainWindow::onSpaceUpgradeTimerTimeout);

    // 初始化游戏状态
    resetSpaceBattle();
}

void MainWindow::startSpaceBattle()
{
    if (spaceGameState == GameState::Playing) return;

    spaceGameState = GameState::Playing;
    spaceBgPlayer->play();
    spaceGameTimer->start(50); // 20 FPS
    spaceSpawnTimer->start(2000); // 每2秒生成一个敌机
    if (spaceRewardMode) {
        spaceRewardTimer->start(10000); // 每10秒生成奖励单词
        requestRewardWordList();
    }
    spaceUpgradeTimer->start(spaceUpgradeInterval * 1000);
    updateSpaceGameStateLabels();
    if (spaceGameArea) {
        spaceGameArea->setFocus();
    }
}

void MainWindow::pauseSpaceBattle()
{
    if (spaceGameState != GameState::Playing) return;

    spaceGameState = GameState::Paused;
    spaceBgPlayer->pause();
    spaceGameTimer->stop();
    spaceSpawnTimer->stop();
    spaceRewardTimer->stop();
    spaceUpgradeTimer->stop();
    updateSpaceGameStateLabels();
}

void MainWindow::resetSpaceBattle()
{
    spaceGameState = GameState::Idle;
    spaceScore = 0;
    spaceLife = 18;
    spaceTime = 0;
    spaceMaxEnemyCount = spaceEnemyCountSlider->value();
    spaceEnemySpeed = spaceEnemySpeedSlider->value();
    spaceUpgradeInterval = spaceUpgradeIntervalSlider->value();
    spaceRewardMode = spaceRewardModeToggle->isChecked();
    shipMovingRight = true;
    nextEnemyId = 0;

    // 清除所有敌机、子弹、奖励单词
    for (auto &enemy : enemies) {
        if (enemy.enemyLabel) enemy.enemyLabel->deleteLater();
        if (enemy.letterLabel) enemy.letterLabel->deleteLater();
    }
    enemies.clear();

    for (auto &bullet : bullets) {
        if (bullet.bulletLabel) bullet.bulletLabel->deleteLater();
    }
    bullets.clear();

    for (auto &word : rewardWords) {
        if (word.wordLabel) word.wordLabel->deleteLater();
    }
    rewardWords.clear();

    // 重置飞船位置
    if (shipLabel) positionSpaceShip();

    spaceBgPlayer->stop();
    spaceGameTimer->stop();
    spaceSpawnTimer->stop();
    spaceRewardTimer->stop();
    spaceUpgradeTimer->stop();

    updateSpaceGameStateLabels();
}

void MainWindow::positionSpaceShip()
{
    if (!shipLabel || !spaceGameArea) return;
    int x = qMax(0, (spaceGameArea->width() - shipLabel->width()) / 2);
    int y = qMax(0, spaceGameArea->height() - shipLabel->height() - 10);
    shipLabel->move(x, y);
}

void MainWindow::spawnEnemy()
{
    if (enemies.size() >= spaceMaxEnemyCount) return;

    EnemyItem enemy;
    enemy.id = nextEnemyId++;
    enemy.active = true;
    enemy.speed = spaceEnemySpeed;
    enemy.position = QPoint(QRandomGenerator::global()->bounded(850), -50); // 随机X，Y在屏幕外

    // 随机字母，不重复
    QChar letter;
    do {
        letter = QChar('A' + QRandomGenerator::global()->bounded(26));
    } while (std::any_of(enemies.begin(), enemies.end(), [letter](const EnemyItem &e) { return e.letter == letter; }));

    enemy.letter = letter;

    enemy.enemyLabel = new QLabel(spaceGameArea);
    QPixmap enemyPixmap(":/resource/Space/Images/SPACE_ENEMY_0.png");
    if (!enemyPixmap.isNull()) {
        enemy.enemyLabel->setPixmap(enemyPixmap);
        enemy.enemyLabel->setScaledContents(true);
        enemy.enemyLabel->setFixedSize(40, 40);
        enemy.enemyLabel->move(enemy.position);
        enemy.enemyLabel->show();
    }

    enemy.letterLabel = new QLabel(spaceGameArea);
    enemy.letterLabel->setText(QString(enemy.letter));
    enemy.letterLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold; background: transparent;");
    enemy.letterLabel->move(enemy.position.x() + 15, enemy.position.y() + 10);
    enemy.letterLabel->show();

    enemies.append(enemy);
    spacePlaneOutSound.play();
}

void MainWindow::spawnBullet(const QPoint &start, const QPoint &target, int targetEnemyId)
{
    BulletItem bullet;
    bullet.active = true;
    bullet.position = QPointF(start);
    bullet.target = QPointF(target);
    bullet.targetEnemyId = targetEnemyId;

    bullet.bulletLabel = new QLabel(spaceGameArea);
    QPixmap bulletPixmap(":/resource/Space/Images/SPACE_BOMB.png");
    if (!bulletPixmap.isNull()) {
        bullet.bulletLabel->setPixmap(bulletPixmap);
        bullet.bulletLabel->setScaledContents(true);
        bullet.bulletLabel->setFixedSize(10, 10);
        bullet.bulletLabel->move(start);
        bullet.bulletLabel->show();
    }

    bullets.append(bullet);
    spaceShootSound.play();
}

void MainWindow::spawnRewardWord()
{
    RewardWord word;
    word.active = true;
    word.word = getRandomWord();
    word.position = QPoint(-200, QRandomGenerator::global()->bounded(400) + 100); // 从左侧进入

    word.wordLabel = new QLabel(spaceGameArea);
    word.wordLabel->setText(word.word);
    word.wordLabel->setStyleSheet("color: yellow; font-size: 20px; font-weight: bold; background: transparent;");
    word.wordLabel->move(word.position);
    word.wordLabel->show();

    rewardWords.append(word);
    spaceWordOutSound.play();
}

void MainWindow::updateSpaceGameStateLabels()
{
    QString statusText;
    switch (spaceGameState) {
    case GameState::Idle: statusText = QString::fromUtf16(u"\u672A\u5F00\u59CB"); break;
    case GameState::Playing: statusText = QString::fromUtf16(u"\u6B63\u5728\u6E38\u620F"); break;
    case GameState::Paused: statusText = QString::fromUtf16(u"\u6682\u505C"); break;
    case GameState::End: statusText = QString::fromUtf16(u"\u7ED3\u675F"); break;
    }
    spaceStatusLabel->setText(QString::fromUtf16(u"\u72B6\u6001: %1").arg(statusText));
    spaceScoreLabel->setText(QString::fromUtf16(u"\u5F97\u5206: %1").arg(spaceScore));
    spaceLifeLabel->setText(QString::fromUtf16(u"\u751F\u547D\u503C: %1").arg(spaceLife));
    spaceTimeLabel->setText(QString::fromUtf16(u"\u65F6\u95F4: %1").arg(spaceTime / 20)); // 假设20 FPS
    spaceLevelValueLabel->setText(QString::number(spaceEnemySpeed));
    spaceEnemyCountValueLabel->setText(QString::number(spaceMaxEnemyCount));
    spaceCurrentCountLabel->setText(QString::number(enemies.size()));
}

void MainWindow::removeEnemy(int index)
{
    if (index < 0 || index >= enemies.size()) return;

    auto &enemy = enemies[index];
    if (enemy.enemyLabel) enemy.enemyLabel->deleteLater();
    if (enemy.letterLabel) enemy.letterLabel->deleteLater();
    enemies.removeAt(index);
}

void MainWindow::removeBullet(int index)
{
    if (index < 0 || index >= bullets.size()) return;

    auto &bullet = bullets[index];
    if (bullet.bulletLabel) bullet.bulletLabel->deleteLater();
    bullets.removeAt(index);
}

void MainWindow::removeRewardWord(int index)
{
    if (index < 0 || index >= rewardWords.size()) return;

    auto &word = rewardWords[index];
    if (word.wordLabel) word.wordLabel->deleteLater();
    rewardWords.removeAt(index);
}

void MainWindow::handleKeyPressForLetterSpace(const QChar &letter)
{
    if (spaceGameState != GameState::Playing) return;

    // 发射子弹瞄准匹配敌机，不直接移除敌机
    for (const EnemyItem &enemy : qAsConst(enemies)) {
        if (enemy.active && enemy.letter == letter) {
            QPoint start = shipLabel->pos() + QPoint(shipLabel->width() / 2 - 5, 0); // 飞船顶部中央
            QPoint target = enemy.position + QPoint(enemy.enemyLabel->width() / 2, enemy.enemyLabel->height() / 2);
            spawnBullet(start, target, enemy.id);
            return;
        }
    }

    // 检查奖励单词
    for (int i = 0; i < rewardWords.size(); ++i) {
        if (rewardWords[i].word.startsWith(letter, Qt::CaseInsensitive)) {
            // 移除单词首字母
            rewardWords[i].word = rewardWords[i].word.mid(1);
            rewardWords[i].wordLabel->setText(rewardWords[i].word);
            if (rewardWords[i].word.isEmpty()) {
                // 单词完成，恢复生命
                spaceLife = 18;
                removeRewardWord(i);
                updateSpaceGameStateLabels();
            }
            return;
        }
    }
}

void MainWindow::onSpaceGameTimerTimeout()
{
    spaceTime++;

    // 更新敌机位置
    for (int i = 0; i < enemies.size(); ++i) {
        auto &enemy = enemies[i];
        enemy.position.setY(enemy.position.y() + enemy.speed);
        enemy.enemyLabel->move(enemy.position);
        enemy.letterLabel->move(enemy.position.x() + 15, enemy.position.y() + 10);

        // 检查碰撞或落底
        if (enemy.position.y() > 570 || shipLabel->geometry().intersects(QRect(enemy.position, QSize(40, 40)))) {
            spaceLife--;
            removeEnemy(i);
            i--; // 调整索引
            if (spaceLife <= 0) {
                showSpaceGameEndDialog(false);
                return;
            }
        }
    }

    // 更新子弹位置
    for (int i = bullets.size() - 1; i >= 0; --i) {
        auto &bullet = bullets[i];

        QPointF currentTarget = bullet.target;
        if (bullet.targetEnemyId >= 0) {
            bool foundTarget = false;
            for (const EnemyItem &enemy : qAsConst(enemies)) {
                if (enemy.active && enemy.id == bullet.targetEnemyId) {
                    currentTarget = QPointF(enemy.position + QPoint(enemy.enemyLabel->width() / 2, enemy.enemyLabel->height() / 2));
                    foundTarget = true;
                    break;
                }
            }
            if (!foundTarget) {
                bullet.targetEnemyId = -1;
            }
        }

        QPointF dir = currentTarget - bullet.position;
        double distance = std::hypot(dir.x(), dir.y());
        if (distance < 1.0) {
            removeBullet(i);
            continue;
        }
        QPointF unitDir = dir / distance;
        double step = qMin(10.0, distance);
        QPointF nextPos = bullet.position + unitDir * step;
        bullet.position = nextPos;
        bullet.bulletLabel->move(QPoint(qRound(nextPos.x()), qRound(nextPos.y())));

        QRect bulletRect(bullet.bulletLabel->geometry());
        bool hit = false;
        for (int j = enemies.size() - 1; j >= 0; --j) {
            auto &enemy = enemies[j];
            if (!enemy.active) continue;
            QRect enemyRect(enemy.enemyLabel->geometry());
            if (bulletRect.intersects(enemyRect)) {
                spaceScore += 10;
                spaceBlastSound.play();
                removeBullet(i);
                removeEnemy(j);
                hit = true;
                break;
            }
        }
        if (hit) {
            continue;
        }

        if (bullet.position.x() < 0 || bullet.position.x() > spaceGameArea->width() || bullet.position.y() < 0 || bullet.position.y() > spaceGameArea->height()) {
            removeBullet(i);
        }
    }

    // 自动飞船左右来回移动
    QPoint shipPos = shipLabel->pos();
    int shipStep = 6;
    if (shipMovingRight) {
        shipPos.setX(shipPos.x() + shipStep);
        if (shipPos.x() >= spaceGameArea->width() - shipLabel->width()) {
            shipPos.setX(spaceGameArea->width() - shipLabel->width());
            shipMovingRight = false;
        }
    } else {
        shipPos.setX(shipPos.x() - shipStep);
        if (shipPos.x() <= 0) {
            shipPos.setX(0);
            shipMovingRight = true;
        }
    }
    shipLabel->move(shipPos);

    // 更新奖励单词位置
    for (int i = rewardWords.size() - 1; i >= 0; --i) {
        auto &word = rewardWords[i];
        word.position.setX(word.position.x() + 2); // 向右移动
        word.wordLabel->move(word.position);
        if (word.position.x() > 900) {
            removeRewardWord(i);
        }
    }

    updateSpaceGameStateLabels();
}

void MainWindow::onSpaceSpawnTimerTimeout()
{
    spawnEnemy();
}

void MainWindow::onSpaceRewardTimerTimeout()
{
    spawnRewardWord();
}

void MainWindow::onSpaceUpgradeTimerTimeout()
{
    spaceEnemySpeed++;
    spaceMaxEnemyCount++;
    spaceUpgradeSound.play();
    updateSpaceGameStateLabels();
}

void MainWindow::showSpaceGameEndDialog(bool success)
{
    spaceGameState = GameState::End;
    spaceBgPlayer->stop();
    spaceGameTimer->stop();
    spaceSpawnTimer->stop();
    spaceRewardTimer->stop();
    spaceUpgradeTimer->stop();

    QDialog dialog(this);
    dialog.setWindowTitle(success ? QString::fromUtf16(u"\u80DC\u5229") : QString::fromUtf16(u"\u5931\u8D25"));
    dialog.setFixedSize(300, 200);
    dialog.setStyleSheet("background: rgba(0, 0, 0, 0.8); color: white;");

    QVBoxLayout layout(&dialog);
    QLabel titleLabel(success ? QString::fromUtf16(u"\u606D\u559C\u4F60\u80DC\u5229\u4E86\uFF01") : QString::fromUtf16(u"\u5F88\u9057\u61BE\uFF0C\u4F60\u5931\u8D25\u4E86\u3002"), &dialog);
    titleLabel.setAlignment(Qt::AlignCenter);
    titleLabel.setStyleSheet("font-size: 18px; font-weight: bold;");

    QLabel scoreLabel(QString::fromUtf16(u"\u6700\u7EC8\u5F97\u5206: %1").arg(spaceScore), &dialog);
    scoreLabel.setAlignment(Qt::AlignCenter);

    QPushButton replayBtn(QString::fromUtf16(u"\u91CD\u65B0\u6E38\u620F"), &dialog);
    replayBtn.setStyleSheet("QPushButton { background: rgba(0, 0, 0, 0.8); color: white; border: 1px solid white; padding: 5px; } QPushButton:hover { background: rgba(255, 255, 255, 0.2); }");
    connect(&replayBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    QPushButton exitBtn(QString::fromUtf16(u"\u9000\u51FA"), &dialog);
    exitBtn.setStyleSheet("QPushButton { background: rgba(0, 0, 0, 0.8); color: white; border: 1px solid white; padding: 5px; } QPushButton:hover { background: rgba(255, 255, 255, 0.2); }");
    connect(&exitBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    layout.addWidget(&titleLabel);
    layout.addWidget(&scoreLabel);
    layout.addStretch();
    layout.addWidget(&replayBtn);
    layout.addWidget(&exitBtn);

    if (dialog.exec() == QDialog::Accepted) {
        resetSpaceBattle();
        startSpaceBattle();
    } else {
        onBackToGameSelectClicked();
    }
}

void MainWindow::requestRewardWordList()
{
    generatedRewardWords.clear();
    if (llmApiKey.isEmpty() || llmApiUrl.isEmpty()) {
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(llmApiUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QStringLiteral("application/json")));
    request.setRawHeader("Authorization", QByteArray("Bearer ") + llmApiKey.toUtf8());

    QJsonObject systemMessage;
    systemMessage["role"] = "system";
    systemMessage["content"] = QString::fromUtf16(u"你是一个帮助生成适合太空大战奖励模式的英语单词列表的助手。返回一个仅包含单词的 JSON 数组。每个单词长度不超过 8 个字符。");

    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = QString::fromUtf16(u"请生成 6 个适合太空战奖励模式的简短英文单词，并以 JSON 数组形式返回，例如 [\"laser\",\"shield\",\"boost\"]。不要返回额外说明文字。");

    QJsonArray messages;
    messages.append(systemMessage);
    messages.append(userMessage);

    QJsonObject requestBody;
    requestBody["model"] = "gpt-3.5-turbo";
    requestBody["messages"] = messages;
    requestBody["temperature"] = 0.8;
    requestBody["max_tokens"] = 100;

    networkManager->post(request, QJsonDocument(requestBody).toJson(QJsonDocument::Compact));
}

void MainWindow::handleRewardWordsReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        if (responseDoc.isObject()) {
            QJsonObject responseObject = responseDoc.object();
            QJsonArray choices = responseObject.value("choices").toArray();
            if (!choices.isEmpty()) {
                QJsonObject firstChoice = choices.first().toObject();
                QJsonObject message = firstChoice.value("message").toObject();
                QString content = message.value("content").toString().trimmed();
                QJsonDocument contentDoc = QJsonDocument::fromJson(content.toUtf8());
                if (contentDoc.isArray()) {
                    QJsonArray wordArray = contentDoc.array();
                    for (const QJsonValue &value : wordArray) {
                        QString word = value.toString().trimmed();
                        if (!word.isEmpty()) {
                            generatedRewardWords.append(word);
                        }
                    }
                } else {
                    content.remove('[').remove(']').remove('"');
                    QStringList words = content.split(',', Qt::SkipEmptyParts);
                    for (QString word : words) {
                        word = word.trimmed();
                        if (!word.isEmpty()) {
                            generatedRewardWords.append(word);
                        }
                    }
                }
            }
        }
    }
    reply->deleteLater();
}

QString MainWindow::getRandomWord()
{
    if (!generatedRewardWords.isEmpty()) {
        return generatedRewardWords.at(QRandomGenerator::global()->bounded(generatedRewardWords.size()));
    }
    return localWords.at(QRandomGenerator::global()->bounded(localWords.size()));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && ui->stackedWidget->currentWidget() == pageSpaceBattle) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (!keyEvent->text().isEmpty()) {
            QChar letter = keyEvent->text().toUpper().at(0);
            if (letter.isLetter()) {
                handleKeyPressForLetterSpace(letter);
                return true;
            }
        }
    }

    if (event->type() == QEvent::Enter) {
        // 鼠标悬停在按钮上时播放声音
        aniBtnEnterSound.play();
    } else if (event->type() == QEvent::MouseButtonPress) {
        // 鼠标点击按钮时播放声音
        btnClickSound.play();
    }
    return QMainWindow::eventFilter(obj, event);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QUrl>
#include <QPixmap>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QEvent>

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

    // 初始化音乐播放器
    appleBgPlayer = new QMediaPlayer(this);
    appleBgPlayer->setMedia(QUrl("qrc:/resource/Apple/Sounds/APPLE_BG.wav"));
    appleBgPlayer->setVolume(60);
    connect(appleBgPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);

    btnClickSound.setSource(QUrl("qrc:/resource/Common/Sounds/BTN_CLICK.wav"));
    btnClickSound.setVolume(0.8f);

    aniBtnEnterSound.setSource(QUrl("qrc:/resource/Common/Sounds/ANIBTN_ENTER.wav"));
    aniBtnEnterSound.setVolume(0.8f);

    // 创建主界面背景图片标签
    homeBackgroundLabel = new QLabel(ui->pageHome);
    QIcon homeBgIcon(":/resource/Common/Images/MAIN_DLG_BG.png");
    if (!homeBgIcon.isNull()) {
        QPixmap pixmap = homeBgIcon.pixmap(ui->pageHome->size());
        homeBackgroundLabel->setPixmap(pixmap);
        homeBackgroundLabel->setScaledContents(true);
        homeBackgroundLabel->setGeometry(0, 0, ui->pageHome->width(), ui->pageHome->height());
        homeBackgroundLabel->lower(); // 将背景放在最底层
    }

    // 创建游戏选择页面背景图片标签
    appleBackgroundLabel = new QLabel(ui->pageGameSelect);
    QIcon appleBgIcon(":/resource/Apple/Images/APPLE_BACKGROUND.png");
    if (!appleBgIcon.isNull()) {
        QPixmap pixmap = appleBgIcon.pixmap(ui->pageGameSelect->size());
        appleBackgroundLabel->setPixmap(pixmap);
        appleBackgroundLabel->setScaledContents(true);
        appleBackgroundLabel->setGeometry(0, 0, ui->pageGameSelect->width(), ui->pageGameSelect->height());
        appleBackgroundLabel->lower(); // 将背景放在最底层
    }

    // 安装事件过滤器到所有按钮，用于悬停声音
    ui->btnNewUser->installEventFilter(this);
    ui->btnEnglishTyping->installEventFilter(this);
    ui->btnPinyinTyping->installEventFilter(this);
    ui->btnWubiTyping->installEventFilter(this);
    ui->btnTypingGame->installEventFilter(this);
    ui->btnBackHome->installEventFilter(this);
    ui->btnSaveApple->installEventFilter(this);
    ui->btnSpaceBattle->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete homeBackgroundLabel;
    delete appleBackgroundLabel;
    delete appleBgPlayer;
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
    ui->stackedWidget->setCurrentWidget(ui->pageGameSelect);
}

void MainWindow::onBackHomeClicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageHome);
    appleBgPlayer->stop();
}

void MainWindow::onSelectSaveAppleClicked()
{
    appleBgPlayer->play();
    QMessageBox::information(this, tr("拯救苹果"), tr("正在进入拯救苹果的游戏页面。"));
}

void MainWindow::onSelectSpaceBattleClicked()
{
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
    }
    if (appleBackgroundLabel) {
        appleBackgroundLabel->setGeometry(0, 0, ui->pageGameSelect->width(), ui->pageGameSelect->height());
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

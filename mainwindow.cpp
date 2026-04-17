#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QUrl>

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

    appleBgSound.setSource(QUrl("qrc:/resource/Apple/Sounds/APPLE_BG.wav"));
    appleBgSound.setLoopCount(QSoundEffect::Infinite);
    appleBgSound.setVolume(0.6f);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    appleBgSound.stop();
}

void MainWindow::onSelectSaveAppleClicked()
{
    appleBgSound.play();
    QMessageBox::information(this, tr("拯救苹果"), tr("正在进入拯救苹果的游戏页面。"));
}

void MainWindow::onSelectSpaceBattleClicked()
{
    QMessageBox::information(this, tr("太空大战"), tr("正在进入太空大战游戏页面。"));
}

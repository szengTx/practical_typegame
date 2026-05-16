#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSoundEffect>
#include <QLabel>
#include <QMediaPlayer>
#include <QPushButton>
#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QSlider>
#include <QCheckBox>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewUserClicked();
    void onEnglishTypingClicked();
    void onPinyinTypingClicked();
    void onWubiTypingClicked();
    void onTypingGameClicked();
    void onBackHomeClicked();
    void onSelectSaveAppleClicked();
    void onSelectSpaceBattleClicked();
    void onBackToGameSelectClicked();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    enum class GameState {
        Idle,
        Playing,
        Paused,
        End
    };

    struct AppleItem {
        QLabel *appleLabel;
        QLabel *letterLabel;
        QChar letter;
        int speed;
        bool active;
    };

    struct EnemyItem {
        int id;
        QLabel *enemyLabel;
        QLabel *letterLabel;
        QChar letter;
        int speed;
        QPoint position;
        bool active;
    };

    struct BulletItem {
        QLabel *bulletLabel;
        QPointF position;
        QPointF target;
        int targetEnemyId;
        bool active;
    };

    struct RewardWord {
        QLabel *wordLabel;
        QString word;
        QPoint position;
        bool active;
    };

    Ui::MainWindow *ui;
    QMediaPlayer *appleBgPlayer;
    QSoundEffect btnClickSound;
    QSoundEffect aniBtnEnterSound;
    QSoundEffect actionClickSound;
    QSoundEffect glideSound;
    QSoundEffect typeSound;
    QSoundEffect appleHitSound;
    QSoundEffect appleFailSound;
    QLabel *homeBackgroundLabel;
    QLabel *appleBackgroundLabel;
    QWidget *pageSaveApple;
    QLabel *saveAppleBackgroundLabel;
    QWidget *saveAppleGameArea;
    QLabel *basketLabel;
    QLabel *statusLabel;
    QLabel *scoreLabel;
    QLabel *failLabel;
    QLabel *accuracyLabel;
    QLabel *levelValueLabel;
    QLabel *targetValueLabel;
    QLabel *currentCountLabel;
    QSlider *levelSlider;
    QSlider *maxAppleSlider;
    QSlider *targetSlider;
    QCheckBox *soundToggle;
    QPushButton *btnBackToGameSelect;
    QPushButton *btnStartGame;
    QPushButton *btnPauseGame;
    QPushButton *btnResetGame;
    QTimer *gameTimer;
    QTimer *spawnTimer;
    QVector<AppleItem> apples;
    GameState gameState;
    int currentLevel;
    int maxAppleCount;
    int targetSuccessCount;
    int successCount;
    int failCount;
    int totalInputCount;
    int currentAppleCount;

    // Space Battle members
    QMediaPlayer *spaceBgPlayer;
    QSoundEffect spaceBlastSound;
    QSoundEffect spacePlaneOutSound;
    QSoundEffect spaceShootSound;
    QSoundEffect spaceWordOutSound;
    QSoundEffect spaceUpgradeSound;
    QLabel *spaceBackgroundLabel;
    QWidget *pageSpaceBattle;
    QLabel *spaceBattleBackgroundLabel;
    QWidget *spaceGameArea;
    QLabel *shipLabel;
    QLabel *spaceStatusLabel;
    QLabel *spaceScoreLabel;
    QLabel *spaceLifeLabel;
    QLabel *spaceTimeLabel;
    QLabel *spaceLevelValueLabel;
    QLabel *spaceEnemyCountValueLabel;
    QLabel *spaceCurrentCountLabel;
    QSlider *spaceEnemyCountSlider;
    QSlider *spaceEnemySpeedSlider;
    QSlider *spaceUpgradeIntervalSlider;
    QCheckBox *spaceRewardModeToggle;
    QPushButton *btnBackToGameSelectSpace;
    QPushButton *btnStartSpaceGame;
    QPushButton *btnPauseSpaceGame;
    QPushButton *btnResetSpaceGame;
    QTimer *spaceGameTimer;
    QTimer *spaceSpawnTimer;
    QTimer *spaceRewardTimer;
    QTimer *spaceUpgradeTimer;
    QVector<EnemyItem> enemies;
    QVector<BulletItem> bullets;
    QVector<RewardWord> rewardWords;
    GameState spaceGameState;
    int spaceScore;
    int spaceLife;
    int spaceTime;
    int spaceMaxEnemyCount;
    int spaceEnemySpeed;
    int spaceUpgradeInterval;
    bool spaceRewardMode;
    bool shipMovingRight;
    int nextEnemyId;
    QStringList localWords; // for reward mode fallback

    void createSpaceBattlePage();
    void startSpaceBattle();
    void pauseSpaceBattle();
    void resetSpaceBattle();
    void spawnEnemy();
    void spawnBullet(const QPoint &start, const QPoint &target, int targetEnemyId = -1);
    void spawnRewardWord();
    void updateSpaceGameStateLabels();
    void removeEnemy(int index);
    void removeBullet(int index);
    void removeRewardWord(int index);
    void handleKeyPressForLetterSpace(const QChar &letter);
    void onSpaceGameTimerTimeout();
    void onSpaceSpawnTimerTimeout();
    void onSpaceRewardTimerTimeout();
    void onSpaceUpgradeTimerTimeout();
    void showSpaceGameEndDialog(bool success);
    void positionSpaceShip();
    QString getRandomWord(); // for reward mode

    void createSaveApplePage();
    void startAppleGame();
    void pauseAppleGame();
    void resetAppleGame();
    void spawnApple();
    void updateGameStateLabels();
    void removeApple(int index);
    void handleKeyPressForLetter(const QChar &letter);
    void onGameTimerTimeout();
    void onSpawnTimerTimeout();
    void showGameEndDialog(bool success);
};
#endif // MAINWINDOW_H


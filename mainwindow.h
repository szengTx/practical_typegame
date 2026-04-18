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
};
#endif // MAINWINDOW_H


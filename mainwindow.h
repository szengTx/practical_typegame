#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSoundEffect>
#include <QLabel>
#include <QMediaPlayer>

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
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QMediaPlayer *appleBgPlayer;
    QSoundEffect btnClickSound;
    QSoundEffect aniBtnEnterSound;
    QLabel *homeBackgroundLabel;
    QLabel *appleBackgroundLabel;
};
#endif // MAINWINDOW_H


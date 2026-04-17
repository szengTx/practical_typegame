#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSoundEffect>

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

private:
    Ui::MainWindow *ui;
    QSoundEffect appleBgSound;
};
#endif // MAINWINDOW_H


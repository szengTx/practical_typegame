#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

static void loadEnvFile()
{
    auto tryLoad = [](const QString &path) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return false;
        }
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith('#')) {
                continue;
            }
            int eq = line.indexOf('=');
            if (eq <= 0) {
                continue;
            }
            QString key = line.left(eq).trimmed();
            QString value = line.mid(eq + 1).trimmed();
            if (value.startsWith('"') && value.endsWith('"') && value.length() >= 2) {
                value = value.mid(1, value.length() - 2);
            }
            if (qEnvironmentVariableIsEmpty(key.toUtf8())) {
                qputenv(key.toUtf8(), value.toUtf8());
            }
        }
        return true;
    };

    QDir dir(QCoreApplication::applicationDirPath());
    for (int depth = 0; depth < 5; ++depth) {
        QString path = dir.filePath(".env");
        if (tryLoad(path)) {
            return;
        }
        if (!dir.cdUp()) {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadEnvFile();
    MainWindow w;
    w.show();
    return a.exec();
}

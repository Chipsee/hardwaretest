#include "mainwindow.h"
#include <QApplication>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QGuiApplication>
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
#include <QDebug>
#include <QSettings>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);
    QApplication a(argc, argv);

    QString executableName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    //QGuiApplication a(argc, argv);
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int width = screenRect.width();
    int height = screenRect.height();
#else
    QDesktopWidget* desktopWidget = QApplication::desktop();
    int width = desktopWidget->width();
    int height = desktopWidget->height();
    qDebug() << "Desktop is: " + QString::number(QApplication::desktop()->width()) + "x" + QString::number(QApplication::desktop()->height());
    qDebug() << "Screen is: " + QString::number(width,10) + "x" + QString::number(height,10);
#endif
    QFont font = a.font();

    MainWindow w(executableName);

    if(width <= 1024){
        //qDebug()<< "will use 6 font";
        font.setPointSize(6);
        a.setFont(font);
        w.resize(800,500); //This will use min size
    } else {
        // Read the window position from QSettings
        // config file is located in .config/Chipsee/hardwaretest.conf
        //QSettings settings("Chipsee", "hardwaretest");
        w.restoreGeometry(w.settings->value("windowGeometry").toByteArray());
    }

    w.show();
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    w.move((width-w.width())/2,(height-w.height())/2);
#else
    w.move((QApplication::desktop()->width()-w.width())/2,(QApplication::desktop()->height()-w.height())/2);
#endif

    if(width==1024 && height==600)
        w.move(0,0);

    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);
    QApplication a(argc, argv);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    int width = desktopWidget->width();
    int height = desktopWidget->height();
    qDebug() << "Desktop is: " + QString::number(QApplication::desktop()->width()) + "x" + QString::number(QApplication::desktop()->height());
    qDebug() << "Screen is: " + QString::number(width,10) + "x" + QString::number(height,10);
    QFont font = a.font();

    MainWindow w;

    if(height <= 600 && width <= 1024){
        qDebug()<< "will use 6 font";
        //font.setPointSize(6);
        //a.setFont(font);
	w.resize(800,500); //This will use min size
    }

    w.show();
    w.move((QApplication::desktop()->width()-w.width())/2,(QApplication::desktop()->height()-w.height())/2);

    if(width==1024)
        w.showFullScreen();

    return a.exec();
}

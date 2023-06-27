#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    int width = desktopWidget->width();
    int height = desktopWidget->height();
    QString widthstr = QString::number(width,10);
    QString heightstr = QString::number(height,10);
    qDebug() << "Screen is: " + widthstr + "x" + heightstr;
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

    return a.exec();
}

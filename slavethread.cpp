#include "slavethread.h"
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QDebug>

QT_USE_NAMESPACE

SlaveThread::SlaveThread(QObject *parent)
    :QThread(parent), quit(false)
{
}

SlaveThread::~SlaveThread()
{
    quit = true;
    wait();
    qDebug() << "~SlaveThread()";
}

void SlaveThread::startSlave(QString *board)
{

    this->board = board;
    if (!isRunning())
        start();
}

/*
 * echo 34 > /sys/class/gpio/export && sudo echo out > /sys/class/gpio/gpio34/direction
 */
void SlaveThread::pi485Init()
{
    QString cmdstr = "echo 34 > /sys/class/gpio/export && sudo echo out > /sys/class/gpio/gpio34/direction";
    system(cmdstr.toLocal8Bit());
}


/*
 * echo 1 > /sys/class/gpio/gpio34/value
 */
void SlaveThread::enablePi485Send()
{
    QString cmdstr = "echo 1 > /sys/class/gpio/gpio34/value";
    system(cmdstr.toLocal8Bit());
//    qDebug() << "EN-Send!!";
}

/*
 * echo 0 > /sys/class/gpio/gpio34/value
 */
void SlaveThread::enablePi485Receive()
{
    QString cmdstr = "echo 0 > /sys/class/gpio/gpio34/value";
    system(cmdstr.toLocal8Bit());
//    qDebug() << "EN-Receive!!";
}

void SlaveThread::run()
{
    qDebug() << "board is " + *board;


        QSerialPort *serial = new QSerialPort();
        QSerialPort *serial1 = new QSerialPort();
        QSerialPort *serial2 = new QSerialPort();
        QSerialPort *serial3 = new QSerialPort();
        QSerialPort *serial4 = new QSerialPort();
        QByteArray requestData;

        QSerialPort * port[5] = {serial,serial1,serial2,serial3,serial4};

    int i = 0;
    int timeout;
//    QString strValue;
    bool changeflag=false;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        port[i]->setPortName(info.portName());
        port[i]->setBaudRate(QSerialPort::Baud115200);
        port[i]->setDataBits(QSerialPort::Data8);
        port[i]->setParity(QSerialPort::NoParity);
        port[i]->setStopBits(QSerialPort::OneStop);
        port[i]->setFlowControl(QSerialPort::NoFlowControl);
        i++;
    }

    if(*board != "pi"){
        //CAN INIT
        system("echo >/tmp/can0.txt");
        system("canconfig can0 stop");
        system("canconfig can0 bitrate 10000 ctrlmode triple-sampling on loopback off ");
        system("canconfig can0 start");
        system("candump can0 > /tmp/can0.txt &");

        system("echo >/tmp/can1.txt");
        system("canconfig can1 stop");
        system("canconfig can1 bitrate 10000 ctrlmode triple-sampling on loopback off ");
        system("canconfig can1 start");
    }

    while(!quit)
    {
        // Serial AutoTest
        for(i=0; i<5; i++)
        {
            if(*board =="pi"){
                if(i==2){
                    i=1;
                    changeflag=true;
                }
            }

            if(i==0)
                timeout = 20000;
            else
                timeout = 1000;
            requestData = "";
            if(port[i]->isOpen())
                port[i]->close();
            if(port[i]->open(QIODevice::ReadWrite))
            {
                if(port[i]->waitForReadyRead(timeout))
                {
                    requestData = port[i]->readAll();
                    while(port[i]->waitForReadyRead(10))
                        requestData += port[i]->readAll();
                }
                if(*board == "pi") {
                    //enable send
                    enablePi485Send();
                }
                port[i]->write(requestData);
                if(port[i]->waitForBytesWritten(1000)){
                    emit this->request(port[i]->portName() +"\n"+requestData);
                    if(*board == "pi"){
                        //enable receive
                        enablePi485Receive();
                    }
                }
            }
//            strValue = tr("%1").arg(i);
//            qDebug() << "i = " +strValue;
            if(*board =="pi" && changeflag){
                changeflag=false;
                break;
            }
        }

        if(*board !="pi"){
        // CAN AutoTest
        // CAN0
        QFile file("/tmp/can0.txt");
        if (file.open(QIODevice::ReadWrite)){
            QTextStream in(&file);
//            QString line=in.readAll();    // The All Lines
            QString line=in.readLine(); // The First Line
            line = in.readLine();   // The Second Line
            emit this->canrequest("can0\n"+line);
            if(line.contains("11 22 33 44 55 66 77 88"))
                system("cansend can0 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88");
        }
        file.close();

        // CAN1
        QFile file1("/tmp/can1.txt");
        if (file1.open(QIODevice::ReadWrite)){
            QTextStream in(&file1);
//            QString line=in.readAll();    // The All Lines
            QString line=in.readLine(); // The First Line
            line = in.readLine();   // The Second Line
            emit this->canrequest("can1\n"+line);
            if(line.contains("11 22 33 44 55 66 77 88"))
                system("cansend can1 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88");
        }
        file1.close();
        }

    } // while
} // run

#include "slavethread.h"
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>

QT_USE_NAMESPACE

SlaveThread::SlaveThread(QObject *parent)
    :QThread(parent), quit(false)
{
}

SlaveThread::~SlaveThread()
{
    quit = true;
    wait();
}

void SlaveThread::startSlave()
{

    if (!isRunning())
        start();
}

void SlaveThread::run()
{
    //Serial INIT
    QSerialPort *serial = new QSerialPort();
    QSerialPort *serial1 = new QSerialPort();
    QSerialPort *serial2 = new QSerialPort();
    QSerialPort *serial3 = new QSerialPort();
    QSerialPort *serial4 = new QSerialPort();
    QByteArray requestData;

    QSerialPort * port[5] = {serial,serial1,serial2,serial3,serial4};

    int i = 0;
    int timeout;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        port[i]->setPortName(info.portName());
        port[i]->setBaudRate(QSerialPort::Baud115200);
        port[i]->setDataBits(QSerialPort::Data8);
        port[i]->setParity(QSerialPort::NoParity);
        port[i]->setStopBits(QSerialPort::OneStop);
        port[i]->setFlowControl(QSerialPort::NoFlowControl);
        i++;
    }

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
    system("candump can1 > /tmp/can1.txt &");


    while(!quit)
    {
        // Serial AutoTest
        for(i=0; i<5; i++)
        {
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
                port[i]->write(requestData);
                if(port[i]->waitForBytesWritten(1000))
                    emit this->request(port[i]->portName() +"\n"+requestData);
            }
        }

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


    } // while

} // run

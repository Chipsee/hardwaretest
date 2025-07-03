#include "slavethread.h"
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QDebug>
#include <QProcess>

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

QString SlaveThread::GetComResult(QString cmd)
{
    QProcess process;
#if QT_VERSION >= QT_VERSION_CHECK(5,15,8)
    QStringList list = cmd.split(" ");
    QString firstElement;
    QStringList otherElements;
    if(!list.isEmpty()){
        firstElement = list.first();
    }
    if(list.size() > 1) {
        otherElements = list.mid(1);
    } else {
        otherElements = QStringList() << "-c" << firstElement;
        firstElement = "/bin/sh";
    }

    process.start(firstElement, otherElements);
    // Or we can use startCommand method in QT_VERSION >= 6.0.0.
    //process.startCommand(cmd);
#else
    process.start(cmd);
#endif
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();
    QString result = output;
    return result.remove(QChar('\n'),Qt::CaseInsensitive);
}

void SlaveThread::run()
{
    qDebug() << "board is " + *board;


    QSerialPort *serial = new QSerialPort();
    QSerialPort *serial1 = new QSerialPort();
    QSerialPort *serial2 = new QSerialPort();
    QSerialPort *serial3 = new QSerialPort();
    QSerialPort *serial4 = new QSerialPort();
    QSerialPort *serial5 = new QSerialPort();
    QByteArray requestData;

    QSerialPort * port[6] = {serial,serial1,serial2,serial3,serial4,serial5};

    int i = 0;
    int timeout;
    bool changeflag=false;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        port[i]->setPortName(info.portName());
        port[i]->setBaudRate(QSerialPort::Baud115200);
        port[i]->setDataBits(QSerialPort::Data8);
        port[i]->setParity(QSerialPort::NoParity);
        port[i]->setStopBits(QSerialPort::OneStop);
        port[i]->setFlowControl(QSerialPort::NoFlowControl);
        i++;
        if(i>5)
            break;
    }

    if(*board == "CS12720RA4050" || *board == "CS10600RA4070" || *board == "CS10600RA4070D" || *board == "CS12800RA4101" || *board == "LRRA4-101" || *board == "CS12800RA4101A" || *board == "CS12800RA4101BOX" || *board == "CS12800RA4101P" || *board == "CS19108RA4133P" || *board == "CS10768RA4150P" || *board == "CS19108RA4156P" || *board == "CS19108RA4215P" || *board == "CS12800RA4101AV4"){
        if(port[0] != NULL && port[0]->portName() == "ttyACM0"){
            port[0] = port[1];
            port[1] = port[2];
            port[2] = port[3];
            port[3] = port[4];
            port[4] = port[5];
        }

    }

    if(*board == "CS12720RA4050" || *board == "CS10600RA4070" || *board == "CS10600RA4070D" || *board == "AM335XBOARD" || *board == "CS12800RA4101BOX" || *board == "CS12800RA4101P" || *board == "CS19108RA4133P" || *board == "CS10768RA4150P" || *board == "CS19108RA4156P" || *board == "CS19108RA4215P" || *board == "CS10600RA5070P"){
        port[4] = port[3];
        port[3] = port[2];
        port[2] = port[1];
        qDebug() << port[0]->portName();
        qDebug() << port[1]->portName();
        qDebug() << port[2]->portName();
        qDebug() << port[3]->portName();
        qDebug() << port[4]->portName();
    }

    if(*board == "CS19108RA4133PISO") {
        port[5] = port[2];
        port[4] = port[2];
        port[3] = port[2];
        port[2] = port[1];
        qDebug() << port[0]->portName();
        qDebug() << port[1]->portName();
        qDebug() << port[2]->portName();
        qDebug() << port[3]->portName();
        qDebug() << port[4]->portName();
    }

    if(*board == "CS12800RA101"){
        port[3] = port[4];
        port[4] = port[5];
        port[2] = port[1];
        qDebug() << port[0]->portName();
        qDebug() << port[1]->portName();
        qDebug() << port[2]->portName();
        qDebug() << port[3]->portName();
        qDebug() << port[4]->portName();
    }

    if(*board == "CS12800RA4101" || *board == "LRRA4-101" || *board == "CS12800RA4101A" || *board == "CS12800RA4101AV4" || *board == "CS12800RA5101A"){
        port[5] = port[4];
        port[4] = port[3];
        port[3] = port[2];
        port[2] = port[1];
        qDebug() << port[0]->portName();
        qDebug() << port[1]->portName();
        qDebug() << port[2]->portName();
        qDebug() << port[3]->portName();
        qDebug() << port[4]->portName();
    }

    /*
     * PORT[0]=/dev/ttyS3
     * PORT[1]=/dev/ttyS3
     * PORT[2]=/dev/ttyS5
     * PORT[3]=/dev/ttyS5
     * PORT[4]=/dev/ttyS5
     */

    if(*board == "CS12800PX101"){
        port[0] = port[1];
        port[3] = port[2];
        port[4] = port[2];
        qDebug() << port[0]->portName();
        qDebug() << port[1]->portName();
        qDebug() << port[2]->portName();
        qDebug() << port[3]->portName();
        qDebug() << port[4]->portName();
    }

    if(*board == "CS12800R101P")
    {
        port[5] = port[4];
        port[4] = port[3];
        port[3] = port[2];
        port[2] = port[1];
        port[1] = port[5];
        qDebug() << *board + " New Serial Port list(thread): ";
        for(i=0; i<6; i++)
        {
            qDebug() << port[i]->portName();
        }
    }

    if(*board == "RK3568"){
        port[5] = port[0];
        port[0] = port[1];
        port[1] = port[5];
        port[5] = port[4];
        port[4] = port[3];
        port[3] = port[2];
        port[2] = port[1];
        port[1] = port[5];
        qDebug() << *board + " New Serial Port list(thread): ";
        for(i=0; i<6; i++)
        {
            qDebug() << port[i]->portName();
        }
    }

    if(*board == "RK3588"){
        port[5] = port[4];
        port[4] = port[0];
        port[0] = port[2];
        port[2] = port[5];
        qDebug() << *board + " New Serial Port list(thread): ";
        for(i=0; i<6; i++)
        {
            qDebug() << port[i]->portName();
        }
    }

    if(*board == "IMX8MP"){
        port[0] = port[2];
        port[2] = port[3];
        port[3] = port[1];
        port[1] = port[5];
        qDebug() << *board + " New Serial Port list(thread): ";
        for(i=0; i<6; i++)
        {
            qDebug() << port[i]->portName();
        }
    }

    if(*board == "STMP25"){
        port[4] = port[3];
        port[3] = port[1];
        port[1] = port[5];
        qDebug() << *board + " New Serial Port list(thread): ";
        for(i=0; i<6; i++)
        {
            qDebug() << port[i]->portName();
        }
    }
    if(*board != "CS10600RA070" && *board != "CS12800RA4101" && *board != "LRRA4-101" && *board != "CS12800RA4101A" && *board != "CS12800RA4101AV4" && *board != "CS12800RA5101A"){
        //CAN INIT
        if(*board == "CS12800R101P" || *board == "RK3568" || *board == "RK3588" || *board == "IMX8MP"){
            system("echo >/tmp/can0.txt");
            system("ip link set can0 down");
            system("ip link set can0 type can bitrate 100000 triple-sampling on loopback off ");
            system("ip link set can0 up");
        }else if (*board == "CS10600RA5070P") {
            system("echo >/tmp/can0.txt");
            system("ip link set can0 down");
            system("ip link set can0 type can bitrate 10000");
            system("ip link set can0 up");
        }else if(*board == "STMP25"){
            system("echo >/tmp/can0.txt");
            system("ip link set can0 down");
            system("ip link set can0 type can bitrate 100000");
            system("ip link set can0 up");
        }else{
            system("echo >/tmp/can0.txt");
            system("canconfig can0 stop");
            system("canconfig can0 bitrate 10000 ctrlmode triple-sampling on loopback off ");
            system("canconfig can0 start");
        }

        if(*board == "CS12800PX101"){
            system("sleep 5");
            system("canconfig can0 start");
        }
        system("candump can0 > /tmp/can0.txt &");
        if(*board != "CS12720RA4050" && *board != "CS10600RA4070" && *board != "CS10600RA4070D" && *board != "CS12800RA4101BOX" && *board != "CS12800RA4101P" && *board != "CS19108RA4133P" && *board != "CS10768RA4150P" && *board != "CS19108RA4156P" && *board != "CS19108RA4215P" && *board != "CS12800PX101" && *board !="CS12800R101P" && *board != "CS10600RA5070P" && *board != "CS19108RA4133PISO") {
            if(*board == "RK3568" || *board == "RK3588" || *board == "IMX8MP"){
                system("echo >/tmp/can1.txt");
                system("ip link set can1 down");
                system("ip link set can1 type can bitrate 100000 triple-sampling on loopback off ");
                system("ip link set can1 up");
            }else if(*board == "STMP25"){
                system("echo >/tmp/can1.txt");
                system("ip link set can1 down");
                system("ip link set can1 type can bitrate 100000");
                system("ip link set can1 up");
            }else{
                system("echo >/tmp/can1.txt");
                system("canconfig can1 stop");
                system("canconfig can1 bitrate 10000 ctrlmode triple-sampling on loopback off ");
                system("canconfig can1 start");
            }
            system("candump can1 > /tmp/can1.txt &");
        }
    }

    QString startteststr("@@STARTTEST\n");
    if(port[0]->isOpen())
        port[0]->close();
    if(port[0]->open(QIODevice::ReadWrite)) {
        port[0]->write(startteststr.toLatin1());
        if(port[0]->waitForReadyRead(1000)) {
            port[0]->close();
        }
    }

    while(!quit)
    {
        // Serial AutoTest
        for(i=0; i<5; i++)
        {
            if(*board == "CS10600RA070"){
                if(i==2){
                    i=1;
                    changeflag=true;
                }
            }

            if(i==0 && *board != "CS12800PX101"){
                timeout = 20000;
            }
            else{
                timeout = 1000;
            }
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
                if(*board == "CS10600RA070") {
                    //enable send
                    enablePi485Send();
                }
                port[i]->write(requestData);
                if(port[i]->waitForBytesWritten(1000)){
                    emit this->request(port[i]->portName() +"\n"+requestData);
                    if(*board == "CS10600RA070"){
                        //enable receive
                        enablePi485Receive();
                    }
                }
            }
//            strValue = tr("%1").arg(i);
//            qDebug() << "i = " +strValue;
            if(*board =="CS10600RA070" && changeflag){
                changeflag=false;
                break;
            }
        }

        if(*board !="CS10600RA070" && *board != "CS12800RA4101" && *board != "LRRA4-101" && *board != "CS12800RA4101A" && *board != "CS12800RA4101AV4" && *board != "CS12800RA5101A"){
            // CAN AutoTest
            // CAN0
            //qDebug() <<"test can";
            QFile file("/tmp/can0.txt");
            if (file.open(QIODevice::ReadWrite)){
                //qDebug() << "read can file";
                QTextStream in(&file);
                QString line=in.readLine(); // The First Line
                //qDebug() << "1" + line;
                if(*board !="CS12800R101P" && *board != "RK3568" && *board !="RK3588" && *board != "IMX8MP" && *board != "CS10600RA5070P" && *board != "STMP25")
                    line = in.readLine();   // The Second Line
                emit this->canrequest("can0\n"+line);
                //qDebug() << "2" + line;
                if(line.contains("11 22 33 44 55 66 77 88"))
                {
                    //qDebug() << "have 11223344";
                    if(*board == "CS12800R101P" || *board == "RK3568" || *board == "RK3588" || *board == "IMX8MP" || *board == "CS10600RA5070P" || *board == "STMP25"){
                        system("cansend can0 5A1#1122334455667788");
                        //qDebug() << "Cansend can0";
                    }else{
                        system("cansend can0 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88");
                    }
                }
            }
            file.close();

            // CAN1
            if(*board != "CS12720RA4050" && *board != "CS10600RA4070" && *board != "CS10600RA4070D" && *board != "CS12800RA4101BOX" && *board != "CS12800RA4101P" && *board != "CS19108RA4133P" && *board != "CS10768RA4150P" && *board != "CS19108RA4156P" && *board != "CS19108RA4215P" && *board != "CS12800PX101" && *board != "CS12800R101P" && *board != "CS10600RA5070P" && *board != "CS19108RA4133PISO") {
                QFile file1("/tmp/can1.txt");
                if (file1.open(QIODevice::ReadWrite)){
                    QTextStream in(&file1);
                    QString line=in.readLine(); // The First Line
                    //qDebug() << "1" + line;
                    if(*board != "RK3568" && *board != "RK3588" && *board != "IMX8MP" && *board != "STMP25")
                    	line = in.readLine();   // The Second Line
                    emit this->canrequest("can1\n"+line);
                    //qDebug() << "2" + line;
                    if(line.contains("11 22 33 44 55 66 77 88"))
                    {
                        //qDebug() << "have 11223344";
                        if(*board == "RK3568" || *board == "RK3588" || *board == "IMX8MP" || *board == "STMP25"){
                            system("cansend can1 5A1#1122334455667788");
                            //qDebug() << "Cansend can1";
                        }else{
                            system("cansend can1 0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88");
                        }
                    }
                }
                file1.close();
            }
        }

    } // while
} // run

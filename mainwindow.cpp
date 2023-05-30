#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timedialog.h"
#include <QDateTime>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include "version.h"
#include <QDesktopWidget>

/*
 * If you want to change the board,
 * 如果需要修改底板
 * Just modify the boardInit() function
 * 仅仅需要修改boardInit()函数即可。
 */

/* Common Define */
#define GPIOBASEPATH "/dev/chipsee-gpio"
#define GPIOEXPORTPATH "/sys/class/gpio/export"
#define CANSENDDATA "0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88"
#define CANSENDCANFRAMENEW "5A1#1122334455667788"
#define CANTEMPFILEPATH "/tmp/cantmp.txt"
#define SERIALTEMPFILEPATH "/tmp/serialtmp.txt"
#define TEMPFILEPATH "/tmp/tmp.txt"
#define TIMEOUT 2000
#define USBDEBUGFILEPATH "/sys/kernel/debug/usb/devices"
#define AUDIOPATH "/usr/hardwaretest/AudioTest.aac"
#define CUEAUDIOPATH "/usr/hardwaretest/AutoTestFinish.aac"
#define IPPATH "/usr/hardwaretest/ipaddr"
#define VIDEOPATH "/usr/hardwaretest/VideoTest.mp4"
#define BUZZERPATH "/dev/buzzer"
#define BACKLIGHTPATH "/sys/class/backlight/backlight/brightness"
#define MAXBACKLIGHTPATH "/sys/class/backlight/backlight/max_brightness"
#define DLANLOGFILE "/var/log/r8153bvb.log"

/* IMX6Q Define */
#define IMX6QLED4PATH "/sys/class/leds/led0/brightness"
#define IMX6QLED5PATH "/sys/class/leds/led1/brightness"
#define IMX6QAUDIOPATH "/usr/hardwaretest/AudioTest.aac"
#define IMX6QCUEAUDIOPATH "/usr/hardwaretest/AutoTestFinish.aac"
#define IMX6QVOLUMEPATH "name='PCM Playback Volume'"
/* 7 10 inch /sys/class/gpio/gpio80/value*/
#define IMX6QBUZZERPATH "/sys/class/gpio/gpio80/value"
#define IMX6QIPPATH "/usr/hardwaretest/ipaddr"
#define IMX6QVIDEOPATH "/usr/hardwaretest/VideoTest.mp4"
#define IMX6QBACKLIGHTPATH "/sys/class/backlight/backlight.33/brightness"
#define IMX6QMAXBACKLIGHTPATH "/sys/class/backlight/backlight.33/max_brightness"
#define IMX6QUSBDEBUGFILEPATH "/sys/kernel/debug/usb/devices"

/* IMX6D Define */
#define IMX6DLED0PATH "/sys/class/leds/led0/brightness"
#define IMX6DLED1PATH "/sys/class/leds/led1/brightness"
#define IMX6DAUDIOPATH "/usr/hardwaretest/AudioTest.aac"
#define IMX6DCUEAUDIOPATH "/usr/hardwaretest/AutoTestFinish.aac"
#define IMX6DVOLUMEPATH "name='PCM Playback Volume'"
/* 7 10 inch /sys/class/gpio/gpio80/value*/
#define IMX6DBUZZERPATH "/sys/class/gpio/gpio80/value"
#define IMX6DIPPATH "/usr/hardwaretest/ipaddr"
#define IMX6DVIDEOPATH "/usr/hardwaretest/VideoTest.mp4"
#define IMX6DBACKLIGHTPATH "/sys/class/backlight/backlight.31/brightness"
#define IMX6DMAXBACKLIGHTPATH "/sys/class/backlight/backlight.31/max_brightness"
#define IMX6DUSBDEBUGFILEPATH "/sys/kernel/debug/usb/devices"

/* IMX6U Define */
#define IMX6ULED0PATH "/sys/class/leds/Heartbeat/brightness"
#define IMX6ULED1PATH "/sys/class/leds/led-mmc1/brightness"
#define IMX6UAUDIOPATH "/usr/hardwaretest/AudioTest.aac"
#define IMX6UCUEAUDIOPATH "/usr/hardwaretest/AutoTestFinish.aac"
#define IMX6UVOLUMEPATH "name='Headphone Playback Volume'"
#define IMX6UBUZZERPATH "/dev/buzzer"
#define IMX6UIPPATH "/usr/hardwaretest/ipaddr"
#define IMX6UVIDEOPATH "/usr/hardwaretest/VideoTest.mp4"
#define IMX6UBACKLIGHTPATH "/sys/class/backlight/backlight.8/brightness"
#define IMX6UMAXBACKLIGHTPATH "/sys/class/backlight/backlight.8/max_brightness"
#define IMX6UUSBDEBUGFILEPATH "/sys/kernel/debug/usb/devices"

/* Industtrial Pi Define */
#define PILED0PATH "/sys/class/leds/led0/brightness"
#define PILED1PATH ""
#define PIAUDIOPATH "/usr/hardwaretest/AudioTest.wav"
#define PICUEAUDIOPATH "/usr/hardwaretest/AutoTestFinish.aac"
#define PIVOLUMEPATH "name='Master Playback Volume'"
#define PIBUZZERPATH "/dev/buzzer"
#define PIIPPATH "/usr/hardwaretest/ipaddr"
#define PIVIDEOPATH "/usr/hardwaretest/VideoTest.mp4"
#define PIUSBDEBUGFILEPATH "/sys/kernel/debug/usb/devices"
#define RS485ENIO "34"

/* AM335X Define*/
#define AM335XLED0PATH "/sys/class/leds/led\:yellow\:heartbeet/brightness"
#define AM335XLED1PATH ""
#define AM335XAUDIOPATH AUDIOPATH
#define AM335XCUEAUDIOPATH CUEAUDIOPATH
#define AM335XVOLUMEPATH "name='PCM Playback Volume'"
#define AM335XBUZZERPATH BUZZERPATH
#define AM335XIPPATH IPPATH
#define AM335XVIDEOPATH VIDEOPATH
#define AM335XBACKLIGHTPATH "/sys/class/backlight/backlight/brightness"
#define AM335XMAXBACKLIGHTPATH "/sys/class/backlight/backlight/max_brightness"
#define AM335XUSBDEBUGFILEPATH USBDEBUGFILEPATH

/* PX30 Devices*/
#define PX30LED0PATH "/sys/class/leds/work-led/brightness"
#define PX30LED1PATH ""
#define PX30AUDIOPATH AUDIOPATH
#define PX30CUEAUDIOPATH CUEAUDIOPATH
#define PX30VOLUMEPATH "name='PCM Playback Volume'"
#define PX30BUZZERPATH BUZZERPATH
#define PX30IPPATH IPPATH
#define PX30VIDEOPATH VIDEOPATH
#define PX30BACKLIGHTPATH "/sys/class/backlight/backlight/brightness"
#define PX30MAXBACKLIGHTPATH "/sys/class/backlight/backlight/max_brightness"
#define PX30USBDEBUGFILEPATH USBDEBUGFILEPATH

/* RK3399 Devices*/
#define RK3399LED0PATH "/sys/class/leds/work-led/brightness"
#define RK3399LED1PATH ""
#define RK3399AUDIOPATH AUDIOPATH
#define RK3399CUEAUDIOPATH CUEAUDIOPATH
#define RK3399VOLUMEPATH "name='HP Playback Volume'"
#define RK3399BUZZERPATH BUZZERPATH
#define RK3399IPPATH IPPATH
#define RK3399VIDEOPATH VIDEOPATH
#define RK3399BACKLIGHTPATH "/sys/class/backlight/backlight/brightness"
#define RK3399MAXBACKLIGHTPATH "/sys/class/backlight/backlight/max_brightness"
#define RK3399USBDEBUGFILEPATH USBDEBUGFILEPATH

/* RK3568 Devices*/
#define RK3568LED0PATH "/sys/class/leds/work/brightness"
#define RK3568LED1PATH ""
#define RK3568AUDIOPATH AUDIOPATH
#define RK3568CUEAUDIOPATH CUEAUDIOPATH
#define RK3568VOLUMEPATH "name='Master Playback Volume'"
#define RK3568BUZZERPATH BUZZERPATH
#define RK3568IPPATH IPPATH
#define RK3568VIDEOPATH VIDEOPATH
#define RK3568BACKLIGHTPATH "/sys/class/backlight/backlight/brightness"
#define RK3568MAXBACKLIGHTPATH "/sys/class/backlight/backlight/max_brightness"
#define RK3568USBDEBUGFILEPATH USBDEBUGFILEPATH

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    audioflag = true;
    autoflag = false;

    // boardInit
    boardInit();

    // DateTime
    dateTimeInit();

    // LED
    ledInit();

    // Audio
    audioInit();

    // Display
    displayInit();

    // Serial
    serialInit();

    // NetWork
    networkInit();

    // 4G
    mobile4gInit();

    // GPS
    gpsInit();

    // GPIO
    gpioInit();

    // CAN
    canInit();

    // Alltest for self test board
    alltestInit();

    // AutoTest, used for chipsee autotest
    //autotestInit();
}

MainWindow::~MainWindow()
{
    qDebug() << "I will gone";
    if(cpuplat == "pi"){
        QString cmdstr = "cp /usr/bin/vlc.bak /usr/bin/vlc && sync";
        system(cmdstr.toLocal8Bit());
    }
    delete ui;
    qDebug() << "I had gone";
}

/*
 *  Global Function
 *
 *  GetTempFileValue() GetFileValue() GetComResult() GetPlat()
 *
 */

QString MainWindow::GetTempFileValue()
{
    QString tmpath = TEMPFILEPATH;
    QFile file(tmpath);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream in(&file);
        QString line =in.readAll();
        return line;
    } else {
        QMessageBox::critical(this,"Error","Open config file error!!");
        return NULL;
    }

    file.close();
}

QString MainWindow::GetFileValue(QString filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line =in.readAll();
        return line;
    } else
        return NULL;

    file.close();
}

QString MainWindow::GetComResult(QString cmd)
{
    QProcess process;
    process.start(cmd);
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();
    QString result = output;
    return result;
}

QString MainWindow::GetPlat()
{
    QString plat;
    QString cpucore = GetComResult("grep -c processor /proc/cpuinfo");
    QString imx6qdlul = GetComResult("grep -c Freescale /proc/cpuinfo");
    QString pi3 = GetComResult("grep -c BCM2835 /proc/cpuinfo");
    QString pi4 = GetComResult("grep -c BCM2711 /proc/cpuinfo");
    QString am335x = GetComResult("grep -c AM33XX /proc/cpuinfo");
    QString px30 = GetComResult("grep -c px30 /proc/device-tree/compatible");
    QString rk3399 = GetComResult("grep -c rk3399 /proc/device-tree/compatible");
    QString rk3568 = GetComResult("grep -c rk3568 /proc/device-tree/compatible");
//    qDebug() << cpucore.left(1);
//    qDebug() << imx6qdlul.left(1);

    if(imx6qdlul.left(1) == "1"){
        if(cpucore.left(1) == "4"){
            plat = "imx6q";
        } else if (cpucore.left(1) == "2"){
            plat = "imx6dl";
        } else if (cpucore.left(1) == "1"){
            plat = "imx6ul";
        }
    } else if(pi3.left(1) == "1" || pi4.left(1) == "1"){
        if(cpucore.left(1) == "4"){
            plat = "pi";
        }
    } else if(am335x.left(1) == "1"){
        plat= "am335x";
    } else if(px30.left(1) == "2") {
        plat = "px30";
    } else if(rk3399.left(1) == "2") {
        plat = "rk3399";
    } else if(rk3568.left(1) == "2"){
        plat = "rk3568";
    }

    return plat;
}
QString MainWindow::GetResolution()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    int width = desktopWidget->width();
    int height = desktopWidget->height();
    QString widthstr = QString::number(width,10);
    QString heightstr = QString::number(height,10);
    return widthstr+heightstr;
}

QString MainWindow::GetPiBoard()
{
    QString board;
    if(GetPlat() == "pi" || GetPlat() == "px30")
    {
        board = GetFileValue("/opt/chipsee/.board");
        // Remove "\n" from board
        board.remove(QChar('\n'),Qt::CaseInsensitive);
    } else
    {
        board = "NULL";
    }

    return board;

}

QString MainWindow::GetBoard()
{
    QString board;
    if(GetPlat() == "pi" || GetPlat() == "px30")
    {
        board = GetFileValue("/opt/chipsee/.board");
        // Remove "\n" from board
        board.remove(QChar('\n'),Qt::CaseInsensitive);
    } else
    {
        board = "NULL";
    }

    if(GetPlat() == "rk3399")
    {
        QString cs10600r070 = GetComResult("grep -c rk3399-eisd-1024600-mipi /proc/device-tree/compatible");
        QString cs12800r101 = GetComResult("grep -c rk3399-eisd-1280800-lvds-linux /proc/device-tree/compatible");
        QString cs12800r101p = GetComResult("grep -c pwm1 /proc/device-tree/compatible");
        if(cs10600r070.left(1) == "1"){
            board = "CS10600R070";
        } else if(cs12800r101.left(1) == "1"){
            board = "CS12800R101";
        } else if(cs12800r101p.left(1) == "1"){
            board = "CS12800R101P";
        } else
            board = "CS40230RB";
    }
    if(GetPlat() == "rk3568")
    {
        QString CS12800_RK3568_101 = GetComResult("grep -c rk3568-eisd-1280800 /proc/device-tree/compatible");
        QString CS10600_RK3568_070 = GetComResult("grep -c rk3568-eisd-1024600 /proc/device-tree/compatible");
        QString CS12720_RK3568_050 = GetComResult("grep -c rk3568-eisd-1280720 /proc/device-tree/compatible");
        if(CS12800_RK3568_101.left(1) == "1"){
            board = "CS12800_RK3568_101";
        } else if(CS10600_RK3568_070.left(1) == "1"){
            board = "CS10600_RK3568_070";
        } else if(CS12720_RK3568_050.left(1) == "1"){
            board = "CS12720_RK3568_050";
        }
    }

    return board;

}

QString MainWindow::GetDebianCodeName()
{
    QString codename="NULL";
    if(GetPlat() == "pi"){
        codename = GetComResult("lsb_release -c");
        if(codename.contains("bullseye"))
            codename = "bullseye";
    }
    if(GetPlat() == "rk3568"){
        codename = GetComResult("lsb_release -c");
        if(codename.contains("bullseye"))
            codename = "bullseye";
    }
    if(GetPlat() == "imx6q"){
        codename = GetComResult("lsb_release -sc");
        if(codename.contains("bionic"))
            codename = "bionic";
    }
    //qDebug() << "CodeName is: " + codename;
    return codename;
}

void MainWindow::Delay_MSec_Suspend(int msec)
{

    QTime _Timer = QTime::currentTime();

    QTime _NowTimer;
    do{
        _NowTimer=QTime::currentTime();
    }while (_Timer.msecsTo(_NowTimer)<=msec);
}

/*
 *  Board
 *
 *  BoardSetting() boardInit()
 *
 */

void MainWindow::BoardSetting()
{
    if(GetPlat() == "imx6q"){
        board = "imx6q";
        ledpath = IMX6QLED4PATH;
        ledpath2 = IMX6QLED5PATH;
        audiopath = IMX6QAUDIOPATH;
        cueaudiopath = IMX6QCUEAUDIOPATH;
        volumepath = IMX6QVOLUMEPATH;
        buzzerpath = IMX6QBUZZERPATH;
        videopath = IMX6QVIDEOPATH;
        backlightpath = IMX6QBACKLIGHTPATH;
        maxbacklightpath = IMX6QMAXBACKLIGHTPATH;
        ipaddrpath = IMX6QIPPATH;
        gpioOutArray[0] = "1";
        gpioOutArray[1] = "2";
        gpioOutArray[2] = "3";
        gpioOutArray[3] = "4";
        gpioInArray[0] = "5";
        gpioInArray[1] = "6";
        gpioInArray[2] = "7";
        gpioInArray[3] = "8";
    }else if(GetPlat() == "imx6dl"){
        board = "imx6d";
        ledpath = IMX6DLED0PATH;
        ledpath2 = IMX6DLED1PATH;
        audiopath = IMX6DAUDIOPATH;
        cueaudiopath = IMX6DCUEAUDIOPATH;
        volumepath = IMX6DVOLUMEPATH;
        buzzerpath = IMX6DBUZZERPATH;
        videopath = IMX6DVIDEOPATH;
        backlightpath = IMX6DBACKLIGHTPATH;
        maxbacklightpath = IMX6DMAXBACKLIGHTPATH;
        ipaddrpath = IMX6DIPPATH;
        gpioOutArray[0] = "1";
        gpioOutArray[1] = "2";
        gpioOutArray[2] = "3";
        gpioOutArray[3] = "4";
        gpioInArray[0] = "5";
        gpioInArray[1] = "6";
        gpioInArray[2] = "7";
        gpioInArray[3] = "8";
    }else if(GetPlat() == "imx6ul"){
        board = "imx6u";
        ledpath = IMX6ULED0PATH;
        ledpath2 = IMX6ULED1PATH;
        audiopath = IMX6UAUDIOPATH;
        cueaudiopath = IMX6UCUEAUDIOPATH;
        volumepath = IMX6UVOLUMEPATH;
        buzzerpath = IMX6UBUZZERPATH;
        videopath = IMX6UVIDEOPATH;
        backlightpath = IMX6UBACKLIGHTPATH;
        maxbacklightpath = IMX6UMAXBACKLIGHTPATH;
        ipaddrpath = IMX6UIPPATH;
        gpioOutArray[0] = "1";
        gpioOutArray[1] = "2";
        gpioOutArray[2] = "3";
        gpioOutArray[3] = "4";
        gpioInArray[0] = "5";
        gpioInArray[1] = "6";
        gpioInArray[2] = "7";
        gpioInArray[3] = "8";
    }else if(GetPlat() == "am335x"){
        board = "AM335XBOARD";
        ledpath = AM335XLED0PATH;
        ledpath2 = AM335XLED1PATH;
        audiopath = AM335XAUDIOPATH;
        cueaudiopath = AM335XCUEAUDIOPATH;
        volumepath = AM335XVOLUMEPATH;
        buzzerpath = AM335XBUZZERPATH;
        videopath = AM335XVIDEOPATH;
        backlightpath = AM335XBACKLIGHTPATH;
        maxbacklightpath = AM335XMAXBACKLIGHTPATH;
        ipaddrpath = AM335XIPPATH;
        gpioOutArray[0] = "1";
        gpioOutArray[1] = "2";
        gpioOutArray[2] = "3";
        gpioOutArray[3] = "4";
        gpioInArray[0] = "5";
        gpioInArray[1] = "6";
        gpioInArray[2] = "7";
        gpioInArray[3] = "8";
    }else if(GetPlat() == "bbbexp"){
        board = "bbbexp";
    }else if(GetPlat() == "pi"){
        board = GetPiBoard();
        ledpath = PILED0PATH;
        ledpath2 = PILED1PATH;
        audiopath = PIAUDIOPATH;
        cueaudiopath = PICUEAUDIOPATH;
        volumepath = PIVOLUMEPATH;
        buzzerpath = PIBUZZERPATH;
        videopath = PIVIDEOPATH;
        ipaddrpath = PIIPPATH;
        gpioOutArray[0] = "1";
        gpioOutArray[1] = "2";
        gpioOutArray[2] = "3";
        gpioOutArray[3] = "4";
        gpioInArray[0] = "5";
        gpioInArray[1] = "6";
        gpioInArray[2] = "7";
        gpioInArray[3] = "8";
    }else if(GetPlat() == "px30"){
        board = GetPiBoard();
        ledpath = PX30LED0PATH;
        ledpath2 = PX30LED1PATH;
        audiopath = PX30AUDIOPATH;
        cueaudiopath = PX30CUEAUDIOPATH;
        volumepath = PX30VOLUMEPATH;
        buzzerpath = PX30BUZZERPATH;
        backlightpath = PX30BACKLIGHTPATH;
        maxbacklightpath = PX30MAXBACKLIGHTPATH;
        videopath = PX30VIDEOPATH;
        ipaddrpath = PX30IPPATH;
        gpioOutArray[0] = "1";
        gpioOutArray[1] = "2";
        gpioOutArray[2] = "3";
        gpioOutArray[3] = "4";
        gpioInArray[0] = "5";
        gpioInArray[1] = "6";
        gpioInArray[2] = "7";
        gpioInArray[3] = "8";
    }else if(GetPlat() == "rk3568"){
        board = GetBoard();
        ledpath = RK3568LED0PATH;
        ledpath2 = RK3568LED1PATH;
        audiopath = RK3568AUDIOPATH;
        cueaudiopath = RK3568CUEAUDIOPATH;
        volumepath = RK3568VOLUMEPATH;
        buzzerpath = RK3568BUZZERPATH;
        backlightpath = RK3568BACKLIGHTPATH;
        maxbacklightpath = RK3568MAXBACKLIGHTPATH;
        videopath = RK3568VIDEOPATH;
        ipaddrpath = RK3568IPPATH;
        gpioOutArray[0] = "1";
        gpioOutArray[1] = "2";
        gpioOutArray[2] = "3";
        gpioOutArray[3] = "4";
        gpioInArray[0] = "5";
        gpioInArray[1] = "6";
        gpioInArray[2] = "7";
        gpioInArray[3] = "8";
    }
}

void MainWindow::boardInit()
{
    cpuplat = GetPlat();

    //qDebug() << cpuplat;

    if (cpuplat == "imx6q" || cpuplat =="imx6dl" || cpuplat == "imx6ul") {
        ui->labelBoard->setText("IMX6QDLUL");
    } else if (cpuplat == "pi"){

        /*change vlc to allow run for root*/
        QString cmdstr = "cp /usr/bin/vlc /usr/bin/vlc.bak && sync";
        system(cmdstr.toLocal8Bit());
        cmdstr = "chmod a+w /usr/bin/vlc && sed 's/geteuid/getppid/g' /usr/bin/vlc > /tmp/vlc && cp /tmp/vlc /usr/bin/vlc";
        system(cmdstr.toLocal8Bit());

        ui->labelBoard->setText(GetPiBoard());

#if 0
        // Backlight init
        if(GetPiBoard() == "CS10600RA070" || GetPiBoard() == "CS12800RA101") {
            gpioExport("41");
            setGPIOModelRaw("41","out");
            setGPIOValueRaw("41","1");
            backlightpath = "/sys/class/gpio/gpio41/value";
            maxbacklightpath = "/sys/class/gpio/gpio41/value";
        }

        if(GetPiBoard() == "CS10600RA4070") {
            gpioExport("18");
            setGPIOModelRaw("18","out");
            setGPIOValueRaw("18","1");
            backlightpath = "/sys/class/gpio/gpio18/value";
            maxbacklightpath = "/sys/class/gpio/gpio18/value";
        }

        if(GetPiBoard() == "CS12800RA4101" || GetPiBoard() == "LRRA4-101" || GetPiBoard() == "CS12800RA4101A") {
            gpioExport("13");
            setGPIOModelRaw("13","out");
            setGPIOValueRaw("13","1");
            backlightpath = "/sys/class/gpio/gpio13/value";
            maxbacklightpath = "/sys/class/gpio/gpio13/value";
            gpioExport("17");
            setGPIOModelRaw("17","out");
            setGPIOValueRaw("17","0");
            relaypath = "/sys/class/gpio/gpio17/value";
        }
#endif
	// Relay init
        if(GetPiBoard() == "CS12800RA4101" || GetPiBoard() == "LRRA4-101" || GetPiBoard() == "CS12800RA4101A") {
            gpioExport("17");
            setGPIOModelRaw("17","out");
            setGPIOValueRaw("17","0");
            relaypath = "/sys/class/gpio/gpio17/value";
        }

        // use new backlight path
        backlightpath = "/sys/class/backlight/pwm-backlight/brightness";
        maxbacklightpath = "/sys/class/backlight/pwm-backlight/max_brightness";

    } else if(cpuplat == "am335x"){
        ui->labelBoard->setText("AM335XBOARD");
    } else if(cpuplat == "px30") {
        ui->labelBoard->setText(GetBoard());
        relaypath = "/dev/relay";
    } else {
        ui->labelBoard->setText(GetBoard());
    }


    ui->labelVersion->setText(VERSION);

    BoardSetting();
    connect(ui->pushButton_exit,&QPushButton::clicked,this,&MainWindow::close);
}

/*
 *  DateTime
 *
 *  dateTimeInit() SetCurrentTime() syncTime()
 *
 */

void MainWindow::SetCurrentTime()
{
    ui->label_currentDateTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::syncTime()
{
    QMessageBox::warning(this,"Tips","Connect to Internet, Set the time closer to now time manual first.");
    QString cmdstr = "ntpdate-sync";
    system(cmdstr.toLocal8Bit());
    cmdstr = "ntpdate-debian";
    system(cmdstr.toLocal8Bit());
    QMessageBox::warning(this,"Tips","If the time not update, don't worry, press ok to Wait Complete.");
}

void MainWindow::dateTimeInit()
{
    timeset = new timedialog;
    SetCurrentTime();
    setTimeTimer= new QTimer(this);
    connect(setTimeTimer,SIGNAL(timeout()),this,SLOT(SetCurrentTime()));
    setTimeTimer->start(1000);
    connect(ui->pushButton_timeSet,&QPushButton::clicked,timeset,&timedialog::ShowCurrentTime);
    connect(ui->pushButton_timeSync,&QPushButton::clicked,this,&MainWindow::syncTime);
    if(cpuplat == "pi" || cpuplat == "am335x" || cpuplat == "px30"){
        ui->pushButton_timeSync->setVisible(false);
    }
}

/*
 *  LED
 *
 *  OpenLed() OpenLed2() CloseLed() CloseLed2() ledInit()
 *
 */
void MainWindow::OpenLed()
{
    QString cmdstr = "echo 1 >"+ledpath+"&";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::CloseLed()
{
    QString cmdstr = "echo 0 >"+ledpath+"&";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::OpenLed2()
{
    QString cmdstr = "echo 1 >"+ledpath2+"&";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::CloseLed2()
{
    QString cmdstr = "echo 0 >"+ledpath2+"&";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::ledInit()
{
    QString cmdstr = "chmod 777 " + ledpath+"&";
    system(cmdstr.toLocal8Bit());
    ui->radioButton_led1_off->setChecked(true);
    ui->radioButton_led2_on->setVisible(false);
    ui->radioButton_led2_off->setVisible(false);
    ui->label_led2->setVisible(false);
    connect(ui->radioButton_led1_on,&QRadioButton::clicked,this,&MainWindow::OpenLed);
    connect(ui->radioButton_led1_off,&QRadioButton::clicked,this,&MainWindow::CloseLed);
    if(board == "imx6q" || board == "imx6d" || board =="imx6u"){
        QString cmdstr2 = "chmod 777 " + ledpath2+"&";
        system(cmdstr2.toLocal8Bit());
        ui->label_led2->setVisible(true);
        ui->radioButton_led2_on->setVisible(true);
        ui->radioButton_led2_off->setVisible(true);
        ui->radioButton_led2_off->setChecked(true);
        connect(ui->radioButton_led2_on,&QRadioButton::clicked,this,&MainWindow::OpenLed2);
        connect(ui->radioButton_led2_off,&QRadioButton::clicked,this,&MainWindow::CloseLed2);
    }else if (board == "bbbexp"){}
}
/*
 *  Audio
 *
 *  AudioLoop() AudioTest() RecordTest() CueAudio() ChangeVolume() EnableBuzzer() DisableBuzzer() ChangeBuzzerState() audioInit()
 *
 *
 */

void MainWindow::AudioLoop()
{
    if(ui->checkBox_audioloop->isChecked()){
	autoflag=true;
        AudioTest();
        audioloopTimer->start(20000);
    }
    else {
	autoflag=false;
        audioloopTimer->stop();
        system("killall gst-play-1.0");
    }
}

void MainWindow::AudioTest()
{
    if(!autoflag){
	QMessageBox::information(this,"Tips","Press OK to Play Audio!");
    }
    QString cmdstr = "";
    if(cpuplat == "pi") {
        system("killall vlc");
        cmdstr = "cvlc "+ audiopath + " vlc://quit" +"&";
    }else if(cpuplat == "px30"){
        system("killall -9 gst-play-1.0");
        cmdstr = "pactl set-sink-port 1 '[Out] Headphone' && pactl set-sink-port 1 '[Out] Speaker' >/dev/null &";
        system(cmdstr.toLocal8Bit());
        cmdstr = "gst-play-1.0 "+audiopath+" >/dev/null &";
    }else{
        system("killall gst-play-1.0");
        cmdstr = "gst-play-1.0 "+audiopath+" >/dev/null &";
    }
    system(cmdstr.toLocal8Bit());
}

void MainWindow::RecordTest()
{
    if(!autoflag){
        QMessageBox::warning(this,"Tips","Open Outside Audio First!!");
    }

    if(board == "imx6q" && GetDebianCodeName() != "bionic"){
        system("killall gst-play-1.0");
        system("rm /usr/hardwaretest/output.mp3");
        QString cmdstr = "amixer cset numid=7,iface=MIXER,name='Mic Volume' 3 > /dev/null && "
                         "gst-launch-1.0 alsasrc num-buffers=430 blocksize=4096 ! imxmp3enc ! filesink location=/usr/hardwaretest/output.mp3 &";

        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(16000, &eventloop,SLOT(quit()));
        eventloop.exec();

        system("killall gst-play-1.0");
        system("gst-play-1.0 /usr/hardwaretest/output.mp3 >/dev/null &");
    }else if(board == "imx6d" || board == "imx6u") {
        system("rm /usr/hardwaretest/output.wav");
        QString cmdstr = "amixer cset numid=7,iface=MIXER,name='Mic Volume' 3 > /dev/null && "
                         "arecord -N -M -r 44100 -f S16_LE -c 2 -d 18 /usr/hardwaretest/output.wav &";

        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(16000, &eventloop,SLOT(quit()));
        eventloop.exec();

        system("aplay /usr/hardwaretest/output.wav &");

    }else if(cpuplat == "pi"){
        system("test -f /usr/hardwaretest/output.wav && rm /usr/hardwaretest/output.wav");
        QString cmdstr = "arecord -f cd -d 18 /usr/hardwaretest/output.wav & ";
        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(16000, &eventloop,SLOT(quit()));
        eventloop.exec();

        system("aplay /usr/hardwaretest/output.wav &");
    }else if(cpuplat == "px30"){
        system("test -f /usr/hardwaretest/output.wav && rm /usr/hardwaretest/output.wav");
        QString cmdstr = "arecord -f cd -d 18 /usr/hardwaretest/output.wav & ";
        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(16000, &eventloop,SLOT(quit()));
        eventloop.exec();

        cmdstr = "pactl set-sink-port 1 '[Out] Headphone' && pactl set-sink-port 1 '[Out] Speaker' >/dev/null &";
        system(cmdstr.toLocal8Bit());

        system("aplay /usr/hardwaretest/output.wav &");
    }else if(board == "AM335XBOARD"){   //need check again
        system("killall gst-play-1.0");
        system("rm /usr/hardwaretest/output.wav");
        QString cmdstr = "gst-launch-1.0 alsasrc device=\"default\" num-buffers=1600 ! wavenc ! filesink location=/usr/hardwaretest/output.wav &";

        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(18000, &eventloop,SLOT(quit()));
        eventloop.exec();

        system("killall gst-play-1.0");
        system("gst-play-1.0 /usr/hardwaretest/output.wav >/dev/null &");
    }else if(cpuplat == "rk3399" || cpuplat == "rk3568"){   //need check again
        system("killall gst-play-1.0");
        system("rm /usr/hardwaretest/output.wav");
        QString cmdstr = "arecord -f cd -c 1 -d 18 /usr/hardwaretest/output.wav & ";

        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(18000, &eventloop,SLOT(quit()));
        eventloop.exec();

        system("killall gst-play-1.0");
        system("gst-play-1.0 /usr/hardwaretest/output.wav >/dev/null &");
    }else if(board == "imx6q" && GetDebianCodeName() == "bionic") {
        system("test -f /usr/hardwaretest/output.wav && rm /usr/hardwaretest/output.wav");
        QString cmdstr = "arecord -f cd -d 18 /usr/hardwaretest/output.wav & ";
        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(16000, &eventloop,SLOT(quit()));
        eventloop.exec();

        system("aplay /usr/hardwaretest/output.wav &");
    }
}

void MainWindow::CueAudio()
{
    if(audioflag) {
        audioflag = false;
        QString cmdstr = "";
        if(cpuplat == "pi") {
            cmdstr = "cvlc "+ cueaudiopath + " vlc://quit" + " &";
            //cmdstr = "aplay " + cueaudiopath + " >/dev/null &";
        }else if(cpuplat == "px30"){
            system("killall -9 gst-play-1.0");
            cmdstr = "pactl set-sink-port 1 '[Out] Headphone' && pactl set-sink-port 1 '[Out] Speaker' >/dev/null &";
            system(cmdstr.toLocal8Bit());
            cmdstr = "gst-play-1.0 "+cueaudiopath+" >/dev/null &";
        }else{
            system("killall gst-play-1.0");
            cmdstr = "gst-play-1.0 "+cueaudiopath+" >/dev/null &";
        }
        system(cmdstr.toLocal8Bit());

        QEventLoop eventloop;
        QTimer::singleShot(5000, &eventloop,SLOT(quit()));
        eventloop.exec();

        audioflag = true;
    }
    qDebug() << "CueAudio!";
}

void MainWindow::ChangeVolume()
{
    int volumevalue = ui->horizontalSlider_audio_volume->value();
    QString value = QString::number(volumevalue,10); // int to string
    QString cmdstr = "";
    if(cpuplat == "pi"){
        if(GetDebianCodeName()=="bullseye")
            cmdstr = "amixer cset "+volumepath+" "+value+"% &";
        else
            cmdstr = "pactl set-sink-volume 0 "+value+"% &";
        qDebug()<<"STR:"+cmdstr;
    }else if(cpuplat =="px30"){
        cmdstr = "pactl set-sink-volume 1 "+value+"% &";
    }else if(cpuplat =="rk3399"){
        cmdstr = "amixer cset "+volumepath+" "+value+"% &";
    }else if(cpuplat == "rk3568") {
        cmdstr = "amixer cset "+volumepath+" "+value+"% "+value+"% &";
    }else if(board == "imx6q" && GetDebianCodeName() == "bionic") {
        cmdstr = "pactl set-sink-volume @DEFAULT_SINK@ "+value+"% &";
    }else{
        cmdstr = "amixer cset "+volumepath+" "+value+"&";
    }
    system(cmdstr.toLocal8Bit()); // int to const char*
}

void MainWindow::EnableBuzzer()
{
    QString cmdstr = "echo 1 >"+buzzerpath+"&";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::DisableBuzzer()
{
    QString cmdstr = "echo 0 >"+buzzerpath+"&";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::ChangeBuzzerState()
{
    if(buzzerflag)
    {
        EnableBuzzer();
        buzzerflag=!buzzerflag;
    }
    else
    {
        DisableBuzzer();
        buzzerflag=!buzzerflag;
    }
}

void MainWindow::audioInit()
{
    QString cmdstr;
    buzzerflag = true;
    connect(ui->pushButton_audio,&QPushButton::clicked,this,&MainWindow::AudioTest);
    connect(ui->pushButton_audio,&QPushButton::clicked,this,&MainWindow::ChangeVolume);
    connect(ui->pushButton_record,&QPushButton::clicked,this,&MainWindow::RecordTest);
    connect(ui->horizontalSlider_audio_volume,&QSlider::valueChanged,this,&MainWindow::ChangeVolume);
    if(board == "imx6q" || board == "imx6d" || board == "AM335XBOARD"){
        ui->horizontalSlider_audio_volume->setRange(60,127);
        ui->horizontalSlider_audio_volume->setValue(127);
    }else if(board == "imx6u"){
        ui->horizontalSlider_audio_volume->setRange(60,127);
        ui->horizontalSlider_audio_volume->setValue(127);
    }else if (board == "bbbexp"){
    }else if (cpuplat == "pi"){
        ui->horizontalSlider_audio_volume->setRange(0,100);
        ui->horizontalSlider_audio_volume->setValue(50);
        if(GetDebianCodeName() == "bullseye"){
            //cmdstr = "amixer cset "+volumepath+" 50%";
            ui->horizontalSlider_audio_volume->setVisible(false);
            ui->label_audio_volume->setVisible(false);
        }
        else
            cmdstr = "pactl set-sink-mute 0 false; pactl set-sink-volume 0 50%";
        system(cmdstr.toLocal8Bit());
	if(board != "LRRA4-101" && board != "CS12800RA4101A" ) {
		ui->pushButton_record->setVisible(false);
	}
    }else if (cpuplat == "px30" || cpuplat == "rk3568"){
        ui->horizontalSlider_audio_volume->setRange(0,100);
        ui->horizontalSlider_audio_volume->setValue(100);
    }else if (cpuplat == "rk3399"){
        ui->horizontalSlider_audio_volume->setRange(0,100);
        ui->horizontalSlider_audio_volume->setValue(70);
    }

    if(cpuplat == "rk3568" && GetDebianCodeName() != "bullseye"){
	ui->horizontalSlider_audio_volume->setVisible(false);
	ui->label_audio_volume->setVisible(false);
    }

    if (board == "imx6q" && GetDebianCodeName() == "bionic"){
        ui->horizontalSlider_audio_volume->setRange(0,100);
        ui->horizontalSlider_audio_volume->setValue(100);
    }

    ui->checkBox_buzzer->setChecked(false);
    connect(ui->checkBox_buzzer,SIGNAL(toggled(bool)),this,SLOT(ChangeBuzzerState()));

    audioloopTimer = new QTimer(this);
    audioloopTimer->stop();
    ui->checkBox_audioloop->setChecked(false);
    connect(audioloopTimer,SIGNAL(timeout()),SLOT(AudioTest()));
    connect(ui->checkBox_audioloop,SIGNAL(toggled(bool)),this,SLOT(AudioLoop()));
}

/*
 *  Display
 *
 *  VideoTest() ChangeBacklight() MaxBacklightValue() GetBacklightValue() LCDTest() TouchTest() displayInit()
 *
 */

void MainWindow::VideoTest()
{
    if(board == "imx6u"){
        QMessageBox::critical(this,"ERROR","IMX6U is Can't play video well!!");
        return;
    }
    QMessageBox::warning(this,"Tips","Press OK to Play Video!");
    QString cmdstr="";
    if(cpuplat == "pi") {
        cmdstr = "cvlc "+ videopath + " vlc://quit" +"&";
    } else if(cpuplat == "px30"){
        system("killall -9 gst-play-1.0");
        cmdstr = "pactl set-sink-port 1 '[Out] Headphone' && pactl set-sink-port 1 '[Out] Speaker' >/dev/null &";
        system(cmdstr.toLocal8Bit());
        cmdstr = "gst-play-1.0 "+videopath+"&";
    }else if(board == "AM335XBOARD"){
        cmdstr = "runMpeg4AacDec.sh &";
    }else{
        system("killall gst-play-1.0");
        cmdstr = "gst-play-1.0 "+videopath+"&";
    }
    system(cmdstr.toLocal8Bit());
}

void MainWindow::ChangeBacklight()
{
    int backlightvalue = ui->horizontalSlider_backlight->value();
    if(backlightvalue < 5) // disable close backlight
        backlightvalue = 5;
    QString value = QString::number(backlightvalue,10); // int to string
#if 0
    if(cpuplat == "pi" && value == "0"){
        QMessageBox::warning(this,"Tips","If you trun off the backlight of LCD, It will back after 5 seconds!");
    }
#endif
    QString cmdstr = "echo "+value+" >"+backlightpath+"&";
    system(cmdstr.toLocal8Bit()); // int to const char*
#if 0
    if(cpuplat == "pi"){
        if (value == "0"){
            Delay_MSec_Suspend(5000);
            cmdstr = "echo 1 >"+backlightpath+"&";
            system(cmdstr.toLocal8Bit()); // int to const char*
            ui->horizontalSlider_backlight->setValue(1);
        }

    }
#endif
}

int MainWindow::MaxBacklighValue()
{
#if 0
    if(cpuplat == "pi")
        return 1;
#endif
    if( GetFileValue(maxbacklightpath) == NULL) {
        maxbacklightpath = "/dev/max_brightness";
    }

    QString cmdstr = "cat "+maxbacklightpath+" >"+TEMPFILEPATH;
    system(cmdstr.toLocal8Bit());
    if(GetFileValue(TEMPFILEPATH)==NULL){
        QMessageBox::critical(this,"Error","Open TempPath Error!! Don't Get MaxBacklighValue!! Return 0!!");
        return 0;
    }else
        return GetFileValue(TEMPFILEPATH).toInt();
}

int MainWindow::GetBacklightValue()
{
    if(GetFileValue(backlightpath)==NULL){
        backlightpath = "/dev/brightness";
    }

    if(GetFileValue(backlightpath)==NULL){
        QMessageBox::critical(this,"Error","Open Backlightpath Error!! Don't Get BacklightValue!! Return 0!!");
        return 0;
    }else
        return GetFileValue(backlightpath).toInt();
}

void MainWindow::LCDTest()
{
    system("/usr/bin/LCDTester");
}

void MainWindow::TouchTest()
{
    system("/usr/bin/ts_test > /dev/null"); //cross-compile tslib https://github.com/libts/tslib
}

void MainWindow::displayInit()
{
    connect(ui->pushButton_video,&QPushButton::clicked,this,&MainWindow::VideoTest);
    connect(ui->pushButton_lcdtest,&QPushButton::clicked,this,&MainWindow::LCDTest);
    //connect(ui->pushButton_touchtest,&QPushButton::clicked,this,&MainWindow::TouchTest);
    connect(ui->pushButton_video,&QPushButton::clicked,this,&MainWindow::ChangeVolume);

    ui->horizontalSlider_backlight->setRange(1,MaxBacklighValue());
    ui->horizontalSlider_backlight->setValue(GetBacklightValue());
#if 0
    if(cpuplat == "pi"){
        ui->horizontalSlider_backlight->setRange(0,1);
        ui->horizontalSlider_backlight->setValue(1);
    }
#endif

    if(cpuplat == "am335x"){
        ui->pushButton_video->setVisible(false);
    }

    connect(ui->horizontalSlider_backlight,&QSlider::valueChanged,this,&MainWindow::ChangeBacklight);
}

/*
 *
 *  Serial
 *
 *  fillPortsParameters() checkCustomBaudRatePolicy(int idx) checkCustomDevicePathPolicy(int idx) updateSettings() openSerialPort() closeSerialPort()
 *
 *  clearSerialText() readDate() serialInit()
 *
 *
 */

void MainWindow::fillPortsParameters()
{
    ui->comboBox_com->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        list<<info.portName();
        ui->comboBox_com->addItem(list.first(),list);
    }
    ui->comboBox_com->addItem(tr("Custom"));
    ui->comboBox_com->setCurrentIndex(0);

    ui->comboBox_baud->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baud->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baud->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baud->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBox_baud->addItem(tr("Custom"));
    ui->comboBox_baud->setCurrentIndex(3);

    ui->comboBox_dataBit->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_dataBit->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_dataBit->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_dataBit->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_dataBit->setCurrentIndex(3);

    ui->comboBox_checkBit->addItem(tr("None"), QSerialPort::NoParity);
    ui->comboBox_checkBit->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->comboBox_checkBit->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->comboBox_checkBit->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->comboBox_checkBit->addItem(tr("Space"), QSerialPort::SpaceParity);
    ui->comboBox_checkBit->setCurrentIndex(0);

    ui->comboBox_stopBit->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->comboBox_stopBit->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    ui->comboBox_stopBit->setCurrentIndex(0);

    ui->comboBox_flow->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->comboBox_flow->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->comboBox_flow->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
    ui->comboBox_flow->setCurrentIndex(0);
}

void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->comboBox_baud->itemData(idx).isValid();
    ui->comboBox_baud->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->comboBox_baud->clearEditText();
        QLineEdit *edit = ui->comboBox_baud->lineEdit();
        edit->setValidator(intValidator);
    }
}

void MainWindow::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->comboBox_com->itemData(idx).isValid();
    ui->comboBox_com->setEditable(isCustomPath);
    if (isCustomPath)
        ui->comboBox_com->clearEditText();
}

void MainWindow::updateSettings()
{
    currentSettings.name = ui->comboBox_com->currentText();

    if (ui->comboBox_baud->currentIndex() == 4) {
       currentSettings.baudRate = ui->comboBox_baud->currentText().toInt();
    } else {
       currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->comboBox_baud->itemData(ui->comboBox_baud->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                    ui->comboBox_dataBit->itemData(ui->comboBox_dataBit->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->comboBox_dataBit->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                    ui->comboBox_checkBit->itemData(ui->comboBox_checkBit->currentIndex()).toInt());
    currentSettings.stringParity = ui->comboBox_checkBit->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                    ui->comboBox_stopBit->itemData(ui->comboBox_stopBit->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->comboBox_stopBit->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                    ui->comboBox_flow->itemData(ui->comboBox_flow->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->comboBox_flow->currentText();

}

void MainWindow::openSerialPort()
{
    updateSettings();
    serial->setPortName(currentSettings.name);
    serial->setBaudRate(currentSettings.baudRate);
    serial->setDataBits(currentSettings.dataBits);
    serial->setParity(currentSettings.parity);
    serial->setStopBits(currentSettings.stopBits);
    serial->setFlowControl(currentSettings.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        ui->pushButton_serialOpen->setEnabled(false);
        ui->pushButton_serialSend->setEnabled(true);
        ui->pushButton_serialClose->setEnabled(true);
        QString textstr = "Open "+currentSettings.name+" successd!!";
        ui->textBrowser_serial_text->clear();
        ui->textBrowser_serial_text->setText(textstr);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
    serialReceiveTimer->start(1000);
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    ui->pushButton_serialOpen->setEnabled(true);
    ui->pushButton_serialSend->setEnabled(false);
    ui->pushButton_serialClose->setEnabled(false);
    QString textstr = "Closed "+currentSettings.name+" successd!!";
    ui->textBrowser_serial_text->clear();
    ui->textBrowser_serial_text->setText(textstr);
    serialReceiveTimer->stop();
}

void MainWindow::clearSerialText()
{
    ui->textBrowser_serial_text->clear();
}

void MainWindow::readDate()
{
    if(board == "CS10600RA070") {
        rs485SendEnable(false);
    }
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QByteArray date = serial->readAll();
    while (serial->waitForReadyRead(10))
        date += serial->readAll();
    ui->textBrowser_serial_text->setText(currenttime+"\n"+currentSettings.name+" Received:"+ (date=="NULL"?"NULL":"\n"+date));
}

void MainWindow::writeDate()
{
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QByteArray sendDate = "Message from serial by manual!!!\n";
    if(board == "CS10600RA070") {
        rs485SendEnable(true);
    }
    for(int i=0; i<100; i++){
        serial->write(sendDate);
        ui->textBrowser_serial_text->setText(currenttime+"\n"+currentSettings.name+" Sended Message by manual!!");
    }
}

void MainWindow::rs485Init()
{
    gpioExport(RS485ENIO);
    setGPIOModelRaw(RS485ENIO,"out");
    setGPIOValueRaw(RS485ENIO,"0");
}

void MainWindow::rs485SendEnable(bool enable)
{
    if(enable)
        setGPIOValueRaw(RS485ENIO,"1");
    else
        setGPIOValueRaw(RS485ENIO,"0");
}

void MainWindow::serialInit()
{
    serial = new QSerialPort(this);
    fillPortsParameters();

    intValidator = new QIntValidator(0, 4000000, this);
    ui->comboBox_baud->setInsertPolicy(QComboBox::NoInsert);
    ui->pushButton_serialClose->setEnabled(false);
    ui->pushButton_serialSend->setEnabled(false);
    if(board == "CS10600RA070")
    {
        rs485Init();
    }
    connect(ui->comboBox_baud, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::checkCustomBaudRatePolicy);
    connect(ui->comboBox_com, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::checkCustomDevicePathPolicy);
    connect(ui->pushButton_serialOpen,&QPushButton::clicked, this, &MainWindow::openSerialPort);
    connect(ui->pushButton_serialClose,&QPushButton::clicked,this,&MainWindow::closeSerialPort);
    connect(ui->pushButton_serialClear,&QPushButton::clicked,this,&MainWindow::clearSerialText);
    connect(ui->pushButton_serialSend,&QPushButton::clicked,this,&MainWindow::writeDate);

    // use QTimer can see the change of time. we also can use follow readyRead signal.
    //connect(serial,&QSerialPort::readyRead,this,&MainWindow::readDate);
    serialReceiveTimer = new QTimer(this);
    connect(serialReceiveTimer,SIGNAL(timeout()),SLOT(readDate()));
}

/*
 *  NetWork
 *
 *  wifiEnable() wifiDisable() wifiDisplayInfo() networkInit() getipInfo() networkautotest()
 *
 */

void MainWindow::wifiEnable()
{
    ui->textBrowser_network_text->clear();
#if 0
    ui->textBrowser_network_text->setText("WIFI Enabling!!");
    system("ifconfig wlan0 up &");
    system("wifienable.sh &");
    QMessageBox::warning(this,"Tips","Need Time to complete, Press OK to wait.");
    this->ui->textBrowser_network_text->setText("WIFI Enabled!");
#else
    popen("ifconfig wlan0 up","r");
    popen("wifienable.sh","r");
    this->ui->textBrowser_network_text->setText("WIFI will be open in background, press \"Network Info\" to check\n\n\n\n*NOTE:\nUsing follow command to config YOURS wifi ssid and password first\n\n#wpa_passphrase \"wifi_ssid\" \"wifi_password\" >> /etc/wpa_supplicant.conf\n\nExample:\nctrl_interface=/var/run/wpa_supplicant\nctrl_interface_group=0\nctrl_interface_group=0\nnetwork={\n        ssid=\"wifi_ssid\"\n        #psk=\"wifi_password\"\n        psk=0fbbe9e700815707d38305d89e3260f3b2334d0a0979c61be8eeea6ca9b0cd64\n}");
    this->ui->pushButton_netInfo->setText("Network Info");
    QMessageBox::warning(this,"Tips","Need Time to complete, Press OK to wait.");
#endif
}

void MainWindow::wifiDisable()
{
    ui->textBrowser_network_text->clear();
    ui->textBrowser_network_text->setText("WIFI Disabling!!");
    system("ifconfig wlan0 down");
    ui->textBrowser_network_text->setText("WIFI Disabled!!");
    this->ui->pushButton_netInfo->setText("Network Info");
}

void MainWindow::wifiInfoDisplay()
{
    QString cmdstr = "ifconfig >" +QString("%1").arg(TEMPFILEPATH); // Note when this has olny one "+".
    system(cmdstr.toLocal8Bit());
    ui->textBrowser_network_text->clear();
    ui->textBrowser_network_text->setText(GetFileValue(TEMPFILEPATH));
    this->ui->pushButton_netInfo->setText("Refresh");

    // Use Network functions
    QList<QNetworkInterface> listImpl = QNetworkInterface::allInterfaces();
       if(!listImpl.isEmpty())
       {
           QString networkinfo = "";
           for (int i = 0; i < listImpl.length(); ++i) {
               QNetworkInterface interface = listImpl.at(i);
               if(interface.name()=="lo")
                   continue;
               QList<QNetworkAddressEntry> enrtyList = interface.addressEntries();
               if(enrtyList.length() > 0){
                    QNetworkAddressEntry entry = enrtyList.at(0);
                    qDebug() << interface.name();
                    qDebug() << entry.ip().toString();
                    networkinfo += interface.name() + "\n";
                    networkinfo += "IP: " + entry.ip().toString() + "\n";
                    networkinfo += "MAC: " + interface.hardwareAddress() + "\n";
               }
               networkinfo += "\n";
           }
           ui->textBrowser_network_text->setText(networkinfo);
       }
}

void MainWindow::getipInfo()
{
    QString cmdstr = "";
    if (cpuplat == "pi" || cpuplat == "am335x"){
        cmdstr = "echo `ifconfig eth0 | grep 'inet ' | grep -v '127.0.0.1'` >"+ipaddrpath;
    } else if (cpuplat == "rk3568"){
        cmdstr = "echo `hostname -I` >"+ipaddrpath;
    } else {
        cmdstr = "echo `ifconfig eth0 | grep 'inet addr:' | grep -v '127.0.0.1' | cut -d: -f2 | awk '{print $1}'` >"+ipaddrpath;
        cmdstr = "echo `ifconfig eth1 | grep 'inet addr:' | grep -v '127.0.0.1' | cut -d: -f2 | awk '{print $1}'` >>"+ipaddrpath;
    }
    system(cmdstr.toLocal8Bit());
    if(GetFileValue(ipaddrpath).contains("192.168"))
        CueAudio();
}

void MainWindow::networkInit()
{
    connect(ui->pushButton_wifiEnable,&QPushButton::clicked,this,&MainWindow::wifiEnable);
    connect(ui->pushButton_wifiDisable,&QPushButton::clicked,this,&MainWindow::wifiDisable);
    connect(ui->pushButton_netInfo,&QPushButton::clicked,this,&MainWindow::wifiInfoDisplay);
    if(cpuplat == "pi" || cpuplat == "px30" || cpuplat == "rk3399" || cpuplat == "rk3568" || (board == "imx6q" && GetDebianCodeName() == "bionic")){
        ui->pushButton_wifiEnable->setVisible(false);
        ui->pushButton_wifiDisable->setVisible(false);
    }
}

void MainWindow::networkautotest()
{
    ipautotestTimer = new QTimer(this);
    connect(ipautotestTimer,SIGNAL(timeout()),this,SLOT(getipInfo()));
    ipautotestTimer->start(5000);
}

/*
 * 4G
 *
 * mobile4gEnable() mobile4gDisable() checkCustom4gNumPolicy(int idx) mobile4gInit()
 *
 */
void MainWindow::mobile4gEnable()
{
    ui->pushButton_4gEnable->setDisabled(true);
    ui->pushButton_4gDisable->setDisabled(false);
    ui->comboBox_4g->setDisabled(true);
    QString nettype = ui->comboBox_4g->currentText();
    QString cmdstr = "ifconfig wwan0 down && quectel-CM -s " + nettype + "&";
    system(cmdstr.toLocal8Bit());
    ui->textBrowser_network_text->setText("4G Disabling, press 'Netinfo' to know status.");
}

void MainWindow::mobile4gDisable()
{
    ui->pushButton_4gDisable->setDisabled(true);
    ui->pushButton_4gEnable->setDisabled(false);
    ui->comboBox_4g->setDisabled(false);
    QString cmdstr = "killall -9 quectel-CM";
    system(cmdstr.toLocal8Bit());
    ui->textBrowser_network_text->setText("4G Disabled");
}

void MainWindow::checkCustom4gNumPolicy(int idx)
{
    bool isCustom4gNum = !ui->comboBox_4g->itemData(idx).isValid();
    ui->comboBox_4g->setEditable(isCustom4gNum);
    if (isCustom4gNum)
        ui->comboBox_4g->clearEditText();
}

void MainWindow::mobile4gInit()
{
    if(board == "AM335XBOARD" || cpuplat == "rk3399" || board == "CS12720_RK3568_050"){
        ui->comboBox_4g->setVisible(false);
        ui->pushButton_4gDisable->setVisible(false);
        ui->pushButton_4gEnable->setVisible(false);
        return;
    }

    ui->comboBox_4g->addItem("3gnet","3gnet");
    ui->comboBox_4g->addItem("cmnet","cmnet");
    ui->comboBox_4g->addItem("ctnet","ctnet");
    ui->comboBox_4g->addItem("Custome");
    ui->pushButton_4gEnable->setDisabled(false);
    ui->pushButton_4gDisable->setDisabled(true);
    connect(ui->comboBox_4g, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::checkCustom4gNumPolicy);
    ui->pushButton_4gDisable->setDisabled(true);
    connect(ui->pushButton_4gEnable,&QPushButton::clicked,this,&MainWindow::mobile4gEnable);
    connect(ui->pushButton_4gDisable,&QPushButton::clicked,this,&MainWindow::mobile4gDisable);
}

/*
 * GPS
 *
 * readGPSData() gpsEnable() gpsDisable() gpsInit()
 *
 */

void MainWindow::readGPSData()
{
    QByteArray gpsdata;
    if(gpsport->waitForReadyRead(1000)){
        gpsdata= gpsport->readAll();
        while (gpsport->waitForReadyRead(10)) {
            gpsdata += gpsport->readAll();
        }
    }

    ui->textBrowser_network_text->setText(gpsdata);
}

void MainWindow::gpsEnable()
{
    QString cfgcmd("AT+QGPSCFG=\"gpsnmeatype\",2\r\n");
    QString startcmd("AT+QGPS=1\r\n");
    if(atport->isOpen())
        atport->close();
    if(atport->open(QIODevice::ReadWrite)){
        ui->pushButton_GPSEnable->setDisabled(true);
        ui->pushButton_GPSDisable->setDisabled(false);
        atport->write(cfgcmd.toLatin1());
        while(atport->waitForReadyRead(100));
        atport->write(startcmd.toLatin1());
        qDebug() << "GPS open ";
        while(atport->waitForReadyRead(100));
        atport->close();
        qDebug() << "serialport closed";
    }else
    {
        QMessageBox::critical(this, tr("Error"), "No GPS");
        return;
    }

    if(gpsport->isOpen())
        gpsport->close();
    if(!gpsport->open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, tr("Error"), "No GPS");
        return;
    }

    gpsreadTimer->start(2000);
}

void MainWindow::gpsDisable()
{
    QString stopcmd("AT+QGPSEND\r");
    if(atport->isOpen())
        atport->close();
    if(atport->open(QIODevice::ReadWrite)){
        ui->pushButton_GPSEnable->setDisabled(false);
        ui->pushButton_GPSDisable->setDisabled(true);
        atport->write(stopcmd.toLatin1());
        while(atport->waitForReadyRead(100));
        atport->close();
    }

    gpsreadTimer->stop();
}

void MainWindow::gpsInit()
{
    if(board == "AM335XBOARD" || cpuplat == "rk3399" || board == "CS12720_RK3568_050"){
        ui->pushButton_GPSEnable->setVisible(false);
        ui->pushButton_GPSDisable->setVisible(false);
        return;
    }
    atport = new QSerialPort(this);
    atport->setPortName("/dev/ttyUSB2");
    atport->setBaudRate(QSerialPort::Baud9600);
    atport->setDataBits(QSerialPort::Data8);
    atport->setParity(QSerialPort::NoParity);
    atport->setStopBits(QSerialPort::OneStop);
    atport->setFlowControl(QSerialPort::NoFlowControl);

    gpsport = new QSerialPort(this);
    gpsport->setPortName("/dev/ttyUSB1");
    gpsport->setBaudRate(QSerialPort::Baud9600);
    gpsport->setDataBits(QSerialPort::Data8);
    gpsport->setParity(QSerialPort::NoParity);
    gpsport->setStopBits(QSerialPort::OneStop);
    gpsport->setFlowControl(QSerialPort::NoFlowControl);

    ui->pushButton_GPSEnable->setDisabled(false);
    ui->pushButton_GPSDisable->setDisabled(true);

    connect(ui->pushButton_GPSEnable,&QPushButton::clicked,this,&MainWindow::gpsEnable);
    connect(ui->pushButton_GPSDisable,&QPushButton::clicked,this,&MainWindow::gpsDisable);

    gpsreadTimer = new QTimer(this);
    connect(gpsreadTimer,SIGNAL(timeout()),SLOT(readGPSData()));

}

/*
 * AllTest
 *
 *  readalltestresult() alltest() alltestInit()
 *
 */

void MainWindow::readalltestresult()
{
    QString result;
    result = GetFileValue("/tmp/autotest.log");
    if(result != "")
        ui->textBrowser_network_text->setText(result);
    if(result.contains("Test END")) {
    alltestTimer->stop();
    ui->pushButton_AllTest->setDisabled(false);
    gpioOutStatuTimer->start(50);
    }

}

void MainWindow::alltest()
{
    system("echo > /tmp/autotest.log");
    system("/opt/test/alltest/autorun_test.sh &");
    alltestTimer->start(500);
    ui->textBrowser_network_text->setText("Starting All Test:");
    ui->pushButton_AllTest->setDisabled(true);
    gpioOutStatuTimer->stop();
}

void MainWindow::alltestInit()
{
    QString enable = GetComResult("grep -c ENABLE /opt/test/alltest/switch");
    if (enable.left(1) == "1")
    {
        ui->pushButton_AllTest->setText("AllTest");
        connect(ui->pushButton_AllTest,&QPushButton::clicked,this,&MainWindow::alltest);
        alltestTimer = new QTimer(this);
        connect(alltestTimer, SIGNAL(timeout()),SLOT(readalltestresult()));
    }else
    {
        ui->pushButton_AllTest->setVisible(false);
    }
}

/*
 *  USB
 *
 *  getusbInfo() usbInit
 *
 *
 */

void MainWindow::getusbInfo()
{
    QString line = GetFileValue(IMX6QUSBDEBUGFILEPATH);
    if(line.contains("USB Flash Disk")||
            line.contains("Mass Storage")||
            line.contains("USB Receiver")||
            line.contains("Keyboard Mouse")||
            line.contains("Mouse")||
            line.contains("USB DISK")||
            line.contains("Card Reader")||
            line.contains("Wireless Optical Mouse")||
            line.contains("MXT USB Device")||
            line.contains("Mass-Storage")||
            line.contains("USB2.0-CRW")||
            line.contains("DataTraveler 2.0")||
            line.contains("DataTraveler 3.0")||
            line.contains("USB Storage"))
    {
        CueAudio();
    }
}

void MainWindow::usbInit()
{
    usbautotestTimer = new QTimer(this);
    connect(usbautotestTimer,SIGNAL(timeout()),this,SLOT(getusbInfo()));
    usbautotestTimer->start(5000);
}

/*
 *  GPIO
 *
 *  getGPIOValueRaw(QString) setGPIOValueRaw(QString,QString) setGPIOModelRaw(QString)
 *  getGPIOValue(QString) setGPIOValue(QString,QString) setGPIOModel(QString)  getGPIOValue(QString) setGPIOOutStatu() setGPIOOutAllHigh()
 *
 *  setGPIOOutAllLow() gpioExport(QString) gpioInit()
 *
 *  RelayNC() RelayNO() ChangeRelayState()
 *
 */

QString MainWindow::getGPIOValueRaw(QString gpionum)
{
    QString gpiopath = QString("%1").arg("/sys/class/gpio/gpio")+gpionum+"/value";
    if(GetFileValue(gpiopath)==NULL){
        QMessageBox::critical(this,"Error","Open GpioPath Error!! Don't get GPIOValue!! Return NULL!! Try use Libgpiod to control GPIO for kernel version more then 4.9");
        // will disable MessageBox to display again
        // only need to know once time
        gpioInStatuTimer->stop();
        gpioOutStatuTimer->stop();
        return NULL;
    }else
        return GetFileValue(gpiopath);
}

void MainWindow::setGPIOValueRaw(QString gpionum, QString value)
{
    QString gpiopath = QString("%1").arg("/sys/class/gpio/gpio")+gpionum+"/value";
    QString cmdstr = "echo "+value+" >"+gpiopath;
    system(cmdstr.toLocal8Bit());
}

void MainWindow::setGPIOModelRaw(QString gpionum, QString model)
{
    QString gpiomodelpath = QString("%1").arg("/sys/class/gpio/gpio")+gpionum+"/direction";
    QString cmdstr = "echo "+model+" >"+gpiomodelpath;
    system(cmdstr.toLocal8Bit());
}

QString MainWindow::getGPIOValue(QString gpionum)
{
    QString gpiopath = QString("%1").arg(GPIOBASEPATH)+gpionum;
    if(GetFileValue(gpiopath)==NULL){
        QMessageBox::critical(this,"Error","Open GpioPath Error!! Don't get GPIOValue!! Return NULL!! Try use Libgpiod to control GPIO for kernel version more then 4.9");
        // will disable MessageBox to display again
        // only need to know once time
        gpioInStatuTimer->stop();
        gpioOutStatuTimer->stop();
        return NULL;
    }else
        return GetFileValue(gpiopath);
}

void MainWindow::setGPIOValue(QString gpionum, QString value)
{
    QString gpiopath = QString("%1").arg(GPIOBASEPATH)+gpionum;
    QString cmdstr = "echo "+value+" >"+gpiopath;
    system(cmdstr.toLocal8Bit());
}

void MainWindow::setGPIOModel(QString gpionum, QString model)
{
    QString gpiomodelpath = QString("%1").arg(GPIOBASEPATH)+gpionum+"/direction";
    QString cmdstr = "echo "+model+" >"+gpiomodelpath;
    system(cmdstr.toLocal8Bit());
}

void MainWindow::setGPIOInStatu()
{
   if(getGPIOValue(gpioInArray[0])=="1\n")
       ui->label_in_1_in->setPixmap(QPixmap(":/images/IO_high.png"));
   else
       ui->label_in_1_in->setPixmap(QPixmap(":/images/IO_low.png"));
   if(getGPIOValue(gpioInArray[1])=="1\n")
       ui->label_in_2_in->setPixmap(QPixmap(":/images/IO_high.png"));
   else
       ui->label_in_2_in->setPixmap(QPixmap(":/images/IO_low.png"));

   if(getGPIOValue(gpioInArray[2])=="1\n")
       ui->label_in_3_in->setPixmap(QPixmap(":/images/IO_high.png"));
   else
       ui->label_in_3_in->setPixmap(QPixmap(":/images/IO_low.png"));

   if(getGPIOValue(gpioInArray[3])=="1\n")
       ui->label_in_4_in->setPixmap(QPixmap(":/images/IO_high.png"));
   else
       ui->label_in_4_in->setPixmap(QPixmap(":/images/IO_low.png"));

}

void MainWindow::setGPIOOutStatu()
{
    if(ui->radioButton_out_1_high->isChecked())
        setGPIOValue(gpioOutArray[0],"1");
    else
        setGPIOValue(gpioOutArray[0],"0");

    if(ui->radioButton_out_2_high->isChecked())
        setGPIOValue(gpioOutArray[1],"1");
    else
        setGPIOValue(gpioOutArray[1],"0");

    if(ui->radioButton_out_3_high->isChecked())
        setGPIOValue(gpioOutArray[2],"1");
    else
        setGPIOValue(gpioOutArray[2],"0");

    if(ui->radioButton_out_4_high->isChecked())
        setGPIOValue(gpioOutArray[3],"1");
    else
        setGPIOValue(gpioOutArray[3],"0");
}

void MainWindow::setGPIOOutAllHigh()
{
    ui->radioButton_out_1_high->setChecked(true);
    ui->radioButton_out_2_high->setChecked(true);
    ui->radioButton_out_3_high->setChecked(true);
    ui->radioButton_out_4_high->setChecked(true);
}

void MainWindow::setGPIOOutAllLow()
{
    ui->radioButton_out_1_low->setChecked(true);
    ui->radioButton_out_2_low->setChecked(true);
    ui->radioButton_out_3_low->setChecked(true);
    ui->radioButton_out_4_low->setChecked(true);
}

void MainWindow::gpioExport(QString gpionum)
{
    QString cmdstr = "echo "+gpionum+" >"+QString("%1").arg(GPIOEXPORTPATH);
    system(cmdstr.toLocal8Bit());
}

void MainWindow::RelayNC()
{
    QString cmdstr = "echo 0 >"+relaypath+"&";
    system(cmdstr.toLocal8Bit());
    ui->checkBox_Relay->setText("RelayNC");
}

void MainWindow::RelayNO()
{
    QString cmdstr = "echo 1 >"+relaypath+"&";
    system(cmdstr.toLocal8Bit());
    ui->checkBox_Relay->setText("RelayNO");
}

void MainWindow::ChangeRelayState()
{
    if(relayflag)
    {
        RelayNO();
        relayflag=!relayflag;
    }
    else
    {
        RelayNC();
        relayflag=!relayflag;
    }
}

void MainWindow::gpioInit()
{
//    for(int i=0; i<4; i++){
//        gpioExport(gpioOutArray[i]);
//        gpioExport(gpioInArray[i]);
//        setGPIOModel(gpioOutArray[i],"out");
//        setGPIOModel(gpioInArray[i],"in");
//    }

    // GPIO_OUT
    ui->radioButton_out_1_low->setChecked(true);
    ui->radioButton_out_2_low->setChecked(true);
    ui->radioButton_out_3_low->setChecked(true);
    ui->radioButton_out_4_low->setChecked(true);

    if(board == "LRRA4-101" || board == "CS12800RA4101" || board == "CS12800RA4101A" || board == "CS12800PX101" || board == "CS12720RA4050" || board == "CS12720_RK3568_050") {
        ui->radioButton_out_1_high->setCheckable(false);
        ui->radioButton_out_2_high->setCheckable(false);
        ui->radioButton_out_3_high->setCheckable(false);
        ui->radioButton_out_4_high->setCheckable(false);
        ui->radioButton_out_1_low->setCheckable(false);
        ui->radioButton_out_2_low->setCheckable(false);
        ui->radioButton_out_3_low->setCheckable(false);
        ui->radioButton_out_4_low->setCheckable(false);
        ui->pushButton_setAllHigh->setDisabled(true);
        ui->pushButton_setAllLow->setDisabled(true);
        ui->pushButton_gpioRefresh->setDisabled(true);

        ui->label_gpio_out->setVisible(false);
        ui->label_gpio_in->setVisible(false);
        ui->label_out_1->setVisible(false);
        ui->label_out_2->setVisible(false);
        ui->label_out_3->setVisible(false);
        ui->label_out_4->setVisible(false);
        ui->label_high->setVisible(false);
        ui->label_high_high->setVisible(false);
        ui->label_low->setVisible(false);
        ui->label_low_low->setVisible(false);
        ui->radioButton_out_1_high->setVisible(false);
        ui->radioButton_out_2_high->setVisible(false);
        ui->radioButton_out_3_high->setVisible(false);
        ui->radioButton_out_4_high->setVisible(false);
        ui->radioButton_out_1_low->setVisible(false);
        ui->radioButton_out_2_low->setVisible(false);
        ui->radioButton_out_3_low->setVisible(false);
        ui->radioButton_out_4_low->setVisible(false);
        ui->label_in_->setVisible(false);
        ui->label_in_1->setVisible(false);
        ui->label_in_3->setVisible(false);
        ui->label_in_4->setVisible(false);
        ui->label_in_1_in->setVisible(false);
        ui->label_in_2_in->setVisible(false);
        ui->label_in_3_in->setVisible(false);
        ui->label_in_4_in->setVisible(false);
        ui->pushButton_gpioRefresh->setVisible(false);
        ui->pushButton_setAllHigh->setVisible(false);
        ui->pushButton_setAllLow->setVisible(false);
        ui->checkBox_Relay->setVisible(false);
        if(board != "CS12720RA4050" && board != "CS12720_RK3568_050") {
            ui->checkBox_Relay->setVisible(true);
            relayflag=true;
            ui->checkBox_Relay->setChecked(false);
            connect(ui->checkBox_Relay,SIGNAL(toggled(bool)),this,SLOT(ChangeRelayState()));
        }
    } else {
        ui->checkBox_Relay->setVisible(false);
        gpioOutStatuTimer = new QTimer(this);
        connect(gpioOutStatuTimer,SIGNAL(timeout()),this,SLOT(setGPIOOutStatu()));
        gpioOutStatuTimer->start(50);
        connect(ui->pushButton_setAllHigh,&QPushButton::clicked,this,&MainWindow::setGPIOOutAllHigh);
        connect(ui->pushButton_setAllLow,&QPushButton::clicked,this,&MainWindow::setGPIOOutAllLow);


        // GPIO_IN
        ui->label_high_high->setPixmap(QPixmap(":/images/IO_high.png"));
        ui->label_low_low->setPixmap(QPixmap(":/images/IO_low.png"));

        gpioInStatuTimer = new QTimer(this);
        connect(gpioInStatuTimer,SIGNAL(timeout()),this,SLOT(setGPIOInStatu()));
        gpioInStatuTimer->start(50);
        connect(ui->pushButton_gpioRefresh,&QPushButton::clicked,this,&MainWindow::setGPIOInStatu);
    }
}

/*
 *  CAN
 *
 *  canReceive() canStart() canSend() canStop() checkCustomCanNumPolicy(int idx) canInit()
 *
 *
 */
void MainWindow::canReceive()
{
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cannum = ui->comboBox_canNum->currentText();
    ui->textBrowser_can_text->clear();
    if(GetFileValue(CANTEMPFILEPATH)==NULL)
        ui->textBrowser_can_text->setText(currenttime+"\n"+cannum+" Received: "+"NULL");
    else
        ui->textBrowser_can_text->setText(currenttime+"\n"+cannum+" Received:\n"+GetFileValue(CANTEMPFILEPATH));
}

void MainWindow::canStart()
{
    ui->pushButton_canStart->setDisabled(true);
    ui->pushButton_canSend->setDisabled(false);
    ui->pushButton_canStop->setDisabled(false);
    ui->comboBox_canNum->setDisabled(true);
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cannum = ui->comboBox_canNum->currentText();
    QString cmdstr = "";
    
    if(cpuplat == "px30"){
        cmdstr = "canconfig "+cannum+" stop && canconfig "+cannum+" bitrate 10000 ctrlmode triple-sampling on loopback off && canconfig "+cannum+" start && sleep 5 && canconfig "+cannum+" start && candump "+cannum+" >"+QString("%1").arg(CANTEMPFILEPATH)+"&";
    } else if (cpuplat == "pi" && GetDebianCodeName()=="bullseye") {
        cmdstr = "ip link set "+cannum+" down && ip link set "+cannum+" type can bitrate 10000 triple-sampling on && ip link set "+cannum+" up && candump "+cannum+" >"+QString("%1").arg(CANTEMPFILEPATH)+"&";
    } else if (board == "CS12800R101P" || cpuplat == "rk3568" || (board == "imx6q" && GetDebianCodeName() == "bionic")){
        cmdstr = "ip link set "+cannum+" down && ip link set "+cannum+" type can bitrate 10000 triple-sampling on && ip link set "+cannum+" up && candump "+cannum+" >"+QString("%1").arg(CANTEMPFILEPATH)+"&";
    } else {
        cmdstr = "canconfig "+cannum+" stop && canconfig "+cannum+" bitrate 10000 ctrlmode triple-sampling on loopback off && canconfig "+cannum+" start && candump "+cannum+" >"+QString("%1").arg(CANTEMPFILEPATH)+"&";
    }
    system(cmdstr.toLocal8Bit());
    canReceiveTimer->start(50);
    ui->textBrowser_can_text->clear();
    ui->textBrowser_can_text->setText(currenttime+"\n"+cannum+" Started!!");
    //QMessageBox::critical(this,"M",cmdstr);
}

void MainWindow::canSend()
{
    QString canframe;
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cannum = ui->comboBox_canNum->currentText();
    if((cpuplat == "pi" && GetDebianCodeName()=="bullseye") || board == "CS12800R101P" || cpuplat == "rk3568" || (board == "imx6q" && GetDebianCodeName() == "bionic")){
	canframe = CANSENDCANFRAMENEW;
    } else {
	canframe = CANSENDDATA;
    }
    QString cmdstr = "cansend "+ui->comboBox_canNum->currentText()+" "+QString("%1").arg(canframe);

    system(cmdstr.toLocal8Bit());
    ui->textBrowser_can_text->clear();
    ui->textBrowser_can_text->setText(currenttime+"\n"+cannum+" Sended: "+QString("%1").arg(canframe));
    //QMessageBox::critical(this,"M",cmdstr);
}

void MainWindow::canStop()
{
    QString cmdstr;
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cannum = ui->comboBox_canNum->currentText();
    if((cpuplat == "pi" && GetDebianCodeName()=="bullseye") || (board == "imx6q" && GetDebianCodeName() == "bionic") || board == "CS12800R101P" || cpuplat == "rk3568"){
        cmdstr = "ip link set "+cannum+" down";
    } else {
        cmdstr = "canconfig "+cannum+" stop";
    }
    system(cmdstr.toLocal8Bit());
    canReceiveTimer->stop();
    ui->comboBox_canNum->setDisabled(false);
    ui->pushButton_canStart->setDisabled(false);
    ui->pushButton_canSend->setDisabled(true);
    ui->pushButton_canStop->setDisabled(true);
    ui->textBrowser_can_text->clear();
    ui->textBrowser_can_text->setText(currenttime+":\n"+cannum+" Stoped!");
    //QMessageBox::critical(this,"M",cmdstr);
}

void MainWindow::checkCustomCanNumPolicy(int idx)
{
    bool isCustomCanNum = !ui->comboBox_canNum->itemData(idx).isValid();
    ui->comboBox_canNum->setEditable(isCustomCanNum);
    if (isCustomCanNum)
        ui->comboBox_canNum->clearEditText();
}

void MainWindow::Enable2Can()
{
    QString cmdstr = "cp /run/media/mmcblk0p1/am335x-chipsee-som-2can.dtb /run/media/mmcblk0p1/am335x-chipsee-som.dtb && sync && "
            "echo 1 >/usr/hardwaretest/.2can";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::Disable2Can()
{
    QString cmdstr = "cp /run/media/mmcblk0p1/am335x-chipsee-som-1can.dtb /run/media/mmcblk0p1/am335x-chipsee-som.dtb && sync && "
            "echo 0 >/usr/hardwaretest/.2can";
    system(cmdstr.toLocal8Bit());
}

void MainWindow::ChangeCanState()
{
    if(!canflag)
    {
        Enable2Can();
        QMessageBox::warning(this,"Tips","System will reboot to change to 2CAN");
        ui->checkBox_2can->setVisible(false);
    }
    else
    {
        Disable2Can();
        QMessageBox::warning(this,"Tips","System will reboot to change to 1CAN");
        ui->checkBox_2can->setVisible(false);
    }

    system("reboot");
}

void MainWindow::canInit()
{
    // UI
    ui->checkBox_2can->setVisible(false);
    if(board == "imx6q" || board == "imx6d" || board == "imx6u" || board == "CS12800_RK3568_101" || board == "CS10600_RK3568_070"){
        ui->comboBox_canNum->addItem("can0","can0");
        ui->comboBox_canNum->addItem("can1","can1");
        ui->comboBox_canNum->addItem("Custum");
    }else if(board == "CS10600RA070" || board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A" || board == "CS10600R070" || board == "CS12800R101" || board == "CS40230RB"){
        ui->comboBox_canNum->setDisabled(true);
        ui->textBrowser_can_text->setText("This device don't support CAN Bus.");
    }else if(board == "AM335XBOARD"){
        ui->checkBox_2can->setVisible(true);
        QString cmdstr = "test -f /usr/hardwaretest/.2can || touch /usr/hardwaretest/.2can";
        system(cmdstr.toLocal8Bit());
        if(GetFileValue("/usr/hardwaretest/.2can").remove(QChar('\n'),Qt::CaseInsensitive)=="1"){
            canflag = true;
            ui->comboBox_canNum->addItem("can0","can0");
            ui->comboBox_canNum->addItem("can1","can1");
        } else {
            canflag = false;
            ui->comboBox_canNum->addItem("can0","can0");
        }
        ui->comboBox_canNum->addItem("Custum");
        ui->checkBox_2can->setChecked(canflag);
    }else if(board == "CS12800RA101") {
        if(GetComResult("ifconfig -a").contains("can1")) {
                ui->comboBox_canNum->addItem("can0","can0");
                ui->comboBox_canNum->addItem("can1","can1");
                ui->comboBox_canNum->addItem("Custum");
        } else {
                ui->comboBox_canNum->addItem("can0","can0");
                ui->comboBox_canNum->addItem("Custum");
        }
    }else{
        ui->comboBox_canNum->addItem("can0","can0");
        ui->comboBox_canNum->addItem("Custum");
    }

    connect(ui->checkBox_2can,SIGNAL(toggled(bool)),this,SLOT(ChangeCanState()));
    connect(ui->comboBox_canNum, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::checkCustomCanNumPolicy);

    // Button
    ui->pushButton_canSend->setDisabled(true);
    ui->pushButton_canStop->setDisabled(true);
    if(board == "CS10600RA070" || board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A" || board== "CS10600R070" || board == "CS12800R101" || board == "CS40230RB"){
        ui->pushButton_canStart->setDisabled(true);
    }
    connect(ui->pushButton_canStart,&QPushButton::clicked,this,&MainWindow::canStart);
    connect(ui->pushButton_canSend,&QPushButton::clicked,this,&MainWindow::canSend);
    connect(ui->pushButton_canStop,&QPushButton::clicked,this,&MainWindow::canStop);

    // Receive
    canReceiveTimer = new QTimer(this);
    connect(canReceiveTimer,SIGNAL(timeout()),this,SLOT(canReceive()));
}

/*
 *  AutoTest
 *
 *  autoTest() showRequest(const QString &s) showcanRequest(const QString &s) autotestInit()
 *
 *
 */

void MainWindow::autoTest()
{   
    autoflag = true;
    autoTesttimer->stop();
    // Test Beeper And Led
    QEventLoop eventloop;
    for(int i=0; i<2; i++)
    {
        EnableBuzzer();
        OpenLed();
        OpenLed2();
        if(board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A" || board == "CS12800PX101"){
            RelayNO();
        }

        QTimer::singleShot(1000, &eventloop,SLOT(quit()));
        eventloop.exec();

        DisableBuzzer();
        CloseLed();
        CloseLed2();
        if(board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A" || board == "CS12800PX101"){
            RelayNC();
        }

        QTimer::singleShot(1000, &eventloop,SLOT(quit()));
        eventloop.exec();
    }

    if(cpuplat != "pi"){
        usbInit();
    }
    //if(board !="CS12720RA4050" && board !="CS10600RA4070" && board !="CS12800RA4101" && board != "LRRA4-101" && board !="CS12800RA4101A" && board !="CS12800RA4101BOX" && board !="CS12800PX101") {
    if(cpuplat != "pi" && board !="CS12800PX101" && board != "CS12800R101P") {
        networkautotest();
    }

    if(board != "CS12800RA4101" && board != "LRRA4-101" && board != "CS12800RA4101A" && board !="CS10600RA070") {
        // Test Serial and CAN
        connect(&thread, &SlaveThread::canrequest, this,&MainWindow::showcanRequest);
    }
    connect(&thread, &SlaveThread::request, this,&MainWindow::showRequest);

    QSerialPort *serial = new QSerialPort();
    QSerialPort *serial1 = new QSerialPort();
    QSerialPort *serial2 = new QSerialPort();
    QSerialPort *serial3 = new QSerialPort();
    QSerialPort *serial4 = new QSerialPort();
    QSerialPort *serial5 = new QSerialPort();
    QSerialPort * port[6] = {serial,serial1,serial2,serial3,serial4,serial5};

    int i = 0;
    qDebug() << board + " Default Serial Port list: ";
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        port[i]->setPortName(info.portName());
        port[i]->setBaudRate(QSerialPort::Baud115200);
        port[i]->setDataBits(QSerialPort::Data8);
        port[i]->setParity(QSerialPort::NoParity);
        port[i]->setStopBits(QSerialPort::OneStop);
        port[i]->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << port[i]->portName();
        i++;
        if(i>5)
            break;
        //qDebug() << port[i-1]->portName();
    }

    if(board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A"){
        port[5] = port[4];
    }

    if(board == "CS12800PX101"){
        port[0] = port[1];
    }

    if(board == "CS10600RA4070" || board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A" || board == "CS12800RA4101BOX" || board == "CS12800RA4101P" || board == "CS19108RA4133P" || board == "CS10768RA4150P" || board == "CS19108RA4156P" || board == "CS19108RA4215P"){
        if(port[0] != NULL && port[0]->portName() == "ttyACM0"){
            port[0] = port[1];
            port[1] = port[2];
            port[2] = port[3];
            port[3] = port[4];
            port[4] = port[5];
	    port[5]->setPortName("ttyACM0");
        }

        for(i=0; i<6; i++)
        {
            qDebug() << port[i]->portName();
        }
    }

    if(cpuplat == "rk3568"){
        port[5] = port[0];
        port[0] = port[1];
        port[1] = port[5];
        port[5] = port[4];
        port[4] = port[3];
        port[3] = port[2];
        port[2] = port[1];
        port[1] = port[5];
        board = "RK3568";
    }

    QString str("#################\n");
    if(port[0]->isOpen())
        port[0]->close();
    if(port[0]->open(QIODevice::ReadWrite)) {
        port[0]->write(str.toLatin1());

        QTimer::singleShot(100, &eventloop,SLOT(quit()));
        eventloop.exec();

        port[0]->close();
    }

    if(cpuplat == "pi"){
        if(board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A"){
            QString audiostr("@@AUDIO\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(audiostr.toLatin1());

                QTimer::singleShot(100, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }
            RecordTest();
        }
    } else if(board == "AM335XBOARD"){

    }else{
        QString audiostr("@@AUDIO\n");
        if(port[0]->isOpen())
            port[0]->close();
        if(port[0]->open(QIODevice::ReadWrite)) {
            port[0]->write(audiostr.toLatin1());

            QTimer::singleShot(100, &eventloop,SLOT(quit()));
            eventloop.exec();

            port[0]->close();
        }
        // Test Record And Speaker
        RecordTest();
    }

    // send board information to master
    // format:
    // @@[Board Version]
    // like: @@CS10600RA4070
    // note: the whole characters don't bigger then 16
    QString boardstr("@@"+board+"\n");
    if(port[0]->isOpen())
        port[0]->close();
    if(port[0]->open(QIODevice::ReadWrite)) {
        port[0]->write(boardstr.toLatin1());
        qDebug() << "send RTCISOK";
        QTimer::singleShot(100, &eventloop,SLOT(quit()));
        eventloop.exec();
        qDebug() << "eventloop 100";

        port[0]->close();
        qDebug() << "board port close";
    }

    if(board == "CS10600RA4070" || board == "CS12800RA4101" || board == "LRRA4-101" || board == "CS12800RA4101A" || board == "CS12800RA4101BOX" || board == "CS12800RA4101P" || board == "CS19108RA4133P" || board == "CS10768RA4150P" || board == "CS19108RA4156P" || board == "CS19108RA4215P"){
        if(port[5] != NULL && port[5]->portName() == "ttyACM0")
        {
            qDebug() << "ZIGBEEISOK";
            QString zigbeestr("ZIGBEEISOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(zigbeestr.toLatin1());

                QTimer::singleShot(100, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }
        }else{
            qDebug() << "ZIGBEEISNOTOK";
            QString zigbeestr("ZIGBEEISNOTOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(zigbeestr.toLatin1());

                QTimer::singleShot(100, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }

        }
    }

    if(board == "CS12800PX101"){
        if(port[3] != NULL && port[3]->portName() == "ttyACM0")
        {
            qDebug() << "ZIGBEEISOK";
            QString zigbeestr("ZIGBEEISOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(zigbeestr.toLatin1());

                QTimer::singleShot(100, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }
        }else{
            qDebug() << "ZIGBEEISNOTOK";
            QString zigbeestr("ZIGBEEISNOTOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(zigbeestr.toLatin1());

                QTimer::singleShot(100, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }

        }
    }

    // RTC
    qDebug()<< GetComResult("find /dev/rtc0");
    if(GetComResult("find /dev/rtc0").remove(QChar('\n'),Qt::CaseInsensitive) == "/dev/rtc0") {
        QString rtcstr("RTCISOK\n");
        if(port[0]->isOpen())
            port[0]->close();
        if(port[0]->open(QIODevice::ReadWrite)) {
            port[0]->write(rtcstr.toLatin1());
            qDebug() << "send RTCISOK";
            QTimer::singleShot(500, &eventloop,SLOT(quit()));
            eventloop.exec();
            qDebug() << "eventloop 500";
            port[0]->close();
            qDebug() << "rtc close port";
        }
    }

    //For CS12800RA101 /dev/ttyUSB0 and /dev/ttyUSB1 test
    if(board == "CS12800RA101"){
        QString cmdstr = "/usr/hardwaretest/receive /dev/ttyUSB0 > /tmp/serial.txt &";
        system(cmdstr.toLocal8Bit());
        cmdstr = "/usr/hardwaretest/send /dev/ttyUSB1 &";
        system(cmdstr.toLocal8Bit());
        QTimer::singleShot(5000, &eventloop,SLOT(quit()));
        eventloop.exec();
        cmdstr = "/usr/hardwaretest/receive /dev/ttyUSB1 >> /tmp/serial.txt &";
        system(cmdstr.toLocal8Bit());
        cmdstr = "/usr/hardwaretest/send /dev/ttyUSB0 &";
        system(cmdstr.toLocal8Bit());
        QTimer::singleShot(5000, &eventloop,SLOT(quit()));
        eventloop.exec();
        if(GetFileValue("/tmp/serial.txt") == "/dev/ttyUSB0\n/dev/ttyUSB1\n"){
            qDebug() << "ttyUSB0 and ttyUSB1 is OK.";
            QString usbserialstr("TTYUSBISOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(usbserialstr.toLatin1());

                QTimer::singleShot(500, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }
        } else{
            qDebug() << "ttyUSB0 and ttyUSB1 is Not OK.";
            QString usbserialstr("TTYUSBISNOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(usbserialstr.toLatin1());

                QTimer::singleShot(500, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }
        }
    }

    //For CS12800R101P /dev/ttyS2 Debug port self test.
    if(board == "CS12800R101P"){
        QString cmdstr = "/usr/hardwaretest/receive /dev/ttyS2 > /tmp/serial.txt &";
        system(cmdstr.toLocal8Bit());
        cmdstr = "/usr/hardwaretest/send /dev/ttyS2 &";
        system(cmdstr.toLocal8Bit());
        QTimer::singleShot(5000, &eventloop,SLOT(quit()));
        eventloop.exec();
        if(GetFileValue("/tmp/serial.txt") == "/dev/ttyS2 Received 1000 data\n"){
            qDebug() << "ttyS2 is OK.";
            QString debugserialstr("TTYS2ISOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(debugserialstr.toLatin1());

                QTimer::singleShot(500, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }
        } else{
            qDebug() << "ttyS2 is Not OK.";
            QString debugserialstr("TTYS2ISNOK\n");
            if(port[0]->isOpen())
                port[0]->close();
            if(port[0]->open(QIODevice::ReadWrite)) {
                port[0]->write(debugserialstr.toLatin1());

                QTimer::singleShot(500, &eventloop,SLOT(quit()));
                eventloop.exec();

                port[0]->close();
            }
        }
    }

    thread.startSlave(&board);

}

void MainWindow::showRequest(const QString &s)
{
    ui->textBrowser_serial_text->setText("AutoTest Recieved Data:\n"
                                         +s);
}

void MainWindow::showcanRequest(const QString &s)
{
    ui->textBrowser_can_text->setText("AutoTest Recieved Data:\n"
                                      +s);
}

void MainWindow::displayInfo()
{
    if(GetFileValue(DLANLOGFILE).contains("Found RTL8153B")) {
        ui->textBrowser_network_text->clear();
        ui->textBrowser_network_text->setText(GetFileValue(DLANLOGFILE));
    }
}

void MainWindow::autotestInit()
{
#if 0
    if(cpuplat != "pi"){
        usbInit();
    }
    if(board !="CS10600RA4070" && board !="CS12800RA4101" && board != "LRRA4-101" && board !="CS12800RA4101A" && board !="CS12800RA4101BOX") {
        networkautotest();
    }
#endif
    if(GetFileValue(DLANLOGFILE).contains("Found RTL8153B")) {
        ui->textBrowser_network_text->clear();
        ui->textBrowser_network_text->setText(GetFileValue(DLANLOGFILE));
    }
    disconnect(serial,&QSerialPort::readyRead,this,&MainWindow::readDate);
    autoTesttimer = new QTimer(this);
    connect(autoTesttimer,SIGNAL(timeout()),SLOT(autoTest()));
    autoTesttimer->start(2000);

    displayinfotimer = new QTimer(this);
    connect(displayinfotimer,SIGNAL(timeout()),SLOT(displayInfo()));
    displayinfotimer->start(5000);
}

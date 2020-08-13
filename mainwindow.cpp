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
#include <QtNetwork/QHostAddress>

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
#define CANTEMPFILEPATH "/tmp/cantmp.txt"
#define SERIALTEMPFILEPATH "/tmp/serialtmp.txt"
#define TEMPFILEPATH "/tmp/tmp.txt"
#define TIMEOUT 2000

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
#define PIAUDIOPATH "/usr/hardwaretest/AudioTest.aac"
#define PICUEAUDIOPATH "/usr/hardwaretest/AutoTestFinish.aac"
#define PIVOLUMEPATH "name='PCM Playback Volume'"
#define PIBUZZERPATH "/dev/buzzer"
#define PIIPPATH "/usr/hardwaretest/ipaddr"
#define PIVIDEOPATH "/usr/hardwaretest/VideoTest.mp4"
#define PIBACKLIGHTPATH "/sys/class/gpio/gpio41/value"
#define PIMAXBACKLIGHTPATH "/sys/class/gpio/gpio41/value"
#define PIUSBDEBUGFILEPATH "/sys/kernel/debug/usb/devices"
#define RS485ENIO "34"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    audioflag = true;

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

    // GPIO
    gpioInit();

    // CAN
    canInit();

    // AutoTest, used for chipsee autotest
//    autotestInit();
}

MainWindow::~MainWindow()
{
    qDebug() << "I will gone";
    //if(board == "pi"){
        //QString cmdstr = "cp /usr/bin/vlc.bak /usr/bin/vlc && sync";
        //system(cmdstr.toLocal8Bit());
    //}
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
    QString pi = GetComResult("grep -c BCM2835 /proc/cpuinfo");
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
    } else if(pi.left(1) == "1"){
        if(cpucore.left(1) == "4"){
            plat = "pi";
        }
    }
    return plat;
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
    if(ui->radioButton_imx6q->isChecked()){
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
    }else if(ui->radioButton_imx6d->isChecked()){
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
    }else if(ui->radioButton_imx6u->isChecked()){
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
    }else if(ui->radioButton_am335x->isChecked()){
        board = "am3354";
    }else if(ui->radioButton_bbb_exp->isChecked()){
        board = "bbbexp";
    }else if(ui->radioButton_pi->isChecked()){
        board = "pi";
        ledpath = PILED0PATH;
        ledpath2 = PILED1PATH;
        audiopath = PIAUDIOPATH;
        cueaudiopath = PICUEAUDIOPATH;
        volumepath = PIVOLUMEPATH;
        buzzerpath = PIBUZZERPATH;
        videopath = PIVIDEOPATH;
        backlightpath = PIBACKLIGHTPATH;
        maxbacklightpath = PIMAXBACKLIGHTPATH;
        ipaddrpath = PIIPPATH;
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
    QString cpu = GetPlat();

    //qDebug() << cpu;

    if (cpu == "imx6q") {
        ui->radioButton_imx6q->setChecked(true);
        ui->radioButton_imx6d->setDisabled(true);
        ui->radioButton_imx6u->setDisabled(true);
        ui->radioButton_am335x->setDisabled(true);
        ui->radioButton_bbb_exp->setDisabled(true);
        ui->radioButton_pi->setDisabled(true);
    } else if (cpu == "imx6dl") {
        ui->radioButton_imx6q->setDisabled(true);
        ui->radioButton_imx6d->setChecked(true);
        ui->radioButton_imx6u->setDisabled(true);
        ui->radioButton_am335x->setDisabled(true);
        ui->radioButton_bbb_exp->setDisabled(true);
        ui->radioButton_pi->setDisabled(true);
    } else if (cpu == "imx6ul"){
        ui->radioButton_imx6q->setDisabled(true);
        ui->radioButton_imx6d->setDisabled(true);
        ui->radioButton_imx6u->setChecked(true);
        ui->radioButton_am335x->setDisabled(true);
        ui->radioButton_bbb_exp->setDisabled(true);
        ui->radioButton_pi->setDisabled(true);
    } else if (cpu == "pi"){
        ui->radioButton_imx6q->setDisabled(true);
        ui->radioButton_imx6d->setDisabled(true);
        ui->radioButton_imx6u->setDisabled(true);
        ui->radioButton_am335x->setDisabled(true);
        ui->radioButton_bbb_exp->setDisabled(true);
        ui->radioButton_pi->setChecked(true);

        /*change vlc to allow run for root*/
        //QString cmdstr = "cp /usr/bin/vlc.bak /usr/bin/vlc && sync";
        //system(cmdstr.toLocal8Bit());
        //cmdstr = "chmod a+w /usr/bin/vlc && sed 's/geteuid/getppid/g' /usr/bin/vlc";
        //system(cmdstr.toLocal8Bit());
    }

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
    if(board == "pi"){
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
    }else if(board == "am3354"){
    }else if (board == "bbbexp"){}
}
/*
 *  Audio
 *
 *  AudioTest() RecordTest() CueAudio() ChangeVolume() EnableBuzzer() DisableBuzzer() ChangeBuzzerState() audioInit()
 *
 *
 */

void MainWindow::AudioTest()
{
    QMessageBox::warning(this,"Tips","Press OK to Play Audio!");
    QString cmdstr = "";
    if(board == "pi") {
        system("killall vlc");
        cmdstr = "cvlc "+ audiopath + " vlc://quit" +"&";
    }else{
        system("killall gst-play-1.0");
        cmdstr = "gst-play-1.0 "+audiopath+" >/dev/null &";
    }
    system(cmdstr.toLocal8Bit());
}

void MainWindow::RecordTest()
{
    QMessageBox::warning(this,"Tips","Open Outside Audio First!!");
    if(board == "imx6q"){
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
    }
}

void MainWindow::CueAudio()
{
    if(audioflag) {
        audioflag = false;
        QString cmdstr = "";
        if(board == "pi") {
            cmdstr = "cvlc "+ cueaudiopath + " vlc://quit" +"&";
            //cmdstr = "aplay " + cueaudiopath + " >/dev/null &";
        }else{
            system("killall gst-play-1.0");
            cmdstr = "gst-play-1.0 "+cueaudiopath+" >/dev/null &";
        }
        system(cmdstr.toLocal8Bit());
        audioflag = true;
    }
    qDebug() << "CueAudio!";
}

void MainWindow::ChangeVolume()
{
    int volumevalue = ui->horizontalSlider_audio_volume->value();
    QString value = QString::number(volumevalue,10); // int to string
    QString cmdstr = "amixer cset "+volumepath+" "+value+"&";
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
    buzzerflag = true;
    connect(ui->pushButton_audio,&QPushButton::clicked,this,&MainWindow::AudioTest);
    connect(ui->pushButton_audio,&QPushButton::clicked,this,&MainWindow::ChangeVolume);
    connect(ui->pushButton_record,&QPushButton::clicked,this,&MainWindow::RecordTest);
    if(board == "imx6q" || board == "imx6d"){
        ui->horizontalSlider_audio_volume->setRange(60,127);
        ui->horizontalSlider_audio_volume->setValue(127);
    }else if(board == "imx6u"){
        ui->horizontalSlider_audio_volume->setRange(60,100);
        ui->horizontalSlider_audio_volume->setValue(100);
    }else if(board == "am3354"){
    }else if (board == "bbbexp"){
    }else if (board == "pi"){
        ui->pushButton_record->setVisible(false);
        ui->horizontalSlider_audio_volume->setRange(0,400);
        ui->horizontalSlider_audio_volume->setValue(0);
    }
    connect(ui->horizontalSlider_audio_volume,&QSlider::valueChanged,this,&MainWindow::ChangeVolume);

    ui->checkBox_buzzer->setChecked(false);
    connect(ui->checkBox_buzzer,SIGNAL(toggled(bool)),this,SLOT(ChangeBuzzerState()));
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
    if(board == "pi") {
        cmdstr = "cvlc -f "+ videopath + " vlc://quit" +"&";
    } else{
        system("killall gst-play-1.0");
        cmdstr = "gst-play-1.0 "+videopath+"&";
    }
    system(cmdstr.toLocal8Bit());
}

void MainWindow::ChangeBacklight()
{
    int backlightvalue = ui->horizontalSlider_backlight->value();
    QString value = QString::number(backlightvalue,10); // int to string
    if(board == "pi" && value == "0"){
        QMessageBox::warning(this,"Tips","If you trun off the backlight of LCD, It will back after 5 seconds!");
    }
    QString cmdstr = "echo "+value+" >"+backlightpath+"&";
    system(cmdstr.toLocal8Bit()); // int to const char*
    if(board == "pi"){
        if (value == "0"){
            Delay_MSec_Suspend(5000);
            cmdstr = "echo 1 >"+backlightpath+"&";
            system(cmdstr.toLocal8Bit()); // int to const char*
            ui->horizontalSlider_backlight->setValue(1);
        }

    }
}

int MainWindow::MaxBacklighValue()
{
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
    system("/usr/bin/ts_test > /dev/null");
}

void MainWindow::displayInit()
{
    connect(ui->pushButton_video,&QPushButton::clicked,this,&MainWindow::VideoTest);
    connect(ui->pushButton_lcdtest,&QPushButton::clicked,this,&MainWindow::LCDTest);
    //connect(ui->pushButton_touchtest,&QPushButton::clicked,this,&MainWindow::TouchTest);
    connect(ui->pushButton_video,&QPushButton::clicked,this,&MainWindow::ChangeVolume);

    ui->horizontalSlider_backlight->setRange(1,MaxBacklighValue());
    ui->horizontalSlider_backlight->setValue(GetBacklightValue());
    if(board == "pi"){
        ui->horizontalSlider_backlight->setRange(0,1);
        ui->horizontalSlider_backlight->setValue(1);
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
    if(board == "pi") {
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
    if(board == "pi") {
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
    if(board == "pi")
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
}

void MainWindow::getipInfo()
{
    QString cmdstr = "";
    if (board == "pi"){
        cmdstr = "echo `ifconfig eth0 | grep 'inet ' | grep -v '127.0.0.1'` >"+ipaddrpath;
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
    if(board == "pi"){
        ui->pushButton_wifiEnable->setVisible(false);
        ui->pushButton_wifiDisable->setVisible(false);
    }
}

void MainWindow::networkautotest()
{
    ipautotestTimer = new QTimer(this);
    connect(ipautotestTimer,SIGNAL(timeout()),this,SLOT(getipInfo()));
    ipautotestTimer->start(2000);
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
            line.contains("Mass Storage Device")||
            line.contains("USB Receiver")||
            line.contains("Keyboard Mouse")||
            line.contains("Wireless Optical Mouse")||
            line.contains("MXT USB Device")||
            line.contains("Mass-Storage")||
            line.contains("USB2.0-CRW")||
            line.contains("DataTraveler 2.0")||
            line.contains("USB Storage"))
    {
        CueAudio();
    }
}

void MainWindow::usbInit()
{
    usbautotestTimer = new QTimer(this);
    connect(usbautotestTimer,SIGNAL(timeout()),this,SLOT(getusbInfo()));
    usbautotestTimer->start(2000);
}

/*
 *  GPIO
 *
 *  getGPIOValueRaw(QString) setGPIOValueRaw(QString,QString) setGPIOModelRaw(QString)
 *  getGPIOValue(QString) setGPIOValue(QString,QString) setGPIOModel(QString)  getGPIOValue(QString) setGPIOOutStatu() setGPIOOutAllHigh()
 *
 *  setGPIOOutAllLow() gpioExport(QString) gpioInit()
 *
 */

QString MainWindow::getGPIOValueRaw(QString gpionum)
{
    QString gpiopath = QString("%1").arg("/sys/class/gpio/gpio")+gpionum+"/value";
    if(GetFileValue(gpiopath)==NULL){
        QMessageBox::critical(this,"Error","Open GpioPath Error!! Don't get GPIOValue!! Return NULL!!");
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
        QMessageBox::critical(this,"Error","Open GpioPath Error!! Don't get GPIOValue!! Return NULL!!");
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
    QString cmdstr = "canconfig "+cannum+" stop && canconfig "+cannum+" bitrate 10000 ctrlmode triple-sampling on loopback off && canconfig "+cannum+" start && candump "+cannum+" >"+QString("%1").arg(CANTEMPFILEPATH)+"&";
    system(cmdstr.toLocal8Bit());
    canReceiveTimer->start(50);
    ui->textBrowser_can_text->clear();
    ui->textBrowser_can_text->setText(currenttime+"\n"+cannum+" Started!!");
    //QMessageBox::critical(this,"M",cmdstr);
}

void MainWindow::canSend()
{
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cannum = ui->comboBox_canNum->currentText();
    QString cmdstr = "cansend "+ui->comboBox_canNum->currentText()+" "+QString("%1").arg(CANSENDDATA);
    system(cmdstr.toLocal8Bit());
    ui->textBrowser_can_text->clear();
    ui->textBrowser_can_text->setText(currenttime+"\n"+cannum+" Sended: "+QString("%1").arg(CANSENDDATA));
    //QMessageBox::critical(this,"M",cmdstr);
}

void MainWindow::canStop()
{
    QString currenttime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cannum = ui->comboBox_canNum->currentText();
    QString cmdstr = "canconfig "+cannum+" stop";
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

void MainWindow::canInit()
{
    // UI
    if(board == "imx6q" || board == "imx6d" || board == "imx6u"){
        ui->comboBox_canNum->addItem("can0","can0");
        ui->comboBox_canNum->addItem("can1","can1");
        ui->comboBox_canNum->addItem("Custum");
    }else if(board == "pi"){
        ui->comboBox_canNum->setDisabled(true);
        ui->textBrowser_can_text->setText("This device don't support CAN Bus.");
    }else{
        ui->comboBox_canNum->addItem("can0","can0");
        ui->comboBox_canNum->addItem("Custum");
    }
    connect(ui->comboBox_canNum, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, &MainWindow::checkCustomCanNumPolicy);

    // Button
    ui->pushButton_canSend->setDisabled(true);
    ui->pushButton_canStop->setDisabled(true);
    if(board == "pi"){
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
    autoTesttimer->stop();
    // Test Beeper And Led
    QEventLoop eventloop;
    for(int i=0; i<2; i++)
    {
        EnableBuzzer();
        OpenLed();
        OpenLed2();

        QTimer::singleShot(1000, &eventloop,SLOT(quit()));
        eventloop.exec();

        DisableBuzzer();
        CloseLed();
        CloseLed2();

        QTimer::singleShot(1000, &eventloop,SLOT(quit()));
        eventloop.exec();
    }

    if(board == "pi"){

    } else {
        // Test Record And Speaker
        RecordTest();

        // Test Serial and CAN
        connect(&thread, &SlaveThread::canrequest, this,&MainWindow::showcanRequest);
    }
    connect(&thread, &SlaveThread::request, this,&MainWindow::showRequest);
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

void MainWindow::autotestInit()
{
    usbInit();
    networkautotest();
    disconnect(serial,&QSerialPort::readyRead,this,&MainWindow::readDate);
    autoTesttimer = new QTimer(this);
    connect(autoTesttimer,SIGNAL(timeout()),SLOT(autoTest()));
    autoTesttimer->start(2000);
}

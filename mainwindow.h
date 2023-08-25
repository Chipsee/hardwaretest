#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QDateTime>
#include "slavethread.h"
#include "device.h"
#include "lcdtester.h"

namespace Ui {
class MainWindow;
}

class timedialog;
class QIntValidator;
class DeviceDiscoveryDialog;
class LCDTester;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    ~MainWindow();

private slots:
    void SetCurrentTime();
    void syncTime();
    void OpenLed();
    void CloseLed();
    void OpenLed2();
    void CloseLed2();
    void AudioTest();
    void RecordTest();
    void ChangeVolume();
    void EnableBuzzer();
    void DisableBuzzer();
    void ChangeBuzzerState();
    void Enable2Can();
    void Disable2Can();
    void ChangeCanState();
    void RelayNC();
    void RelayNO();
    void ChangeRelayState();
    void VideoTest();
    void LCDTest();
    void TouchTest();
    void ChangeBacklight();
    int MaxBacklighValue();
    int GetBacklightValue();
    void checkCustomBaudRatePolicy(int);
    void checkCustomDevicePathPolicy(int);
    void updateSettings();
    void openSerialPort();
    void closeSerialPort();
    void clearSerialText();
    void rs485Init();
    void rs485SendEnable(bool);
    void readDate();
    void writeDate();
    void BluetoothTest();
    void wifiEnable();
    void wifiDisable();
    void wifiInfoDisplay();
    QString getGPIOValue(QString);
    void setGPIOValue(QString,QString);
    void setGPIOModel(QString,QString);
    QString getGPIOValueRaw(QString);
    void setGPIOValueRaw(QString,QString);
    void setGPIOModelRaw(QString,QString);
    void setGPIOInStatu();
    void setGPIOOutStatu();
    void setGPIOOutAllHigh();
    void setGPIOOutAllLow();
    void checkCustomCanNumPolicy(int);
    void canStart();
    void canSend();
    void canStop();
    void canReceive();
    void checkCustom4gNumPolicy(int idx);
    void mobile4gEnable();
    void mobile4gDisable();
    void readGPSData();
    void gpsEnable();
    void gpsDisable();
    void gpsInit();
    void autoTest();
    void getusbInfo();
    void getipInfo();
    void AudioLoop();
    void VideoLoop();
    void readalltestresult();
    void alltest();
    void alltestInit();
    void displayInfo();


private:
    Ui::MainWindow *ui;

    QString ledpath;
    QString ledpath2;
    QString audiopath;
    QString cueaudiopath;
    QString videopath;
    QString buzzerpath;
    QString relaypath;
    QString volumepath;
    QString backlightpath;
    QString maxbacklightpath;
    QString board;
    QString cpuplat;
    QString ipaddrpath;
    QByteArray autodata;
    QIntValidator *intValidator;
    timedialog *timeset;
    DeviceDiscoveryDialog *bluetoothdialog;
    LCDTester *lcd;
    QTimer *setTimeTimer;
    QTimer *gpioInStatuTimer;
    QTimer *gpioOutStatuTimer;
    QTimer *serialReceiveTimer;
    QTimer *canReceiveTimer;
    QTimer *usbautotestTimer;
    QTimer *ipautotestTimer;
    QSerialPort *serial;
    QSerialPort *serial1;
    QSerialPort *serial2;
    QSerialPort *serial3;
    QSerialPort *serial4;
    QSerialPort *atport;
    QSerialPort *gpsport;
    QTimer *autoTesttimer;
    QTimer * readTimer;
    QTimer * writeTimer;
    QTimer * gpsreadTimer;
    QTimer * audioloopTimer;
    QTimer * videoloopTimer;
    QTimer * alltestTimer;
    QTimer *displayinfotimer;
    SlaveThread thread;
    bool buzzerflag;
    bool relayflag;
    bool audioflag;
    bool canflag;
    bool autoflag;
    bool vautoflag;
    int lcdwidth;
    int lcdheight;

    struct serailSettings {
            QString name;
            qint32 baudRate;
            QString stringBaudRate;
            QSerialPort::DataBits dataBits;
            QString stringDataBits;
            QSerialPort::Parity parity;
            QString stringParity;
            QSerialPort::StopBits stopBits;
            QString stringStopBits;
            QSerialPort::FlowControl flowControl;
            QString stringFlowControl;
        };
    serailSettings currentSettings;

    QString gpioInArray[4];
    QString gpioOutArray[4];

    void BoardSetting();
    QString GetTempFileValue();
    QString GetFileValue(QString);
    QString GetComResult(QString);
    QString GetPlat();
    QString GetResolution();
    QString GetPiBoard();
    QString GetBoard();
    QString GetDebianCodeName();
    QString GetKernelVersion();
    void Delay_MSec_Suspend(int);
    void fillPortsParameters();
    void gpioExport(QString);
    void screenInit();
    void boardInit();
    void dateTimeInit();
    void ledInit();
    void audioInit();
    void CueAudio();
    void displayInit();
    void serialInit();
    void networkInit();
    void networkautotest();
    void usbInit();
    void gpioInit();
    void canInit();
    void autotestInit();
    void mobile4gInit();
    void showRequest(const QString &s);
    void showcanRequest(const QString &s);

};

#endif // MAINWINDOW_H

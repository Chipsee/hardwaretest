#include "timedialog.h"
#include "ui_timedialog.h"
#include <QProcess>

timedialog::timedialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::timedialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(SetTime()));
}

timedialog::~timedialog()
{
    delete ui;
}

void timedialog::SetTime()
{
#if 0
    QString timestr = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmdstr = "date -s \""+timestr+"\"";
    system(cmdstr.toLocal8Bit());//system use "const char*",not string.
    system("hwclock -w");
#endif
#if 1
    QString timestr = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    system("timedatectl set-ntp 0");

    QProcess process;
    process.start("sudo", QStringList() << "date" << "-s" << timestr);
    process.waitForFinished();
    process.start("sudo", QStringList() << "hwclock" << "-w");
    process.waitForFinished();

    if (process.exitCode() != 0) {
        qDebug() << "Date set Error:" << process.readAllStandardError();
    }
    system("timedatectl set-ntp 1");
#endif
}

void timedialog::ShowCurrentTime()
{
    QDateTime datetime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(datetime);
    this->show();
}

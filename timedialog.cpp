#include "timedialog.h"
#include "ui_timedialog.h"


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
    QString timestr = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmdstr = "date -s \""+timestr+"\"";
    system(cmdstr.toLocal8Bit());//system use "const char*",not string.
    system("hwclock -w");
}

void timedialog::ShowCurrentTime()
{
    QDateTime datetime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(datetime);
    this->show();
}

#include "epflash.h"
#include "ui_epflash.h"
#include <QDebug>
#include <QMessageBox>

epflash::epflash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::epflash),
    epsettings(new QSettings("Chipsee", "devicesize", this))
{
    ui->setupUi(this);

    if(epsettings->value("Device", "").toString() == "") {
        selectRadioButton(ui->buttonGroup_device_size, "7");
        epsettings->setValue("Device","7");
    } else {
        selectRadioButton(ui->buttonGroup_device_size, epsettings->value("Device", "").toString());
    }

    if(epsettings->value("SameDevice","").toString() == "") {
        epsettings->setValue("SameDevice","NO");
        ui->checkBox_same_device->setChecked(false);
        samedeviceflag = false;
    } else if (epsettings->value("SameDevice","").toString() == "YES") {
        ui->checkBox_same_device->setChecked(true);
        samedeviceflag = true;
    } else if (epsettings->value("SameDevice","").toString() == "NO") {
        ui->checkBox_same_device->setChecked(false);
        samedeviceflag = false;
    }

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(SaveDeviceSize()));
    connect(&utils, &Utils::commandFinished, this, &epflash::onCommandFinished);
    connect(ui->checkBox_same_device,SIGNAL(toggled(bool)),this,SLOT(ChangeSameDeviceState()));
}

epflash::~epflash()
{
    delete ui;
}

void epflash::ChangeSameDeviceState(){
    if(ui->checkBox_same_device->isChecked()){
        epsettings->setValue("SameDevice","YES");
        samedeviceflag = true;
    } else {
        epsettings->setValue("SameDevice","NO");
        samedeviceflag = false;
    }
}

void epflash::modifyButtonId(QAbstractButton *button, int newId) {
    ui->buttonGroup_device_size->removeButton(button);
    ui->buttonGroup_device_size->addButton(button, newId);
}
void epflash::ShowEpflashDiaManual()
{
    this->show();

    modifyButtonId(ui->radioButton_050, 1);
    modifyButtonId(ui->radioButton_070, 2);
    modifyButtonId(ui->radioButton_070D, 3);
    modifyButtonId(ui->radioButton_101, 4);
    modifyButtonId(ui->radioButton_101A, 5);
    modifyButtonId(ui->radioButton_121, 6);
    modifyButtonId(ui->radioButton_133, 7);
    modifyButtonId(ui->radioButton_150, 8);
    modifyButtonId(ui->radioButton_156, 9);
    modifyButtonId(ui->radioButton_170, 10);
    modifyButtonId(ui->radioButton_185, 11);
    modifyButtonId(ui->radioButton_190, 12);
    modifyButtonId(ui->radioButton_215, 13);
    modifyButtonId(ui->radioButton_236, 14);

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    connect(ui->buttonGroup_device_size, QOverload<int>::of(&QButtonGroup::idClicked), this, &epflash::onButtonClicked);
#else
    connect(ui->buttonGroup_device_size, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &epflash::onButtonClicked);
#endif
}

void epflash::ShowEpflashDia()
{
    if(samedeviceflag) {
        SaveDeviceSize();
        return;
    }

    ShowEpflashDiaManual();
}

void epflash::selectRadioButton(QButtonGroup* group, const QString& name) {
    for (QAbstractButton* button : group->buttons()) {
        if (button->text() == name) {
            button->setChecked(true);
            break;
        }
    }
}

void epflash::onButtonClicked(int id) {
    QAbstractButton *button = ui->buttonGroup_device_size->button(id);
    if (button) {
        QString dev = button->text();
        //qDebug() << "Button with ID" << id << "and text" << dev << "clicked.";
        epsettings->setValue("Device",dev);
    }
}

void epflash::onCommandFinished(bool success, const QString &errorString){
        if (success) {
            qDebug() << "Command finished successfully.";
        } else {
            qDebug() << "Command failed with error:" << errorString;
            // The size is not final size
            // The final size will be changed by the contents of message
            // Get the final size from log:
            // MessageBox final size: QSize(306, 84) comes from utils.cpp showCustomMessage()
            utils.showCustomMessage("Error","EEPROM Flash Error", "EEPROM Flash with error: " + errorString, 306, 84);
        }
}

void epflash::SaveDeviceSize(){

    QString dev = epsettings->value("Device", "").toString();
    QString cmdstr = "fe " + dev;
    utils.executeCommand(cmdstr);
    qDebug() << cmdstr;
}

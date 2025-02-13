#ifndef EPFLASH_H
#define EPFLASH_H

#include <QDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSettings>
#include "utils.h"

namespace Ui {
class epflash;
}

class epflash : public QDialog
{
    Q_OBJECT

public:
    explicit epflash(QWidget *parent = nullptr);
    ~epflash();
    void ShowEpflashDia();
    void ShowEpflashDiaManual();
    void modifyButtonId(QAbstractButton *button, int newId);
    void selectRadioButton(QButtonGroup* group, const QString& name);

public slots:
    void ChangeSameDeviceState();

private:
    Ui::epflash *ui;
    Utils utils;
    bool samedeviceflag;

private slots:
    void onButtonClicked(int id);
    void SaveDeviceSize();
    void onCommandFinished(bool success, const QString &errorString);

public:
    QSettings *epsettings;
};

#endif // EPFLASH_H

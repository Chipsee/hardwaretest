#ifndef LCDTESTER_H
#define LCDTESTER_H

#include <QDialog>
#include <QColor>
#include <QTouchEvent>

namespace Ui {
class LCDTester;
}

class LCDTester : public QDialog
{
    Q_OBJECT

public:
    explicit LCDTester(QWidget *parent = 0);
    ~LCDTester();

protected:
    // Override the touchEvent method to handle touch events
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *event) override;

public slots:
    void LCDTesterShow();

private:
    // Declare a list of colors to test
    QList<QColor> colors;
    Ui::LCDTester *ui;
    int index;
    int lcdwidth;
    int lcdheight;
};

#endif // LCDTESTER_H

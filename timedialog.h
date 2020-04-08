#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

#include <QDialog>

namespace Ui {
class timedialog;
}

class timedialog : public QDialog
{
    Q_OBJECT

public:
    explicit timedialog(QWidget *parent = 0);
    ~timedialog();

private slots:
    void SetTime();

public slots:
    void ShowCurrentTime();

private:
    Ui::timedialog *ui;

};

#endif // TIMEDIALOG_H

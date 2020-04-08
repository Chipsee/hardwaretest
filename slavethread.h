#ifndef SLAVETHREAD_H
#define SLAVETHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QTimer>

class SlaveThread : public QThread
{
    Q_OBJECT

public:
    SlaveThread(QObject *parent = 0);
    ~SlaveThread();

    void startSlave();
    void run();

signals:
    void request(const QString &s);
    void canrequest(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    bool quit;
    QWaitCondition cond;
};

#endif // SLAVETHREAD_H

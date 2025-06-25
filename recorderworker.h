#ifndef RECORDERWORKER_H
#define RECORDERWORKER_H

#include <QObject>
#include <QString>

class RecorderWorker : public QObject {
    Q_OBJECT
public:
    explicit RecorderWorker(QObject *parent = nullptr);

public slots:
    void recordAndPlay(QStringList recordArgs, QStringList playArgs); // Main logic function

signals:
    void finished();       // Emitted when the task is done
    void status(QString);  // Emit status messages to UI
};

#endif // RECORDERWORKER_H

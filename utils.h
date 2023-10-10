#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QProcess>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);
    void executeCommand(const QString &command);

signals:
    void commandFinished(bool success, const QString &errorString);

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onError(QProcess::ProcessError error);
};

#endif // UTILS_H

#include "utils.h"

Utils::Utils(QObject *parent)
    : QObject{parent}
{

}

void Utils::executeCommand(const QString &command) {
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Utils::onFinished);
    connect(process, &QProcess::errorOccurred,
            this, &Utils::onError);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    process->startCommand(command);
#else
    process->start(command);
#endif
}

void Utils::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        emit commandFinished(true, QString());
    } else {
        emit commandFinished(false, process->readAllStandardError());
    }
    process->deleteLater();
}

void Utils::onError(QProcess::ProcessError error) {
    Q_UNUSED(error)
    QProcess *process = qobject_cast<QProcess *>(sender());
    emit commandFinished(false, process->errorString());
    process->deleteLater();
}

#include "utils.h"

Utils::Utils(QObject *parent)
    : QObject{parent}
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    lcdWidth = screenGeometry.width();
    lcdHeight = screenGeometry.height();
#else
    QDesktopWidget* desktopWidget = QApplication::desktop();
    lcdWidth = desktopWidget->width();
    lcdHeight = desktopWidget->height();
#endif

    qDebug() << "Utils==Screen is:" + QString::number(lcdWidth) + "x" +QString::number(lcdHeight);
}

void Utils::executeCommand(const QString &command) {
    QProcess *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Utils::onFinished);
    connect(process, &QProcess::errorOccurred,
            this, &Utils::onError);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    process->startCommand(command);
#elif QT_VERSION >= QT_VERSION_CHECK(5,15,8)
    QStringList list = command.split(" ");
    QString firstElement;
    QStringList otherElements;
    if(!list.isEmpty()){
        firstElement = list.first();
    }
    if(list.size() > 1) {
        otherElements = list.mid(1);
    } else {
        otherElements = QStringList() << "-c" << firstElement;
        firstElement = "/bin/sh";
    }

    process->start(firstElement, otherElements);
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

void Utils::showCustomMessage(const QString &msgtype, const QString &title,const QString &message, int width, int height) {
    QMessageBox msgBox;
    if (msgtype == "Error") {
        msgBox.setIcon(QMessageBox::Critical);
    } else if (msgtype == "Info") {
        msgBox.setIcon(QMessageBox::Information);
    } else if (msgtype == "Warn") {
        msgBox.setIcon(QMessageBox::Warning);
    } else {
        msgBox.setIcon(QMessageBox::NoIcon);
        qDebug() << "Support msgtype Error, Info, Warn";
    }
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);

    msgBox.setMinimumSize(width,height);
    msgBox.setMaximumSize(width,height);

    int x = (lcdWidth - msgBox.width()) / 2;
    int y = (lcdHeight - msgBox.height()) / 2;

    msgBox.move(x,y);
    msgBox.exec();

    QSize size = msgBox.size();
    qDebug() << "MessageBox final size:" << size;
}

void Utils::checkAndFixOwnershipAsync(const QString &path,
                                      const QString &targetUser,
                                      const QString &targetGroup)
{
        QFileInfo fileInfo(path);

        if (!fileInfo.exists()) {
            qCritical() << "File does not exist:" << path;
            return;
        }

        // Retrieve current user and group via `stat`
        QProcess idProcess;
        idProcess.start("stat", QStringList() << "-c" << "%U %G" << path);
        idProcess.waitForFinished();
        QStringList output = QString(idProcess.readAllStandardOutput()).trimmed().split(" ");

        if (output.size() != 2) {
            qCritical() << "Failed to retrieve user/group info for" << path;
            return;
        }

        QString currentUser = output[0];
        QString currentGroup = output[1];

        if (currentUser != targetUser || currentGroup != targetGroup) {
            QString chownCommand = targetUser + ":" + targetGroup;
            QProcess chownProcess;
            chownProcess.start("chown", QStringList() << chownCommand << path);
            chownProcess.waitForFinished();

            if (chownProcess.exitStatus() != QProcess::NormalExit || chownProcess.exitCode() != 0) {
                qCritical() << "Failed to change owner/group of" << path << ":" << chownProcess.readAllStandardError();
                return;
            }

            qInfo() << "Changed owner/group of" << path << "to" << targetUser << ":" << targetGroup;
        }
}

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QProcess>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QGuiApplication>
#include <QScreen>
#else
#include <QApplication>
#include <QDesktopWidget>
#endif
#include <QMessageBox>
#include <QDebug>

#include <QFileInfo>
#include <QStringList>
#include <QLoggingCategory>
#include <QFile>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);
    void executeCommand(const QString &command);
    void showCustomMessage(const QString &msgtype, const QString &title,const QString &message, int width, int height);
    void checkAndFixOwnershipAsync(const QString &path,
                                          const QString &targetUser,
                                          const QString &targetGroup);
    int lcdWidth;
    int lcdHeight;

signals:
    void commandFinished(bool success, const QString &errorString);

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onError(QProcess::ProcessError error);
};

#endif // UTILS_H

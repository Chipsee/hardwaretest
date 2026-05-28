#ifndef SHELLWORKER_H
#define SHELLWORKER_H

#include <QObject>
#include <QString>

/**
 * @brief Worker class to execute shell commands in a separate thread
 *        and send real-time output via status signals
 */
class ShellWorker : public QObject
{
    Q_OBJECT
public:
    explicit ShellWorker(QObject *parent = nullptr);

signals:
    void status(const QString &msg);
    void finished();
    void error(const QString &err);

public slots:
    void execshell(const QStringList &args);

private slots:
    void onReadStdOutput();
    void onReadStdError();
};

#endif // SHELLWORKER_H

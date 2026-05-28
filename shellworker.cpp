#include "shellworker.h"
#include <QProcess>
#include <QThread>

ShellWorker::ShellWorker(QObject *parent) : QObject(parent)
{
}

void ShellWorker::execshell(const QStringList &args)
{
    // Send initial status message
    emit status("Start Execute shell script");
    emit status("");

    // Create QProcess instance in the worker thread
    QProcess process;
    process.setProcessChannelMode(QProcess::SeparateChannels);

    // Connect output signals to read slots
    connect(&process, &QProcess::readyReadStandardOutput,
            this, &ShellWorker::onReadStdOutput);
    connect(&process, &QProcess::readyReadStandardError,
            this, &ShellWorker::onReadStdError);

    // Start the bash process with given arguments
    process.start("/bin/bash", args);

    // Wait for process to finish (non-blocking for UI thread)
    if (!process.waitForFinished(-1)) {
        emit error("Execution timeout or process failed");
        emit finished();
        return;
    }

    // Check process exit code
    int exitCode = process.exitCode();
    if (exitCode != 0) {
        emit error(QString("Command failed with exit code: %1").arg(exitCode));
    } else {
        emit status("Execution successfully");
    }

    // Notify that task is done
    emit finished();
}

void ShellWorker::onReadStdOutput()
{
    // Get the sender process object
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (process) {
        // Read all standard output and send via status signal
        QString output = process->readAllStandardOutput().trimmed();
        if (!output.isEmpty()) {
            emit status(output);
        }
    }
}

void ShellWorker::onReadStdError()
{
    // Get the sender process object
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (process) {
        // Read all standard error and send via status signal
        QString err = process->readAllStandardError().trimmed();
        if (!err.isEmpty()) {
            emit status(err);
        }
    }
}

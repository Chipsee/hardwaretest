#include "recorderworker.h"
#include <QProcess>
#include <QFile>
#include <QThread>

RecorderWorker::RecorderWorker(QObject *parent) : QObject(parent) {}

void RecorderWorker::recordAndPlay(QStringList recordArgs, QStringList playArgs) {
    QString outputFile = "/tmp/output.wav";
    QFile::remove(outputFile);

    emit status("Recording...");
    QProcess::execute("arecord", recordArgs);

    QThread::sleep(1);  // Optional delay

    emit status("Playing...");
    QProcess::execute("aplay", playArgs);

    emit status("Playback finished");
    emit finished();
}


#include <QDebug>
#include <QMessageBox>

#include "rrwc.hpp"
#include "globals.hpp"

Rrwc::Rrwc() {
    connect(&m_outputManager, SIGNAL(progressChanged(int)),
            this, SLOT(onProgressChanged(int)));
    connect(&m_outputManager, SIGNAL(statusChanged(QString)),
            this, SLOT(onStatusChanged(QString)));
    connect(&m_outputManager, SIGNAL(done()),
            this, SLOT(onDone()));
    connect(&m_outputManager, SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));
    connect(&m_outputManager, SIGNAL(entryListFull()),
            this, SLOT(slotEntryListFull()));
}

void Rrwc::go(const QString &inputPath, const QString &sort) {
    emit writeLog(LOG_ERROR, LOG_CLEAR);
    emit writeLog(LOG_PROGRESS, LOG_CLEAR);
    m_totalTime.start();
    ErrorManager errorManager(m_outputManager);
    connect(&errorManager, SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));
    if (errorManager.checkInputErrors(inputPath)) {
        emit started();
        m_outputManager.fillEntryList(inputPath, sort);
    } else {
        QMessageBox::warning(0, "Configuration Error", "Check the error log for details.");
    }
}

void Rrwc::slotEntryListFull() {
    emit writeLog(LOG_PROGRESS, "Starting outputs...");
    for (int i = 0; i < m_outputManager.outputs().size(); i++) {
        m_outputManager.startOutput(i);
    }
    emit writeLog(LOG_PROGRESS, "All outputs are active.");

}

void Rrwc::onWriteLog(QString log, QString str) {
    emit writeLog(log, str);
}

void Rrwc::onProgressChanged(int progress) {
    emit progressChanged(progress);
}

void Rrwc::onStatusChanged(QString status) {
    emit statusChanged(status);
}

void Rrwc::onDone() {
    emit writeLog(LOG_PROGRESS, "Cleaning output manager");
    m_outputManager.clean();
    emit writeLog(LOG_PROGRESS, QString("Total time task: %1 ms").arg(m_totalTime.elapsed()));
    emit done(100);
}

Rrwc::~Rrwc() {
}

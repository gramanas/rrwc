#include <QDebug>
#include <QMessageBox>

#include "rrwc.hpp"
#include "globals.hpp"

Rrwc::Rrwc() {
    connect(&m_outputManager, SIGNAL(progressChanged(int)),
            this, SLOT(onProgressChanged(int)));
    connect(&m_outputManager, SIGNAL(done()),
            this, SLOT(onDone()));
    connect(&m_outputManager, SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));
}

void Rrwc::go(const QString &inputPath, const QString &sort) {
    emit writeLog(LOG_ERROR, LOG_CLEAR);
    emit writeLog(LOG_PROGRESS, LOG_CLEAR);
    ErrorManager errorManager(m_outputManager);
    connect(&errorManager, SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));
    if (errorManager.checkInputErrors(inputPath)) {
        emit writeLog(LOG_PROGRESS, "Starting outputs...");
        for (int i = 0; i < m_outputManager.outputs().size(); i++) {
            m_outputManager.startOutput(i, inputPath, sort);
        }
        emit writeLog(LOG_PROGRESS, "All outputs are active.");
        emit started();
    } else {
        QMessageBox::warning(0, "Configuration Error", "Check the error log for details.");
    }
}

void Rrwc::onWriteLog(QString log, QString str) {
    emit writeLog(log, str);
}

void Rrwc::onProgressChanged(int progress) {
    emit progressChanged(progress);
}

void Rrwc::onDone() {
    emit writeLog(LOG_PROGRESS, "Cleaning output manager");
    m_outputManager.clean();
    emit done(100);
}

Rrwc::~Rrwc() {
}

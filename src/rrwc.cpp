#include <QDebug>
#include <QMessageBox>

#include "rrwc.hpp"
#include "globals.hpp"

Rrwc::Rrwc() {
  connect(&m_outputManager, SIGNAL(statusChanged(QString)),
          this, SLOT(onStatusChanged(QString)));
  connect(&m_outputManager, SIGNAL(done()),
          this, SLOT(onDone()));
  connect(&m_outputManager, SIGNAL(entryListFull()),
          this, SLOT(slotEntryListFull()));
}

void Rrwc::clearLogs() {
  //emit writeLog(LOG_ERROR, LOG_CLEAR);
  //emit writeLog(LOG_PROGRESS, LOG_CLEAR);
}

void Rrwc::go(const QString &inputPath, const QString &sort, const int &threadNumber) {
  m_threadNumber = threadNumber;
  clearLogs();
  m_totalTime.start();

  ErrorManager errorManager(m_outputManager);

  if (errorManager.checkInputErrors(inputPath)) {
    emit started();
    m_outputManager.fillEntryList(inputPath, sort);
  }
  else {
    QMessageBox::warning(0, "Configuration Error", "Check the error log for details.");
  }
}

void Rrwc::slotEntryListFull() {
  //emit writeLog(LOG_PROGRESS, "Starting outputs...");
  m_outputManager.startOutputs(m_threadNumber);
  //emit writeLog(LOG_PROGRESS, "All outputs are active.");
}

void Rrwc::onStatusChanged(QString status) {
  emit statusChanged(status);
}

void Rrwc::onDone() {
  //emit writeLog(LOG_PROGRESS, "Freeing up memory...");
  m_outputManager.clean();
  //emit writeLog(LOG_PROGRESS, TIME_TOTAL.arg(m_totalTime.elapsed()));
  emit done();
}

Rrwc::~Rrwc() {
}

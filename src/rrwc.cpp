#include <QDebug>
#include <QMessageBox>

#include "rrwc.hpp"
#include "globals.hpp"

Rrwc::Rrwc()
  : m_outputManager(&m_logger) {
  connect(&m_outputManager, SIGNAL(done()),
          this, SLOT(onDone()));
  connect(&m_outputManager, SIGNAL(entryListFull()),
          this, SLOT(slotEntryListFull()));
}

void Rrwc::go(const QString &inputPath, const QString &sort, const int &threadNumber) {
  m_threadNumber = threadNumber;
  m_totalTime.start();
  m_logger.setItemsDone(0);

  ErrorManager errorManager(m_outputManager, &m_logger);

  if (errorManager.checkInputErrors(inputPath)) {
    emit started();
    m_outputManager.fillEntryList(inputPath, sort);
  }
  else {
    QMessageBox::warning(0, "Configuration Error", "Check the error log for details.");
  }
}

void Rrwc::slotEntryListFull() {
  m_logger.log("Starting outputs...");
  m_outputManager.startOutputs(m_threadNumber);
  m_logger.log("All outputs are active.");
}

void Rrwc::onDone() {
  m_logger.log("Freeing up memory...");
  m_outputManager.clean();
  m_logger.log(TIME_TOTAL.arg(m_totalTime.elapsed()));
  emit done();
}

Rrwc::~Rrwc() {
}

#ifndef RRWC_HPP
#define RRWC_HPP

#include <QString>
#include <QTime>
#include <QApplication>

#include "output/outputmanager.hpp"
#include "error/errormanager.hpp"
#include "logger.hpp"


// TODO: Add cli support, initializing the same stuff
// but without the GUI compartments

class Rrwc : public QObject {
  Q_OBJECT
public:
  Rrwc();
  ~Rrwc();

  OutputManager * outputManager() {
    return &m_outputManager;
  }

  Logger * logger() {
    return &m_logger;
  }

  void go(const QString &inputPath,
          const QString &sort,
          const int &threadNumber);

public slots:
  void slotEntryListFull();
  void onDone();

signals:
  void started();
  void done();

private:
  Logger m_logger;
  int m_threadNumber;
  OutputManager m_outputManager;
  QTime m_totalTime;
};

#endif // RRWC_HPP

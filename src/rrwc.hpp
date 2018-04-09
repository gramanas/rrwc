#ifndef RRWC_HPP
#define RRWC_HPP

#include <QString>
#include <QTime>
#include <QApplication>

#include "output/outputmanager.hpp"
#include "error/errormanager.hpp"


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

  void go(const QString &inputPath,
          const QString &sort,
          const int &threadNumber);

public slots:
  void onStatusChanged(QString status);
  void slotEntryListFull();
  void onDone();

signals:
  void statusChanged(QString status);
  void started();
  void done();

private:
  void clearLogs();

  int m_threadNumber;
  OutputManager m_outputManager;
  QTime m_totalTime;
};

#endif // RRWC_HPP

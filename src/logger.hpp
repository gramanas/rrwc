#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QMutexLocker>
#include <QAtomicInteger>
#include <QCoreApplication>
#include <QString>
#include <QTimer>

class Logger {
public:
  Logger();
  ~Logger();

  void log(QString str);
  void err(QString str);
  void status(QString str);
  void incrementItemsDone();
  void setItemsDone(uint n);
  void setTotal(uint n);
  void setTuiMode() { m_isTui = true; }
  bool tuiMode() { return m_isTui; }
  void killTui();
  void done() { setItemsDone(1); setTotal(1); }

  QString flushLog();
  QString flushErr();
  QString flushStatus();

  bool tuiDead();
  uint getProgress();

private:
  void append(QMutex &m, QString &s, const QString &n);
  void replace(QMutex &m, QString &s, const QString &n);
  QString flush(QMutex &m, QString &s);

  QString makeProgress(const QString &n);

  QMutex m_errMutex;
  QMutex m_logMutex;
  QMutex m_pBarStatusMutex;
  QString m_errors;
  QString m_logs;
  QString m_pBarStatus;

  QAtomicInteger<uint> m_itemsDone = 0;
  QAtomicInteger<uint> m_total = 0;

  QMutex m_killMutex;
  bool m_killTui = false;
  bool m_isTui = false;

  QTimer *p_timer;
  QCoreApplication *p_app;
};

#endif // LOGGER_HPP

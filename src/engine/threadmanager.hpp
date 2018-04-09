#ifndef THREADMANAGER_HPP
#define THREADMANAGER_HPP

#include "output/entrylist.hpp"
#include "output/output.hpp"
#include "engine/outputthread.hpp"
#include "logger.hpp"

class ThreadManager : public QObject {
  Q_OBJECT
public:
  ThreadManager(const EntryList &entryList,
                const QVector<Output *> &outputs,
                const int &threadNumber,
                Logger *logger);
  ~ThreadManager();

  void startThreads();

public slots:
  void onDone();

signals:
  void done();

private:
  const int m_threadNumber;
  const EntryList &m_entryList;
  const QVector<Output *> &m_outputs;
  Logger *p_logger;
  QVector<OutputThread *> m_threads;

  int m_threadsRemaining;
};

#endif // THREADMANAGER_HPP

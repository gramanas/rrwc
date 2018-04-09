#ifndef THREADMANAGER_HPP
#define THREADMANAGER_HPP

#include <QAtomicInteger>

#include "output/entrylist.hpp"
#include "output/output.hpp"
#include "engine/outputthread.hpp"

class ThreadManager : public QObject {
  Q_OBJECT
public:
  ThreadManager(const EntryList &entryList,
                const QVector<Output *> &outputs,
                const int &threadNumber);
  ~ThreadManager();

  int getItemsDone() { return m_itemsDone.fetchAndAddOrdered(0); }
  void startThreads();

public slots:
  void onDone();
  void onProgressChanged();

signals:
  void done();

private:
  const int m_threadNumber;
  const EntryList &m_entryList;
  const QVector<Output *> &m_outputs;
  QVector<OutputThread *> m_threads;

  QAtomicInteger<int> m_itemsDone = 0;
  int m_threadsRemaining;
};

#endif // THREADMANAGER_HPP

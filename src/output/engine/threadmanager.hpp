#ifndef THREADMANAGER_HPP
#define THREADMANAGER_HPP

#include "output/entrylist.hpp"
#include "output/output.hpp"
#include "output/engine/outputthread.hpp"

class ThreadManager : public QObject {
    Q_OBJECT
  public:
    ThreadManager(const EntryList &entryList,
                  const QVector<Output *> &outputs,
                  const int &threadNumber);
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
    QVector<OutputThread *> m_threads;

    int m_threadsRemaining;
};

#endif // THREADMANAGER_HPP

#include <QDebug>
#include "engine/threadmanager.hpp"

ThreadManager::ThreadManager(const EntryList &entryList,
                             const QVector<Output *> &outputs,
                             const int &threadNumber,
                             Logger *logger)
    : m_entryList(entryList),
      m_outputs(outputs),
      m_threadNumber(threadNumber),
      p_logger(logger) {
    m_threadsRemaining = threadNumber;
    m_threads.reserve(threadNumber);
}

ThreadManager::~ThreadManager() {
    for (auto it : m_threads) {
        delete it;
    }
    m_threads.clear();
}

void ThreadManager::startThreads() {
    auto listPtr = m_entryList.get();
    int totalFiles = listPtr->count();
    int filesPerThread = ceil(totalFiles / m_threadNumber);
    QVector<QString> threadFiles = {};
    auto it = listPtr->begin();
    for (int i = 0; i < m_threadNumber; i++) {
        // if we are on the last thread
        if (i == m_threadNumber-1) {
            while (it != listPtr->end()) {
                threadFiles << *it;
                it++;
            }
        } else {
            for (int j = 0; j < filesPerThread && it != listPtr->end(); j++) {
                threadFiles << *it;
                it++;
            }
        }
        m_threads[i] = new OutputThread(m_outputs, threadFiles, p_logger,
                                        i * filesPerThread, i);
        connect(m_threads[i], &QThread::finished, m_threads[i], &QObject::deleteLater);
        connect(m_threads[i], SIGNAL(done()), this, SLOT(onDone()));
        m_threads[i]->start();
    }
}

void ThreadManager::onDone() {
    if (--m_threadsRemaining == 0) {
        emit done();
    }
}

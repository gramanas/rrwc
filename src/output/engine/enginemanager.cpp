#include <QDebug>
#include <QThread>

#include "output/engine/enginemanager.hpp"
#include "globals.hpp"

EngineManager::EngineManager(Output const *output,
                             const QStringList &inputFiles,
                             const int &index)
    : p_output(output),
      m_inputFiles(inputFiles),
      m_index(index){

    m_engineThreads.reserve(p_output->threads);
    m_threadProgress.reserve(p_output->threads);

    for (int i = 0; i < p_output->threads; i++) {
        m_engineThreads[i] = new OutputThread();
        connect(m_engineThreads[i], SIGNAL(progressChanged(int, int)),
                this, SLOT(onProgressChanged(int, int)));
        connect(m_engineThreads[i], SIGNAL(done()),
                this, SLOT(onDone()));
        connect(m_engineThreads[i], SIGNAL(writeLog(QString, QString)),
                this, SLOT(onWriteLog(QString, QString)));
        connect(m_engineThreads[i], SIGNAL(finished()),
                m_engineThreads[i], SLOT(deleteLater()));

        m_threadProgress[i] = 0;
    }
    m_threadsRemaining = p_output->threads;
}

void EngineManager::startThreads() {
    QStringList allFiles = QStringList(m_inputFiles);

    int totalFiles = allFiles.count();
    int filesPerThread = totalFiles / p_output->threads;
    QStringList threadFiles = {};
    m_time.start();
    for (int i = 0; i < p_output->threads; i++) {
        // if we are on the last thread
        if (i == p_output->threads - 1) {
            for (const auto &it : allFiles) {
                threadFiles << it;
            }
            allFiles.clear();
        } else {
            for (int j = 0; j < filesPerThread && j < totalFiles; j++) {
                threadFiles << allFiles.first();
                allFiles.removeFirst();
            }
        }
        m_engineThreads[i]->init(p_output, threadFiles, i * (filesPerThread), i);
        m_engineThreads[i]->start();
    }
}

void EngineManager::onWriteLog(QString log, QString str) {
    emit writeLog(log, str);
}

void EngineManager::onProgressChanged(int thread, int progress) {
    m_threadProgress[thread] = progress;
    int sum = 0;
    for (int i = 0; i < p_output->threads; i++) {
        sum += m_threadProgress[i];
    }

    int tmp = int(float(sum) / float(p_output->threads));
    if (tmp != m_progress) {
        m_progress = tmp;
        emit progressChanged(m_index, m_progress);
    }
}

void EngineManager::onDone() {
    m_threadsRemaining--;
    if (m_threadsRemaining == 0) {
        emit writeLog(LOG_PROGRESS, QString("Output [%1] finished after %2 ms").arg(m_index + 1).arg(m_time.elapsed()));
        emit done();
    }
}

EngineManager::~EngineManager() {
    for (auto thread : m_engineThreads) {
        delete thread;
    }
}

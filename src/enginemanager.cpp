#include "enginemanager.hpp"
#include <QDebug>
#include <QThread>



EngineManager::EngineManager(OutputManager *outputManager,
                             const QString &inputPath)
    : p_outputManager(outputManager),
      m_inputDir(inputPath) {
    m_inputDir.setFilter(QDir::Files);

    for (int i = 0; i < p_outputManager->outputs().size(); i++) {
        m_totalThreads += p_outputManager->outputs()[i]->threads;
    }

    m_threadsRemaning = m_totalThreads;
}

void EngineManager::startNew(int i) {
    if (p_outputManager->outputs()[i]->threads == 1) {
        int currentThread = m_totalThreads - m_threadsRemaning;
        m_engineThreads.append(new OutputEngine());
        connect(m_engineThreads[currentThread], SIGNAL(progressChanged(int, int)),
                this, SLOT(onProgressChanged(int, int)));
        connect(m_engineThreads[currentThread], SIGNAL(done()),
                this, SLOT(onDone()));
        connect(m_engineThreads[currentThread], SIGNAL(finished()),
                m_engineThreads[currentThread], SLOT(deleteLater()));
        // progress
        m_threadProgress.append(0);
        // init
        m_engineThreads[currentThread]->init(p_outputManager->outputs()[i], m_inputDir, currentThread);
        m_engineThreads[currentThread]->start();
        m_threadsRemaning--;
    } else {
        int filesPerThread = (m_inputDir.count() / p_outputManager->outputs()[i]->threads) + 1;
        for (int j = 0; j < p_outputManager->outputs()[i]->threads; j++) {
            int currentThread = m_totalThreads - m_threadsRemaning;
            m_engineThreads.append(new OutputEngine());
            connect(m_engineThreads[currentThread], SIGNAL(progressChanged(int, int)),
                    this, SLOT(onProgressChanged(int, int)));
            connect(m_engineThreads[currentThread], SIGNAL(done()),
                    this, SLOT(onDone()));
            connect(m_engineThreads[currentThread], SIGNAL(finished()),
                    m_engineThreads[currentThread], SLOT(deleteLater()));
            // progress
            m_threadProgress.append(0);
            // init
            m_engineThreads[currentThread]->init(p_outputManager->outputs()[i], m_inputDir, currentThread);
            m_engineThreads[currentThread]->setFilesPerThread(filesPerThread);
            // TI VLAKAS POU EIMAI ELEOS RE
            m_engineThreads[currentThread]->setStartingFilePosition(j * filesPerThread);
            m_engineThreads[currentThread]->start();
            m_threadsRemaning--;
        }
    }
    m_outputProgress.append(0);
}

void EngineManager::onProgressChanged(int thread, int progress) {
    qDebug() << "Thread, Progress" << thread << progress;
    m_threadProgress[thread] = progress;
    int sum = 0;
    int sum2 = 0;
    for (int i = 0; i < p_outputManager->outputs().size(); i+=p_outputManager->outputs()[i]->threads) {
        for (int j = 0; j < p_outputManager->outputs()[i]->threads; j++) {
            sum2 += m_threadProgress[i + j];
        }
        int avgOutput = float(sum2) / float(p_outputManager->outputs()[i]->threads);
        sum += avgOutput;
    }
    int avg = float(sum)/float(m_totalThreads);
    emit progressChanged(avg);
}

void EngineManager::onDone() {
    m_totalThreads--;
    if (m_totalThreads == 0) {
        emit done();
    }
}

EngineManager::~EngineManager() {
}

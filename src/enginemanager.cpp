#include "enginemanager.hpp"
#include <QDebug>
#include <QThread>


EngineManager::EngineManager(OutputManager *outputManager,
                             const QString &inputPath)
    : p_outputManager(outputManager),
      m_inputPath(inputPath) {
    for (int i = 0; i < p_outputManager->outputs().size(); i++) {
        // new thread
        m_engineThreads.append(new OutputEngine());
        connect(m_engineThreads[i], SIGNAL(progressChanged(int, int)),
                this, SLOT(onProgressChanged(int, int)));
        connect(m_engineThreads[i], SIGNAL(done()),
                this, SLOT(onDone()));
        connect(m_engineThreads[i], SIGNAL(finished()),
                m_engineThreads[i], SLOT(deleteLater()));
        // progress
        m_threadProgress.append(0);
    }
    m_threadNumber = p_outputManager->outputs().size();
}

void EngineManager::startNew(int i) {
    m_engineThreads[i]->init(p_outputManager->outputs()[i], m_inputPath, i);
    m_engineThreads[i]->start();
}

void EngineManager::onProgressChanged(int thread, int progress) {
    m_threadProgress[thread] = progress;
    int sum = 0;
    for (int i = 0; i < p_outputManager->outputs().size(); i++) {
        sum = sum + m_threadProgress[i];
    }
    int avg = float(sum)/float(p_outputManager->outputs().size());
    emit progressChanged(avg);
}

void EngineManager::onDone() {
    m_threadNumber--;
    if (m_threadNumber == 0) {
        emit done();
    }
}

EngineManager::~EngineManager() {
}

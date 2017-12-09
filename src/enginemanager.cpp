#include <QDebug>
#include <QThread>

#include "enginemanager.hpp"
#include "exifmanager.hpp"
//#include "../easyexif/exif.h"
EngineManager::EngineManager(Output const *output,
                             const QString &inputPath,
                             const int &index)
    : p_output(output),
      m_inputDir(inputPath),
      m_index(index){

    m_engineThreads.reserve(p_output->threads);
    m_threadProgress.reserve(p_output->threads);

    for (int i = 0; i < p_output->threads; i++) {
        m_engineThreads[i] = new OutputEngine();
        connect(m_engineThreads[i], SIGNAL(progressChanged(int, int)),
                this, SLOT(onProgressChanged(int, int)));
        connect(m_engineThreads[i], SIGNAL(done()),
                this, SLOT(onDone()));
        connect(m_engineThreads[i], SIGNAL(finished()),
                m_engineThreads[i], SLOT(deleteLater()));

        m_threadProgress[i] = 0;
    }

    m_threadsRemaining = p_output->threads;
}

void EngineManager::startThreads() {
    QStringList allFiles = m_inputDir.entryList(
      QStringList({"*.jpg", "*.JPG"}),
      QDir::Files, QDir::Name);

    ExifManager exifManager(m_inputDir.absolutePath());
    exifManager.sortByDateTime(allFiles);


    int totalFiles = allFiles.count();
    qDebug() << "Total files:" << totalFiles;
    int filesPerThread = totalFiles / p_output->threads;
    QStringList threadFiles = {};
    for (int i = 0; i < p_output->threads; i++) {
        // if we are on the last thread
        if (i == p_output->threads - 1) {
            for (const auto &it : allFiles) {
                threadFiles << m_inputDir.absolutePath() + QDir::separator() + it;
            }
            allFiles.clear();
        } else {
            for (int j = 0; j < filesPerThread && j < totalFiles; j++) {
                threadFiles << m_inputDir.absolutePath() + QDir::separator() + allFiles.first();
                allFiles.removeFirst();
            }
        }
        m_engineThreads[i]->init(p_output, threadFiles, i * (filesPerThread), i);
        // m_engineThreads[i]->setFilesPerThread(int(m_inputDir.count() / p_output->threads));
        // m_engineThreads[i]->setStartingFilePosition(i * int(m_inputDir.count() / p_output->threads));
        m_engineThreads[i]->start();
    }
}

void EngineManager::onProgressChanged(int thread, int progress) {
    m_threadProgress[thread] = progress;
    int sum = 0;
    for (int i = 0; i < p_output->threads; i++) {
        sum += m_threadProgress[i];
    }

    emit progressChanged(m_index, int(float(sum) / float(p_output->threads)));
}

void EngineManager::onDone() {
    m_threadsRemaining--;
    if (m_threadsRemaining == 0) {
        emit done();
    }
}

EngineManager::~EngineManager() {
    for (auto thread : m_engineThreads) {
        delete thread;
    }
}

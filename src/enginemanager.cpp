#include "enginemanager.hpp"
#include "outputengine.hpp"
#include <QDebug>
#include <QThread>


EngineManager::EngineManager(OutputManager *outputManager,
                             const QString &inputPath)
    : p_outputManager(outputManager),
      m_inputPath(inputPath){
}

void EngineManager::startNew(int i) {
    OutputEngine *engineThread =
        new OutputEngine(p_outputManager->outputs()[i],
                         m_inputPath, i);
    engineThread->start();
}

EngineManager::~EngineManager() {
}

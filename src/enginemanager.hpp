#ifndef ENGINEMANAGER_HPP
#define ENGINEMANAGER_HPP

#include <QObject>

#include "outputmanager.hpp"

class EngineManager : public QObject {
    Q_OBJECT
  public:
    explicit EngineManager(OutputManager *outputManager,
                           const QString &inputPath);
    ~EngineManager();

    void startNew(int i);

  private:
    OutputManager *p_outputManager;
    const QString m_inputPath;
};

#endif // ENGINEMANAGER_HPP

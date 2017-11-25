#ifndef ENGINEMANAGER_HPP
#define ENGINEMANAGER_HPP

#include <QObject>
#include <QDir>

#include "outputengine.hpp"
#include "outputmanager.hpp"

/////////////////////////////////////////////////////////////////////////////
// The EngineManager object must be created just before the output threads //
// start doing their job, and must be deleted when they finish.            //
/////////////////////////////////////////////////////////////////////////////

class EngineManager : public QObject {
    Q_OBJECT
  public:
    explicit EngineManager(OutputManager *outputManager,
                           const QString &inputPath);
    ~EngineManager();

    // Start threads corresponding to output i
    void startNew(int i);

  public slots:
    void onProgressChanged(int thread, int progress);
    void onDone();

  signals:
    void progressChanged(int progress);
    void done();

  private:
    OutputManager *p_outputManager;
    int m_totalThreads = 0;
    int m_threadsRemaning;
    QVector<OutputEngine *> m_engineThreads;
    QVector<int> m_threadProgress;
    QVector<int> m_outputProgress;
    QDir m_inputDir;
};

#endif // ENGINEMANAGER_HPP

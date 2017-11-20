#ifndef ENGINEMANAGER_HPP
#define ENGINEMANAGER_HPP

#include <QObject>

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

    // Start thread corresponding to output i
    void startNew(int i);

  public slots:
    void onProgressChanged(int thread, int progress);
    void onDone();

  signals:
    void progressChanged(int progress);
    void done();

  private:
    OutputManager *p_outputManager;
    int m_threadNumber;
    QVector<OutputEngine *> m_engineThreads;
    QVector<int> m_threadProgress;
    const QString m_inputPath;
};

#endif // ENGINEMANAGER_HPP

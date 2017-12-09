#ifndef ENGINEMANAGER_HPP
#define ENGINEMANAGER_HPP

#include <QVector>
#include <QDir>

#include "outputengine.hpp"
#include "output.hpp"
//#include "outputmanager.hpp"

/////////////////////////////////////////////////////////////////////////////
// The EngineManager object must be created just before the output threads //
// start doing their job, and must be deleted when they finish.            //
/////////////////////////////////////////////////////////////////////////////

class EngineManager : public QObject {
    Q_OBJECT
  public:
    explicit EngineManager(Output const *output,
                           const QStringList &inputFiles,
                           const int &index);
    ~EngineManager();

    // Start threads corresponding to output i
    void startThreads();

  public slots:
    void onProgressChanged(int thread, int progress);
    void onDone();

  signals:
    void progressChanged(int output, int progress);
    void done();

  private:
    const int m_index;
    Output const *p_output;
    int m_threadsRemaining;
    QVector<OutputEngine *> m_engineThreads;
    QVector<int> m_threadProgress;
    const QStringList &m_inputFiles;
};

#endif // ENGINEMANAGER_HPP

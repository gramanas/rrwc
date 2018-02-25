#ifndef ENGINEMANAGER_HPP
#define ENGINEMANAGER_HPP

#include <QVector>
#include <QDir>
#include <QTime>

#include "output/output.hpp"
#include "output/engine/outputthread.hpp"
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
                           const int &index,
                           const int &threads);
    ~EngineManager();

    // Start threads corresponding to output i
    void startThreads();

  public slots:
    void onProgressChanged(int thread, int progress);
    void onDone();
    void onWriteLog(QString log, QString str);

  signals:
    void progressChanged(int output, int progress);
    void done();
    void writeLog(QString log, QString str);

  private:
    QTime m_time;
    const int m_index;
    const int m_threads;
    Output const *p_output;
    OutputThread *p_engineThread;
    QVector<int> m_threadProgress;
    int m_progress = 0;
    const QStringList &m_inputFiles;
};

#endif // ENGINEMANAGER_HPP

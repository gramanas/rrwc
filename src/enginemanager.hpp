#ifndef ENGINEMANAGER_HPP
#define ENGINEMANAGER_HPP

#include <QVector>
#include <QDir>

#include "outputthread.hpp"
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
    void onWriteLog(QString log, QString str);

  signals:
    void progressChanged(int output, int progress);
    void done();
    void writeLog(QString log, QString str);

  private:
    const int m_index;
    Output const *p_output;
    int m_threadsRemaining;
    QVector<OutputThread *> m_engineThreads;
    QVector<int> m_threadProgress;
    const QStringList &m_inputFiles;
};

#endif // ENGINEMANAGER_HPP

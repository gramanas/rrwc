#ifndef OUTPUTTHREAD_HPP
#define OUTPUTTHREAD_HPP

#include <QThread>
#include <QObject>
#include <QDir>

#include "output/output.hpp"
#include "engine/outputengine.hpp"

class OutputThread : public QThread {
    Q_OBJECT
  public:
    void init(Output const *output,
              QStringList &inputFiles,
              const int current,
              const int index);

    // parallel code
    void run() override;


  public slots:
    void onWriteLog(QString log, QString str);

  signals:
    void progressChanged(int thread, int progress);
    void writeLog(QString log, QString str);
    void done();

  private:
    OutputEngine m_engine;
    int m_current = 0;
    int m_progress = 0;
    int m_total;
    int m_index;
    Output const *p_output;
    QStringList m_inputFiles = {};
};

#endif // OUTPUTTHREAD_HPP

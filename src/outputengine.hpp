#ifndef OUTPUTENGINE_HPP
#define OUTPUTENGINE_HPP

#include <QThread>
#include <QObject>
#include <QDir>

#include "output.hpp"

class OutputEngine : public QThread {
    Q_OBJECT
  public:
    void init(Output const *output,
              QStringList &inputFiles,
              const int current,
              const int index);

    // parallel code
    void run() override;


  signals:
    void progressChanged(int thread, int progress);
    void writeLog(QString log, QString str);
    void done();

  private:
    int m_current = 0;
    int m_total;
    int m_index;
    Output const *p_output;
    QStringList m_inputFiles = {};
};

#endif // OUTPUTENGINE_HPP

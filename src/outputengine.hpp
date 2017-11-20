#ifndef OUTPUTENGINE_HPP
#define OUTPUTENGINE_HPP

#include <QThread>
#include <QObject>
//#include <QString>

#include "outputmanager.hpp"

class OutputEngine : public QThread {
    Q_OBJECT
  public:
    void init(Output const *output,
              const QString &inputPath,
              int index);

    // parallel code
    void run() override;

  signals:
    void progressChanged(int thread, int progress);
    void done();

  private:
    int m_index;
    Output const *p_output;
    QString m_inputPath;
};

#endif // OUTPUTENGINE_HPP

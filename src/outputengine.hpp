#ifndef OUTPUTENGINE_HPP
#define OUTPUTENGINE_HPP

#include <QThread>
#include <QObject>
#include <QDir>

#include "outputmanager.hpp"

class OutputEngine : public QThread {
    Q_OBJECT
  public:
    void init(Output const *output,
              const QDir &inputDir,
              int index);

    // parallel code
    void run() override;

    void setFilesPerThread(int files) {
        m_filesPerThread = files;
    }

    void setStartingFilePosition(int pos) {
        m_startingFilePosition = pos;
    }

  signals:
    void progressChanged(int thread, int progress);
    void done();

  private:
    int m_current = 0;
    int m_startingFilePosition = 0;
    int m_filesPerThread;
    int m_index;
    int m_total;
    Output const *p_output;
    QDir m_inputDir;
};

#endif // OUTPUTENGINE_HPP

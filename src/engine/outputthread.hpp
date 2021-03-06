#ifndef OUTPUTTHREAD_HPP
#define OUTPUTTHREAD_HPP

#include <QThread>
#include <QVector>
#include <QObject>
#include <QDir>

#include "output/output.hpp"
#include "engine/outputengine.hpp"
#include "logger.hpp"

class OutputThread : public QThread {
  Q_OBJECT
public:
  OutputThread(const QVector<Output *> &outputs,
               QVector<QString> &inputFiles,
               Logger *logger,
               const int current,
               const int index);
  ~OutputThread();

  // parallel code
  void run() override;

signals:
  void done();

private:
  int m_current = 0;
  int m_progress = 0;
  int m_total;
  int m_index;

  Logger *p_logger;
  const QVector<Output *> m_outputs;
  QVector<QString> m_inputFiles = {};
  OutputEngine m_engine;
};

#endif // OUTPUTTHREAD_HPP

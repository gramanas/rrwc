#ifndef OUTPUTENGINE_HPP
#define OUTPUTENGINE_HPP

#include <QThread>
#include <QObject>
//#include <QString>

#include "outputmanager.hpp"

class OutputEngine : public QThread {
    Q_OBJECT
  public:
    OutputEngine(Output const *const output,
                 const QString &inputPath,
                 int index);
    ~OutputEngine();

    //void initiate();

    void run() override;

  private:
    const int m_index;
    Output const *const p_output;
    const QString m_inputPath;
};

#endif // OUTPUTENGINE_HPP

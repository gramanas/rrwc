#ifndef ERRORMANAGER_HPP
#define ERRORMANAGER_HPP

#include "outputmanager.hpp"

class ErrorManager : public QObject {
    Q_OBJECT
  public:
    ErrorManager(const OutputManager &outputManager);
    ~ErrorManager();

    bool checkInputErrors(const QString &inputPath);

  signals:
    void writeLog(QString log, QString str);

  private:
    const OutputManager &m_outputManager;
};

#endif // ERRORMANAGER_HPP
